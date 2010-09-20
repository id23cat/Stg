/* micro_proxy - really small HTTP proxy
**
** Copyright © 1999 by Jef Poskanzer <jef@acme.com>. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
*/

#include <time.h>
//#include <winsock2.h>
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
//#include <afxtempl.h>

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
/*
extern BOOL g_SingleThreaded;
extern BOOL g_Passthru;
*/
BOOL g_SingleThreaded = FALSE;
BOOL g_Passthru = FALSE;
int iNumThreads = 0;

// Externs
int		g_Threads = 10;

extern int		gThreadsRunning;
extern void decrementThreadCount();
/////////////////////////////////////////////////////////////////////////////
//
// main program loop
//
// Description
//   
//
// Parameters
//   argc, argv
//
unsigned long serverThread = 0;

int ProxyMain(unsigned short int uiPort)
{
	unsigned long uiThreadID = 0;
//	unsigned short int *puiPort;


//	puiPort = (unsigned short int *)malloc(sizeof(unsigned short int));
//	if (!puiPort)
//		exit(999);

//	*puiPort = uiPort;
//	uiThreadID = _beginthread(ProxyLoop, 0, (void *)puiPort);
//	if (uiThreadID == (unsigned long)-1) {
//		exit(888);
//	}
	ProxyLoop(uiPort);
	serverThread = uiThreadID;	
	return 1;
}

SOCKET skListen;

extern BOOL bisRunning;

void ProxyQuit()
{

	bisRunning = FALSE;
//	shutdown(skListen, SD_BOTH);
	closesocket(skListen);
//	WSACleanup();

}

CSTFSocket csockProxy;

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

		if (skAccept != -1) {
			if (g_SingleThreaded) {
				SingleThreadedDoProxy((void *)&skAccept);
			} else {
				pskAccept = (SOCKET *)malloc(sizeof(SOCKET));
				if (!pskAccept)
					exit(999);
				*pskAccept = skAccept;

				// Limit number of threads running
		//		while (iNumThreads >= g_Threads) {
		//			Sleep(50);
		//		}

				//ulThreadID = _beginthread(MultiThreadedDoProxy, 0,(void *)pskAccept);
				//ulThreadID = _beginthreadex(SingleThreadedDoProxy, 0,(void *)pskAccept);
				AfxBeginThread(SingleThreadedDoProxy, pskAccept);
//				if (ulThreadID == (unsigned long)-1) {
//					printf("Unable to spawn ThreadDoProxy()\n");
//					exit(0);
//				}

				iNumThreads++;

			}
		}
		else {
			decrementThreadCount();
			AfxEndThread(2, TRUE);
		//	_endthread();
			loop = 0;
		}
    }

	return;
}

/////////////////////////////////////////////////////////////////////////////
//
// ThreadDoProxy
//
// Description
//   Performs the actual Sleep and calls sigcatch() when time expires
//
UINT SingleThreadedDoProxy(LPVOID lpArg)
{
//	static CProxy *cproxyWork = NULL;
	SOCKET skAccept;
	int iError;
	CProxy	myProxy;

	skAccept = *((SOCKET *)lpArg);

	myProxy.bSingleThreaded = g_SingleThreaded;
	myProxy.Init(skAccept);


//	if (!cproxyWork)
//		cproxyWork = new CProxy(skAccept, g_SingleThreaded);
//	else
//		cproxyWork->Init(skAccept);


//	if (!cproxyWork)
//		AfxEndThread(0);
		

	struct sockaddr_in connectedIP;
	int		namelen;

	memset(&connectedIP, '\000', sizeof(connectedIP));
	namelen = sizeof(connectedIP);
	int ret = getpeername(skAccept, (struct sockaddr *)&connectedIP, &namelen);
	if (ret == SOCKET_ERROR) {
		int error = WSAGetLastError();
	}

	myProxy.connectedTo = inet_ntoa(connectedIP.sin_addr);

	iError = myProxy.DoProxy();
	if (iError == 0) {
		printf("Error returned after DoProxy()\n");
	}

	(void)closesocket(skAccept);
//	AfxEndThread(1);
	return 1;
}

void MultiThreadedDoProxy(void *lpArg)
{
	CProxy *cproxyWork = NULL;
	SOCKET *pskAccept;
	SOCKET skAccept;
	int iError;

	pskAccept = ((SOCKET *)lpArg);
	skAccept = *pskAccept;

	cproxyWork = new CProxy(skAccept, g_SingleThreaded);

	if (!cproxyWork)
		exit(999);

	struct sockaddr_in connectedIP;
	int		namelen;

	memset(&connectedIP, '\000', sizeof(connectedIP));
	namelen = sizeof(connectedIP);
	int ret = getpeername(skAccept, (struct sockaddr *)&connectedIP, &namelen);
	if (ret == SOCKET_ERROR) {
		int error = WSAGetLastError();
	}

	cproxyWork->connectedTo = inet_ntoa(connectedIP.sin_addr);
	
	iError = cproxyWork->DoProxy();
	if (iError == 0) {
		printf("Error returned after DoProxy()\n");
	}

	(void)closesocket(skAccept);
	delete cproxyWork;
	free(pskAccept);

	iNumThreads--;

	_endthread();
#ifdef ALARM_TEST
	Sleep(2 * THREAD_TIMEOUT * 1000);
#endif
	return;
}

/////////////////////////////////////////////////////////////////////////////
//
// Alarm
//
// Description
//   Generates a separate thread that times out when
//   the seconds have expired.  An error message is returned to the
//   browser afterward.
//
// Parameters
//   seconds - time to Sleep.  Set to zero to remove previous alarm
//
void Alarm(void *lpArg)
{
	ThreadData *tdData;
//	unsigned long ulThreadID;

	//tdData = (ThreadData *)malloc(sizeof(ThreadData));
	//memcpy((void *)tdData, lpArg, sizeof(tdData));
	tdData = (ThreadData *)lpArg;

	// If we get 0 for time, just kill old alarm() and don't start another one
	if (tdData->uiSeconds == 0) {
		free(tdData);
		return;
	}

//	if ((ulThreadID = _beginthread(ThreadAlarm, 0,(void *)tdData)) == (unsigned long)-1) {
//		printf("Unable to set alarm\n");
//		exit(0);
//	}

}

/////////////////////////////////////////////////////////////////////////////
//
// ThreadAlarm
//
// Description
//   Performs the actual Sleep, calls sigcatch() when time expires, and kills
//   off old thread.
//
void ThreadAlarm(void *lpArg)
{
	ThreadData* tdData;

	//memcpy((void *)&tdData, lpArg, sizeof(tdData));
	tdData = (ThreadData *)lpArg;

	Sleep(tdData->uiSeconds * 1000);
	//sigcatch(seconds);
	//printf("Request timed out, killing thread %ld\n", tdData.ulThreadID);
	TerminateThread((void *)tdData->ulThreadID, 408);
	free(tdData);
}

