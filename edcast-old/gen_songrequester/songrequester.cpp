/*
Oddsock - SongRequester
Copyright (C) 2000  Oddsock

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

oddsock@oddsock.org
*/

/*
This code is free....Just like the air we breathe...But just like the air, it most
likely will contain some impurities....So please, don't choke on it...And don't 
complain (too much) if it billows smoke....


Thank you.

  oddsock@oddsock.org
*/
//////////////////////////////////////////
//
// INCLUDE
//
#include <fstream.h>
#include <afxwin.h>
#include <afxinet.h>
#include <afxdisp.h>
#include <afxtempl.h>
#include <afxmt.h>
#include "gen.h"
#include "frontend.h"
#include "resource.h"
#include "stdafx.h"
#include "stdafx.h"
#include <process.h>
#include <io.h>
//#include <id3.h>
//#include <id3/tag.h>

/////////////////////////////////////////
//
// FUNCTIONS
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK BigWindowHandler(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
int init();
void config();
void quit();
void config_write();
void config_read();
void PlaySong(int);

extern int ProxyMain(unsigned short int uiPort);
extern void ProxyQuit();
#define WM_WA_MPEG_EOF (WM_USER+2)
void Start_Processing(void *pParam);

/////////////////////////////////////////
//
// VARIABLES
//

HWND	g_hwndDlg;
char	g_WebRoot[1024] = "";
char	g_Playlistfile[1024] = "";
char	g_MaxEntries[25] = "";
char	g_MaxQueueSize[25] = "";
char	g_AdminPassword[255] = "";
char	g_Port[10] = "";
int		g_iPort = 0;
int 	g_CurrentSongLength;
int		g_CurrentPctLeft;
int		g_CurrentSongLeft;

char	g_MaxRequestsPerIP[25] = "";


char	szSongTitle[1024];
char	szArtist[1024];
char	szAlbum[1024];
char	szComment[1024];

// STRINGS



typedef struct tag_Element {
	CString		value;
	long		time;
} MP3Element;
typedef struct tag_SongInfo {
	CString	Title;
	CString	Artist;
	CString	Album;
} SongInfo;


CArray<MP3Element, MP3Element &> RecentArtists;
CArray<MP3Element, MP3Element &> RecentAlbums;
CArray<MP3Element, MP3Element &> RecentSongs;

HWND	g_StatusDialog;
char	StatusText[1024] = "";
BOOL	bisRunning = FALSE;

CWinThread	*gProcessingThread;
CWinThread	*gMonitoringThread;

CRITICAL_SECTION GlobalCriticalSection_QueueList; 
CRITICAL_SECTION GlobalCriticalSection_ThreadCount; 

// OTHER STUFF

char szAppName[]                    = "HTTP Request";

/////////////////////////////////////////
//
// PLUGIN DEFINITION
//
winampGeneralPurposePlugin plugin = {GPPHDR_VER,"",init,config,quit,};


/////////////////////////////////////////
// 
// EXTERN STUFF
//

#ifdef __cplusplus
BOOL WINAPI _DllMainCRTStartup(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
	__declspec( dllexport ) winampGeneralPurposePlugin * winampGetGeneralPurposePlugin()
	{
		return &plugin;
	}
#ifdef __cplusplus
}
#endif


////////////////////////////////////////////////////
//
// UploadInfo
//
BOOL	bWindowOpen;
BOOL	bRunning;
BOOL	bDisabled;
int		gThreadsRunning = 0;

int		gNumInPlaylist = 0;

CArray<CString, CString &> PlaylistFileCache;
CArray<CString, CString &> PlaylistTitleCache;

char *getSongTitle(int playlistpos)
{
	if (playlistpos >= gNumInPlaylist) {
		return 0;
	}


	if (playlistpos < PlaylistTitleCache.GetSize()) {
		return (char *)(LPCSTR)PlaylistTitleCache.GetAt(playlistpos);
	}
	else {
		return NULL;
	}
	
//	char *ret = (char*)SendMessage(plugin.hwndParent,WM_WA_IPC, playlistpos ,IPC_GETPLAYLISTTITLE);
//	return ret;
}

int getArtist(int playlistpos, char *result)
{
	char *p1;
	char *p2;

//	p1 = (char*)SendMessage(plugin.hwndParent,WM_WA_IPC, playlistpos ,IPC_GETPLAYLISTTITLE);
	if (playlistpos >= gNumInPlaylist) {
		return 0;
	}
	p1 = getSongTitle(playlistpos);
	if (p1 != NULL) {
		p2 = strstr(p1, " -");
		if (p2 != NULL) {
			strncpy(result, p1, p2-p1);
			return 1;
		}
		return 1;
	}
	return 0;
}

void repopulatePlaylistCache()
{
	CString file;
	CString	title;
	int length = SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_GETLISTLENGTH);
	if (length != gNumInPlaylist) {
		PlaylistFileCache.RemoveAll();
		PlaylistTitleCache.RemoveAll();
		for (int i=0;i<length;i++) {
			file = (char *)SendMessage(plugin.hwndParent,WM_WA_IPC,i,IPC_GETPLAYLISTFILE);
			title = (char *)SendMessage(plugin.hwndParent,WM_WA_IPC,i,IPC_GETPLAYLISTTITLE);
			if (file && title) {
				PlaylistFileCache.SetAtGrow(i, file);
				PlaylistTitleCache.SetAtGrow(i, title);
			}
		}
		gNumInPlaylist = length;
	}
}

