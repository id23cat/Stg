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
*/// Proxy.cpp: implementation of the CProxy class.
//
//////////////////////////////////////////////////////////////////////

#include <fstream.h>
#include <afxwin.h>
#include <afxinet.h>
#include <afxdisp.h>
#include <afxtempl.h>
#include <afxmt.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>


#include "stdafx.h"
#include "stdafx.h"

#include "Proxy.h"
extern char		g_WebRoot[1024];
extern char		g_Playlistfile[1024];
extern char *getSongTitle(int);
extern char *GetQueueTitle(int);
extern char *getCurrentSong();
extern int getArtist(int , char *);
extern void GetSecondsLeft();
extern int g_CurrentSongLeft;
extern int g_CurrentPctLeft;
extern int g_CurrentSongLength;
extern char g_MaxRequestsPerIP[25];
extern char	g_MaxEntries[25];
extern char g_MaxQueueSize[25];
extern char g_AdminPassword[255];

extern void PlaySong(int);
extern void PlayWinamp();
extern void StopWinamp();
extern void PrevWinamp();
extern void NextWinamp();
extern void PauseWinamp();

#define DATA_TO_BROWSER		0
#define DATA_FROM_BROWSER	1

typedef struct {
	int		playlistpos;
	int		songlength;
	CString	requestingIP;
} Req_Queue;

typedef struct {
	int		playlistpos;
	time_t	timePlayed;
} Played;

extern void repopulatePlaylistCache();

extern CArray<Req_Queue, Req_Queue &> RequestQueue;
extern CCriticalSection QueueList;
extern CArray<CString, CString &> PlaylistFileCache;
CArray<CString, CString &> ArtistList;

extern CRITICAL_SECTION GlobalCriticalSection_QueueList; 
extern CRITICAL_SECTION GlobalCriticalSection_ThreadCount; 

char	errorTable[13][255] = {
			"No Error",
			"Request Accepted!",
			"Too many songs from the same album",
			"Too many consecutive songs from the same album",
			"Too many songs from the same artist",
			"Too many consecutive songs from the same artist",
			"Too many consecutive songs from the same artist/album/title",
			"You cannot request any more songs at this time",
			"The request queue is full",
			"The admin password you supplied was invalid",
			"Queue position deleted!",
			"Position out of range"};

#define ERR_NO_ERROR 0
#define REQUEST_ACCEPTED 1
#define ERR_TOO_MANY_SAME_ALBUM 2
#define ERR_TOO_MANY_CONSECUTIVE_SAME_ALBUM 3
#define ERR_TOO_MANY_SAME_ARTIST 4
#define ERR_TOO_MANY_CONSECUTIVE_SAME_ARTIST 5
#define ERR_TOO_MANY_CONSECUTIVE_ALL 6
#define ERR_MAX_REQUEST_IP 7
#define ERR_QUEUE_FULL 8
#define ERR_BAD_PASSWORD 9
#define DELETE_ACCEPTED 10
#define ERR_POSITION_OUT_OF_RANGE 11

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CProxy::CProxy()
{
}

CProxy::CProxy(SOCKET sock, BOOL bsinglethreaded)
{
	bSingleThreaded = bsinglethreaded;
	Init(sock);
}

CProxy::~CProxy()
{

}

void CProxy::Init(SOCKET sock)
{
	skAccept = sock;
	iWaitTime = 0;

	memset(m_szMethod, 0, MAX_LEN);
	memset(m_szPath, 0, MAX_LEN);
	memset(m_szURL, 0, MAX_LEN);
	memset(m_szHost, 0, MAX_LEN);
	memset(m_szProtocol, 0, MAX_LEN);
	m_iRemotePort = 0;
	m_pszEOL = NULL;
	m_iHeaderLen = 0;
}


char g_choppedrequest[1024] = "";

