// ConfDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "pacsTest3.h"
#include "ConfDlg.h"
#include "Protocol.h"

// CConfDlg �Ի���

IMPLEMENT_DYNAMIC(CConfDlg, CDialog)

CConfDlg::CConfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfDlg::IDD, pParent)
{

}

CConfDlg::~CConfDlg()
{
}

void CConfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipAddrCtrl);
}


BEGIN_MESSAGE_MAP(CConfDlg, CDialog)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CConfDlg::OnIpnFieldchangedIpaddress1)
END_MESSAGE_MAP()


// CConfDlg ��Ϣ�������

void CConfDlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	BYTE b1,b2,b3,b4;
	//num �Ǽ�����Ч���򣬱���211.155.224.*��3����Ч�����֣��ͷ���3��
	int num = m_ipAddrCtrl.GetAddress(b1,b2,b3,b4);
	m_strIPConf.Format(_T("%d.%d.%d.%d"), b1, b2, b3, b4);
}

BOOL CConfDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_strIPConf = _T("");
	//������
	//m_strIPConf = _T("10.13.0.84");
	//m_strIPConf = _T("192.168.56.1");
	//m_strIPConf = _T("127.0.0.1");

	SetDlgItemInt(IDC_EDIT_PORT, PRE_AGREED_PORT);	//����Ĭ��ͨ�Ŷ˿ں�

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