void incrementThreadCount()
{
		gThreadsRunning++;

}
void decrementThreadCount()
{
		gThreadsRunning--;

}

void AddComboItem(UINT	comboBox, char *buffer)
{
	SendMessage(GetDlgItem(g_hwndDlg,comboBox), CB_ADDSTRING, 0, (LPARAM) buffer); 
}

void EnableControl(UINT	control)
{
	EnableWindow(GetDlgItem(g_hwndDlg,control), TRUE);
}
void DisableControl(UINT	control)
{
	EnableWindow(GetDlgItem(g_hwndDlg,control), FALSE);
}

void HideControl(UINT	control)
{
	ShowWindow(GetDlgItem(g_hwndDlg,control), SW_HIDE);
}
void ShowControl(UINT	control)
{
	ShowWindow(GetDlgItem(g_hwndDlg,control), SW_SHOW);
}

void HideAllControls()
{
;
}


void SetComboItemSelected(UINT	comboBox, char *buffer)
{
	SendMessage(GetDlgItem(g_hwndDlg,comboBox), WM_SETTEXT, 0, (LPARAM) buffer);
}
void GetComboItemSelected(UINT	comboBox, char *buffer)
{
	char	tempBuffer[255] = "";

	SendMessage(GetDlgItem(g_hwndDlg,comboBox), WM_GETTEXT, sizeof(tempBuffer), (LPARAM) tempBuffer);
	strcpy(buffer, tempBuffer);
}


/////////////////////////////////////////
//
// Check_Connection
//
typedef struct ID3v1 {
	char	tag[3];
	char	songtitle[30];
	char	artist[30];
	char	album[30];
	char	year[4];
	char	comment[30];
	char	genre;
} ID3V1;




void CleanOutArray(time_t currentTime, time_t maxTime)
{
	int size = RecentSongs.GetSize();

	int j=0;
	int	oldtime;
	oldtime = currentTime - maxTime;
	for (int i=0;i<size-1;i++) {
		if (RecentSongs.GetAt(j).time < oldtime) {
			// whack it
			RecentSongs.RemoveAt(j);
			RecentArtists.RemoveAt(j);
			RecentAlbums.RemoveAt(j);
		}
		else {
			j++;
		}
		
	}
}

void Start_Processing(void *pParam)
{
	char	msg[255] = "";


	bisRunning = TRUE;
	ProxyMain(g_iPort);
	decrementThreadCount();	
	_endthread();
}

char	szCurrentSong[1024] = "";
char	szPrevSong[1024] = "";

typedef struct {
	int		playlistpos;
	int		songlength;
	CString	requestingIP;
} Req_Queue;