void CProxy::PlaylistHandler(char *request, char *searchstring, char *startswithstring, char *g_current, char *errorcode, int *adminFlag)
{
	int	playlistpos = 0;
	CString	tempString;

	char	choppedrequest[1024] = "";
		char	suppliedPass[255] = "";
	char	*p1;
	char	request2[1024] = "";
		char	variable[255] = "";
	char	value[255] = "";

	
//	repopulatePlaylistCache();

	char	tempRequest[1024] = "";

	strcpy(tempRequest, request);


//	if (!strncmp(tempRequest, "/admin.cgi", strlen("/admin.cgi"))) {
		p1 = strchr(tempRequest, '?');
		if (p1) {
			p1++;
			char	*token;
			token = strtok(p1, "&");
			while (token != NULL) {
				char	*p2;
				p2 = strchr(token, '=');
				if (p2) {
					memset(variable, '\000', sizeof(variable));
					memset(value, '\000', sizeof(value));

					int lengthtocopy = 0;
					if ((p2-token) > (sizeof(variable)-1)) {
						lengthtocopy = sizeof(variable)-1;
					}
					else {
						lengthtocopy = p2-token;
					}
					strncpy(variable, token, lengthtocopy);
					p2++;
					strncpy(value, p2, sizeof(value)-1);
					if (!strcmp(variable, "pass")) {
						strncpy(suppliedPass, value, sizeof(suppliedPass)-1);
					}
					if (!strcmp(variable, "errorCode")) {
						strncpy(errorcode, value, sizeof(errorcode)-1);
					}
					if (!strcmp(variable, "current")) {
						strncpy(g_current, value, 25-1);
					}
					if (!strcmp(variable, "psearch")) {
						tempString = value;
						UnURLize(&tempString);
						strncpy(searchstring, tempString.GetBuffer(tempString.GetLength()), 2046-1);
					}
					if (!strcmp(variable, "startswith")) {
						tempString = value;
						UnURLize(&tempString);
						strncpy(startswithstring, tempString.GetBuffer(tempString.GetLength()), 255-1);
					}
				}
				token = strtok(NULL, "&");
			}
		}
		if (strlen(g_AdminPassword) > 0) {
			if (!strcmp(suppliedPass, g_AdminPassword)) {
					*adminFlag = 1;
			}
			else {
				*adminFlag = 0;
			}
		}

//	}
/*
	memset(request2, '\000', sizeof(request2));
	p3 = strstr(request, "current=");
	if (p3 != NULL) {
		strncpy(request2, request, p3 - request);
		p3 = p3 + strlen("current=");
		p4 = strchr(p3, '&');
		if (p4 == NULL) {
			strcpy(g_current, p3);
		}
		else {
			strncpy(g_current, p3, p4-p3);
			p4++;
			strcat(request2, p4);
		}
		strcpy(request, request2);
	}
	memset(choppedrequest, '\000', sizeof(choppedrequest));

	p1 = strchr(request, '&');
	if (p1 > 0) {
		strncpy(choppedrequest, request, p1 - request);
		p1++;
		p2 = strchr(p1, '=');
		if (p2 > 0) {
			p2++;
			strcpy(errorcode, p2);
		}
	}
	else {
		strcpy(choppedrequest, request);
	}


	strcpy(g_choppedrequest, choppedrequest);

    sscanf(choppedrequest, "/playlist.cgi?psearch=%s", searchstring);

	tempString = searchstring;

	UnURLize(&tempString);

	strcpy(searchstring, tempString.GetBuffer(tempString.GetLength()));

	tempString = "";

    sscanf(choppedrequest, "/playlist.cgi?startswith=%s", startswithstring);

	tempString = startswithstring;

	UnURLize(&tempString);

	strcpy(startswithstring, tempString.GetBuffer(tempString.GetLength()));
*/
}
int CProxy::ValidateRequest(int playlistpos)
{
	int		requestsOnQueue = 0;

	if (playlistpos < 0) {
		return ERR_POSITION_OUT_OF_RANGE;
	}
	if (playlistpos > PlaylistFileCache.GetSize()) {
		return ERR_POSITION_OUT_OF_RANGE;
	}
	/* Validate Queue Size Check */
	if (RequestQueue.GetSize() >= atoi(g_MaxQueueSize)) {
		return ERR_QUEUE_FULL;
	}

	/* Validate MAX from IP Check */
	for(int i=0;i<=RequestQueue.GetUpperBound();i++) {
		if (RequestQueue.GetAt(i).requestingIP == connectedTo) {
			requestsOnQueue++;
		}
	}
	if (atoi(g_MaxRequestsPerIP) != 0) {
		if (requestsOnQueue >= atoi(g_MaxRequestsPerIP)) {
			return ERR_MAX_REQUEST_IP;
		}
	}

	int identicalOnQueue = 0;
	for(i=0;i<=RequestQueue.GetUpperBound();i++) {
		if (RequestQueue.GetAt(i).playlistpos == playlistpos) {
			identicalOnQueue++;
		}
	}
	if (identicalOnQueue > 0) {
		return ERR_TOO_MANY_CONSECUTIVE_ALL;
	}
	return REQUEST_ACCEPTED;
}
void CProxy::AdminHandler(char *request, int *adminFlag)
{
	int	playlistpos = 0;
	char	suppliedPass[255] = "";

	char	msg[255] = "";
	int		valid = 0;
	int		errorCode = 0;
	char	variable[255] = "";
	char	value[255] = "";
	int		action = 0;
	char	tempRequest[1024] = "";

	strcpy(tempRequest, request);


//	if (!strncmp(tempRequest, "/admin.cgi", strlen("/admin.cgi"))) {
		char	*p1;
		p1 = strchr(tempRequest, '?');
		if (p1) {
			p1++;
			char	*token;
			token = strtok(p1, "&");
			while (token != NULL) {
				char	*p2;
				p2 = strchr(token, '=');
				if (p2) {
					memset(variable, '\000', sizeof(variable));
					memset(value, '\000', sizeof(value));

					int lengthtocopy = 0;
					if ((p2-token) > (sizeof(variable)-1)) {
						lengthtocopy = sizeof(variable)-1;
					}
					else {
						lengthtocopy = p2-token;
					}
					strncpy(variable, token, lengthtocopy);
					p2++;
					strncpy(value, p2, sizeof(value)-1);
					if (!strcmp(variable, "pass")) {
						strncpy(suppliedPass, value, sizeof(suppliedPass)-1);
					}
					if (!strcmp(variable, "action")) {
						if (!strcmp(value, "delete")) {
							action = 1;
						}
						if (!strcmp(value, "play")) {
							action = 2;
						}
						if (!strcmp(value, "prev")) {
							action = 3;
						}
						if (!strcmp(value, "pause")) {
							action = 4;
						}
						if (!strcmp(value, "stop")) {
							action = 5;
						}
						if (!strcmp(value, "next")) {
							action = 6;
						}
						if (!strcmp(value, "refreshcache")) {
							action = 7;
						}
					}
					if (!strcmp(variable, "listpos")) {
						playlistpos = atoi(value);
					}
				}
				token = strtok(NULL, "&");
			}
		}
		int passok = 0;

		if (strlen(g_AdminPassword) > 0) {
			if (!strcmp(suppliedPass, g_AdminPassword)) {
				*adminFlag = 1;
				passok = 1;
			}
			else {
				*adminFlag = 0;
				passok = 0;
			}
		}

		if (passok) {
			int winampCommand = 0;
			switch (action) {
			case 2:	PlayWinamp();
					winampCommand = 1;
					break;
			case 3:	PrevWinamp();
					winampCommand = 1;
					break;
			case 4:	PauseWinamp();
					winampCommand = 1;
					break;
			case 5:	StopWinamp();
					winampCommand = 1;
					break;
			case 6:	NextWinamp();
					winampCommand = 1;
					break;
			case 7:	repopulatePlaylistCache();
					winampCommand = 1;
					break;
			}
			if (winampCommand) {
				char buf[255] = "";
				_snprintf(buf, sizeof(buf)-1, "Location: /admin.cgi?pass=%s", g_AdminPassword);
				send_headers(302, "Moved Temporarily", (char*) buf, 0, -1, -1 );
			}
		}

//	}
//    if (sscanf(request, "/admin.cgi?pass=%s&action=delete&listpos=%d", suppliedPass, &playlistpos) == 2) {
	if (action == 1) {
		if (strlen(g_AdminPassword) > 0) {

			if (!strcmp(suppliedPass, g_AdminPassword)) {
				*adminFlag = 1;
				if (playlistpos-1 <= RequestQueue.GetUpperBound()) {
					RequestQueue.RemoveAt(RequestQueue.GetUpperBound() - playlistpos + 1);
					errorCode = DELETE_ACCEPTED;
				}
				else {
					errorCode = ERR_POSITION_OUT_OF_RANGE;
				}
			}
			else {
				*adminFlag = 0;
				errorCode = ERR_BAD_PASSWORD;
			}
		}

		char	location[1024] = "";
		if (strlen(m_Referrer) > 0) {
			if (errorCode == 0) {
				_snprintf(location, sizeof(location)-1, "Location: %s", m_Referrer);
			}
			else {
				_snprintf(location, sizeof(location)-1, "Location: %s&errorCode=%d", m_Referrer, errorCode);
			}
		}
		else {
			if (errorCode == 0) {
				_snprintf(location, sizeof(location)-1, "Location: /playlist.cgi");
			}
			else {
				_snprintf(location, sizeof(location)-1, "Location: /playlist.cgi?errorCode=%d", errorCode);
			}
		}
		send_headers(302, "Moved Temporarily", (char*) location, 0, -1, -1 );
	} 
	if (strlen(g_AdminPassword) > 0) {
		if (!strcmp(suppliedPass, g_AdminPassword)) {
				*adminFlag = 1;
		}
		else {
			*adminFlag = 0;
		}
	}
	return;
//    if (sscanf(request, "/admin.cgi?pass=%s&errorCode=%d", suppliedPass, &errorCode) == 2) {
}
void CProxy::RequestHandler(char *request)
{
	int	playlistpos = 0;
	char	suppliedPass[255] = "";

	char	variable[255] = "";
	char	value[255] = "";
	int		action = 0;
	int		adminFlag = 0;

	char	msg[255] = "";
	int		valid = 0;
	int		errorCode = 0;

	int	requestFlag = 0;
	if (!strncmp(request, "/request.cgi", strlen("/request.cgi"))) {
		char	*p1;
		p1 = strchr(request, '?');
		if (p1) {
			p1++;
			char	*token;
			token = strtok(p1, "&");
			while (token != NULL) {
				char	*p2;
				p2 = strchr(token, '=');
				if (p2) {
					memset(variable, '\000', sizeof(variable));
					memset(value, '\000', sizeof(value));

					int lengthtocopy = 0;
					if ((p2-token) > (sizeof(variable)-1)) {
						lengthtocopy = sizeof(variable)-1;
					}
					else {
						lengthtocopy = p2-token;
					}
					strncpy(variable, token, lengthtocopy);
					p2++;
					strncpy(value, p2, sizeof(value)-1);
					if (!strcmp(variable, "pass")) {
						strncpy(suppliedPass, value, sizeof(suppliedPass)-1);
					}
					if (!strcmp(variable, "listpos")) {
						playlistpos = atoi(value);
					}
				}
				token = strtok(NULL, "&");
			}
		}
		if (strlen(g_AdminPassword) > 0) {
			if (!strcmp(suppliedPass, g_AdminPassword)) {
					adminFlag = 1;
			}
			else {
				adminFlag = 0;
			}
		}
		requestFlag = 1;
	}

//    if (sscanf(request, "/request.cgi?listpos=%d", &playlistpos) == 1) {
	if (requestFlag) {
		if (adminFlag) {
			errorCode = REQUEST_ACCEPTED;
		}
		else {
			errorCode = ValidateRequest(playlistpos);
		}

		if (errorCode == REQUEST_ACCEPTED) {
				EnterCriticalSection(&GlobalCriticalSection_QueueList); 
				Req_Queue newQueueItem;
				newQueueItem.playlistpos = playlistpos;
				newQueueItem.requestingIP = connectedTo;

				RequestQueue.InsertAt(0, newQueueItem, 1);
				LeaveCriticalSection(&GlobalCriticalSection_QueueList);
		}
	} 

	char	location[1024] = "";
	if (strlen(m_Referrer) > 0) {
		CString  tS = m_Referrer;
		tS.Replace("action=", "actionx=");
		strcpy(m_Referrer, LPCSTR(tS));
		if (errorCode == 0) {
			_snprintf(location, sizeof(location)-1, "Location: %s", m_Referrer);
		}
		else {
			_snprintf(location, sizeof(location)-1,  "Location: %s&errorCode=%d", m_Referrer, errorCode);
		}
	}
	else {
		if (errorCode == 0) {
			_snprintf(location, sizeof(location)-1,  "Location: /playlist.cgi");
		}
		else {
			_snprintf(location, sizeof(location)-1,  "Location: /playlist.cgi?errorCode=%d", errorCode);
		}
	}
	send_headers(302, "Moved Temporarily", (char*) location, 0, -1, -1 );

}


