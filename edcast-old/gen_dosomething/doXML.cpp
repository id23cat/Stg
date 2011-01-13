#include <afxwin.h>
#include <afxinet.h>
#include <afxdisp.h>
#include <afxtempl.h>
#include "stdafx.h"
#include "Socket.h"
#include <stdio.h>
// Remove the following line if you use
// Microsoft Visual C++ with SGI STL

#include "doXML.h"

typedef struct tag_XMLElement {
	CString		tag;
	CString		value;
	int			counter;
} XMLElement;

extern CArray<XMLElement, XMLElement &> xmlTags;
extern BOOL	bEnableErrors;


CString CDoXML::OpenXmlFile(char* szFileName)
{
	CStdioFile	*file;
	CString		readbuffer;
	CString		result;
	DWORD dwHttpRequestFlags =	INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD;

	CInternetSession	inetSession("Mozilla/4.0 (oddsock)", 1, INTERNET_FLAG_TRANSFER_ASCII, NULL, 0);
//	inetSession = new CInternetSession("Mozilla/4.0 (oddsock)",1,INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_DONT_CACHE, NULL, NULL, 0);

	file = inetSession.OpenURL(szFileName, 1, dwHttpRequestFlags, NULL, 0);

	while (file->ReadString(readbuffer)) {
		result += readbuffer;
	}
//	delete inetSession;

	return result;
}

char* CDoXML::xOpenXmlFile(char* szFileName, char *result)
// ---------------------------------------------
// Load a XML file a return it in a buffer
//
// szFileName:  [in]  file name of the XML file
// nSize:       [out] size of the data returned
// Return:      pointer to XML file content
//
// Note:        The caller must destroy the
//              returned buffer with delete[]
// ---------------------------------------------
{
    // Open the file
	CMySocket	mySocket;
	
	char	host[255] = "";
	char	strport[25] = "";
	char	getrequest[255] = "";
	int		port = 0;

	char	*p1;
	char	*p2;
	char	*p3;

	p1 = strstr(szFileName, "http://");
	if (p1 == NULL) {
		return(0);
	}
	p1 = p1 + strlen("http://");
	p2 = strchr(p1, ':');
	if (p2 == NULL) {
		return(0);
	}
	memset(host, '\000', sizeof(host));
	strncpy(host, p1, p2-p1);
	p2++;
	
	p3 = strchr(p2, '/');
	if (p3 == NULL) {
		return(0);
	}

	memset(strport, '\000', sizeof(strport));
	strncpy(strport, p2, p3-p2);
	port = atoi(strport);
	

	if (port == 0) {
		return(0);
	}

	strcpy(getrequest, p3);


	CString	readbuffer;
	char	buffer[2000] = "";

	SOCKET connectedSocket = 0;
	connectedSocket = mySocket.DoSocketConnect(host, port); 
	if (connectedSocket != -1) {
		sprintf(buffer, "GET %s HTTP/1.0\n\
Accept: */*\n\
Accept-Encoding: gzip, deflate\n\
User-Agent: Mozilla/4.0 (compatible; MSIE 5.0; Windows 98; DigExt)\n\
Host: %s:%d\n\
\r\n\r\n", getrequest, host, port);

		int ret = send(connectedSocket, buffer, strlen(buffer), 0);
		if (ret == (int)strlen(buffer)) {
			memset(buffer, '\000', sizeof(buffer));

			readbuffer = "";
			ret = recv(connectedSocket, result, 1024, 0);
			while (ret > 0) {
				result = result + ret;
				ret = recv(connectedSocket, result, 1024, 0);
			}

			int	error = 0;
			error = WSAGetLastError();
			char	buf[255] = "";
			sprintf(buf, "Error = %d : Read %d bytes", error, strlen(result));
			MessageBox(NULL, buf, "Error", MB_OK);
		}
		closesocket(connectedSocket);
	}


//	strcpy(result, readbuffer.GetBuffer(readbuffer.GetLength()));
/*

    FILE* pFile = fopen("c:\\sc.xml", "rb"); // "b" for binary
    if(pFile == 0) // open failed ?
        return(0);

    // Compute the size of the file
    fseek(pFile, 0, SEEK_END);
    int nSize = ftell(pFile);
    // Put the file pointer at the beginning
    fseek(pFile, 0, SEEK_SET);

    // Allocate a buffer big enough
    char* pBuffer = new char[nSize + 1];
    // Put the XML file data in the buffer
    fread(pBuffer, nSize, 1, pFile);
    // Put a 0 char at the end
    pBuffer[nSize] = 0;
    // Close the file
    fclose(pFile), pFile = 0;
*/
    // Return the buffer
    return((char *)1);
}