typedef struct {
	int		playlistpos;
	time_t	timePlayed;
} Played;

CArray<Req_Queue, Req_Queue &> RequestQueue;
CArray<Played, Played &> RecentlyPlayed;



int		lastPos = 0;
int		currPos = 0;
BOOL	bStopRunning = FALSE;


UINT Start_Monitoring(LPVOID pParam)
{
	int noQueueFlag;
	int playedQueueFlag;
	int currentlySavedPos = 0;


	bisRunning = TRUE;
	noQueueFlag = 1;
	playedQueueFlag = 0;
	while((!bDisabled) && (bisRunning)) {
		int res = SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_ISPLAYING);
		if (res == 1) { // If it is playing then do the check...
			currPos = SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_GETLISTPOS);

			if (currPos != lastPos) {
				int playedSong = 0;

				lastPos = currPos;

//				strcpy(szPrevSong, szCurrentSong);
				char	*pszCurrentSong;
				pszCurrentSong = (char*)SendMessage(plugin.hwndParent,WM_WA_IPC,SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_GETLISTPOS),IPC_GETPLAYLISTTITLE);
				if (pszCurrentSong != NULL) {
					strcpy(szCurrentSong, pszCurrentSong);
				}
			
			//	MessageBox(NULL, szCurrentSong, "Change!", MB_OK);

				if (noQueueFlag) {
					currentlySavedPos = currPos;
				}

				if (RequestQueue.GetUpperBound() != -1) {
					noQueueFlag = 0;
					playedQueueFlag = 1;
					// Request ownership of the critical section.
//					EnterCriticalSection(&GlobalCriticalSection_QueueList); 
					// Access the shared resource.
					PlaySong(RequestQueue[RequestQueue.GetUpperBound()].playlistpos);
					playedSong = RequestQueue[RequestQueue.GetUpperBound()].playlistpos;
					RequestQueue.RemoveAt(RequestQueue.GetUpperBound());
					// Release ownership of the critical section.
//					LeaveCriticalSection(&GlobalCriticalSection_QueueList);
//					QueueList.Unlock();
					lastPos = SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_GETLISTPOS);
				}
				else {
					noQueueFlag = 1;
					playedSong = currPos;
				}
				if ((noQueueFlag) && (playedQueueFlag)) {
					playedQueueFlag = 0;
				//	PlaySong(currentlySavedPos);
					playedSong = currentlySavedPos;
				}
			}
		}
		Sleep(100);
//		if (!bisRunning) {
//			_endthread();
//		}
	}
	decrementThreadCount();
	return 1;
	_endthread();
}	


/////////////////////////////////////////
//
// WNDPROC
//
int currentlySavedPos = -1;

void* lpWndProcOld;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int noQueueFlag;
	int playedQueueFlag;
	int playedSong = 0;

	int	dontCall = 0;

	if (msg==WM_WA_MPEG_EOF)	{

		noQueueFlag = 1;
		playedQueueFlag = 0;

		if (!bDisabled) {

			if (RequestQueue.GetUpperBound() == -1) {
				if (currentlySavedPos != -1) {
					PlaySong(currentlySavedPos);
					SendMessage(plugin.hwndParent, WM_COMMAND,40048,0);
					currentlySavedPos = -1;
					dontCall = 1;
				}
			}


			if (RequestQueue.GetUpperBound() != -1) {
				noQueueFlag = 0;
				playedQueueFlag = 1;

				if (currentlySavedPos == -1) {
					currentlySavedPos = SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_GETLISTPOS);
				}
				PlaySong(RequestQueue[RequestQueue.GetUpperBound()].playlistpos);
				playedSong = RequestQueue[RequestQueue.GetUpperBound()].playlistpos;
				RequestQueue.RemoveAt(RequestQueue.GetUpperBound());
				lastPos = SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_GETLISTPOS);
				dontCall = 1;
			}
			else {
				noQueueFlag = 1;
				playedSong = currPos;
			}
			if ((noQueueFlag) && (playedQueueFlag)) {
				playedQueueFlag = 0;
				playedSong = currentlySavedPos;
			}
		}
	}
	if (!dontCall) {
		return CallWindowProc((WNDPROC)lpWndProcOld,hwnd,msg,wParam,lParam);  
	}
	else {
		return 0;
	}
}



