// Eca.cpp: implementation of the CEca class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "ECARunner.h"

#include <afxinet.h>
#include <afxtempl.h>
#include "Eca.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//extern CInternetSession *inet;

int g_numSongs;

extern char	FTPUsername[1024];
extern char	FTPPassword[1024];
extern char	FTPHost[1024];
extern char	RemoteFTP[1024];
extern char	FTPLocalFile[1024];
extern char	FTPRemoteFile[1024];

extern	CString	lastURLResponse;

extern char	TemplateOut[1024];
extern char	TemplateIn[1024];
CString g_CurrentTime;
BOOL g_DisableFTPErrors;

CEca::CEca()
{

	
}

CEca::~CEca()
{

}

CArray<CString, CString &> g_songs_arr;
CArray<CString, CString &> g_songcache;






void CEca::readSongCache(char *newSong)
{

	CStdioFile	file;
	CString		buffer;
	int			ret = 0;
	char* pszFilename = "c:\\songs.txt";
	CString		*strnewSong;
	int j = 0;
	
	strnewSong= new CString(newSong);


	CFileException fileException;


	g_songcache.RemoveAll();

	if (file.Open(pszFilename, CFile::modeRead, &fileException)) {
		while (file.ReadString(buffer)) {
			g_songcache.Add(buffer);
		}
		file.Close();
		CString	check1;

		int size = g_songcache.GetSize();

		if (size != 0) {
			check1 = g_songcache.GetAt(g_songcache.GetSize() -1);
		}
		if (*strnewSong != check1) {
			g_songcache.Add(*strnewSong);
		}

		if (g_songcache.GetSize() > 50) {
			j = g_songcache.GetSize() - 50;
		}
		else {
			j = 0;
		}
	}
	else {
		g_songcache.Add(*strnewSong);
	}
	if (file.Open(pszFilename, CFile::modeCreate | CFile::modeWrite, &fileException)) {
		for (int i=j;i<g_songcache.GetSize();i++) {
			file.WriteString(LPCTSTR(g_songcache.GetAt(i)));
			file.WriteString("\n");
		}
	}
	else {
		char	msg[255] = "";
		sprintf(msg, "Can't open file %s, error = %u\n",pszFilename, fileException.m_cause );
		MessageBox(NULL, msg, "Error", MB_OK);
	}
	file.Close();
	
}

#define MAX_SONGS 1024

char	g_songs[MAX_SONGS][255];

void CEca::substituteHTML(char *TemplateIn, char *TemplateOut, char *nextSong)
{
	CStdioFile	file;
	CStdioFile	filewrite;
	CString		buffer;
	int			ret = 0;
	char* pszFilename = "template.html";
//	char* pszFilenameOut = "output.html";
	char		tempbuf[25] = "";

	CFileException fileException;
	CTime	currentTime = CTime::GetCurrentTime();

	g_CurrentTime = currentTime.Format("%I:%M:%S %p %m/%d/%Y");


	if (file.Open(TemplateIn, CFile::modeRead, &fileException)) {
		if (filewrite.Open(TemplateOut, CFile::modeCreate | CFile::modeWrite, &fileException)) {
			while (file.ReadString(buffer)) {
				buffer.Replace("%%LISTENERS%%", LPCTSTR(listeners));
				buffer.Replace("%%TOTALLISTENERS%%", LPCTSTR(totallisteners));
				buffer.Replace("%%CURRENTSONG%%", g_songs[0]);
				buffer.Replace("%%LASTUPDATED%%", LPCTSTR(g_CurrentTime));
				buffer.Replace("%%LASTURLRESPONSE%%", LPCTSTR(lastURLResponse));
				if (nextSong != NULL) {
					buffer.Replace("%%NEXTSONG%%", nextSong);
				}
				if (buffer.Find("%%SONG") != -1) {
					for(int i=1;i<g_numSongs;i++) {
						memset(tempbuf, '\000', sizeof(tempbuf));
						sprintf(tempbuf, "%%%%SONG%d%%%%", i);
						buffer.Replace(tempbuf, g_songs[i]);
					}
				}
				filewrite.WriteString(LPCTSTR(buffer));
				filewrite.WriteString("\n");
			}
			filewrite.Close();
		}
		file.Close();
	}

}

