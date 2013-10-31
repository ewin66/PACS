// pacsTest3Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "pacsTest3.h"			//Ӧ�ó���ͷ�ļ�
#include "pacsTest3Dlg.h"	//���Ի���ͷ�ļ�
#include "ConfDlg.h"				//�������öԻ���ͷ�ļ�

#include "Protocol.h"				//Э��ͷ�ļ�
#include "afxmt.h"					//CMutex��ͷ�ļ�
#include "Shlwapi.h"				//����Ŀ¼���ͷ�ļ�

//ȫ�ֳ�������
const unsigned int clientEachTime = 5;	//ÿ�����ӵĿͻ��˸���
const unsigned int maxClientNum = 100;	//���ͻ�����Ŀ
//ȫ�ֱ�������
unsigned int threadID = 0;	//�̱߳�ţ�1,2,3������
unsigned int totalClientNum = 0;	//�ܹ��Ŀͻ��˸�����1,2,3������
unsigned int sucClientNum = 0;	//�ѽ������߳���Ŀ��1,2,3������
//ȫ�ֻ����������
CMutex dataMutex(FALSE, NULL);	//MFC�����������д��������

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CpacsTest3Dlg �Ի���




CpacsTest3Dlg::CpacsTest3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CpacsTest3Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strIP = _T("");
	m_strDCMPath = _T(".\\Client");
	m_graphThread = NULL;
	m_transDataSet.clear();
	m_graphDataSet.clear();
}

void CpacsTest3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CpacsTest3Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CONFIGURE, &CpacsTest3Dlg::OnBnClickedBtnConfigure)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CpacsTest3Dlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_ADDCLIENT, &CpacsTest3Dlg::OnBnClickedBtnAddclient)
END_MESSAGE_MAP()


// CpacsTest3Dlg ��Ϣ�������

BOOL CpacsTest3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//�½����߳����ڻ�ͼ
	m_graphThread = AfxBeginThread(graphThreadFunc, NULL);
	if (NULL != m_graphThread)
	{
		m_graphThread->SuspendThread();		//��ʱ����
		isSuspent = TRUE;
	}
	else
	{
		CString strMsg;
		strMsg.Format(_T("��ͼ�̴߳���ʧ��: %d"), GetLastError());
		AfxMessageBox(strMsg, MB_OK);
	}

	//�ж�DCM�ļ�Ŀ¼�Ƿ����
	if (!PathIsDirectory(m_strDCMPath))
		CreateDirectory(m_strDCMPath, NULL);	//�������򴴽�

	//��������ǰ�޷�����
	GetDlgItem(IDC_BTN_ADDCLIENT)->EnableWindow(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CpacsTest3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CpacsTest3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CpacsTest3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��ֹͨ��esc�����رնԻ���
BOOL CpacsTest3Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)   
	{   
		pMsg->wParam = 0;
	}    
	return CDialog::PreTranslateMessage(pMsg);
}

//��ֹͨ���س����رնԻ���
void CpacsTest3Dlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}

//�����������
void CpacsTest3Dlg::OnBnClickedBtnConfigure()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CConfDlg confDlg;
	if (IDOK == confDlg.DoModal())
	{
		m_strIP = confDlg.m_strIPConf;
		GetDlgItem(IDC_BTN_ADDCLIENT)->EnableWindow(TRUE);
	}
}

//�˳���ť
void CpacsTest3Dlg::OnBnClickedBtnExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}

void CpacsTest3Dlg::OnBnClickedBtnAddclient()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����Ѿ��ﵽ���ͻ�����Ŀ��������
	if (totalClientNum >= maxClientNum)
	{
		GetDlgItem(IDC_BTN_ADDCLIENT)->EnableWindow(FALSE);
		AfxMessageBox(_T("�Ѵﵽ���ͻ�����Ŀ��"), MB_OK);
		return;
	}
	GetDlgItem(IDC_BTN_EXIT)->EnableWindow(FALSE);
	totalClientNum += clientEachTime;
	sucClientNum = 0;

	//���ѻ�ͼ����
	if(isSuspent)
	{
		m_graphThread->ResumeThread();
		isSuspent = FALSE;
	}

	//�Զ���߳�ģ�����ͻ���
	for (int i = 1; i <= clientEachTime; i++)
	{
		//�����ͻ����߳�
		CWinThread* m_pThread = AfxBeginThread(socketThreadFunc, (LPVOID)this);
		if(NULL == m_pThread)	//����ʧ��
		{
			CString strMsg;
			strMsg.Format(_T("Socket�߳�#%d����ʧ��: %d"), i, GetLastError());
			AfxMessageBox(strMsg, MB_OK);
		}
		else
		{
			//CloseHandle(m_pThread);
		}
	}
}