// Initialize the critical section.


/////////////////////////////////////////
//
// INIT
//
int init()
{
	static char c[512];
	char filename[512],*p;
	

	InitializeCriticalSection(&GlobalCriticalSection_QueueList); 
	InitializeCriticalSection(&GlobalCriticalSection_ThreadCount); 
	
	
	GetModuleFileName(plugin.hDllInstance,filename,sizeof(filename));
	p = filename+lstrlen(filename);
	while (p >= filename && *p != '\\') p--;
	wsprintf((plugin.description=c),"SongRequester Plugin v2.5 (%s)", p+1);
	config_read();
	
	
	lpWndProcOld = (void*)GetWindowLong(plugin.hwndParent,GWL_WNDPROC);
	SetWindowLong(plugin.hwndParent,GWL_WNDPROC,(LONG)WndProc);

	
	if (!bisRunning) {
		if (!bDisabled) {
			repopulatePlaylistCache();
			incrementThreadCount();

			_beginthread(Start_Processing, 0, "");
		}
	}

	return 0;
}


/////////////////////////////////////////
//
// CONFIG
//
void config()
{
	DialogBox(plugin.hDllInstance,MAKEINTRESOURCE(IDD_DIALOG2),plugin.hwndParent,BigWindowHandler);
}


/////////////////////////////////////////
//
// QUIT
//
void quit()
{
	bRunning = FALSE;
	ProxyQuit();
	config_write();
	while (gThreadsRunning != 0) {
		Sleep(1000);
	}
}




/////////////////////////////////////////
//
// CONFIGIREPROC
//

char *GetQueueTitle(int pos)
{
	if (pos >= gNumInPlaylist) {
		return 0;
	}
//	EnterCriticalSection(&GlobalCriticalSection_QueueList); 
	// Access the shared resource.
	if ((pos > RequestQueue.GetUpperBound()) || (pos < 0)) {
			return NULL;
	}
	// Release ownership of the critical section.
//	LeaveCriticalSection(&GlobalCriticalSection_QueueList);

//	char *ret=(char*)SendMessage(plugin.hwndParent,WM_WA_IPC, RequestQueue.GetAt(pos).playlistpos,IPC_GETPLAYLISTTITLE);
	if (pos < RequestQueue.GetSize()) {
		if (RequestQueue.GetAt(pos).playlistpos < PlaylistTitleCache.GetSize()) {
			return (char *)(LPCSTR)PlaylistTitleCache.GetAt(RequestQueue.GetAt(pos).playlistpos );
		}
	}
	return NULL;
}


void GetSecondsLeft()
{
	int mode;

	mode = 1; // Song Length in seconds
	int songLength = SendMessage(plugin.hwndParent,WM_WA_IPC,mode,IPC_GETOUTPUTTIME);
	mode = 0;
	int songPos = (SendMessage(plugin.hwndParent,WM_WA_IPC,mode,IPC_GETOUTPUTTIME) / 1000);

	int pctLeft = 100 - (int)(((double)songPos / songLength) * 100);

	g_CurrentSongLength = songLength;
	g_CurrentPctLeft = pctLeft;
	g_CurrentSongLeft = songLength - songPos;

//	char	msg[256] = "";
//
//	sprintf(msg, "Current song length = %d seonds, Current Pos = %d seconds, Pct Left %d ", songLength, songPos, pctLeft);
//	MessageBox(NULL, msg, "Song length", MB_OK);

}

char *getCurrentSong()
{

	int currPos = SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_GETLISTPOS);
//	char *ret = (char*)SendMessage(plugin.hwndParent,WM_WA_IPC, currPos ,IPC_GETPLAYLISTTITLE);
	if (currPos < PlaylistTitleCache.GetSize()) {
		return (char *)(LPCSTR)PlaylistTitleCache.GetAt(currPos);
	}
	else {
		return NULL;
	}
//	return ret;
}



