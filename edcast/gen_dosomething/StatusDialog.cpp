// StatusDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "StatusDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusDialog dialog


CStatusDialog::CStatusDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStatusDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatusDialog)
	m_Status = _T("");
	//}}AFX_DATA_INIT
}


void CStatusDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusDialog)
	DDX_Text(pDX, IDC_STATUS, m_Status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatusDialog, CDialog)
	//{{AFX_MSG_MAP(CStatusDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusDialog message handlers