void CEca::FTPOutput(char *FTPLocalFile, char *FTPRemoteFile, char *FTPUsername, char *FTPPassword, char *FTPHost)
{
//	CInternetSession	inetSession;
	CFtpConnection	*ftp;
	char	buffer[255] = "";

	CInternetSession* inet = new CInternetSession("Mozilla/4.0 (compatible; MSIE 5.0; Windows 98; DigExt)",1, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE);

	ftp = inet->GetFtpConnection(FTPHost, FTPUsername, FTPPassword, 21, FALSE);
	if (ftp != NULL) {
		ftp->PutFile(FTPLocalFile, FTPRemoteFile, FTP_TRANSFER_TYPE_ASCII, 1);
		ftp->Close();
	}
	inet->Close();
	delete inet;

}

void CEca::LoadSongs()
{

	FILE	*filep;
	char	buffer[1024] = "";
	int		bytes_read = 1;
	int		numSongs = 0;
	char	songs[MAX_SONGS][255];

	memset(songs, '\000', sizeof(songs));
	numSongs = 0;

	filep = fopen("c:\\songs.txt", "r");
	if (filep == 0) {
		Sleep(2000);
		filep = fopen("c:\\songs.txt", "r");
	}
	if (filep != 0) {
		memset(buffer, '\000', sizeof(buffer));
		while (fgets(buffer, sizeof(buffer), filep) != NULL) {
				buffer[strlen(buffer)-1] = '\000';
				strcpy(songs[numSongs], buffer);
				numSongs++;
				if (numSongs > 50) {
					break;
				}
				memset(buffer, '\000', sizeof(buffer));
		}
		fclose(filep);
	}
	memset(g_songs, '\000', sizeof(g_songs));
	int j = 0;
	for (int i=numSongs-1;i>=0;i--) {
		strcpy(g_songs[j], songs[i]);
		j++;
		if (j > 50) {
			numSongs = j;
			break;
		}
	}
	g_numSongs = numSongs;

}

void CEca::getLive365Listeners(char *streamID)
{
//	CInternetSession	*inetSession;
	CStdioFile	*file;
	char	buffer[255] = "";
	CString	readbuffer;

	int		error = 0;
	int		start = 0;
	int		end = 0;

//	inetSession = new CInternetSession("Mozilla/4.0 (compatible; MSIE 5.0; Windows 98; DigExt)",1, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE);

	sprintf(buffer, "http://www.live365.com/cgi-bin/ratings.cgi?stream=%s", streamID);

	CInternetSession* inet = new CInternetSession("Mozilla/4.0 (compatible; MSIE 5.0; Windows 98; DigExt)",1, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE);

	file = inet->OpenURL(buffer, 1, INTERNET_FLAG_TRANSFER_ASCII, NULL, 0);


	while (file->ReadString(readbuffer)) {
		if (readbuffer.Find("var gStreamListeners") != -1) {
			start = readbuffer.Find("=");
			if (start == -1) {
				error = 1;
				break;
			}
			start = start + 3;
			end = readbuffer.Find("\"", start);
			if (end == -1) {
				error = 1;
				break;
			}
			listeners = readbuffer.Mid(start, end-start);
		}
		if (readbuffer.Find("var gStreamCapacity") != -1) {
			start = readbuffer.Find("=");
			if (start == -1) {
				error = 1;
				break;
			}
			start = start + 3;
			end = readbuffer.Find("\"", start);
			if (end == -1) {
				error = 1;
				break;
			}
			totallisteners = readbuffer.Mid(start, end-start);
		}

	}
	inet->Close();
	delete inet;
}