void PlayWinamp()
{
	SendMessage(plugin.hwndParent,WM_COMMAND, WINAMP_BUTTON2, 0);
//	SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_STARTPLAY);
}
void StopWinamp()
{
	SendMessage(plugin.hwndParent,WM_COMMAND, WINAMP_BUTTON4, 0);
}
void PrevWinamp()
{
	SendMessage(plugin.hwndParent,WM_COMMAND, WINAMP_BUTTON1, 0);
}
void NextWinamp()
{
	SendMessage(plugin.hwndParent,WM_COMMAND, WINAMP_BUTTON5, 0);
	if (RequestQueue.GetUpperBound() != -1) {
		SendMessage(plugin.hwndParent, WM_WA_MPEG_EOF, 0, 0); 
	}
}
void PauseWinamp()
{
	SendMessage(plugin.hwndParent,WM_COMMAND, WINAMP_BUTTON3, 0);
}


void PlaySong(int playlistpos)
{
	SendMessage(plugin.hwndParent,WM_USER, playlistpos ,IPC_SETPLAYLISTPOS);
	SendMessage(plugin.hwndParent,WM_COMMAND, 40045, 0);
}


int CALLBACK BigWindowHandler(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	char		tmp[10] = "";
	char	selected[255] = "";
	int		selectedIndex = 0;
	char	tempbuf[1024] = "";
	int		listboxcount = 0;
	CString	tempString;
	CString	tempString1;
	CString	tempString2;
	CString	tempString3;
	CString	tempString4;
	CString	tempString5;

	g_hwndDlg = hwndDlg;
	switch (uMsg) 
	{
		
	case WM_INITDIALOG:
		bWindowOpen = TRUE;

		if (bDisabled) {
			CheckDlgButton(hwndDlg, IDC_DISABLE, BST_CHECKED);
		}
		else {
			CheckDlgButton(hwndDlg, IDC_DISABLE, BST_UNCHECKED);
		}

		SetDlgItemText(hwndDlg, IDC_WEBROOT, g_WebRoot);
		SetDlgItemText(hwndDlg, IDC_PORT, g_Port);
		SetDlgItemText(hwndDlg, IDC_PLAYLISTFILE, g_Playlistfile);
		SetDlgItemText(hwndDlg, IDC_MAXPERIP, g_MaxRequestsPerIP);
		SetDlgItemText(hwndDlg, IDC_MAXENTRIES, g_MaxEntries);
		SetDlgItemText(hwndDlg, IDC_MAXQUEUE, g_MaxQueueSize);
		SetDlgItemText(hwndDlg, IDC_PASSWORD, g_AdminPassword);

		if (!bisRunning) {
			if (!bDisabled) {
				incrementThreadCount();
				_beginthread(Start_Processing, 0, "");
			}
		}
		break;
		
	case WM_COMMAND:
		
		int	bleh = LOWORD(wParam);
		int bleh2 = HIWORD(wParam);
		switch (LOWORD(wParam))  
		{

		case ID_ABOUT:
			MessageBox(NULL, "Song Requester\nBy: Oddsock\nhttp://www.oddsock.org/", "Song Requester", MB_OK);
			break;


		case IDC_OK:
			bWindowOpen = FALSE;
			bRunning = FALSE;
			

			GetDlgItemText(hwndDlg, IDC_WEBROOT, g_WebRoot, sizeof(g_WebRoot));
			GetDlgItemText(hwndDlg, IDC_PORT, g_Port, sizeof(g_Port));
			g_iPort = atoi(g_Port);
			GetDlgItemText(hwndDlg, IDC_PLAYLISTFILE, g_Playlistfile, sizeof(g_Playlistfile));
			GetDlgItemText(hwndDlg, IDC_PASSWORD, g_AdminPassword, sizeof(g_AdminPassword));
			if (IsDlgButtonChecked(hwndDlg, IDC_DISABLE) == BST_CHECKED) {
				bDisabled = TRUE;
			}
			else {
				bDisabled = FALSE;
			}

			GetDlgItemText(hwndDlg, IDC_MAXPERIP, g_MaxRequestsPerIP, sizeof(g_MaxRequestsPerIP));
			GetDlgItemText(hwndDlg, IDC_MAXQUEUE, g_MaxQueueSize, sizeof(g_MaxQueueSize));
			GetDlgItemText(hwndDlg, IDC_MAXENTRIES, g_MaxEntries, sizeof(g_MaxEntries));
			if (!bisRunning) {
				if (!bDisabled) {
					incrementThreadCount();
					_beginthread(Start_Processing, 0, "");
				}
			}
			EndDialog(hwndDlg,0);
			break;

		case ID_QUIT:

			bWindowOpen = FALSE;
			bRunning = FALSE;
			EndDialog(hwndDlg,0);
			break;
		}

	}
	
	return FALSE;
}


