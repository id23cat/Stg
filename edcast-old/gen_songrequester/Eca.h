// Eca.h: interface for the CEca class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECA_H__EC041801_CC69_11D3_B1AF_00E0293B4DC4__INCLUDED_)
#define AFX_ECA_H__EC041801_CC69_11D3_B1AF_00E0293B4DC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEca  
{
public:
	void getLive365Listeners(char *);
	void LoadSongs();
	void FTPOutput(char *, char *, char *, char *, char *);
	void substituteHTML(char *, char *, char *);
	void readSongCache(char *);
	CEca();
	virtual ~CEca();
	CString	listeners;
	CString	totallisteners;


};

#endif // !defined(AFX_ECA_H__EC041801_CC69_11D3_B1AF_00E0293B4DC4__INCLUDED_)