void CProxy::StopHandler(char *request)
{
	StopWinamp();
}
void CProxy::PlayHandler(char *request)
{
	PlayWinamp();
}


int CProxy::SubstituteTags(CString *buf, int skAccept, int searchflag, char *searchstring, int startswithflag, char *startswithstring, char *errorcode, int adminFlag)
{
	int ret = 0;
	int	iError = 0;
	char	*pszSongTitle = (char *)1;
	char	szArtist[255] = "";
	int		playlistpos = 0;
	CString	substituted;
	char	tmpbuf[1024] = "";
	int		replaceflag = 0;
	CString tempArtist;
	int		foundit = 0;
	int		loop = 1;
	int		errorCode = 0;

	errorCode = atoi(errorcode);

	ret = buf->Find("%%ARTISTSELECT%%");

	if (ret != -1) {
		while(loop) {
			memset(szArtist, '\000', sizeof(szArtist));
			loop = getArtist(playlistpos, szArtist);
			if(loop) {
				tempArtist = szArtist;
				foundit = 0;
				for(int i=0;i <= ArtistList.GetUpperBound();i++) {
					if (ArtistList.GetAt(i) == tempArtist) {
						foundit = 1;
					}
				}
				if (!foundit) {
					if (strlen(tempArtist) > 0) {	
						ArtistList.Add(tempArtist);
					}
				}
			}
			playlistpos++;
		}

		CArray<CString, CString &> ArtistListSorted;

		for(int j=0;j <= ArtistList.GetUpperBound();j++) {
			int added = 0;
			for (int k=0;k <= ArtistListSorted.GetUpperBound();k++) {
				if (ArtistList.GetAt(j) < ArtistListSorted.GetAt(k)) {
					ArtistListSorted.InsertAt(k, ArtistList.GetAt(j));
					added = 1;
					break;
				}
			}
			if (!added) {
				ArtistListSorted.Add(ArtistList.GetAt(j));
			}
		}

		for(int i=0;i <= ArtistListSorted.GetUpperBound();i++) {
			substituted = *buf;
			substituted.Replace("%%ARTISTSELECT%%", ArtistListSorted.GetAt(i));
			iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
			iError = send(skAccept, "\r\n", strlen("\r\n"), 0);

		}
		return 1;
	}

	ret = buf->Find("%%ADMINWINAMPCONTROL%%");

	if (ret != -1) {
		substituted = *buf;
		if (adminFlag) {
			char	hiddenfield[1024] = "";
			sprintf(hiddenfield, "<table bordercolor=white border=1><tr><td colspan=5>Winamp Admin Control</td></tr><tr><td><a href=\"admin.cgi?pass=%s&action=prev\">Prev</td><td><a href=\"admin.cgi?pass=%s&action=play\">Play</td><td><a href=\"admin.cgi?pass=%s&action=pause\">Pause</td><td><a href=\"admin.cgi?pass=%s&action=stop\">Stop</td><td><a href=\"admin.cgi?pass=%s&action=next\">Next</td><tr><td colspan=5><a href=\"admin.cgi?pass=%s&action=refreshcache\">Refresh Playlist Cache</td></tr></table>", g_AdminPassword, g_AdminPassword, g_AdminPassword, g_AdminPassword, g_AdminPassword, g_AdminPassword);
			substituted.Replace("%%ADMINWINAMPCONTROL%%", hiddenfield);
		}
		else {
			substituted.Replace("%%ADMINWINAMPCONTROL%%", "");
		}
		iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
		iError = send(skAccept, "\r\n", strlen("\r\n"), 0);
		return 1;
	}

	ret = buf->Find("%%ADMINHIDDENFIELD%%");

	if (ret != -1) {
		substituted = *buf;
		if (adminFlag) {
			char	hiddenfield[1024] = "";
			sprintf(hiddenfield, "<input type=hidden name=pass value=\"%s\">", g_AdminPassword);
			substituted.Replace("%%ADMINHIDDENFIELD%%", hiddenfield);
		}
		else {
			substituted.Replace("%%ADMINHIDDENFIELD%%", "");
		}
		iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
		iError = send(skAccept, "\r\n", strlen("\r\n"), 0);
		return 1;
	}
	ret = buf->Find("%%ADMINPASSWORD%%");

	if (ret != -1) {
		substituted = *buf;
		if (adminFlag) {
			char	hiddenfield[1024] = "";
			sprintf(hiddenfield, "&pass=%s", g_AdminPassword);
			substituted.Replace("%%ADMINPASSWORD%%", hiddenfield);
		}
		else {
			substituted.Replace("%%ADMINPASSWORD%%", "");
		}
		iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
		iError = send(skAccept, "\r\n", strlen("\r\n"), 0);
		return 1;
	}

	ret = buf->Find("%%ERROR%%");

	if (ret != -1) {
		substituted = *buf;
		if (errorCode != 0) {
			substituted.Replace("%%ERROR%%", errorTable[errorCode]);
		}
		else {
			substituted.Replace("%%ERROR%%", "");
		}
		iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
		iError = send(skAccept, "\r\n", strlen("\r\n"), 0);
		return 1;
	}

	ret = buf->Find("%%HTTP_REFERRER%%");

	if (ret != -1) {
		substituted = *buf;
		if (errorCode != 0) {
			substituted.Replace("%%HTTP_REFERRER%%", m_Referrer);
		}
		else {
			substituted.Replace("URL=%%HTTP_REFERRER%%", "");
			substituted.Replace("%%HTTP_REFERRER%%", "");
		}
		iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
		iError = send(skAccept, "\r\n", strlen("\r\n"), 0);

		return 1;
	}


	playlistpos = 0;
	ret = buf->Find("%%PLAYLISTENTRY%%");

	if (ret != -1) {
		int startingPos = 0;
		if (strlen(m_Current) > 0) {
			startingPos = atoi(m_Current);
		}

		int currentPos = 0;
		numDisplayed = 0;
		maxEntries = atoi(g_MaxEntries);
		while(pszSongTitle) {
			pszSongTitle = getSongTitle(playlistpos);
			if(pszSongTitle) {
				replaceflag = 1;
				if (searchflag) {
					if (CompareNonCase(pszSongTitle, searchstring, strlen(searchstring))) {
						replaceflag = 1;
					}
					else {
						replaceflag = 0;
					}
				}
				if (startswithflag) {
					if (CompareNonCaseBeginning(pszSongTitle, startswithstring, strlen(startswithstring))) {
						replaceflag = 1;
					}
					else {
						replaceflag = 0;
					}
				}

				if (replaceflag) {
					if ((currentPos >= startingPos) && (numDisplayed < maxEntries)) {
						substituted = *buf;
						substituted.Replace("%%PLAYLISTENTRY%%", pszSongTitle);
						memset(tmpbuf, '\000', sizeof(tmpbuf));
						if (adminFlag) {
							sprintf(tmpbuf, "%d&pass=%s", playlistpos, g_AdminPassword);
						}
						else {
							sprintf(tmpbuf, "%d", playlistpos);
						}

						substituted.Replace("%%PLAYLISTPOS%%", tmpbuf);
						iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
						iError = send(skAccept, "\r\n", strlen("\r\n"), 0);
						numDisplayed++;
					}
					currentPos++;
				}
			}
			playlistpos++;
		}
		return 1;
	}
	ret = buf->Find("%%QUEUELISTENTRY%%");

	pszSongTitle = (char *)1;
	
	playlistpos = RequestQueue.GetUpperBound();

	int	queuepos = 1;
	if (ret != -1) {
		while(pszSongTitle) {
			pszSongTitle = GetQueueTitle(playlistpos);
			if(pszSongTitle) {
				substituted = *buf;
				substituted.Replace("%%QUEUELISTENTRY%%", pszSongTitle);
				memset(tmpbuf, '\000', sizeof(tmpbuf));
				if (adminFlag) {
					sprintf(tmpbuf, "<a href=\"admin.cgi?pass=%s&action=delete&listpos=%d\">Delete</a>&nbsp;%d", g_AdminPassword, queuepos, queuepos);
				}
				else {
					sprintf(tmpbuf, "%d", queuepos);
				}
				substituted.Replace("%%QUEUELISTPOS%%", tmpbuf);
				iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
				iError = send(skAccept, "\r\n", strlen("\r\n"), 0);

			}
			playlistpos--;
			queuepos++;
		}
		return 1;
	}

	ret = buf->Find("%%NAV_NEXT%%");

	if (ret != -1) {
		substituted = *buf;
		char	buff[255] = "";
		if (strchr(g_choppedrequest, '?')) {
			sprintf(buff, "<a href=\"%s&current=%d\">Next..></a>", g_choppedrequest, atoi(m_Current) + atoi(g_MaxEntries));
		}
		else {
			char	params[255] = "";

			if (startswithflag) {
				sprintf(params, "startswith=%s&current=%d", startswithstring, atoi(m_Current) + atoi(g_MaxEntries));
			}
			if (searchflag) {
				sprintf(params, "psearch=%s&current=%d", searchstring, atoi(m_Current) + atoi(g_MaxEntries));
			}
			if (strlen(params) == 0) {
				sprintf(params, "current=%d", atoi(m_Current) + atoi(g_MaxEntries));
			}
			if (adminFlag) {
				sprintf(buff, "<a href=\"admin.cgi?pass=%s&%s\">Next..></a>", g_AdminPassword, params);
			}
			else {
				sprintf(buff, "<a href=\"playlist.cgi?%s\">Next..></a>", params);
			}
		}
		if (numDisplayed < maxEntries) {
			substituted.Replace("%%NAV_NEXT%%", "");
		}
		else {
			substituted.Replace("%%NAV_NEXT%%", buff);
		}
		iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
		iError = send(skAccept, "\r\n", strlen("\r\n"), 0);

		return 1;
	}
	ret = buf->Find("%%NAV_PREV%%");

	if (ret != -1) {
		substituted = *buf;
		char	buff[255] = "";
		if (strchr(g_choppedrequest, '?')) {
			sprintf(buff, "<a href=\"%s&current=%d\"><..Prev</a>", g_choppedrequest, atoi(m_Current) - atoi(g_MaxEntries));
		}
		else {
			char	params[255] = "";

			if (startswithflag) {
				sprintf(params, "startswith=%s&current=%d", startswithstring, atoi(m_Current) - atoi(g_MaxEntries));
			}
			if (searchflag) {
				sprintf(params, "psearch=%s&current=%d", searchstring, atoi(m_Current) - atoi(g_MaxEntries));
			}
			if (strlen(params) == 0) {
				sprintf(params, "current=%d",  atoi(m_Current) - atoi(g_MaxEntries));
			}
			if (adminFlag) {
				sprintf(buff, "<a href=\"admin.cgi?pass=%s&%s\"><...Prev</a>", g_AdminPassword, params);
			}
			else {
				sprintf(buff, "<a href=\"playlist.cgi?%s\"><...Prev</a>", params);
			}
		}
		if ((atoi(m_Current) - atoi(g_MaxEntries)) < 0) {
			substituted.Replace("%%NAV_PREV%%", "");
		}
		else {
			substituted.Replace("%%NAV_PREV%%", buff);
		}
		iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
		iError = send(skAccept, "\r\n", strlen("\r\n"), 0);

		return 1;
	}

	ret = buf->Find("%%CURRENTSONG%%");

	if (ret != -1) {
		pszSongTitle = getCurrentSong();
		if(pszSongTitle) {
			substituted = *buf;
			substituted.Replace("%%CURRENTSONG%%", pszSongTitle);
			iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
			iError = send(skAccept, "\r\n", strlen("\r\n"), 0);

		}
		return 1;
	}

	ret = buf->Find("%%CONTENT_REFRESH%%");
	if (ret != -1) {
		substituted = *buf;
		GetSecondsLeft();
		char tmp[25] = "";
		sprintf(tmp, "%d", g_CurrentSongLeft + 5);
		
		substituted.Replace("%%CONTENT_REFRESH%%", tmp);

		iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
		iError = send(skAccept, "\r\n", strlen("\r\n"), 0);
		return 1;
	}
	ret = buf->Find("%%PCT_LEFT%%");
	if (ret != -1) {
		substituted = *buf;
		GetSecondsLeft();
		char tmp[25] = "";
		sprintf(tmp, "%d", g_CurrentPctLeft);
		
		substituted.Replace("%%PCT_LEFT%%", tmp);

		iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
		iError = send(skAccept, "\r\n", strlen("\r\n"), 0);
		return 1;
	}

	ret = buf->Find("%%SONG_LENGTH%%");
	if (ret != -1) {
		substituted = *buf;
		GetSecondsLeft();
		char tmp[25] = "";
		sprintf(tmp, "%d", g_CurrentSongLength);
		
		substituted.Replace("%%SONG_LENGTH%%", tmp);

		iError = send(skAccept, substituted.GetBuffer(substituted.GetLength()), substituted.GetLength(), 0);
		iError = send(skAccept, "\r\n", strlen("\r\n"), 0);
		return 1;
	}

	return 0;		
}

