#pragma once
#include "afxcmn.h"


// CConfDlg �Ի���

class CConfDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfDlg)

public:
	CConfDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfDlg();

// �Ի�������
	enum { IDD = IDD_CONFDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	CIPAddressCtrl m_ipAddrCtrl;
	CString m_strIPConf;
	virtual BOOL OnInitDialog();
};
