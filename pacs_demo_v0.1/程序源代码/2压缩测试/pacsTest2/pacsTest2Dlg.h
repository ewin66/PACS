// pacsTest2Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CpacsTest2Dlg �Ի���
class CpacsTest2Dlg : public CDialog
{
// ����
public:
	CpacsTest2Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PACSTEST2_DIALOG };

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
public:
	CString m_strDCMPath;	//ѹ��ǰDCM�ļ�����·��
	CString m_strJPLYDCMPath;	//ѹ����DCM�ļ�����·����JPEG lossy
	CString m_strJPLSDCMPath;	//ѹ����DCM�ļ�����·����JPEG lossless
	CString m_strRLEDCMPath;	//ѹ����DCM�ļ�����·����RLE lossless

	CListCtrl m_infoListCtrl;	//�б�ؼ�
	unsigned int countRow;	//�б�ؼ��м���
public:
	void drawUncmprImg();							//����ԭʼDICOMͼ��
	void drawCmprImg(CString strCmp);	//����ѹ����DICOMͼ��

	void cstr2pchar(CString str, char* &pChar);	//unicode��CStringתΪchar*

	void CalDispParam(CString str, int type);	//���㲢��ʾ����ѹ������
	double calcBPP(CString str);			//1.���������
	double calcRMSE(CString str, int type);		//2.������������
	double calcSNR(CString str, int type);			//3.���������
	double calcPSNR(CString str, int type);		//4.�����ֵ�����

	//��ť��Ӧ
	afx_msg void OnBnClickedBtnChoose();	//ѡ���ļ�
	afx_msg void OnBnClickedBtnJpls();		//JPEG����ѹ��
	afx_msg void OnBnClickedBtnJply();		//JPEG����ѹ��
	afx_msg void OnBnClickedBtnRle();			//RLE����ѹ��
};