int CProxy::CompareNonCaseBeginning(char *string1, char *string2, int length)
{
	int i = 0;
	int	match = 1;
	CString	str1;
	CString str2;

	str1 = string1;
	str2 = string2;

	str1.MakeLower();
	str2.MakeLower();


	if (!strncmp(str1.GetBuffer(str1.GetLength()), str2.GetBuffer(str1.GetLength()), length)) {
		match = 1;
	}
	else {
		match = 0;
	}
	return match;
}
int CProxy::CompareNonCase(char *string1, char *string2, int length)
{
	int i = 0;
	int	match = 1;
	CString	str1;
	CString str2;

	str1 = string1;
	str2 = string2;

	str1.MakeLower();
	str2.MakeLower();

	if (str1.Find(str2.GetBuffer(str2.GetLength()), 0) == -1) {
		match = 0;
	}
	else {
		match = 1;
	}
	return match;
}
int CProxy::DoProxy(void)
{
	char line[MAX_LEN];
	int	iError, iLineLen;
	CString	relativefiletoRead;
	CString webroot;
	CString	filetoRead;
   int loop = 1;
   unsigned int nbytes = 255;
   char	*p1;
   char	content_type[25] = "";
   char	GET[25] = "GET";
   struct stat	statStruct;
   CString	strbuffer;
		char	msg[255] = "";
	char	searchstring[2046] = "";
	char	startswithstring[255] = "";
	char	errorcode[10] = "";
	int		searchflag = 0;
	int		startswithflag = 0;
	char	buffer[1024];

   int	totalSent = 0;
	CStdioFile file;
	FILE	*filep;
	char	*pszReferrer;
	char	*p2;
	char	tempURL2[1024] = "";
	char	tempURL3[256] = "";
	char	adminPass[255] = "";
	int		adminFlag = 0;
	int lengthtocopy = 0;
	char	*pp1 = 0;
	char	*pp2 = 0;


	memset(tempURL2, '\000', sizeof(tempURL2));
	memset(tempURL3, '\000', sizeof(tempURL3));
 
	memset(szRecvBuffer, 0, sizeof(szRecvBuffer));

	/* Read the first line of the request. */
	iError = recv(skAccept, szRecvBuffer, sizeof(szRecvBuffer)-1, 0);
	if (CheckSocketError(iError, "Couldn't read initial request")) {
		send_error( 400, "Bad Request", (char*) 0, "No request found." );
		goto ErrorExit;
	}


	memset(m_Referrer, '\000', sizeof(m_Referrer));

	strcpy(m_Referrer, "playlist.cgi");
	// Find end of first line
	pszReferrer = strstr(szRecvBuffer, "Referer: ");
	if (pszReferrer != 0) {
		p2 = strstr(pszReferrer, "&errorCode=");
		if (p2 == 0) {
			p2 = strstr(pszReferrer, "\r\n");
			if (p2 != 0) {
				memset(m_Referrer, '\000', sizeof(m_Referrer));
				strncpy(m_Referrer, pszReferrer + strlen("Referer: "), p2- (pszReferrer + strlen("Referer: ")));
			}
		}
		else {
			memset(m_Referrer, '\000', sizeof(m_Referrer));
			strncpy(m_Referrer, pszReferrer + strlen("Referer: "), p2- (pszReferrer + strlen("Referer: ")));
		}
	}
	m_pszEOL = strchr(szRecvBuffer, '\n');
	if (!m_pszEOL) {
		iLineLen = strlen(szRecvBuffer);

		if (iLineLen == 0) {
			socketErrorExit("Couldn't find end of line for first request");
			return 0;
		}

		m_pszEOL = szRecvBuffer + iLineLen;

	}

	m_pszEOL++;

	// Copy first line of recv() into line
	memset(line, 0, sizeof(line));

//	strncpy(line, szRecvBuffer, m_pszEOL - szRecvBuffer);
		strncpy(line, szRecvBuffer, sizeof(line)-1);

	// Calculate what's left in our buffer (excluding first line)
	m_iHeaderLen = iError - strlen(line);

    /* Parse it. */
    trim( line );

	if (strncmp(line, "GET ", 4)) {
		send_error( 400, "Only GET requests supported", (char*) 0, "Error" );
		goto ErrorExit;
	}
//    if ( sscanf( line, "%[^ ] %[^ ] %[^ ]", m_szMethod, m_szURL, m_szProtocol ) != 3 ) {

	pp1 = line + 4;
	pp2 = strchr(pp1, ' ');
	if (pp2 == 0) {
		send_error( 400, "Bad Request", (char*) 0, "Can't parse request." );
		goto ErrorExit;
	}

	memset(m_szURL, '\000', sizeof(m_szURL));
	if ((pp2-pp1) > sizeof(m_szURL) -1) {
		lengthtocopy = sizeof(m_szURL) - 1;
	}
	else {
		lengthtocopy = pp2-pp1;
	}
	strncpy(m_szURL, pp1, lengthtocopy);

	if (!strcmp(m_szURL, "/")) {
		strcat(m_szURL, "playlist.cgi");
	}
	strncpy(tempURL3, m_szURL, sizeof(tempURL3));
	if (!strncmp(m_szURL, "/?", 2)) {
		_snprintf(tempURL3, sizeof(tempURL3)-1, "/playlist.cgi%s", m_szURL+1);
		
	}
	if (!strncmp(m_szURL, "/&",2 )) {
		_snprintf(tempURL3, sizeof(tempURL3)-1, "/playlist.cgi%s", m_szURL+1);
	}
	strncpy(m_szURL, tempURL3, sizeof(m_szURL));

//	if (!strncmp(m_szURL, "/play.cgi", strlen("/play.cgi"))) {
//		PlayHandler(m_szURL);
//		goto NormalExit;
//	}
//	if (!strncmp(m_szURL, "/stop.cgi", strlen("/stop.cgi"))) {
//		StopHandler(m_szURL);
//		goto NormalExit;
//	}
	if (!strcmp(m_szURL, "/admin.cgi")) {
		sprintf(buffer, "HTTP/1.1 200 OK\nServer: WinampServer\nConnection: close\nContent-Type: text/html\r\n\r\n");
		iError = send(skAccept, buffer, strlen(buffer), 0);
		sprintf(buffer, "<html><form method=GET action=\"admin.cgi\">Enter Admin Password <input type=text name=pass><input type=Submit></form>");
		iError = send(skAccept, buffer, strlen(buffer), 0);
		goto NormalExit;
	}
	if (!strncmp(m_szURL, "/admin.cgi", strlen("/admin.cgi"))) {
		AdminHandler(m_szURL, &adminFlag);
	}
	if (!strncmp(m_szURL, "/request.cgi", strlen("/request.cgi"))) {
		RequestHandler(m_szURL);
		goto NormalExit;
	}
	
	p1 = strchr(m_szURL, '?');
	if (p1) {
		strncmp(tempURL2, m_szURL, p1-m_szURL);
	}
	else {
		p1 = strchr(m_szURL, '&');
		if (p1) {
			strncpy(tempURL2, m_szURL, p1-m_szURL);
		}
		else {
			strcpy(tempURL2, m_szURL);
		}
	}


	if ((!strncmp(m_szURL, "/playlist.cgi", strlen("/playlist.cgi"))) || (!strncmp(m_szURL, "/admin.cgi", strlen("/admin.cgi"))) || (!strcmp(tempURL2, "/"))) {
		memset(m_Current, '\000', sizeof(m_Current));
		PlaylistHandler(m_szURL, searchstring, startswithstring, m_Current, errorcode, &adminFlag);
		strcpy(content_type, "text/html");
		if (strlen(searchstring) == 0) {
			searchflag = 0;
		}
		else {
			searchflag = 1;
		}
		if (strlen(startswithstring) == 0) {
			startswithflag = 0;
		}
		else {
			startswithflag = 1;
		}
		relativefiletoRead = g_Playlistfile;
	}
	else {
		relativefiletoRead = m_szURL;
	}

	p1 = stristr(m_szURL, ".");
	if (p1) {
		p1++;
		if (strcmp(p1, "jpg") == 0) {
			strcpy(content_type, "image/jpeg");
		}
		if (strcmp(p1, "jpeg") == 0) {
			strcpy(content_type, "image/jpeg");
		}
		if (strcmp(p1, "gif") == 0) {
			strcpy(content_type, "image/gif");
		}
		if (strcmp(p1, "png") == 0) {
			strcpy(content_type, "image/png");
		}
		if (strcmp(p1, "css") == 0) {
			strcpy(content_type, "text/css");
		}
		if (strcmp(p1, "wav") == 0) {
			strcpy(content_type, "audio/wav");
		}
		if (strcmp(p1, "avi") == 0) {
			strcpy(content_type, "video/x-msvideo");
		}
		if (strcmp(p1, "mov") == 0) {
			strcpy(content_type, "video/quicktime");
		}
		if (strcmp(p1, "mpg") == 0) {
			strcpy(content_type, "video/mpeg");
		}
		if (strcmp(p1, "mpeg") == 0) {
			strcpy(content_type, "video/mpeg");
		}
		if (strcmp(p1, "vrml") == 0) {
			strcpy(content_type, "model/vrml");
		}
		if (strcmp(p1, "mp3") == 0) {
			strcpy(content_type, "audio/mpeg");
		}
		if (strcmp(p1, "pls") == 0) {
			strcpy(content_type, "audio/x-scpls");
		}
		if (strcmp(p1, "html") == 0) {
			strcpy(content_type, "text/html");
		}
	}

	if (relativefiletoRead.Left(2) == "/.") {
		sprintf(msg, "Invalid File %s - relative paths not supported", LPCSTR(relativefiletoRead));
		send_error( 400, msg, (char*) 0, "" );
		goto ErrorExit;
	}
	
	relativefiletoRead.Replace("/", "\\");
	webroot = g_WebRoot;
	if (g_WebRoot[strlen(g_WebRoot)] != '\\') {
		filetoRead = webroot + "\\" + relativefiletoRead;
	}
	else {
		filetoRead = webroot + relativefiletoRead;
	}

	filetoRead.Replace("\\\\", "\\");
	
	if (stat(filetoRead.GetBuffer(filetoRead.GetLength()), &statStruct) != 0) {
		sprintf(msg, "Invalid File %s", LPCSTR(relativefiletoRead));
		send_error( 400, msg, (char*) 0, "" );
		goto ErrorExit;
  	}

	
	
	if (!strcmp(content_type, "text/html")) {
		sprintf(buffer, "HTTP/1.1 200 OK\nServer: WinampServer\nConnection: close\nContent-Type: %s\r\n\r\n",content_type);
	}
	else {
		sprintf(buffer, "HTTP/1.1 200 OK\nServer: WinampServer\nContent-Length: %d\nConnection: close\nContent-Type: %s\r\n\r\n", statStruct.st_size,content_type);
	}
	iError = send(skAccept, buffer, strlen(buffer), 0);

	/* Read in input: */

	char	buf[8096];
	memset(buf, '\000', sizeof(buf));


	if (!strcmp(content_type, "text/html")) {
		filep = fopen(LPCSTR(filetoRead), "r");
		if (filep == NULL) {
			send_error( 400, "Invalid File", (char*) 0, "" );
			goto ErrorExit;
		}
		strbuffer = "";
		while (fgets(buf, sizeof(buf), filep) != NULL) {
			strbuffer = buf;
			if (!SubstituteTags(&strbuffer,skAccept, searchflag, searchstring, startswithflag, startswithstring, errorcode, adminFlag)) {
				iError = send(skAccept, strbuffer.GetBuffer(strbuffer.GetLength()), strbuffer.GetLength(), 0);
				iError = send(skAccept, "\r\n", strlen("\r\n"), 0);
				strbuffer = "";
			}
			memset(buf, '\000', sizeof(buf));
		}
		fclose(filep);
   }
   else {
	   FILE	*filep;
	   filep = fopen(LPCSTR(filetoRead), "rb");

		if (filep == 0) {
			send_error( 400, "Invalid File", (char*) 0, "" );
			goto ErrorExit;
		}
		int ret = 0;
		while (!feof(filep)) {
			memset(buf, '\000', sizeof(buf));
			ret = fread(buf, 1, sizeof(buf), filep);
			iError = send(skAccept, buf, ret, 0);
			totalSent = totalSent + iError;
		}
		fclose(filep);

   }
	closesocket(skClient);
	return 1;

NormalExit:
	closesocket(skClient);
	return 1;

ErrorExit:
	closesocket(skClient);
	return 0;
}



