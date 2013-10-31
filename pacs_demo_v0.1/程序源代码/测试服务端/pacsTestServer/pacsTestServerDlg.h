// pacsTestServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

// CpacsTestServerDlg �Ի���
class CpacsTestServerDlg : public CDialog
{
// ����
public:
	CpacsTestServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PACSTESTSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

//�Զ������
public:
	CString m_strIP;	//IP��ַ
	CListCtrl m_msgListCtrl;	//��Ϣ�б�ؼ�
	CWinThread* m_pThread;	//�������߳�
	CString  strDCMPath;	//DCM�ļ�Ŀ¼
// �Զ����Ա����
protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//����������
	afx_msg void OnBnClickedBtnStart();
	//�������̺߳���
	friend UINT serverThreadFunc( LPVOID pParam );
	//�����socket�̺߳���
	friend UINT socketThreadFunc(LPVOID pParam);

};
