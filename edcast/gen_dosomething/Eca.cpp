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

  oddsock@mcs.com
*/

#include "stdafx.h"

#include <sys/types.h>
#include <sys/stat.h>

#include <afxinet.h>
#include <afxtempl.h>
#include "Eca.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "Socket.h"
#include <winsock2.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int g_numSongs;

extern char	FTPUsername[1024];
extern char	FTPPassword[1024];
extern char	FTPHost[1024];
extern char	RemoteFTP[1024];
extern char	FTPLocalFile[1024];
extern char	FTPRemoteFile[1024];

extern bool gForceSongChange;
extern	CString	lastURLResponse;

extern char	TemplateOut[1024];
extern char	TemplateIn[1024];
extern char	g_TmpDir[1024];
extern char	szSongTitle[1024];
extern char	szArtist[1024];
extern char	szAlbum[1024];
extern char	szComment[1024];
extern CFtpConnection	*ftp;
extern void URLize(char *input, char *output);
//extern char DO_szCurrentSong[1024];

void splitSongInfo(char *buf, char *field1, char *field2, char *field3, char *field4, char *field5, char *field6);


CString g_CurrentTime;
BOOL g_DisableFTPErrors;

typedef struct tag_XMLElement {
	CString		tag;
	CString		value;
	int			counter;
} XMLElement;

CArray<CString, CString &> g_songs_arr;
CArray<CString, CString &> g_songcache;

extern CArray<XMLElement, XMLElement &> xmlTags;

extern int GetSongLength(char *);
extern CEca		eca;

#define RECORD_SEPARATOR "__oddsock__"

