// pacsTest1Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CpacsTest1Dlg �Ի���
class CpacsTest1Dlg : public CDialog
{
// ����
public:
	CpacsTest1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PACSTEST1_DIALOG };

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
// �Զ����Ա����
public:
	//ͼ����Ϣ�б�ؼ�
	CListCtrl m_infoListCtrl1;
	CListCtrl m_infoListCtrl2;
	CListCtrl m_infoListCtrl3;
	//������IP��ַ
	CString m_strIP;
	//DCM�ļ�Ŀ¼
	CString  m_strDCMPath;
// �Զ����Ա����
public:
	void initListCtrl(CListCtrl* listCtrl);		//��ʼ��ͼ����Ϣ�б�ؼ�
	void rstImgAndListCtrl();		//����ͼ����ʾ�ؼ�����Ϣ�б�ؼ�
	afx_msg void OnBnClickedBtnConfigure();	//�����������
	afx_msg void OnBnClickedBtnExit();		//�˳���ť
	afx_msg void OnBnClickedBtnTest();	//�������
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//�ͻ���socket�̺߳���
	friend UINT socketThreadFunc1( LPVOID pParam );
	friend UINT socketThreadFunc2( LPVOID pParam );
	friend UINT socketThreadFunc3( LPVOID pParam );

protected:
	virtual void OnOK();
};
