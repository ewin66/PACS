// pacsTest3Dlg.h : ͷ�ļ�
//
#pragma once

#include "DrawGraph.h"			//��ͼ��ͷ�ļ�
#include <list>
using namespace std;

// CpacsTest3Dlg �Ի���
class CpacsTest3Dlg : public CDialog
{
// ����
public:
	CpacsTest3Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PACSTEST3_DIALOG };

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
protected:
	virtual void OnOK();
// �Զ����Ա����
public:
	CString m_strIP;	//������IP��ַ
	CWinThread* m_graphThread;	//��ͼ�߳�
	CString  m_strDCMPath;	//DCM�ļ�Ŀ¼
	list<GraphData> m_transDataSet;	//�洢���д������ݣ����ڼ����ͼ����
	list<GraphData> m_graphDataSet;	//�洢��ͼ���ݣ����ڻ�ͼ����л�ͼ

// �Զ����Ա����
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnConfigure();	//��������
	afx_msg void OnBnClickedBtnExit();				//�˳�
	afx_msg void OnBnClickedBtnAddclient();	//���ӿͻ���
	//�ͻ���socket�߳�
	friend	UINT socketThreadFunc(LPVOID pParam);
	//�ͻ��˻�ͼ�߳�
	bool isSuspent;	//�Ƿ����
	friend UINT graphThreadFunc( LPVOID pParam );

};