void CProxy::CleanAndExit(int iError)
{
	int iSocketError;

	iSocketError = WSAGetLastError();
	closesocket(skAccept);
	WSACleanup();

}

int CProxy::myfputs(char *szLine, SOCKET sock)
{
	int iSend;
	int iError;

	iSend = send(sock, szLine, strlen(szLine), 0);
	if (iSend == SOCKET_ERROR) {
		iError = WSAGetLastError();

		if (iError == WSAECONNRESET) {
			return(0);
		}
		
		if (CheckSocketError(iSend, "Error on send() in myfputs()"))
			return SOCKET_ERROR;
	}

	return iSend;
}


/////////////////////////////////////////////////////////////////////////////
//
// trim
//
// Description
//   Removes CR and LF from line
//
void CProxy::trim( char* line )
{
    int l;

	if (!line)
		return;

    l = strlen( line );
    while ( line[l-1] == '\n' || line[l-1] == '\r' )
	line[--l] = '\0';
}



void CProxy::send_error( int status, char* title, char* extra_header, char* text )
{
	char line[MAX_LEN];
	char szBuffer[MAX_LEN];
	int iSend;

    send_headers(200, title, extra_header, "text/html", -1, -1 );
    (void) sprintf( szBuffer, "<HTML><HEAD><TITLE>%d %s</TITLE></HEAD>\n<BODY><H4>%d %s</H4>\n", status, title, status, title );
    (void) sprintf( line, "%s\n", text );
	strcat(szBuffer, line);
    (void) sprintf( line, "<HR>\n<ADDRESS><A HREF=\"http://www.oddsock.org\">Oddsock Song Requester</A></ADDRESS>\n</BODY></HTML>\n");
	strcat(szBuffer, line);
	iSend = send(skAccept, szBuffer, strlen(szBuffer), 0);
	if (CheckSocketError(iSend, "Error on send() in send_error()"))
		return;

	return;
}


