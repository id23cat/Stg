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
#include <winsock2.h>

#include "websvr.h"

#define BUFFER_SIZE 1024*34
#define SERVER_NAME "Winamp Web Server Plugin"
#define SERVER_URL ""
#define PROTOCOL "HTTP/1.0"
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"
#define MAX_LEN 2*1024

class CProxy
{
public:
	void UnURLize(CString *tempString);
	CProxy();
	CProxy(SOCKET sock, BOOL bsinglethreaded);
	virtual ~CProxy();

	void Init(SOCKET sock);
	int DoProxy(void);
	void ProxyMain(void);
	int myfputs(char *szLine, SOCKET sock);
	void trim( char* line );
	char *stristr(const char *phaystack, const char *pneedle);
	void send_error( int status, char* title, char* extra_header, char* text );
	void send_headers( int status, char* title, char* extra_header, char* mime_type, int length, time_t mod );
	BOOL CheckSocketError(int iError, char *szMessage);
	void socketErrorExit(char *szError);
	void CleanAndExit(int iError);
	int SubstituteTags(CString *buf, int, int, char *, int, char *, char *, int);
	void PlayHandler(char *request);
	void StopHandler(char *request);
	void RequestHandler(char *request);
	void AdminHandler(char *request, int *adminFlag);
	int ValidateRequest(int);
	void PlaylistHandler(char *request, char *, char *, char *, char *, int *);
	int CompareNonCase(char *, char *, int);
	int CompareNonCaseBeginning(char *, char *, int);


	BOOL bSingleThreaded;
	
	char m_Referrer[1024];
	char m_szMethod[MAX_LEN];
	char m_szPath[MAX_LEN];
	char m_szURL[MAX_LEN];
	char m_szHost[MAX_LEN];
	char m_szProtocol[MAX_LEN];
	int m_iRemotePort;
	char *m_pszEOL;
	int m_iHeaderLen;
	char szRecvBuffer[BUFFER_SIZE];
	char	szNullString[10];
	char	m_Current[25];
	int		numDisplayed;
	int		maxEntries;

	unsigned int iWaitTime;
	SOCKET skAccept;
	SOCKET skClient;
	CString	connectedTo;
};