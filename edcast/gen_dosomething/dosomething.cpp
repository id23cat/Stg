/*
Oddsock - DoSomething
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
#include <time.h>

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
#include "Eca.h"
#include "stdafx.h"
#include "Socket.h"
#include <process.h>
#include "doXML.h"
#include <io.h>
#define RECORD_SEPARATOR "__oddsock__"

#include "StatusDialog.h"

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

#define RUN_COMMAND "Run A Command"
#define SUBMIT_URL "Submit A URL"
#define FTP_FILE "FTP A File"
#define GENERATE_HTML "Generate HTML Playlist"
#define XMLURL "Gather XML Statistics"
/////////////////////////////////////////
//
// VARIABLES
//
BOOL bWindowOpen  = FALSE;
BOOL bRunning     = FALSE;
BOOL bLoggedIn    = FALSE;
BOOL bDisplayIcon = TRUE;
BOOL bQuitting    = FALSE;
int uploads       = 0;
HICON Icon0;

HWND	g_hwndDlg;

char	szSongTitle[1024];
char	szArtist[1024];
char	szAlbum[1024];
char	szComment[1024];

#define WM_WA_MPEG_EOF (WM_USER+2)

CStatusDialog	gStatus;
// Global Variables
void *lpWndProcOldPE;

// STRINGS
char		RunCommand[1024];
char		TemplateIn[1024];
char		TemplateOut[1024];
char		URL[1024];
char		FTPLocalFile[1024];
char		FTPRemoteFile[1024];
char		FTPUsername[1024];
char		FTPPassword[1024];
char		FTPHost[1024];

char		gCurrentSong[1024];
//CMutex	*mutex;

CArray<CString, CString &> Command_arr;
CArray<CString, CString &> TemplateIn_arr;
CArray<CString, CString &> TemplateOut_arr;
CArray<CString, CString &> URL_arr;
CArray<CString, CString &> URLOUT_arr;
CArray<CString, CString &> LocalFile_arr;
CArray<CString, CString &> RemoteFile_arr;
CArray<CString, CString &> Username_arr;
CArray<CString, CString &> Password_arr;
CArray<CString, CString &> Host_arr;
CArray<CString, CString &> StreamID_arr;
CArray<CString, CString &> XMLURL_arr;
CArray<CString, CString &> Actions_arr;

typedef struct tag_Element {
	CString		value;
	long		time;
} MP3Element;

CArray<MP3Element, MP3Element &> DO_RecentArtists;
CArray<MP3Element, MP3Element &> DO_RecentAlbums;
CArray<MP3Element, MP3Element &> DO_RecentSongs;

typedef struct tag_XMLElement {
	CString		tag;
	CString		value;
	int			counter;
} XMLElement;

CArray<XMLElement, XMLElement &> xmlTags;


int			gTimerHandle = 0;

char		g_TmpDir[1024];

int			numActions;

CString		listenerCount;
CString		totallistenerCount;


BOOL		bRunCommand;
BOOL		bGenerateHTML;
BOOL		bSubmitURL;
BOOL		bFTPFile;
BOOL		bDisabled;
BOOL		bIsrunning = FALSE;
BOOL		bShowStatus;
BOOL		bEnableErrors = FALSE;
BOOL		bGeneratedHTMLFlag = FALSE;
BOOL		bGatherID3Info = TRUE;
BOOL		bDisableWinampStopped = TRUE;

char	g_XMLURL[1024] = "";

int		g_TimedTrigger = 0;

HWND	g_StatusDialog;
char	StatusText[1024] = "";

typedef struct tag_SongInfo {
	CString	Title;
	CString	Artist;
	CString	Album;
} SongInfo;

// OTHER STUFF

CFtpConnection* ftp                 = NULL;
char szAppName[]                    = "Do Something";
char* DO_pszCurrentSong                = NULL;
char* DO_pszNextSong                = NULL;
char DO_szCurrentSong[1024] = "";
char szNextSong[1024] = "";
BOOL	bStopCheckXX;
BOOL	bStatusDisplayed;
CMySocket					*mySocket = NULL;

VOID CALLBACK SongChangeCheck(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);
int ProcessSongChange();
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

void HideCommandControls();
void ShowCommandControls();
void HideFTPControls();
void ShowFTPControls();
void HideHTMLControls();
void ShowHTMLControls();
void HideURLControls();
void ShowURLControls();
void HideXMLURLControls();
void ShowXMLURLControls();


CEca	eca;
extern void TagReplace(CString *, char *, char *, char *, char *, char *);



int GetSongLength(char	*songLen)
{
	int mode;

	mode = 1; // Song Length in seconds
	int songLength = SendMessage(plugin.hwndParent,WM_WA_IPC,mode,IPC_GETOUTPUTTIME);

	char	hours[3] = "";
	char	minutes[3] = "";
	char	seconds[3] = "";

	int		h = 0;
	int		m = 0;
	int		s = 0;

	h = (int)songLength / 3600;
	m = ((int)songLength / 60) - (h*60);
	s = (int)songLength - (h*3600) - (m*60);
	if (h != 0) {
		sprintf(songLen, "%d:%02d:%02d", h, m, s);
	}
	else {
		sprintf(songLen, "%d:%02d", m, s);
	}
	return songLength;
}


char	g_Status[1024] = "";

void SetStatus(char *buffer)
{
	if (bShowStatus) {
		strcpy(g_Status, buffer);
//		SendMessage(g_StatusDialog,WM_SETTEXT, 0, 0);
		gStatus.m_Status = g_Status;
		if (gStatus.IsWindowEnabled()) {
			gStatus.SetDlgItemText(IDC_STATUS, gStatus.m_Status);
//			gStatus.UpdateData(FALSE);
		}
//		SetDlgItemText(g_StatusDialog, IDC_STATUS, (char *)g_Status);
	}
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
	HideFTPControls();
	HideCommandControls();
	HideHTMLControls();
	HideURLControls();
	HideXMLURLControls();
}

void HideCommandControls()
{
	HideControl(IDC_COMMAND);
	HideControl(IDC_STATIC_COMMAND1);
}
void ShowCommandControls()
{
	ShowControl(IDC_COMMAND);
	ShowControl(IDC_STATIC_COMMAND1);
	HideFTPControls();
	HideHTMLControls();
	HideURLControls();
}
void ZeroFTPControls()
{
	SetDlgItemText(g_hwndDlg, IDC_LOCALFTP, (char *)"");
	SetDlgItemText(g_hwndDlg, IDC_REMOTEFTP, (char *)"");
	SetDlgItemText(g_hwndDlg, IDC_FTPHOST, (char *)"");
	SetDlgItemText(g_hwndDlg, IDC_USERNAME, (char *)"");
	SetDlgItemText(g_hwndDlg, IDC_PASSWORD, (char *)"");
}
void ZeroCommandControls()
{
	SetDlgItemText(g_hwndDlg, IDC_COMMAND, (char *)"");
}
void ZeroXMLURLControls()
{
	SetDlgItemText(g_hwndDlg, IDC_XMLURL, (char *)"");
}
void ZeroHTMLControls()
{
	SetDlgItemText(g_hwndDlg, IDC_TEMPLATEIN, (char *)"");
	SetDlgItemText(g_hwndDlg, IDC_TEMPLATEOUT, (char *)"");
}
void ZeroURLControls()
{
	SetDlgItemText(g_hwndDlg, IDC_URL, (char *)"");
	SetDlgItemText(g_hwndDlg, IDC_URLOUT, (char *)"");
}

void HideXMLURLControls()
{
	HideControl(IDC_STATIC_XMLURL);
	HideControl(IDC_XMLURL);

}
void ShowXMLURLControls()
{
	ShowControl(IDC_STATIC_XMLURL);
	ShowControl(IDC_XMLURL);
	HideCommandControls();
	HideHTMLControls();
	HideURLControls();
	HideFTPControls();
}

void HideFTPControls()
{
	HideControl(IDC_USERNAME);
	HideControl(IDC_PASSWORD);
	HideControl(IDC_LOCALFTP);
	HideControl(IDC_REMOTEFTP);
	HideControl(IDC_FTPHOST);
	HideControl(IDC_STATIC_FTP1);
	HideControl(IDC_STATIC_FTP2);
	HideControl(IDC_STATIC_FTP3);
	HideControl(IDC_STATIC_FTP4);
	HideControl(IDC_STATIC_FTP5);

}
void ShowFTPControls()
{
	ShowControl(IDC_USERNAME);
	ShowControl(IDC_PASSWORD);
	ShowControl(IDC_LOCALFTP);
	ShowControl(IDC_REMOTEFTP);
	ShowControl(IDC_FTPHOST);
	ShowControl(IDC_STATIC_FTP1);
	ShowControl(IDC_STATIC_FTP2);
	ShowControl(IDC_STATIC_FTP3);
	ShowControl(IDC_STATIC_FTP4);
	ShowControl(IDC_STATIC_FTP5);
	HideCommandControls();
	HideHTMLControls();
	HideURLControls();
	HideXMLURLControls();

}
void HideHTMLControls()
{
	HideControl(IDC_TEMPLATEIN);
	HideControl(IDC_TEMPLATEOUT);
	HideControl(IDC_STATIC_HTML1);
	HideControl(IDC_STATIC_HTML2);
}
void ShowHTMLControls()
{
	ShowControl(IDC_TEMPLATEIN);
	ShowControl(IDC_TEMPLATEOUT);
	ShowControl(IDC_STATIC_HTML1);
	ShowControl(IDC_STATIC_HTML2);
	HideCommandControls();
	HideFTPControls();
	HideURLControls();
	HideXMLURLControls();
}
void HideURLControls()
{
	HideControl(IDC_URL);
	HideControl(IDC_STATIC_URL1);
	HideControl(IDC_URLOUT);
	HideControl(IDC_STATIC_URLOUT);
}
void ShowURLControls()
{
	ShowControl(IDC_URL);
	ShowControl(IDC_STATIC_URL1);
	ShowControl(IDC_URLOUT);
	ShowControl(IDC_STATIC_URLOUT);
	HideCommandControls();
	HideFTPControls();
	HideHTMLControls();
	HideXMLURLControls();
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

void DoRunCommand(char *RunCommand)
{
	system(RunCommand);
}


void DoGenHTML(char	*newSong, char *nextSong, char *templateIn, char *templateOut)
{
	CString newSongEntry;
	char	songLen[25];

	

	memset(songLen, '\000', sizeof(songLen));
	GetSongLength(songLen);
	
	newSongEntry = newSong;
	newSongEntry += RECORD_SEPARATOR;
	newSongEntry += szSongTitle;
	newSongEntry += RECORD_SEPARATOR;
	newSongEntry += szArtist;
	newSongEntry += RECORD_SEPARATOR;
	newSongEntry += szAlbum;
	newSongEntry += RECORD_SEPARATOR;
	newSongEntry += szComment;
	newSongEntry += RECORD_SEPARATOR;
	newSongEntry += songLen;

	if (!bGeneratedHTMLFlag) {
		bGeneratedHTMLFlag = TRUE;
		if ((strcmp(DO_szCurrentSong, "Winamp Stopped!")) &&
			(strcmp(DO_szCurrentSong, "Broadcast Stopped!"))) {
			eca.readSongCache(newSongEntry.GetBuffer(newSongEntry.GetLength()));
		}
	}
	eca.listeners = listenerCount;
	eca.totallisteners = totallistenerCount;
	strcpy(eca.szCurrentSong, newSong);
	strcpy(eca.szCurrentArtist, szArtist);
	strcpy(eca.szCurrentAlbum, szAlbum);
	strcpy(eca.szCurrentComment, szComment);
	eca.substituteHTML(templateIn, templateOut, nextSong);
}

CString	lastURLResponse;


void URLize(char *input, char *output)
{
	CString tempString;

	tempString = input;

	tempString.Replace("%", "%25");
	tempString.Replace(";", "%3B");
	tempString.Replace("/", "%2F");
	tempString.Replace("?", "%3F");
	tempString.Replace(":", "%3A");
	tempString.Replace("@", "%40");
	tempString.Replace("&", "%26");
	tempString.Replace("=", "%3D");
	tempString.Replace("+", "%2B");
	tempString.Replace(" ", "%20");
	tempString.Replace("\"", "%22");
	tempString.Replace("#", "%23");
	
	tempString.Replace("<", "%3C");
	tempString.Replace(">", "%3E");
	tempString.Replace("!", "%21");
	tempString.Replace("*", "%2A");
	tempString.Replace("'", "%27");
	tempString.Replace("(", "%28");
	tempString.Replace(")", "%29");
	tempString.Replace(",", "%2C");

	strcpy(output, tempString.GetBuffer(tempString.GetLength()));

}
void DoSubmitURL(char	*Song, char *nextSong, char *URL, char *URLOUT)
{
	CString tempString;
	CString tempNextString;
	CString tempURL;
	CString	buffer;
	CStdioFile	*filep;
	char	tempSong[1024] = "";
	char	tempNextSong[1024] = "";
	char	tempBuf[1024] = "";

	tempURL = URL;

	TagReplace(&tempURL, nextSong, Song, szAlbum, szArtist, szComment);

	FILE *myFile;
	CFileException fileException;

	lastURLResponse.Empty();

	if (strlen(URLOUT) == 0) {

			CInternetSession* inet = new CInternetSession("Mozilla/4.0 (compatible; MSIE 5.0; Windows 98; DigExt)",1, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE);
			filep = inet->OpenURL(tempURL.GetBuffer(tempURL.GetLength()), 1, INTERNET_FLAG_TRANSFER_ASCII |INTERNET_FLAG_RELOAD, NULL, 0);
			if (filep != 0) {
				while (filep->ReadString(buffer)) {
					lastURLResponse += buffer;
				}
			}
			filep->Close();
			inet->Close();
			delete inet;
	}
	else {
		myFile = fopen(URLOUT, "w");
		if (myFile == NULL) {
			char	buf[1024] = "";
			sprintf(buf, "Error Opening File (%s) for Write : Error (%s)", URLOUT, strerror(errno));
			MessageBox(NULL, buf, "Error Message", MB_OK);
		}
		else {
			CStdioFile	*outputFile = new CStdioFile(myFile);

			CInternetSession* inet = new CInternetSession("Mozilla/4.0 (compatible; MSIE 5.0; Windows 98; DigExt)",1, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE);
			filep = inet->OpenURL(tempURL.GetBuffer(tempURL.GetLength()), 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD, NULL, 0);


			if (filep != 0) {
				while (filep->ReadString(buffer)) {
					fputs(buffer, myFile);
					lastURLResponse += buffer;
				}
				fclose(myFile);
			}


			filep->Close();
			inet->Close();
			delete outputFile;
			delete inet;
		}
	}
}  

int CALLBACK StatusWindow(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg) 
	{
		
	case WM_INITDIALOG:
		SetDlgItemText(hwndDlg, IDC_STATUS, g_Status);
		g_StatusDialog = hwndDlg;
		break;
		
	case 666:
		EndDialog(hwndDlg, 0);
		break;

	case WM_SETTEXT:
		SetDlgItemText(hwndDlg, IDC_STATUS, (char *)g_Status);
		break;
	case WM_COMMAND:
		
		int	bleh = LOWORD(wParam);
		int bleh2 = HIWORD(wParam);
		switch (LOWORD(wParam))  
		{

		case IDC_DISMISS:
			EndDialog(hwndDlg, 0);
			break;
		}
		break;
	}
	
	return FALSE;
}

UINT ShowStatusDialog(LPVOID pParam)
{
	if (bShowStatus) {
		gStatus.DoModal();
	}
	AfxEndThread(0);
	return NULL;
}

typedef struct ID3v1 {
	char	tag[3];
	char	songtitle[30];
	char	artist[30];
	char	album[30];
	char	year[4];
	char	comment[30];
	char	genre;
} ID3V1;

void trimspaces(char *buffer)
{
	for(int i=strlen(buffer)-1;i>0;i--) {
		if (buffer[i] == ' ') {
			buffer[i] = '\000';
		}
		else {
			break;
		}
	}
}

void GetID3Info()
{
	int ok = 0;
	ID3V1	v1Tag;

	memset(&v1Tag, '\000', sizeof(v1Tag));

	memset(szSongTitle, '\000', sizeof(szSongTitle));
	memset(szArtist, '\000', sizeof(szArtist));
	memset(szAlbum, '\000', sizeof(szAlbum));
	memset(szComment, '\000', sizeof(szComment));

	if (bGatherID3Info) {
		int currentPos = SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_GETLISTPOS);
		char *pMp3file = (char *)SendMessage(plugin.hwndParent,WM_WA_IPC,currentPos,IPC_GETPLAYLISTFILE);
		if (pMp3file != 0) {
			FILE	*filep;
			filep = fopen(pMp3file, "rb");
			if (filep) {
				int filesize = _filelength(filep->_file);
				int seekret = fseek(filep, filesize-128, SEEK_SET);
				if (seekret == 0) {
					int ret = fread(&v1Tag, 1, sizeof(v1Tag), filep);
					if (ret == 128) {
						if (!strncmp(v1Tag.tag, "TAG", 3)) {
							strncpy(szSongTitle, v1Tag.songtitle, sizeof(v1Tag.songtitle));
							trimspaces(szSongTitle);
							strncpy(szArtist, v1Tag.artist, sizeof(v1Tag.artist));
							trimspaces(szArtist);
							strncpy(szAlbum, v1Tag.album, sizeof(v1Tag.album));
							trimspaces(szAlbum);
							strncpy(szComment, v1Tag.comment, sizeof(v1Tag.comment));
							trimspaces(szComment);
						}
					}
				}
				fclose(filep);
			}
		}
	}
}



void GetSongInfo(int playlistpos, SongInfo *songInfo)
{
	BOOL	songInfoGotten;
	songInfoGotten = FALSE;


	if (bGatherID3Info) {
		GetID3Info();
		if ((strlen(szSongTitle) == 0) && (strlen(szArtist) == 0) && (strlen(szAlbum) == 0)) {
			songInfoGotten = FALSE;
		}
		else {
			songInfo->Title = szSongTitle;
			songInfo->Artist = szArtist;
			songInfo->Album = szAlbum;
			songInfoGotten = TRUE;
		}
	}
	if (!songInfoGotten) {
		char	*DO_pszCurrentSong;
		DO_pszCurrentSong = (char*)SendMessage(plugin.hwndParent,WM_WA_IPC,playlistpos,IPC_GETPLAYLISTTITLE);
		if (DO_pszCurrentSong != NULL) {
			char	*p1;
			char	Artist[255] = "";
			char	Title[255] = "";
			p1 = strchr(DO_pszCurrentSong, '-');
			if (p1 != NULL) {
				memset(Artist, '\000', sizeof(Artist));
				strncpy(Artist, DO_pszCurrentSong, p1-DO_pszCurrentSong);
				p1++;
				memset(Title, '\000', sizeof(Title));
				strcpy(Title, p1);
				trimspaces(Title);
				trimspaces(Artist);
				songInfo->Title = Title;
				songInfo->Artist = Artist;
				songInfo->Album = "";
			}
			else {
				songInfo->Title = "";
				songInfo->Artist = "";
				songInfo->Album = "";
			}
		}
	}
}

int gXMLSeqNumber = 0;

int	gInRunDefinedActions = 0;


void t_RunDefinedActions(char	 *pszCurrentSong)
{
		char	pszNextSong[255] = "";

		gInRunDefinedActions = 1;
		CString	action;
		bGeneratedHTMLFlag = FALSE;
		gXMLSeqNumber = 0;
		xmlTags.RemoveAll();

		if (bShowStatus) {
			AfxBeginThread(ShowStatusDialog, "");
			Sleep(500);
		}

		for (int i=0;i<Actions_arr.GetSize();i++) {
			action = Actions_arr.GetAt(i);
			if (action.Compare(XMLURL) == 0) {
				TRY {
					SetStatus("Getting XML Statistics");
					CDoXML	xml;
					xml.mSeqNumber = gXMLSeqNumber;
					xml.start(XMLURL_arr.GetAt(i).GetBuffer(XMLURL_arr.GetAt(i).GetLength()));
					gXMLSeqNumber++;
				}
				CATCH_ALL(e) {
					if (bEnableErrors) {
						MessageBox(NULL, "Error Getting XML Statistics", "Error Message", MB_OK);
					}
				}
				END_CATCH_ALL
			}

			if (action.Compare(GENERATE_HTML) == 0) {
				TRY {
					SetStatus("Generating HTML");
					GetID3Info();
					DoGenHTML(pszCurrentSong, pszNextSong,
							TemplateIn_arr.GetAt(i).GetBuffer(TemplateIn_arr.GetAt(i).GetLength()),
							TemplateOut_arr.GetAt(i).GetBuffer(TemplateOut_arr.GetAt(i).GetLength()));
				}
				CATCH_ALL(e) {
					if (bEnableErrors) {
						MessageBox(NULL, "Error Generating HTML", "Error Message", MB_OK);
					}
				}
				END_CATCH_ALL
			}
			if (action.Compare(FTP_FILE) == 0) {
				TRY {
					CEca	eca;

					SetStatus("FTPing File");

		//							inet = new CInternetSession("Mozilla/4.0 (compatible; MSIE 5.0; Windows 98; DigExt)",1, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE);
					eca.FTPOutput(LocalFile_arr.GetAt(i).GetBuffer(LocalFile_arr.GetAt(i).GetLength()),
							RemoteFile_arr.GetAt(i).GetBuffer(RemoteFile_arr.GetAt(i).GetLength()),
							Username_arr.GetAt(i).GetBuffer(Username_arr.GetAt(i).GetLength()),
							Password_arr.GetAt(i).GetBuffer(Password_arr.GetAt(i).GetLength()),
							Host_arr.GetAt(i).GetBuffer(Host_arr.GetAt(i).GetLength()));
				}
				CATCH_ALL(e) {
					if (bEnableErrors) {
						MessageBox(NULL, "Error FTPing File", "Error Message", MB_OK);
					}
				}
				END_CATCH_ALL
			}
			if (action.Compare(SUBMIT_URL) == 0) {
				TRY {
					SetStatus("Submitting URL");
					DoSubmitURL(pszCurrentSong, pszNextSong,
						URL_arr.GetAt(i).GetBuffer(URL_arr.GetAt(i).GetLength()),
						URLOUT_arr.GetAt(i).GetBuffer(URLOUT_arr.GetAt(i).GetLength()));
				}
				CATCH_ALL(e) {
					if (bEnableErrors) {
						MessageBox(NULL, "Error Submitting URL", "Error Message", MB_OK);
					}
				}
				END_CATCH_ALL
			}
			if (action.Compare(RUN_COMMAND) == 0) {
				TRY {
					SetStatus("Running Command");
					DoRunCommand(Command_arr.GetAt(i).GetBuffer(Command_arr.GetAt(i).GetLength()));
				}
				CATCH_ALL(e) {
					if (bEnableErrors) {
						MessageBox(NULL, "Error Running Commmand", "Error Message", MB_OK);
					}
				}
				END_CATCH_ALL
			}
		}
		if (bShowStatus) {
			if (gStatus.IsWindowEnabled()) {
				gStatus.PostMessage(WM_CLOSE, 0, 0);
				
			}
		}
		gInRunDefinedActions = 0;
}

UINT RunDefinedActions(LPVOID pParam)
{
	t_RunDefinedActions((char *)pParam);
	AfxEndThread(0);
	return NULL;
}

bool	inTimerProc = FALSE;
bool	gForceSongChange = FALSE;

void CALLBACK myTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	if (!inTimerProc) {
		inTimerProc = TRUE;
		gForceSongChange = TRUE;
	}
	inTimerProc = FALSE;
}

void startTimer()
{
	KillTimer(plugin.hwndParent, 3);
	SetTimer(plugin.hwndParent, 3, 1000, (TIMERPROC)SongChangeCheck);
	if (gTimerHandle == 0) {
		if (g_TimedTrigger > 0) {
			gTimerHandle = SetTimer(plugin.hwndParent, 0, g_TimedTrigger*1000, (TIMERPROC)myTimerProc);
		}
	}
	else {
		KillTimer(NULL, gTimerHandle);
		if (g_TimedTrigger > 0) {
			gTimerHandle = SetTimer(plugin.hwndParent, 0, g_TimedTrigger*1000, (TIMERPROC)myTimerProc);
		}
	}

}

void getCurrentSongTitle(char *song, char *artist, char *full)
{
	char	songTitle[1024] = "";
	char	songTitle2[1024] = "";
	
	memset(songTitle2, '\000', sizeof(songTitle2));
	if (GetWindowText(plugin.hwndParent, songTitle, sizeof(songTitle)) > 0) {
		strcpy(full, songTitle);
		char	*p1 = strrchr(songTitle, '-');
		if (p1) {
			char *p2 = strchr(songTitle, '.');
			if (p2) {
				p2++;
				char *p3 = strchr(p2, '-');
				if (p3) {
					strncpy(artist, p2, p3-p2);
					p3++;
					if (p1 > p3) {
						strncpy(song, p3, p1-p3);
					}
				}
				else {
					strncpy(songTitle2, p2, p1-p2);
					strcpy(song, songTitle2);
					strcpy(artist, "");
				}
			}
			else {
				char *p3 = strchr(songTitle, '-');
				if (p3) {
					strncpy(artist, songTitle, p3-songTitle);
					p3++;
					if (p1 > p3) {
						strncpy(song, p3, p1-p3);
					}
				}
				else {
					strncpy(songTitle2, songTitle, p1-songTitle);
					strcpy(song, songTitle2);
					strcpy(artist, "");
				}
			}

		}
	}
}

// This timer proc is called to continually check to see if the song
// title has changed.  If it detects a song change, Then it called doUpdateSong()
// Which will update the song in the appropriate server
VOID CALLBACK SongChangeCheck(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{

	SongInfo	songInfo;
	char	buf[255] = "";
	char	buf2[255] = "";

	char	FullTitle[1024] = "";
	char	SongTitle[1024] = "";
	char	Artist[1024] = "";
	char	Song[1024] = "";

	if (bQuitting) {
		return;
	}
	if (SendMessage(plugin.hwndParent, WM_USER, 0, 104)) {
		memset(Artist, '\000', sizeof(Artist));
		memset(SongTitle, '\000', sizeof(SongTitle));
		memset(FullTitle, '\000', sizeof(FullTitle));
		memset(Song, '\000', sizeof(Song));
		getCurrentSongTitle(SongTitle, Artist, FullTitle);
		strcpy(Song, Artist);
		strcat(Song, " - ");
		strcat(Song, SongTitle);
		CString tempSong = Song;
		if (((!strcmp(Song, "[Connecting]")) || (!strcmp(Song, "[Buffer:"))) | (gForceSongChange)) {
			;
		}
		else {
			if (strcmp(Song, gCurrentSong)) {
				strcpy(gCurrentSong, Song);
				int currentPos = SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_GETLISTPOS);
				GetSongInfo(currentPos, &songInfo);
				AfxBeginThread(RunDefinedActions, gCurrentSong);
				
				gForceSongChange = FALSE;
			}
		}
	}
	SetTimer(hwnd, idEvent, 1000, (TIMERPROC)SongChangeCheck);
}





/////////////////////////////////////////
//
// WNDPROC
//
void* lpWndProcOld;
int clicked = 0;
int numcalled = 0;
int isplaying = 0;
/////////////////////////////////////////
//
// INIT
//
int init()
{
	static char c[512];
	char filename[512],*p;
	

	memset(RunCommand, '\000', sizeof(RunCommand));
	memset(TemplateIn, '\000', sizeof(TemplateIn));
	memset(TemplateOut, '\000', sizeof(TemplateOut));
	memset(URL, '\000', sizeof(URL));
	memset(FTPLocalFile, '\000', sizeof(FTPLocalFile));
	memset(FTPRemoteFile, '\000', sizeof(FTPRemoteFile));
	memset(FTPUsername, '\000', sizeof(FTPUsername));
	memset(FTPPassword, '\000', sizeof(FTPPassword));
	memset(FTPHost, '\000', sizeof(FTPHost));
	bRunCommand = FALSE;
	bGenerateHTML = FALSE;
	bSubmitURL = FALSE;
	bFTPFile = FALSE;
	bDisabled = TRUE;
	bShowStatus = FALSE;
	bGatherID3Info = TRUE;
	

	
	GetModuleFileName(plugin.hDllInstance,filename,sizeof(filename));
	p = filename+lstrlen(filename);
	while (p >= filename && *p != '\\') p--;
	wsprintf((plugin.description=c),"DoSomething Plugin (%s)", p+1);
	config_read();
	


	mySocket = new CMySocket();
	mySocket->initWinsockLib();

	AfxWinInit(plugin.hDllInstance, NULL, NULL, SW_SHOW);
		

	if (!bDisabled) {
		startTimer();
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
	bQuitting = TRUE;
	bRunning = FALSE;
	KillTimer(plugin.hwndParent, 3);
	KillTimer(plugin.hwndParent, 0);
	config_write();
	
	if (!bDisabled) {
		bDisabled = TRUE;

		/*
		strcpy(DO_szCurrentSong, "Broadcast Stopped!");
		BOOL bhold;
		bhold = bShowStatus;
		bShowStatus = FALSE;
		AfxBeginThread(RunDefinedActions, "Broadcast Stopped");
		bShowStatus = bhold;
		bDisplayIcon = FALSE;
		*/
		WSACleanup();
		delete mySocket;
	}

}