void CProxy::send_headers(int status, char* title, char* extra_header, char* mime_type, int length, time_t mod )
{
    time_t now;
    char timebuf[100];
	char line[MAX_LEN];
	char szBuffer[MAX_LEN];
	int	iSend;

    (void) sprintf(szBuffer,  "%s %d %s\r\n", PROTOCOL, status, title );
    (void) sprintf(line, "Server: %s\r\n", SERVER_NAME );
	strcat(szBuffer, line);

    now = time( (time_t*) 0 );
    (void) strftime( timebuf, sizeof(timebuf), RFC1123FMT, gmtime( &now ) );
    (void) sprintf(line, "Date: %s\r\n", timebuf );
	strcat(szBuffer, line);
    if ( extra_header != (char*) 0 ) {
		(void) sprintf(line, "%s\r\n", extra_header );
		strcat(szBuffer, line);
	}
    if ( mime_type != (char*) 0 ) {
		(void) sprintf(line, "Content-type: %s\r\n", mime_type );
		strcat(szBuffer, line);
	}
    if ( length >= 0 ) {
		(void) sprintf(line, "Content-length: %d\r\n", length );
		strcat(szBuffer, line);
	}
    if ( mod != (time_t) -1 )
	{
		(void) strftime( timebuf, sizeof(timebuf), RFC1123FMT, gmtime( &mod ) );
		strcat(szBuffer, line);
		(void) sprintf(line, "Last-modified: %s\r\n", timebuf );
		strcat(szBuffer, line);
	}
	(void) sprintf(line, "Connection: close\r\n" );
	strcat(szBuffer, line);
	(void) sprintf(line, "\r\n" );
	strcat(szBuffer, line);
	iSend = send(skAccept, szBuffer, strlen(szBuffer), 0);
}