void CDoXML::ShowElemAndChilds(const Element& elem, int nIndent = 0, char *parent = "")
// ---------------------------------------------
// Enumerate childs of elem, recursivly and 
// display their name (with identation)
//
// elem:    [in] element to display
// nIndent: [in] number of identation
// ---------------------------------------------
{
    // Indentation (2 characters per indentation)
    for(int i = 0; i < 2 * nIndent; ++i)
        cout << ' ';
    // Display the name of the element

    // Contrary to version 1.0.x, you don't have direct
    // access the the children (vector). Instead you
    // can use Iterator or ConstIterator. It's far safer.

    // For each child
    ConstIterator<Element> it = elem.Begin();
    ConstIterator<Element> itEnd = elem.End();

    String par;


	par = elem.GetName();
	char	paren[1024] = "";
	sprintf(paren, "%s_%s", parent, (char *)par.c_str());

	if (it == itEnd) {
		
	    String name;

		name = elem.GetName();
		String val;

		val = elem.GetValue();


		
//	    cout << elem.GetValue() << endl;
		XMLElement	xmlElem;


		char	tag[1024] = "";

		sprintf(tag, "%s_%s", parent, name.c_str());

		xmlElem.tag = tag;
		xmlElem.value = val.c_str();
		xmlElem.counter = 0;

		for (int i=0;i<xmlTags.GetSize();i++) {
			if (xmlTags.GetAt(i).tag == xmlElem.tag) {
				int count = 0;
				XMLElement tempEle = xmlTags.GetAt(i);
				tempEle.counter++;
				xmlTags.SetAt(i, tempEle);

				sprintf(tag, "%s_%s%d", parent, name.c_str(), xmlTags.GetAt(i).counter);
				xmlElem.tag = tag;
				break;
			}
		}
		/*
		if (!strcmp(name.c_str(), "TITLE")) {
			sprintf(tag, "%s_%s%d", parent, name.c_str(), counter);
			counter++;
		}
		else {
			if (!strcmp(name.c_str(), "PLAYEDAT")) {
				sprintf(tag, "%s_%s%d", parent, name.c_str(), counter);

			}
			else {
				sprintf(tag, "%s_%s", parent, name.c_str());
			}
		}
		*/

		
		xmlTags.Add(xmlElem);
//		fprintf(stdout, "%s_%s = <%s>\n", parent, name.c_str(), val.c_str());
	}

    for(; it < itEnd; ++it)
    {
        // Display the child and its sub-childs.
        // Contrary to version 1.0.x, *it returns the element (not more **it)
        ShowElemAndChilds(*it, nIndent + 1, (char *)paren);
    }
}

int CDoXML::start(char *hoststring)
// ---------------------------------------------
// Entry point
// ---------------------------------------------
{
    // Get the content of the XML file
    long nSize = 0;
	char	result[10000] = "";
	char	*pStart;
	CString	strResult;

	
    strResult = OpenXmlFile(hoststring);
//    if(pBuffer == 0)
  //      return(false);


//	pStart = strstr(LPCSTR(strResult), "<?");
//	if (pStart == 0) {
//		return(false);
//	}
//	strResult = pStart;
//	strResult.Replace(" encoding=\"ISO-8859-1\"", "");
//	pStart--;
    try
    {
        // Parse the XML document
        Parser parser;
        // Parse return now a Document* object instead of an Element&
        // It's important to use auto_ptr to destroy the Document later
        auto_ptr<Document> pDocument(parser.Parse(strResult.GetBuffer(strResult.GetLength()), strResult.GetLength()));

		
		counter = 0;

        // Display the elements, starting at the root
		char	baseTag[255] = "";
		if (mSeqNumber > 0) {
			sprintf(baseTag, "XML%d", mSeqNumber);
		}
		else {
			strcpy(baseTag, "XML");
		}
        ShowElemAndChilds(pDocument->GetRoot(), 0, baseTag);

    }
    catch(ParsingException e)
    {
        // Parsing error
		if (bEnableErrors) {
	        MessageBox(NULL, "XML Parsing error ", "Error", MB_OK);
		}
    }

    // Delete the buffer
//    delete[] pBuffer;
//	CString	message;
//	for (int i = 0;i<xmlTags.GetSize();i++) {
//		message += xmlTags.GetAt(i).tag;
//		message += " = ";
//		message += xmlTags.GetAt(i).value;
//		message += "\n";
//	}
//	MessageBox(NULL, message, "Mess", MB_OK);
    return(1);
}