/////////////////////////////////////////
//
// CONFIGIREPROC
//

#define DEFAULT_LOC 1
#define AFTER_LOC 2
#define AT_LOC 3


#define ADD 1
#define UPDATE 2


int ProcessAction(HWND	hwndDlg, int location, int AddUpdate)
{
	char		tmp[10] = "";
	char	selected[255] = "";
	int		selectedIndex = 0;
	char	tempbuf[1024] = "";
	int		listboxcount = 0;
	int		i;
	CString	tempString;
	CString	tempString1;
	CString	tempString2;
	CString	tempString3;
	CString	tempString4;
	CString	tempString5;
	CString	empty;

	empty = "";

	SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTION_CB), CB_GETLBTEXT, SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTION_CB), CB_GETCURSEL, 0, 0), (LPARAM) selected);

	if (location == DEFAULT_LOC) {
		listboxcount = SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_GETCOUNT, 0,0);
	}
	if (location == AFTER_LOC) {
		listboxcount = SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_GETCURSEL, 0, 0);
		listboxcount += 1;
	}
	if (location == AT_LOC) {
		listboxcount = SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_GETCURSEL, 0, 0);
		
	}


	if (!strcmp(selected, XMLURL)) {
		GetDlgItemText(hwndDlg, IDC_XMLURL, tempbuf,sizeof(tempbuf));
		if (strlen(tempbuf) == 0) {
			MessageBox(NULL, "All fields are required", "Error", MB_OK);
			return 1;
		}

		tempString = tempbuf;

		if (AddUpdate == ADD) {
			XMLURL_arr.InsertAt(listboxcount, tempString);
			tempString = XMLURL;
			Actions_arr.InsertAt(listboxcount, tempString);
			StreamID_arr.InsertAt(listboxcount, empty);
			Command_arr.InsertAt(listboxcount, empty);
			URL_arr.InsertAt(listboxcount, empty);
			URLOUT_arr.InsertAt(listboxcount, empty);
			LocalFile_arr.InsertAt(listboxcount, empty);
			RemoteFile_arr.InsertAt(listboxcount, empty);
			Host_arr.InsertAt(listboxcount, empty);
			Username_arr.InsertAt(listboxcount, empty);
			Password_arr.InsertAt(listboxcount, empty);
			TemplateOut_arr.InsertAt(listboxcount, empty);
			TemplateIn_arr.InsertAt(listboxcount, empty);
		}
		else {
			XMLURL_arr[listboxcount] = tempString;
			tempString = XMLURL;
			Actions_arr[listboxcount] = tempString;
		}

	}
	if (!strcmp(selected, RUN_COMMAND)) {
		GetDlgItemText(hwndDlg, IDC_COMMAND, tempbuf,sizeof(tempbuf));
		if (strlen(tempbuf) == 0) {
			MessageBox(NULL, "All fields are required", "Error", MB_OK);
			return 1;
		}

		tempString = tempbuf;

		if (AddUpdate == ADD) {
			Command_arr.InsertAt(listboxcount, tempString);
			tempString = RUN_COMMAND;
			Actions_arr.InsertAt(listboxcount, tempString);
			StreamID_arr.InsertAt(listboxcount, empty);
			URL_arr.InsertAt(listboxcount, empty);
			URLOUT_arr.InsertAt(listboxcount, empty);
			LocalFile_arr.InsertAt(listboxcount, empty);
			RemoteFile_arr.InsertAt(listboxcount, empty);
			Host_arr.InsertAt(listboxcount, empty);
			Username_arr.InsertAt(listboxcount, empty);
			Password_arr.InsertAt(listboxcount, empty);
			TemplateOut_arr.InsertAt(listboxcount, empty);
			TemplateIn_arr.InsertAt(listboxcount, empty);
			XMLURL_arr.InsertAt(listboxcount, empty);
		}
		else {
			Command_arr[listboxcount] = tempString;
			tempString = RUN_COMMAND;
			Actions_arr[listboxcount] = tempString;
		}

	}
	if (!strcmp(selected, SUBMIT_URL)) {
		GetDlgItemText(hwndDlg, IDC_URL, tempbuf,sizeof(tempbuf));
		if (strlen(tempbuf) == 0) {
			MessageBox(NULL, "All fields are required", "Error", MB_OK);
			return 1;
		}

		tempString = tempbuf;


		if (AddUpdate == ADD) {
			URL_arr.InsertAt(listboxcount, tempString);
		}
		else {
			URL_arr[listboxcount] = tempString;
		}
		GetDlgItemText(hwndDlg, IDC_URLOUT, tempbuf,sizeof(tempbuf));
		tempString = tempbuf;
		if (AddUpdate == ADD) {
			URLOUT_arr.InsertAt(listboxcount, tempString);
		}
		else {
			URLOUT_arr[listboxcount] = tempString;
		}

		tempString = SUBMIT_URL;
		if (AddUpdate == ADD) {
			Actions_arr.InsertAt(listboxcount, tempString);
		}
		else {
			Actions_arr[listboxcount] = tempString;
		}

		if (AddUpdate == ADD) {
			StreamID_arr.InsertAt(listboxcount, empty);
			Command_arr.InsertAt(listboxcount, empty);
			LocalFile_arr.InsertAt(listboxcount, empty);
			RemoteFile_arr.InsertAt(listboxcount, empty);
			Host_arr.InsertAt(listboxcount, empty);
			Username_arr.InsertAt(listboxcount, empty);
			Password_arr.InsertAt(listboxcount, empty);
			TemplateOut_arr.InsertAt(listboxcount, empty);
			TemplateIn_arr.InsertAt(listboxcount, empty);
			XMLURL_arr.InsertAt(listboxcount, empty);
		}
	}
	if (!strcmp(selected, FTP_FILE)) {
		GetDlgItemText(hwndDlg, IDC_LOCALFTP, tempbuf,sizeof(tempbuf));
		if (strlen(tempbuf) == 0) {
			MessageBox(NULL, "All fields are required", "Error", MB_OK);
			return 1;
		}
		tempString1 = tempbuf;
		GetDlgItemText(hwndDlg, IDC_REMOTEFTP, tempbuf,sizeof(tempbuf));
		if (strlen(tempbuf) == 0) {
			MessageBox(NULL, "All fields are required", "Error", MB_OK);
			return 1;
		}
		tempString2 = tempbuf;
		GetDlgItemText(hwndDlg, IDC_FTPHOST, tempbuf,sizeof(tempbuf));
		if (strlen(tempbuf) == 0) {
			MessageBox(NULL, "All fields are required", "Error", MB_OK);
			return 1;
		}
		tempString3 = tempbuf;
		GetDlgItemText(hwndDlg, IDC_USERNAME, tempbuf,sizeof(tempbuf));
		if (strlen(tempbuf) == 0) {
			MessageBox(NULL, "All fields are required", "Error", MB_OK);
			return 1;
		}
		tempString4 = tempbuf;
		GetDlgItemText(hwndDlg, IDC_PASSWORD, tempbuf,sizeof(tempbuf));
		if (strlen(tempbuf) == 0) {
			MessageBox(NULL, "All fields are required", "Error", MB_OK);
			return 1;
		}
		tempString5 = tempbuf;


		if (AddUpdate == ADD) {
			LocalFile_arr.InsertAt(listboxcount, tempString1);
			RemoteFile_arr.InsertAt(listboxcount, tempString2);
			Host_arr.InsertAt(listboxcount, tempString3);
			Username_arr.InsertAt(listboxcount, tempString4);
			Password_arr.InsertAt(listboxcount, tempString5);

			tempString = FTP_FILE;
			Actions_arr.InsertAt(listboxcount, tempString);
			StreamID_arr.InsertAt(listboxcount, empty);
			Command_arr.InsertAt(listboxcount, empty);
			URL_arr.InsertAt(listboxcount, empty);
			URLOUT_arr.InsertAt(listboxcount, empty);
			TemplateOut_arr.InsertAt(listboxcount, empty);
			TemplateIn_arr.InsertAt(listboxcount, empty);
			XMLURL_arr.InsertAt(listboxcount, empty);
		}
		else {
			LocalFile_arr[listboxcount] = tempString1;
			RemoteFile_arr[listboxcount] = tempString2;
			Host_arr[listboxcount] = tempString3;
			Username_arr[listboxcount] = tempString4;
			Password_arr[listboxcount] = tempString5;

			tempString = FTP_FILE;
			Actions_arr[listboxcount] = tempString;
		}
	}
	if (!strcmp(selected, GENERATE_HTML)) {
		GetDlgItemText(hwndDlg, IDC_TEMPLATEIN, tempbuf,sizeof(tempbuf));
		if (strlen(tempbuf) == 0) {
			MessageBox(NULL, "All fields are required", "Error", MB_OK);
			return 1;
		}
		tempString1 = tempbuf;
		GetDlgItemText(hwndDlg, IDC_TEMPLATEOUT, tempbuf,sizeof(tempbuf));
		if (strlen(tempbuf) == 0) {
			MessageBox(NULL, "All fields are required", "Error", MB_OK);
			return 1;
		}
		tempString2 = tempbuf;


		if (AddUpdate == ADD) {
			TemplateIn_arr.InsertAt(listboxcount, tempString1);
			TemplateOut_arr.InsertAt(listboxcount, tempString2);

			tempString = GENERATE_HTML;
			Actions_arr.InsertAt(listboxcount, tempString);
			StreamID_arr.InsertAt(listboxcount, empty);
			Command_arr.InsertAt(listboxcount, empty);
			URL_arr.InsertAt(listboxcount, empty);
			URLOUT_arr.InsertAt(listboxcount, empty);
			LocalFile_arr.InsertAt(listboxcount, empty);
			RemoteFile_arr.InsertAt(listboxcount, empty);
			Host_arr.InsertAt(listboxcount, empty);
			Username_arr.InsertAt(listboxcount, empty);
			Password_arr.InsertAt(listboxcount, empty);
			XMLURL_arr.InsertAt(listboxcount, empty);
		}
		else {
			TemplateIn_arr[listboxcount] = tempString1;
			TemplateOut_arr[listboxcount] = tempString2;
			tempString = GENERATE_HTML;
			Actions_arr[listboxcount] = tempString;
		}
	}
	SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_RESETCONTENT, 0, 0);
	for (i =0;i < Actions_arr.GetSize();i++) {
		SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_ADDSTRING, 0, (LPARAM) Actions_arr.GetAt(i).GetBuffer(Actions_arr.GetAt(i).GetLength()));
	}
	SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_SETCURSEL, listboxcount, 0);

	return 1;
}
int AddAction(HWND	hwndDlg, int location)
{
	return ProcessAction(hwndDlg, location, ADD);
}
int UpdateAction(HWND	hwndDlg, int location)
{
	return ProcessAction(hwndDlg, AT_LOC, UPDATE);
}
void ListClick(HWND hwndDlg)
{
	char	selected[255] = "";
	int		selectedIndex = 0;

	selectedIndex =  SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_GETCURSEL, 0, 0);
	SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_GETTEXT, selectedIndex , (LPARAM) selected);
	if (!strcmp(selected, XMLURL)) {
		ShowXMLURLControls();
		SetDlgItemText(hwndDlg, IDC_XMLURL, XMLURL_arr.GetAt(selectedIndex));			
		SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTION_CB), CB_SELECTSTRING, 0, (LPARAM) XMLURL);
	}
	if (!strcmp(selected, RUN_COMMAND)) {
		ShowCommandControls();
		SetDlgItemText(hwndDlg, IDC_COMMAND, Command_arr.GetAt(selectedIndex));			
		SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTION_CB), CB_SELECTSTRING, 0, (LPARAM) RUN_COMMAND);
	}
	if (!strcmp(selected, SUBMIT_URL)) {
		ShowURLControls();
		SetDlgItemText(hwndDlg, IDC_URL, URL_arr.GetAt(selectedIndex));			
		SetDlgItemText(hwndDlg, IDC_URLOUT, URLOUT_arr.GetAt(selectedIndex));			
		SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTION_CB), CB_SELECTSTRING, 0, (LPARAM) SUBMIT_URL);
	}
	if (!strcmp(selected, FTP_FILE)) {
		ShowFTPControls();
		SetDlgItemText(hwndDlg, IDC_LOCALFTP, LocalFile_arr.GetAt(selectedIndex));			
		SetDlgItemText(hwndDlg, IDC_REMOTEFTP, RemoteFile_arr.GetAt(selectedIndex));			
		SetDlgItemText(hwndDlg, IDC_USERNAME, Username_arr.GetAt(selectedIndex));			
		SetDlgItemText(hwndDlg, IDC_PASSWORD, Password_arr.GetAt(selectedIndex));			
		SetDlgItemText(hwndDlg, IDC_FTPHOST, Host_arr.GetAt(selectedIndex));
		SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTION_CB), CB_SELECTSTRING, 0, (LPARAM) FTP_FILE);
	}
	if (!strcmp(selected, GENERATE_HTML)) {
		ShowHTMLControls();
		SetDlgItemText(hwndDlg, IDC_TEMPLATEIN, TemplateIn_arr.GetAt(selectedIndex));			
		SetDlgItemText(hwndDlg, IDC_TEMPLATEOUT, TemplateOut_arr.GetAt(selectedIndex));
		SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTION_CB), CB_SELECTSTRING, 0, (LPARAM) GENERATE_HTML);
	}
}
int CALLBACK BigWindowHandler(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	char		tmp[10] = "";
	char	selected[255] = "";
	int		selectedIndex = 0;
	char	tempbuf[1024] = "";
	char	cachefile[1024] = "";
	int		listboxcount = 0;
	int		i;
	FILE	*filep;
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
		if (bShowStatus) {
			CheckDlgButton(hwndDlg, IDC_SHOWSTATUS, BST_CHECKED);
		}
		else {
			CheckDlgButton(hwndDlg, IDC_SHOWSTATUS, BST_UNCHECKED);
		}
		if (bEnableErrors) {
			CheckDlgButton(hwndDlg, IDC_ENABLEERRORS, BST_CHECKED);
		}
		else {
			CheckDlgButton(hwndDlg, IDC_ENABLEERRORS, BST_UNCHECKED);
		}
		if (bGatherID3Info) {
			CheckDlgButton(hwndDlg, IDC_GATHERID3INFO, BST_CHECKED);
		}
		else {
			CheckDlgButton(hwndDlg, IDC_GATHERID3INFO, BST_UNCHECKED);
		}
		if (bDisableWinampStopped) {
			CheckDlgButton(hwndDlg, IDC_DISABLE_WINAMP_STOPPED, BST_CHECKED);
		}
		else {
			CheckDlgButton(hwndDlg, IDC_DISABLE_WINAMP_STOPPED, BST_UNCHECKED);
		}

		AddComboItem(IDC_ACTION_CB, RUN_COMMAND);
		AddComboItem(IDC_ACTION_CB, SUBMIT_URL);
		AddComboItem(IDC_ACTION_CB, FTP_FILE);
		AddComboItem(IDC_ACTION_CB, GENERATE_HTML);
		AddComboItem(IDC_ACTION_CB, XMLURL);
		SetDlgItemText(hwndDlg, IDC_TMPDIR, g_TmpDir);
		SetDlgItemText(hwndDlg, IDC_XMLURL, g_XMLURL);
		sprintf(tmp, "%d", g_TimedTrigger);
		SetDlgItemText(hwndDlg, IDC_TIMER_TRIGGER, tmp);

		HideAllControls();

		for (i =0;i < Actions_arr.GetSize();i++) {
			SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_ADDSTRING, 0, (LPARAM) Actions_arr.GetAt(i).GetBuffer(Actions_arr.GetAt(i).GetLength()));
		}
		break;
		
	case WM_COMMAND:
		
		int	bleh = LOWORD(wParam);
		int bleh2 = HIWORD(wParam);
		switch (LOWORD(wParam))  
		{

		case IDC_SAMEARTIST_CK:
			if (IsDlgButtonChecked(hwndDlg, IDC_SAMEARTIST_CK) == BST_CHECKED) {
				EnableControl(IDC_SAMEARTIST_ED);
				EnableControl(IDC_SAMEARTIST_TIME);
			}
			else {
				DisableControl(IDC_SAMEARTIST_ED);
				DisableControl(IDC_SAMEARTIST_TIME);
			}
			break;
		case IDC_SAMEALBUM_CK:
			if (IsDlgButtonChecked(hwndDlg, IDC_SAMEALBUM_CK) == BST_CHECKED) {
				EnableControl(IDC_SAMEALBUM_ED);
				EnableControl(IDC_SAMEALBUM_TIME);
			}
			else {
				DisableControl(IDC_SAMEALBUM_ED);
				DisableControl(IDC_SAMEALBUM_TIME);
			}
			break;
		case IDC_CONSECUTIVEALBUM_CK:
			if (IsDlgButtonChecked(hwndDlg, IDC_CONSECUTIVEALBUM_CK) == BST_CHECKED) {
				EnableControl(IDC_CONSECUTIVEALBUM_ED);
			}
			else {
				DisableControl(IDC_CONSECUTIVEALBUM_ED);
			}
			break;
		case IDC_CONSECUTIVEARTIST_CK:
			if (IsDlgButtonChecked(hwndDlg, IDC_CONSECUTIVEARTIST_CK) == BST_CHECKED) {
				EnableControl(IDC_CONSECUTIVEARTIST_ED);
			}
			else {
				DisableControl(IDC_CONSECUTIVEARTIST_ED);
			}
			break;
		case IDC_CONSECUTIVEALL_CK:
			if (IsDlgButtonChecked(hwndDlg, IDC_CONSECUTIVEALL_CK) == BST_CHECKED) {
				EnableControl(IDC_CONSECUTIVEALL_ED);
			}
			else {
				DisableControl(IDC_CONSECUTIVEALL_ED);
			}
			break;

		case ID_ABOUT:
			MessageBox(NULL, "DoSomething V2.9\nBy: Oddsock\nhttp://www.oddsock.org", "Do Something", MB_OK);
			break;

		case IDC_DELETE:
			selectedIndex = SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_GETCURSEL, 0, 0);
			Command_arr.RemoveAt(selectedIndex, 1);
			TemplateIn_arr.RemoveAt(selectedIndex, 1);
			TemplateOut_arr.RemoveAt(selectedIndex, 1);
			URL_arr.RemoveAt(selectedIndex, 1);
			URLOUT_arr.RemoveAt(selectedIndex, 1);
			LocalFile_arr.RemoveAt(selectedIndex, 1);
			RemoteFile_arr.RemoveAt(selectedIndex, 1);
			Username_arr.RemoveAt(selectedIndex, 1);
			Password_arr.RemoveAt(selectedIndex, 1);
			Host_arr.RemoveAt(selectedIndex, 1);
			StreamID_arr.RemoveAt(selectedIndex, 1);
			XMLURL_arr.RemoveAt(selectedIndex, 1);
			Actions_arr.RemoveAt(selectedIndex, 1);
			SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_RESETCONTENT, 0, 0);
			for (i =0;i < Actions_arr.GetSize();i++) {
				SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_ADDSTRING, 0, (LPARAM) Actions_arr.GetAt(i).GetBuffer(Actions_arr.GetAt(i).GetLength()));
			}

			SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTIONS), LB_SETCURSEL, selectedIndex, 0);
			HideAllControls();
			break;

		case IDC_CLEARCACHE:
			if (g_TmpDir[strlen(g_TmpDir)-1] != '\\') {
				sprintf(cachefile, "%s\\songs.txt", g_TmpDir);
			}
			else {
				sprintf(cachefile, "%s", g_TmpDir);
				strcat(cachefile, "songs.txt");
			}
			filep = fopen(cachefile, "w");
			if (filep != 0) {
				fclose(filep);
			}
			MessageBox(NULL, "All Songs Cleared From the Cache", "Information", MB_OK);
			break;
		case IDC_ADD_ACTION:
			AddAction(hwndDlg, DEFAULT_LOC);
			break;
		case IDC_UPDATE:
			UpdateAction(hwndDlg, AT_LOC);
			break;
		case IDC_INSERT_AFTER:
			AddAction(hwndDlg, AT_LOC);
			break;

		case IDC_ACTIONS:

			switch (HIWORD(wParam)) {
			case LBN_DBLCLK :
				ListClick(hwndDlg);
				break;
			}
			break;

		case IDC_ACTION_CB:

			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE :

				SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTION_CB), CB_GETLBTEXT, SendMessage(GetDlgItem(g_hwndDlg,IDC_ACTION_CB), CB_GETCURSEL, 0, 0), (LPARAM) selected);

				if (!strcmp(selected, XMLURL)) {
					ShowXMLURLControls();
					ZeroXMLURLControls();
					SetDlgItemText(hwndDlg, IDC_XMLURL, "http://shoutserver:port/admin.cgi?pass=PASSWORD&mode=viewxml");			
				}
				if (!strcmp(selected, RUN_COMMAND)) {
					ShowCommandControls();
					ZeroCommandControls();
				}
				if (!strcmp(selected, SUBMIT_URL)) {
					ShowURLControls();
					ZeroURLControls();
				}
				if (!strcmp(selected, FTP_FILE)) {
					ShowFTPControls();
					ZeroFTPControls();
				}
				if (!strcmp(selected, GENERATE_HTML)) {
					ShowHTMLControls();
					ZeroHTMLControls();
				}

				break;
			}
			break;

		case IDOK:
			bWindowOpen = FALSE;
			bRunning = FALSE;
			bLoggedIn = FALSE;
			

			if (IsDlgButtonChecked(hwndDlg, IDC_DISABLE) == BST_CHECKED) {
				bDisabled = TRUE;
			}
			else {
				bDisabled = FALSE;
			}
			if (IsDlgButtonChecked(hwndDlg, IDC_SHOWSTATUS) == BST_CHECKED) {
				bShowStatus = TRUE;
			}
			else {
				bShowStatus = FALSE;
			}
			if (IsDlgButtonChecked(hwndDlg, IDC_ENABLEERRORS) == BST_CHECKED) {
				bEnableErrors = TRUE;
			}
			else {
				bEnableErrors = FALSE;
			}
			if (IsDlgButtonChecked(hwndDlg, IDC_GATHERID3INFO) == BST_CHECKED) {
				bGatherID3Info = TRUE;
			}
			else {
				bGatherID3Info = FALSE;
			}
			if (IsDlgButtonChecked(hwndDlg, IDC_DISABLE_WINAMP_STOPPED) == BST_CHECKED) {
				bDisableWinampStopped = TRUE;
			}
			else {
				bDisableWinampStopped = FALSE;
			}

			GetDlgItemText(hwndDlg, IDC_TMPDIR, g_TmpDir, sizeof(g_TmpDir));

			memset(tmp, '\000', sizeof(tmp));
			GetDlgItemText(hwndDlg, IDC_TIMER_TRIGGER, tmp, sizeof(tmp));
			g_TimedTrigger = atoi(tmp);

			startTimer();

			EndDialog(hwndDlg,0);
			break;

		case ID_QUIT:

			bWindowOpen = FALSE;
			bRunning = FALSE;
			bLoggedIn = FALSE;
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
	char	strRunCommand[25] = "";
	char	strGenerateHTML[25] = "";
	char	strSubmitURL[25] = "";
	char	strFTPFile[25] = "";
	char	strDisabled[25] = "";
	char	strShowStatus[25] = "";
	char	strCommandOrder[25] = "";
	char	strFTPOrder[25] = "";
	char	strHTMLOrder[25] = "";
	char	strURLOrder[25] = "";
	char	strEnableErrors[25] = "";
	char	strGatherID3Info[25] = "";
	char	strDisableWinampStopped[25] = "";
	char	strTimedTrigger[25] = "";

	char ini_file[MAX_PATH], *p;
	GetModuleFileName(plugin.hDllInstance,ini_file,sizeof(ini_file));
	p=ini_file+lstrlen(ini_file);
	while (p >= ini_file && *p != '\\') p--;
	if (++p >= ini_file) *p = 0;
	lstrcat(ini_file,"plugin.ini");
	
	char	tmp[1024] = "";
	CString	tempString;

	GetPrivateProfileString(szAppName,"NumActions","0", tmp, sizeof(tmp), ini_file);
	numActions = atoi(tmp);
	if (numActions > 0) {
		for (int i=0;i<numActions;i++) {
			memset(tmp, '\000', sizeof(tmp));
			char	key[25] = "";


			sprintf(key, "Action%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			Actions_arr.SetAtGrow(i, tempString);
			sprintf(key, "Command%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			Command_arr.SetAtGrow(i, tempString);
			memset(tmp, '\000', sizeof(tmp));
			sprintf(key, "TemplateIn%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			TemplateIn_arr.SetAtGrow(i, tempString);
			memset(tmp, '\000', sizeof(tmp));
			sprintf(key, "TemplateOut%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			TemplateOut_arr.SetAtGrow(i, tempString);
			memset(tmp, '\000', sizeof(tmp));
			sprintf(key, "URL%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			URL_arr.SetAtGrow(i, tempString);
			memset(tmp, '\000', sizeof(tmp));
			sprintf(key, "URLOUT%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			URLOUT_arr.SetAtGrow(i, tempString);
			memset(tmp, '\000', sizeof(tmp));
			sprintf(key, "FTPLocalFile%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			LocalFile_arr.SetAtGrow(i, tempString);
			memset(tmp, '\000', sizeof(tmp));
			sprintf(key, "FTPRemoteFile%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			RemoteFile_arr.SetAtGrow(i, tempString);
			memset(tmp, '\000', sizeof(tmp));
			sprintf(key, "FTPUsername%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			Username_arr.SetAtGrow(i, tempString);
			memset(tmp, '\000', sizeof(tmp));
			sprintf(key, "FTPPassword%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			Password_arr.SetAtGrow(i, tempString);
			memset(tmp, '\000', sizeof(tmp));
			sprintf(key, "FTPHost%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			Host_arr.SetAtGrow(i, tempString);
			memset(tmp, '\000', sizeof(tmp));
			sprintf(key, "StreamID%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			StreamID_arr.SetAtGrow(i, tempString);
			memset(tmp, '\000', sizeof(tmp));
			sprintf(key, "XMLURL%d", i);
			GetPrivateProfileString(szAppName,key,"", tmp, sizeof(tmp), ini_file);
			tempString = tmp;
			XMLURL_arr.SetAtGrow(i, tempString);
		}
	}
	GetPrivateProfileString(szAppName,"bDisabled","TRUE",strDisabled,sizeof(strDisabled), ini_file);
	GetPrivateProfileString(szAppName,"bShowStatus","FALSE",strShowStatus,sizeof(strShowStatus), ini_file);
	GetPrivateProfileString(szAppName,"bEnableErrors","FALSE",strEnableErrors,sizeof(strEnableErrors), ini_file);
	GetPrivateProfileString(szAppName,"TmpDir","c:\\",g_TmpDir,sizeof(g_TmpDir), ini_file);
	GetPrivateProfileString(szAppName,"bGatherID3Info","TRUE",strGatherID3Info,sizeof(strGatherID3Info), ini_file);
	GetPrivateProfileString(szAppName,"bDisableWinampStopped","FALSE",strDisableWinampStopped,sizeof(strDisableWinampStopped), ini_file);

	if (!strcmp(strDisabled, "FALSE")) {
		bDisabled = FALSE;
	}
	else {
		bDisabled = TRUE;
	}
	if (!strcmp(strShowStatus, "FALSE")) {
		bShowStatus = FALSE;
	}
	else {
		bShowStatus = TRUE;
	}
	if (!strcmp(strEnableErrors, "FALSE")) {
		bEnableErrors = FALSE;
	}
	else {
		bEnableErrors = TRUE;
	}
	if (!strcmp(strGatherID3Info, "FALSE")) {
		bGatherID3Info = FALSE;
	}
	else {
		bGatherID3Info = TRUE;
	}
	if (!strcmp(strDisableWinampStopped, "FALSE")) {
		bDisableWinampStopped = FALSE;
	}
	else {
		bDisableWinampStopped = TRUE;
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

	for (int i=0;i<Actions_arr.GetSize();i++) {

		char	key[25] = "";
		sprintf(key, "Action%d", i);
		if (i < Actions_arr.GetSize()) {
			if (! Actions_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,Actions_arr.GetAt(i), ini_file);
			}
		}
			
		sprintf(key, "Command%d", i);
		if (i < Command_arr.GetSize()) {
			if (! Command_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,Command_arr.GetAt(i), ini_file);
			}
		}
		sprintf(key, "TemplateIn%d", i);
		if (i < TemplateIn_arr.GetSize()) {
			if (! TemplateIn_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,TemplateIn_arr.GetAt(i), ini_file);
			}
		}
		sprintf(key, "TemplateOut%d", i);
		if (i < TemplateOut_arr.GetSize()) {
			if (! TemplateOut_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,TemplateOut_arr.GetAt(i), ini_file);
			}
		}
		sprintf(key, "URL%d", i);
		if (i < URL_arr.GetSize()) {
			if (! URL_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,URL_arr.GetAt(i), ini_file);
			}
		}
		sprintf(key, "URLOUT%d", i);
		if (i < URLOUT_arr.GetSize()) {
			if (! URLOUT_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,URLOUT_arr.GetAt(i), ini_file);
			}
		}
		sprintf(key, "FTPLocalFile%d", i);
		if (i < LocalFile_arr.GetSize()) {
			if (! LocalFile_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,LocalFile_arr.GetAt(i), ini_file);
			}
		}
		sprintf(key, "FTPRemoteFile%d", i);
		if (i < RemoteFile_arr.GetSize()) {
			if (! RemoteFile_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,RemoteFile_arr.GetAt(i), ini_file);
			}
		}
		sprintf(key, "FTPUsername%d", i);
		if (i < Username_arr.GetSize()) {
			if (! Username_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,Username_arr.GetAt(i), ini_file);
			}
		}
		sprintf(key, "FTPPassword%d", i);
		if (i < Password_arr.GetSize()) {
			if (! Password_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,Password_arr.GetAt(i), ini_file);
			}
		}
		sprintf(key, "FTPHost%d", i);
		if (i < Host_arr.GetSize()) {
			if (! Host_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,Host_arr.GetAt(i), ini_file);
			}
		}
		sprintf(key, "StreamID%d", i);
		if (i < StreamID_arr.GetSize()) {
			if (! StreamID_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,StreamID_arr.GetAt(i), ini_file);
			}
		}
		sprintf(key, "XMLURL%d", i);
		if (i < XMLURL_arr.GetSize()) {
			if (! XMLURL_arr.GetAt(i).IsEmpty()) {
				WritePrivateProfileString(szAppName,key,XMLURL_arr.GetAt(i), ini_file);
			}
		}
	}
	char	buf[25] = "";
	sprintf(buf, "%d", Actions_arr.GetSize());
	WritePrivateProfileString(szAppName,"NumActions",buf, ini_file);
	
	if (bDisabled) {
		WritePrivateProfileString(szAppName,"bDisabled","TRUE", ini_file);
	}
	else {
		WritePrivateProfileString(szAppName,"bDisabled","FALSE", ini_file);
	}
	if (bShowStatus) {
		WritePrivateProfileString(szAppName,"bShowStatus","TRUE", ini_file);
	}
	else {
		WritePrivateProfileString(szAppName,"bShowStatus","FALSE", ini_file);
	}
	if (bEnableErrors) {
		WritePrivateProfileString(szAppName,"bEnableErrors","TRUE", ini_file);
	}
	else {
		WritePrivateProfileString(szAppName,"bEnableErrors","FALSE", ini_file);
	}
	if (bGatherID3Info) {
		WritePrivateProfileString(szAppName,"bGatherID3Info","TRUE", ini_file);
	}
	else {
		WritePrivateProfileString(szAppName,"bGatherID3Info","FALSE", ini_file);
	}
	if (bDisableWinampStopped) {
		WritePrivateProfileString(szAppName,"bDisableWinampStopped","TRUE", ini_file);
	}
	else {
		WritePrivateProfileString(szAppName,"bDisableWinampStopped","FALSE", ini_file);
	}
	char	tmp[255] = "";

	sprintf(tmp, "%d", g_TimedTrigger);
	WritePrivateProfileString(szAppName,"g_TimedTrigger",tmp, ini_file);

}