/////////////////////////////////////////////////////////////////////////////
//
// CheckSocketError
//
// Description
//   Checks the iError input for SOCKET_ERROR, if an error exists, print
//   the supplied szMessage and exit the program
//
// Parameters
//
BOOL CProxy::CheckSocketError(int iError, char *szMessage)
{
	char szErrMessage[MAX_LEN];

	if (iError == SOCKET_ERROR) {
		sprintf(szErrMessage, "%s: %d\n", szMessage, iError);
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// socketErrorExit
//
// Description
//   Print supplied error message and exit the program.
//
// Parameters
//   szError - Error message text
//
void CProxy::socketErrorExit(char *szError)
{
    printf("Socket error: %s\n", szError);
    CleanAndExit(0);
}

typedef unsigned chartype;

char *CProxy::stristr(const char *phaystack, const char *pneedle)
{
/* From GNU libc 2.0.6.  */
/* Modified to do case-insensitive comparison */
/* Return the first ocurrence of NEEDLE in HAYSTACK.  */
/*
 * My personal strstr() implementation that beats most other algorithms.
 * Until someone tells me otherwise, I assume that this is the
 * fastest implementation of strstr() in C.
 * I deliberately chose not to comment it.  You should have at least
 * as much fun trying to understand it, as I had to write it :-).
 *
 * Stephen R. van den Berg, berg@pool.informatik.rwth-aachen.de	*/

  register const unsigned char *haystack, *needle;
  register chartype b, c;
  int junk;

  haystack = (const unsigned char *) phaystack;
  needle = (const unsigned char *) pneedle;

  b = *needle;
  if (b != '\0')
    {
      haystack--;				/* possible ANSI violation */
      do
	{
	  c = *++haystack;
	  if (c == '\0')
	    goto ret0;
	}
      while (tolower(c) != tolower(b));

      c = *++needle;
      if (c == '\0')
	goto foundneedle;
      ++needle;
      goto jin;

      for (;;)
        {
          register chartype a;
	  register const unsigned char *rhaystack, *rneedle;

	  do
	    {
	      a = *++haystack;
	      if (a == '\0')
		goto ret0;
	      if (tolower(a) == tolower(b))
		break;
	      a = *++haystack;
	      if (a == '\0')
		goto ret0;
shloop:
		  junk = 0;
		}
          while (tolower(a) != tolower(b));

jin:	  a = *++haystack;
	  if (a == '\0')
	    goto ret0;

	  if (tolower(a) != tolower(c))
	    goto shloop;

	  rhaystack = haystack-- + 1;
	  rneedle = needle;
	  a = *rneedle;

	  if (tolower(*rhaystack) == tolower(a))
	    do
	      {
		if (a == '\0')
		  goto foundneedle;
		++rhaystack;
		a = *++needle;
		if (tolower(*rhaystack) != tolower(a))
		  break;
		if (a == '\0')
		  goto foundneedle;
		++rhaystack;
		a = *++needle;
	      }
	    while (tolower(*rhaystack) == tolower(a));

	  needle = rneedle;		/* took the register-poor approach */

	  if (a == '\0')
	    break;
        }
    }
foundneedle:
  return (char*) haystack;
ret0:
  return 0;
}


void CProxy::UnURLize(CString *tempString)
{
	tempString->Replace("+"," ");
	tempString->Replace("%25","%");
	tempString->Replace("%3B",";");
	tempString->Replace("%2F","/");
	tempString->Replace("%3F","?");
	tempString->Replace("%3A",":");
	tempString->Replace("%40","@");
	tempString->Replace("%26","&");
	tempString->Replace("%3D","=");
	tempString->Replace("%2B","+");
	tempString->Replace("%20"," ");
	tempString->Replace("%5C","\"");
	tempString->Replace("%23","#");
	
	tempString->Replace("%3C","<");
	tempString->Replace("%3E",">");
	tempString->Replace("%21","!");
	tempString->Replace("%2A","*");
	tempString->Replace("%27","'");
	tempString->Replace("%28","(");
	tempString->Replace("%29",")");
	tempString->Replace("%2C",",");

}