void TagReplace(CString *buffer, char *nextSong, char *pszCurrentSong, char *pszCurrentAlbum, char *pszCurrentArtist, char *pszCurrentComment)
{
	int			ret = 0;
	char		tempbuf[25] = "";
	char		szlSong[1024];
	char		szlArtist[1024];
	char		szlAlbum[1024];
	char		szlSongTitle[1024];
	char		szlComment[1024];
	char		szlSongLength[25];
	char	tempStr[1024] = "";
	char		szlTimestamp[30];
	time_t		currentTime = 0;

	time(&currentTime);

	sprintf(szlTimestamp, "%d", currentTime);

	buffer->Replace("%%TIMESTAMP%%", LPCTSTR(szlTimestamp));
	buffer->Replace("%%LISTENERS%%", LPCTSTR(eca.listeners));

	buffer->Replace("%%TOTALLISTENERS%%", LPCTSTR(eca.totallisteners));
	buffer->Replace("%%CURRENTSONG%%", pszCurrentSong);
	memset(tempStr, '\000', sizeof(tempStr));
	URLize(pszCurrentSong, tempStr);
	buffer->Replace("%%URL_CURRENTSONG%%", tempStr);

	buffer->Replace("%%LASTUPDATED%%", LPCTSTR(g_CurrentTime));
	memset(tempStr, '\000', sizeof(tempStr));
	URLize((char *)LPCTSTR(g_CurrentTime), tempStr);
	buffer->Replace("%%URL_LASTUPDATED%%", tempStr);

	buffer->Replace("%%LASTURLRESPONSE%%", LPCTSTR(lastURLResponse));
	memset(tempStr, '\000', sizeof(tempStr));
	URLize((char *)LPCTSTR(lastURLResponse), tempStr);
	buffer->Replace("%%URL_LASTURLRESPONSE%%", tempStr);

	buffer->Replace("%%CURRENTARTIST%%", pszCurrentArtist);
	memset(tempStr, '\000', sizeof(tempStr));
	URLize(pszCurrentArtist, tempStr);
	buffer->Replace("%%URL_CURRENTARTIST%%", tempStr);

	buffer->Replace("%%CURRENTALBUM%%", pszCurrentAlbum);
	memset(tempStr, '\000', sizeof(tempStr));
	URLize(pszCurrentAlbum, tempStr);
	buffer->Replace("%%URL_CURRENTALBUM%%", tempStr);

	buffer->Replace("%%CURRENTSONGTITLE%%", szSongTitle);
	memset(tempStr, '\000', sizeof(tempStr));
	URLize(szSongTitle, tempStr);
	buffer->Replace("%%URL_CURRENTSONGTITLE%%", tempStr);

	buffer->Replace("%%CURRENTCOMMENT%%", pszCurrentComment);
	memset(tempStr, '\000', sizeof(tempStr));
	URLize(pszCurrentComment, tempStr);
	buffer->Replace("%%URL_CURRENTCOMMENT%%", tempStr);

	if (nextSong != NULL) {
		buffer->Replace("%%NEXTSONG%%", nextSong);
		memset(tempStr, '\000', sizeof(tempStr));
		URLize(nextSong, tempStr);
		buffer->Replace("%%URL_NEXTSONG%%", tempStr);
	}
	char	songLen[25];

	memset(songLen, '\000', sizeof(songLen));
	int songLengthSeconds = GetSongLength(songLen);
	char	songLengthSec[25] = "";
	sprintf(songLengthSec, "%d", songLengthSeconds);

	buffer->Replace("%%CURRENTSONGLENGTH%%", songLen);
	buffer->Replace("%%CURRENTSONGLENGTHSECONDS%%", songLengthSec);
	memset(tempStr, '\000', sizeof(tempStr));
	URLize(songLen, tempStr);
	buffer->Replace("%%URL_CURRENTSONGLENGTH%%", tempStr);

	for (int i=1;i<=g_songcache.GetUpperBound();i++) {
		memset(szlSong, '\000', sizeof(szlSong));
		memset(szlArtist, '\000', sizeof(szlArtist));
		memset(szlAlbum, '\000', sizeof(szlAlbum));
		memset(szlSongTitle, '\000', sizeof(szlSongTitle));
		memset(szlComment, '\000', sizeof(szlComment));
		memset(szlSongLength, '\000', sizeof(szlSongLength));
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%SONG%d%%%%", i);
		if (buffer->Find(tempbuf) != -1) {
			splitSongInfo(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), szlSong, szlSongTitle, szlArtist, szlAlbum, szlComment,szlSongLength);
			buffer->Replace(tempbuf, szlSong);
		}
		sprintf(tempbuf, "%%%%URL_SONG%d%%%%", i);
		if (buffer->Find(tempbuf) != -1) {
			splitSongInfo(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), szlSong, szlSongTitle, szlArtist, szlAlbum, szlComment,szlSongLength);
			memset(tempStr, '\000', sizeof(tempStr));
			URLize(szlSong, tempStr);
			buffer->Replace(tempbuf, tempStr);
		}
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%ARTIST%d%%%%", i);
		if (buffer->Find(tempbuf) != -1) {
			splitSongInfo(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), szlSong, szlSongTitle, szlArtist, szlAlbum, szlComment,szlSongLength);
			buffer->Replace(tempbuf, szlArtist);
		}
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%URL_ARTIST%d%%%%", i);
		if (buffer->Find(tempbuf) != -1) {
			splitSongInfo(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), szlSong, szlSongTitle, szlArtist, szlAlbum, szlComment,szlSongLength);
			memset(tempStr, '\000', sizeof(tempStr));
			URLize(szlArtist, tempStr);
			buffer->Replace(tempbuf, tempStr);
		}
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%ALBUM%d%%%%", i);
		if (buffer->Find(tempbuf) != -1) {
			splitSongInfo(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), szlSong, szlSongTitle, szlArtist, szlAlbum, szlComment,szlSongLength);
			buffer->Replace(tempbuf, szlAlbum);
		}
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%URL_ALBUM%d%%%%", i);
		if (buffer->Find(tempbuf) != -1) {
			splitSongInfo(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), szlSong, szlSongTitle, szlArtist, szlAlbum, szlComment,szlSongLength);
			memset(tempStr, '\000', sizeof(tempStr));
			URLize(szlAlbum, tempStr);
			buffer->Replace(tempbuf, tempStr);
		}
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%SONGTITLE%d%%%%", i);
		if (buffer->Find(tempbuf) != -1) {
			splitSongInfo(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), szlSong, szlSongTitle, szlArtist, szlAlbum, szlComment,szlSongLength);
			buffer->Replace(tempbuf, szlSongTitle);
		}
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%URL_SONGTITLE%d%%%%", i);
		if (buffer->Find(tempbuf) != -1) {
			splitSongInfo(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), szlSong, szlSongTitle, szlArtist, szlAlbum, szlComment,szlSongLength);
			memset(tempStr, '\000', sizeof(tempStr));
			URLize(szlSongTitle, tempStr);
			buffer->Replace(tempbuf, tempStr);
		}
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%COMMENT%d%%%%", i);
		if (buffer->Find(tempbuf) != -1) {
			splitSongInfo(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), szlSong, szlSongTitle, szlArtist, szlAlbum, szlComment,szlSongLength);
			buffer->Replace(tempbuf, szlComment);
		}
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%URL_COMMENT%d%%%%", i);
		if (buffer->Find(tempbuf) != -1) {
			splitSongInfo(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), szlSong, szlSongTitle, szlArtist, szlAlbum, szlComment,szlSongLength);
			memset(tempStr, '\000', sizeof(tempStr));
			URLize(szlComment, tempStr);
			buffer->Replace(tempbuf, tempStr);
		}
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%SONGLENGTH%d%%%%", i);
		if (buffer->Find(tempbuf) != -1) {
			splitSongInfo(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), szlSong, szlSongTitle, szlArtist, szlAlbum, szlComment,szlSongLength);
			buffer->Replace(tempbuf, szlSongLength);
		}
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%URL_SONGLENGTH%d%%%%", i);
		if (buffer->Find(tempbuf) != -1) {
			splitSongInfo(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), szlSong, szlSongTitle, szlArtist, szlAlbum, szlComment,szlSongLength);
			memset(tempStr, '\000', sizeof(tempStr));
			URLize(szlSongLength, tempStr);
			buffer->Replace(tempbuf, tempStr);
		}
	}
	for (i=g_songcache.GetUpperBound();i<50;i++) {
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%SONG%d%%%%", i);
		buffer->Replace(tempbuf, "");
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%ARTIST%d%%%%", i);
		buffer->Replace(tempbuf, "");
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%ALBUM%d%%%%", i);
		buffer->Replace(tempbuf, "");
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%SONGTITLE%d%%%%", i);
		buffer->Replace(tempbuf, "");
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%COMMENT%d%%%%", i);
		buffer->Replace(tempbuf, "");
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%SONGLENGTH%d%%%%", i);
		buffer->Replace(tempbuf, "");
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%URL_SONG%d%%%%", i);
		buffer->Replace(tempbuf, "");
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%URL_ARTIST%d%%%%", i);
		buffer->Replace(tempbuf, "");
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%URL_ALBUM%d%%%%", i);
		buffer->Replace(tempbuf, "");
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%URL_SONGTITLE%d%%%%", i);
		buffer->Replace(tempbuf, "");
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%URL_COMMENT%d%%%%", i);
		buffer->Replace(tempbuf, "");
		memset(tempbuf, '\000', sizeof(tempbuf));
		sprintf(tempbuf, "%%%%URL_SONGLENGTH%d%%%%", i);
		buffer->Replace(tempbuf, "");
	}
	if (buffer->Find("%%XML", 0) != -1) {
		for (i=0;i<xmlTags.GetSize();i++) {
			buffer->Replace("%%" + xmlTags.GetAt(i).tag + "%%", xmlTags.GetAt(i).value);
		}
		int find = buffer->Find("%%XML", 0);
		while (find != -1) {
			int ind1 = buffer->Find("%%XML", 0);
			int ind2 = buffer->Find("%%", ind1+1);
			if (ind2 == -1) {
				break;
			}
			ind2 = ind2 + 2;
			CString substring;
			substring = buffer->Mid(ind1, ind2-ind1);
			buffer->Replace(substring, "");
			find = buffer->Find("%%XML", 0);
		}
	}
}