//��ͼ���̴�����
UINT graphThreadFunc(LPVOID pParam)
{
	CpacsTest3Dlg* pMainWnd = (CpacsTest3Dlg*)(AfxGetApp()->GetMainWnd());	//������ָ��
	DrawGraph dg;
	dg.drawFrame();	//���Ƴ�ʼ���
	while (1)
	{
		//����Ϊ����һֱ�ȴ�
		while (pMainWnd->m_graphDataSet.size() == 0);
		//��ȡ���ݻ�����
		dataMutex.Lock();
		//��ͼ���������ݶ���
		dg.drawGraph(pMainWnd->m_graphDataSet.front());
		pMainWnd->m_graphDataSet.pop_front();
		//�ͷ����ݻ�����
		dataMutex.Unlock();
	}
	return 0;
}

//Ϊÿ���µ�socket���Ӵ������̴߳�����
UINT socketThreadFunc(LPVOID pParam)
{
	CpacsTest3Dlg* pThreadDlg = (CpacsTest3Dlg*)pParam;
	ASSERT(pThreadDlg);

	//step 1�������ͻ����׽���
	AfxSocketInit(NULL);	//�����ʼ��socket
	CSocket clientSocket;
	if(!clientSocket.Create(0, SOCK_STREAM))	//��0��ɹ�
	{
		CString str;
		str.Format(_T("�ͻ���Socket����ʧ��: %d"), clientSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		clientSocket.Close();
		return -1;
	}
	//�����������
	if(!clientSocket.Connect(pThreadDlg->m_strIP, PRE_AGREED_PORT)) //��0��ɹ�
	{
		CString str;
		str.Format(_T("�ͻ���Socket����ʧ��: %d"), clientSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		clientSocket.Close();
		return -1;
	}
	//��������
	time_t start = time(NULL);	//��ʼʱ��
	LPCTSTR c_move_req = _T("C-MOVE");
	if(!clientSocket.Send(c_move_req, _tcslen(c_move_req) * 2, 0))	//ע���ֽڳ���
	{
		CString str;
		str.Format(_T("�ͻ���Socket����ʧ��: %d"), clientSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		clientSocket.Close();
		return -1;
	}

	//step 2��ȷ��д���ļ����͸��»�ͼ����
	CString fileName;			//�ļ���
	GraphData newData;	//��ͼ����

	//��ȡ���ݻ�����
	dataMutex.Lock();
	//�߳�������
	threadID ++;
	//д���ļ���
	fileName.Format(_T("%s\\retrieval%d.dcm"), pThreadDlg->m_strDCMPath, threadID);
	//���»�ͼ����
	newData.clientID = threadID;
	//�ͷ����ݻ�����
	dataMutex.Unlock();

	//step 3�����շ���˷�����Ϣ
	TCHAR databuf[1024];
	DWORD dwTotalWrite = 0;	//�ܵ�д���ַ�����
	DWORD dwWrite = 0;
	BOOL bRet = FALSE;
	int read = 0;
	HANDLE hFile = CreateFile(fileName,GENERIC_WRITE,0,0,CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,0);
	while (1)
	{
		memset(databuf,0,1024); 
		read = clientSocket.Receive(databuf, 1024, 0 );
		bRet = WriteFile(hFile,databuf,read,&dwWrite,NULL);
		dwTotalWrite += dwWrite;	//�ܵ�д���ַ�����
		if(read <= 0)
			break;
		if(bRet == FALSE)
		{
			CString strMsg;
			strMsg.Format(_T("д���ļ� %s ʧ��: %d"), fileName, GetLastError());
			AfxMessageBox(strMsg, MB_OK);
			break;
		} 
	}
	//�ر��ļ����
	CloseHandle(hFile);
	//����ͨ�Ž������ر�socket
	clientSocket.Close();
	time_t end = time(NULL);	//����ʱ��

	//step4�������ͼ����
	//��ȡ���ݻ�����
	dataMutex.Lock();
	sucClientNum ++;
	if (sucClientNum == clientEachTime)
		pThreadDlg->GetDlgItem(IDC_BTN_EXIT)->EnableWindow(TRUE);
	//���»�ͼ����
	newData.efficiency = difftime(end, start);	//ʱ���
	newData.transRate = (double)dwTotalWrite;
	pThreadDlg->m_transDataSet.push_back(newData);
	//�����ͼ�õĴ�������
	double sumSpentTime = 0.0;	//�ܵĴ���ʱ��
	double sumTransData = 0.0;		//�ܵĴ���������
	unsigned int sumThread = pThreadDlg->m_transDataSet.size();	//�ܵ��̣߳�client������
	for(list<GraphData>::iterator iter = pThreadDlg->m_transDataSet.begin();
		iter != pThreadDlg->m_transDataSet.end(); iter++)
	{
		sumSpentTime += (*iter).efficiency;
		sumTransData += (*iter).transRate;
	}
	newData.clientID = sumThread;
	newData.efficiency = sumSpentTime / sumThread;
	newData.transRate = sumTransData / sumSpentTime;
	//�����ͼ���ݶ���
	pThreadDlg->m_graphDataSet.push_back(newData);
	//�ͷ����ݻ�����
	dataMutex.Unlock();

	return 0;
}