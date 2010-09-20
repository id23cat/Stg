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
#include <time.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <afxwin.h>
#include <afxinet.h>
#include <afxdisp.h>
#include <afxmt.h>

#include "Proxy.h"
#include "Socket.h"

// Typedefs
typedef struct threaddata {
	unsigned int uiSeconds;
	unsigned long ulThreadID;
} ThreadData;

#define THREAD_TIMEOUT 20
#undef ALARM_TEST
#define MAX_THREADS 100

// Forwards
void ProxyLoop(unsigned short);
UINT SingleThreadedDoProxy(LPVOID lpArg);
void MultiThreadedDoProxy(void *lpArg);
void Alarm(void *lpArg);
void ThreadAlarm(void *lpArg);

// Globals
BOOL g_SingleThreaded = FALSE;
BOOL g_Passthru = FALSE;
int iNumThreads = 0;
typedef struct socketStruct_tag {
	SOCKET	skAccept;
	char	connectedTo[1024];
} socketStruct;

// Externs
int		g_Threads = 10;

extern int		gThreadsRunning;
extern void decrementThreadCount();

unsigned long serverThread = 0;

int ProxyMain(unsigned short int uiPort)
{
	unsigned long uiThreadID = 0;
	ProxyLoop(uiPort);
	serverThread = uiThreadID;	
	return 1;
}

SOCKET skListen;

extern BOOL bisRunning;

void ProxyQuit()
{
	bisRunning = FALSE;
	closesocket(skListen);
}

CMySocket csockProxy;


void ProxyLoop(unsigned short port)
{
    SOCKET skAccept;
	SOCKET *pskAccept;
	unsigned long ulThreadID = 0;


	csockProxy.initWinsockLib();
    skListen = csockProxy.DoSocketListen(port);

	int loop = 1;

    while (loop) {
		skAccept = csockProxy.DoSocketAccept(skListen);

		socketStruct *inputSocket;
		inputSocket = (socketStruct *)malloc(sizeof(socketStruct));
		memset(inputSocket, '\000', sizeof(socketStruct));
		inputSocket->skAccept = skAccept;
		strcpy(inputSocket->connectedTo, csockProxy.connectedTo);

		if (skAccept != -1) {
			if (g_SingleThreaded) {
				SingleThreadedDoProxy((void *)inputSocket);
			} else {
//				pskAccept = (SOCKET *)malloc(sizeof(SOCKET));
//				if (!pskAccept)
//					exit(999);
//				*pskAccept = skAccept;

				AfxBeginThread(SingleThreadedDoProxy, inputSocket);

				iNumThreads++;

			}
		}
		else {
			decrementThreadCount();
			AfxEndThread(2, TRUE);
			loop = 0;
		}
    }

	return;
}

UINT SingleThreadedDoProxy(LPVOID lpArg)
{
	socketStruct *skAccept;
	int iError;
	CProxy	myProxy;

	skAccept = (socketStruct *)lpArg;

	myProxy.bSingleThreaded = g_SingleThreaded;
	myProxy.Init(skAccept->skAccept);


	myProxy.connectedTo = skAccept->connectedTo;

	iError = myProxy.DoProxy();
	if (iError == 0) {
		printf("Error returned after DoProxy()\n");
	}

	(void)closesocket(skAccept->skAccept);
	free(skAccept);
	return 1;
}

void MultiThreadedDoProxy(void *lpArg)
{
	CProxy *cproxyWork = NULL;
	socketStruct *skAccept;
	int iError;

	skAccept = (socketStruct *)lpArg;

	cproxyWork = new CProxy(skAccept->skAccept, g_SingleThreaded);

	if (!cproxyWork)
		exit(999);

	cproxyWork->connectedTo = skAccept->connectedTo;
	
	iError = cproxyWork->DoProxy();
	if (iError == 0) {
		MessageBox(NULL, "Error returned after DoProxy()", "Error", MB_OK);
	}

	(void)closesocket(skAccept->skAccept);
	delete cproxyWork;
	free(skAccept);

	iNumThreads--;

	_endthread();
	return;
}