CEca::CEca()
{

	
}

CEca::~CEca()
{

}



void CEca::readSongCache(char *newSong)
{

	CStdioFile	file;
	CString		buffer;
	int			ret = 0;
	CString		strnewSong;
	int j = 0;
	
	char	cachefile[1024] = "";
	if (g_TmpDir[strlen(g_TmpDir)-1] != '\\') {
		sprintf(cachefile, "%s\\songs.txt", g_TmpDir);
	}
	else {
		sprintf(cachefile, "%s", g_TmpDir);
		strcat(cachefile, "songs.txt");
	}

	strnewSong = newSong;


	CFileException fileException;


	g_songcache.RemoveAll();

	if (file.Open(cachefile, CFile::modeRead, &fileException)) {
		while (file.ReadString(buffer)) {
			g_songcache.Add(buffer);
		}
		file.Close();
		CString	check1;

		int size = g_songcache.GetSize();

		if (size != 0) {
			check1 = g_songcache.GetAt(g_songcache.GetUpperBound());
		}
		if (strnewSong != check1) {
			if (!gForceSongChange) {
				if (g_songcache.GetSize() == 0) {
					g_songcache.Add(strnewSong);
				}
				else {
					g_songcache.InsertAt(0,strnewSong);
				}
			}
		}

		if (g_songcache.GetSize() > 50) {
			j = g_songcache.GetSize() - 50;
		}
		else {
			j = 0;
		}
	}
	else {
		if (g_songcache.GetSize() == 0) {
			g_songcache.Add(strnewSong);
		}
		else {
			g_songcache.InsertAt(0,strnewSong);
		}
	}
	if (file.Open(cachefile, CFile::modeCreate | CFile::modeWrite, &fileException)) {
		for (int i=0;i<=g_songcache.GetUpperBound();i++) {
			int skip = 0;
			if (!strncmp(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), "Winamp Stopped!", sizeof("Winamp Stopped!"))) {
				skip = 1;
			}
			if (!strncmp(g_songcache.GetAt(i).GetBuffer(g_songcache.GetAt(i).GetLength()), "Broadcast Stopped!", sizeof("Broadcast Stopped!"))) {
				skip = 1;
			}
			if (!skip) {
				file.WriteString(LPCTSTR(g_songcache.GetAt(i)));
				file.WriteString("\n");
			}
		}
	}
	else {
		char	msg[255] = "";
		sprintf(msg, "Can't open file %s, error = %u\n",cachefile, fileException.m_cause );
		MessageBox(NULL, msg, "Error", MB_OK);
	}
	file.Close();
	
}