/////////////////////////////////////////
//
// CONFIG_READ
//
void config_read()
{


	char	strDisabled[25] = "";
	char	strID3info[25] = "";
	
	char ini_file[MAX_PATH], *p;
	GetModuleFileName(plugin.hDllInstance,ini_file,sizeof(ini_file));
	p=ini_file+lstrlen(ini_file);
	while (p >= ini_file && *p != '\\') p--;
	if (++p >= ini_file) *p = 0;
	lstrcat(ini_file,"plugin.ini");
	
	char	tmp[1024] = "";
	CString	tempString;

	GetPrivateProfileString(szAppName,"bDisabled","TRUE",strDisabled,sizeof(strDisabled), ini_file);
	GetPrivateProfileString(szAppName,"WebRoot","c:\\",g_WebRoot,sizeof(g_WebRoot), ini_file);
	GetPrivateProfileString(szAppName,"Port","80",g_Port,sizeof(g_Port), ini_file);
	GetPrivateProfileString(szAppName,"Playlistfile","",g_Playlistfile,sizeof(g_Playlistfile), ini_file);
	GetPrivateProfileString(szAppName,"AdminPassword","",g_AdminPassword,sizeof(g_AdminPassword), ini_file);
	GetPrivateProfileString(szAppName,"g_MaxRequestsPerIP","3",g_MaxRequestsPerIP,sizeof(g_MaxRequestsPerIP), ini_file);
	GetPrivateProfileString(szAppName,"g_MaxEntries","25",g_MaxEntries,sizeof(g_MaxEntries), ini_file);
	GetPrivateProfileString(szAppName,"g_MaxQueueSize","50",g_MaxQueueSize,sizeof(g_MaxQueueSize), ini_file);

	g_iPort = atoi(g_Port);

	if (!strcmp(strDisabled, "FALSE")) {
		bDisabled = FALSE;
	}
	else {
		bDisabled = TRUE;
	}

	
}


/////////////////////////////////////////
//
// CONFIG_WRITE
//
void config_write()
{
	char ini_file[MAX_PATH], *p;
	GetModuleFileName(plugin.hDllInstance,ini_file,sizeof(ini_file));
	p=ini_file+lstrlen(ini_file);
	while (p >= ini_file && *p != '\\') p--;
	if (++p >= ini_file) *p = 0;
	lstrcat(ini_file,"plugin.ini");

	if (bDisabled) {
		WritePrivateProfileString(szAppName,"bDisabled","TRUE", ini_file);
	}
	else {
		WritePrivateProfileString(szAppName,"bDisabled","FALSE", ini_file);
	}
	WritePrivateProfileString(szAppName,"WebRoot", g_WebRoot, ini_file);
	WritePrivateProfileString(szAppName,"Port", g_Port, ini_file);
	WritePrivateProfileString(szAppName,"Playlistfile", g_Playlistfile, ini_file);
	WritePrivateProfileString(szAppName,"AdminPassword", g_AdminPassword, ini_file);
	WritePrivateProfileString(szAppName,"g_MaxRequestsPerIP",g_MaxRequestsPerIP, ini_file);
	WritePrivateProfileString(szAppName,"g_MaxEntries",g_MaxEntries, ini_file);
	WritePrivateProfileString(szAppName,"g_MaxQueueSize",g_MaxQueueSize, ini_file);


}
