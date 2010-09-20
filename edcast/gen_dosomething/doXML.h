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
// Socket.h: interface for the CMySocket class.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
using namespace std;

// Name of header file has changed since version 1.0.x
#include "AdvXMLParser/AdvXMLParser.h"
// Name of the namespace has changed since version 1.0.x
using namespace AdvXMLParser;

#if !defined(AFX_DOXML_H__5EFF0BE2_257B_11D3_BF65_006008B82A90__INCLUDED_)
#define AFX_DOXML_H__5EFF0BE2_257B_11D3_BF65_006008B82A90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDoXML  
{
public:

	
	CString	OpenXmlFile(char*);
	char*	xOpenXmlFile(char*, char *);
	void	ShowElemAndChilds(const Element& , int , char *parent);
	int	start(char *);

	int		counter;
	int		mSeqNumber;
	CInternetSession	*inetSession;


};

#endif // !defined(AFX_DOXML_H__5EFF0BE2_257B_11D3_BF65_006008B82A90__INCLUDED_)