#define MAX_SONGS 255

char	g_songs[MAX_SONGS][2046];

void splitSongInfo(char *buf, char *field1, char *field2, char *field3, char *field4, char *field5, char *field6)
{
	char	*p1;
	char	*p2;
	char	*p3;
	char	*p4;
	char	*p5;

	p1 = strstr(buf, RECORD_SEPARATOR);
	if (p1 != 0) {
		strncpy(field1, buf, p1 - buf);
		p1 = p1 + strlen(RECORD_SEPARATOR);
		p2 = strstr(p1, RECORD_SEPARATOR);
		if (p2 != 0) {
			strncpy(field2, p1, p2 - p1);
			p2 = p2 + strlen(RECORD_SEPARATOR);
			p3 = strstr(p2, RECORD_SEPARATOR);
			if (p3 != 0) {
				strncpy(field3, p2, p3 - p2);
				p3 = p3 + strlen(RECORD_SEPARATOR);
				p4 = strstr(p3, RECORD_SEPARATOR);
				if (p4 != 0) {
					strncpy(field4, p3, p4 - p3);
					p4 = p4 + strlen(RECORD_SEPARATOR);
					p5 = strstr(p4, RECORD_SEPARATOR);
					if (p5 != 0) {
						strncpy(field5, p4, p5-p4);
						p5 = p5 + strlen(RECORD_SEPARATOR);
						strcpy(field6, p5);						
					}
					else {
						strcpy(field5, p4);
					}
				}
			}
		}
	}
}
void CEca::substituteHTML(char *TemplateIn, char *TemplateOut, char *nextSong)
{
	CStdioFile	file;
	CStdioFile	filewrite;
	CString		buffer;
	int			ret = 0;
	char* pszFilename = "template.html";
	char		tempbuf[25] = "";
	char		szlSong[1024];
	char		szlArtist[1024];
	char		szlAlbum[1024];
	char		szlSongTitle[1024];
	char		szlComment[1024];
	char		szlSongLength[25];


	CFileException fileException;
	CTime	currentTime = CTime::GetCurrentTime();

	g_CurrentTime = currentTime.Format("%I:%M:%S %p %m/%d/%Y");



	if (file.Open(TemplateIn, CFile::modeRead, &fileException)) {
		if (filewrite.Open(TemplateOut, CFile::modeCreate | CFile::modeWrite, &fileException)) {
			while (file.ReadString(buffer)) {

				TagReplace(&buffer, nextSong, eca.szCurrentSong, eca.szCurrentAlbum,eca.szCurrentArtist, eca.szCurrentComment);
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
	char	buffer[255] = "";
	struct	_stat	stat_buf;

	CInternetSession* inet = new CInternetSession("Mozilla/4.0 (compatible; MSIE 5.0; Windows 98; DigExt)",1, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE);

	ftp = inet->GetFtpConnection(FTPHost, FTPUsername, FTPPassword, 21, TRUE);
	if (_stat(FTPLocalFile, &stat_buf) != 0) {
		sprintf(buffer, "Cannot find FTP Local FILE (%s)", FTPLocalFile);
		MessageBox(NULL, buffer, "Error", MB_OK);
	}
	if (ftp != NULL) {
		ftp->PutFile(FTPLocalFile, FTPRemoteFile, FTP_TRANSFER_TYPE_ASCII, 1);
		ftp->Close();
	}
	inet->Close();
	delete inet;
	delete ftp;

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

	char	cachefile[1024] = "";

	if (g_TmpDir[strlen(g_TmpDir)-1] != '\\') {
		sprintf(cachefile, "%s\\songs.txt", g_TmpDir);
	}
	else {
		sprintf(cachefile, "%s", g_TmpDir);
		strcat(cachefile, "songs.txt");
	}


	filep = fopen(cachefile, "r");
	if (filep == 0) {
		Sleep(2000);
		filep = fopen(cachefile, "r");
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
		if (strncmp(songs[i], "Winamp Stopped!", sizeof("Winamp Stopped!"))) {
			strcpy(g_songs[j], songs[i]);
			j++;
			if (j > 50) {
				numSongs = j;
				break;
			}
		}
	}
	g_numSongs = numSongs;

}
extern CMySocket	*mySocket;

void CEca::getLive365Listeners(char *streamID)
{
	char	buffer[10000] = "";
	CString	readbuffer;
	SOCKET	connectedSocket;

	int		error = 0;
	int		start = 0;
	int		end = 0;


	connectedSocket = mySocket->DoSocketConnect("www.live365.com", 80); 
	if (connectedSocket != -1) {
		sprintf(buffer, "\n\
GET /cgi-bin/ratings.cgi?stream=%s HTTP/1.0\n\
Accept: application/vnd.ms-excel, application/msword, application/vnd.ms-powerpoint, image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-comet, */*\n\
Accept-Language: en-us\n\
Accept-Encoding: gzip, deflate\n\
User-Agent: Mozilla/4.0 (compatible; MSIE 5.0; Windows 98; DigExt)\n\
Host: www.live365.com\n\
\r\n\r\n", streamID);

		int ret = send(connectedSocket, buffer, strlen(buffer), 0);
		if (ret == (int)strlen(buffer)) {
			memset(buffer, '\000', sizeof(buffer));

			readbuffer = "";
			while (recv(connectedSocket, buffer, sizeof(buffer), 0)) {
				readbuffer += buffer;
				memset(buffer, '\000', sizeof(buffer));
			}

			int size = readbuffer.GetLength();
			int loop = 1;
			while (loop) {
				loop = 0;
				int place;
				place = readbuffer.Find("parent.gStreamListeners");
				if ( place != -1) {
					start = readbuffer.Find("=", place);
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
				place = readbuffer.Find("var gStreamCapacity");
				if ( place != -1) {
					start = readbuffer.Find("=", place);
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
		}
		closesocket(connectedSocket);
	}
}


