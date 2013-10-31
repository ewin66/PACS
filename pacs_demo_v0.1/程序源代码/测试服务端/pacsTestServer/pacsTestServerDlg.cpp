// pacsTestServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "pacsTestServer.h"
#include "pacsTestServerDlg.h"	//���Ի���ͷ�ļ�
#include "Protocol.h"		//Э��ͷ�ļ�
#include "Shlwapi.h"	//����Ŀ¼���ͷ�ļ�
#include <afxmt.h>	//CMutex������ļ�
#include <vector>
#include <iostream>
#include <ctime>	//ʱ�������Ӳ��������
using namespace std;

//ȫ�ֱ�������
vector<CString> fileNameVec;	//������Ŀ¼�µ�DCM�ļ����б�
unsigned int countRow = 0;	//��Ϣ�б��м���
CMutex fileMutex(FALSE, NULL);	//MFC�����������д�ļ�

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


// CpacsTestServerDlg �Ի���




CpacsTestServerDlg::CpacsTestServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CpacsTestServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strIP = _T("");
	m_pThread = NULL;
	strDCMPath = _T(".\\Server");
}

void CpacsTestServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG, m_msgListCtrl);
}

BEGIN_MESSAGE_MAP(CpacsTestServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CpacsTestServerDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// CpacsTestServerDlg ��Ϣ�������

BOOL CpacsTestServerDlg::OnInitDialog()
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
	//���ö˿ں�
	SetDlgItemInt(IDC_EDIT_PORT, PRE_AGREED_PORT);
	//��ȡ����(������)IP��ַ
	char serverHostName[100];
	gethostname(serverHostName, 100);
	LPHOSTENT hostent;
	hostent = gethostbyname(serverHostName);
	LPSTR ipAddr = inet_ntoa(*(LPIN_ADDR)*(hostent->h_addr_list));
	m_strIP = ipAddr;
	GetDlgItem(IDC_IPADDRESS)->SetWindowText(m_strIP);

	//��ʼ����Ϣ�б�ؼ�
	//����ģʽ
	m_msgListCtrl.ModifyStyle( 0, LVS_REPORT );
	//�����+��ѡ��
	m_msgListCtrl.SetExtendedStyle(m_msgListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_msgListCtrl.InsertColumn(1, _T("���"), LVCFMT_CENTER);
	m_msgListCtrl.InsertColumn(2, _T("ʱ��"), LVCFMT_CENTER);
	m_msgListCtrl.InsertColumn(3, _T("����"), LVCFMT_CENTER);
	m_msgListCtrl.InsertColumn(4, _T("��Ϣ"), LVCFMT_CENTER);
	//��õ�ǰ�ͻ�����Ϣ
	CRect rect;
	m_msgListCtrl.GetClientRect(rect);
	//�����еĿ��
	m_msgListCtrl.SetColumnWidth(0, rect.Width() / 8); 
	m_msgListCtrl.SetColumnWidth(1, rect.Width() / 6);
	m_msgListCtrl.SetColumnWidth(2, rect.Width() / 8);
	m_msgListCtrl.SetColumnWidth(3, 7 * rect.Width() / 12);

	//�ж�DCM�ļ�Ŀ¼�Ƿ����
	if (!PathIsDirectory(strDCMPath))
		CreateDirectory(strDCMPath, NULL);	//�������򴴽�

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CpacsTestServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CpacsTestServerDlg::OnPaint()
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
HCURSOR CpacsTestServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��ֹͨ���س����رնԻ���
void CpacsTestServerDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}
//��ֹͨ��esc�����رնԻ���
BOOL CpacsTestServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)   
	{   
		pMsg->wParam = VK_RETURN;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CpacsTestServerDlg::OnBnClickedBtnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ȷ���������߳�û��������
	if ( m_pThread != NULL )
	{
		AfxMessageBox(_T("�������Ѿ�������!"), MB_OK);
		return;
	}
	//��ȡ�����Ŀ¼�������ļ���
	CFileFind finder;
	CString strDCMDir;
	strDCMDir.Format(_T("%s\\*.dcm"), strDCMPath);
	BOOL bWorking = finder.FindFile(strDCMDir);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if( !finder.IsDirectory() && !finder.IsDots() )//��ΪĿ¼
		{
			//CString filePath = _T(".\\Server\\");
			//fileNameVec.push_back(filePath + finder.GetFileName());
			fileNameVec.push_back(finder.GetFilePath());
		}
	}
	finder.Close();
	//��������Ŀ¼��û��DCM�ļ������ڴ����򱨴�
	if (fileNameVec.size() == 0)
	{
		AfxMessageBox(_T("�����û�п��õ�DCM�ļ�!"), MB_OK);
		return;
	}
	//��ֹ���ε��
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	//�����������߳�
	m_pThread = AfxBeginThread(serverThreadFunc, (LPVOID)this);
	if (m_pThread == NULL)
	{
		CString strMsg;
		strMsg.Format(_T("�������̴߳���ʧ��: "), GetLastError());
		AfxMessageBox(strMsg, MB_OK);

		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		return;
	}
}

//�������̺߳���
UINT serverThreadFunc( LPVOID pParam )
{
	CpacsTestServerDlg* pThreadDlg = (CpacsTestServerDlg*)pParam;
	ASSERT(pThreadDlg);

	//�׽���׼������
	AfxSocketInit(NULL);	//�����ʼ��socket
	CSocket serverSocket;
	//����socket
	if (!serverSocket.Socket())
	{
		CString str;
		str.Format(_T("������׽��ִ���ʧ��: %d"), serverSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		serverSocket.Close();

		pThreadDlg->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
		return -1;
	}

	//����socket��ѡ����10048�������Ĳ���
	//BOOL bOptVal = TRUE;
	//int bOptLen = sizeof(BOOL);
	//m_serverSocket.SetSockOpt(SO_REUSEADDR, (void *)&bOptVal, bOptLen, SOL_SOCKET);

	//��socket
	if (!serverSocket.Bind(PRE_AGREED_PORT))
	{
		CString str;
		str.Format(_T("������׽��ְ�ʧ��: %d"), serverSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		serverSocket.Close();

		pThreadDlg->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
		return -1;
	}

	//����socket
	if(!serverSocket.Listen(10))//���г���Ϊ10
	{    
		CString str;
		str.Format(_T("������׽��ּ���ʧ��: %d"), serverSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		serverSocket.Close();

		pThreadDlg->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
		return -1;
	}

	//����˳ɹ�������ʾ
	CString strTime;
	strTime.Format(_T("%.3d"), countRow + 1);
	pThreadDlg->m_msgListCtrl.InsertItem(countRow, strTime);
	CTime CurrentTime = CTime::GetCurrentTime();
	strTime.Format(_T("%.2d:%.2d:%.2d"), CurrentTime.GetHour(),  CurrentTime.GetMinute(), CurrentTime.GetSecond());
	pThreadDlg->m_msgListCtrl.SetItemText(countRow, 1, strTime);
	pThreadDlg->m_msgListCtrl.SetItemText(countRow, 2, _T("������"));
	pThreadDlg->m_msgListCtrl.SetItemText(countRow, 3, _T("�����������ɹ�!"));
	countRow++;

	//��ʼ����ѭ��
	while(1)
	{
		//��������socket
		CSocket workSocket;
		//�����ⲿ����
		if(!serverSocket.Accept(workSocket))
		{
			continue;
		}
		else
		{
			//��ȡ�ͻ���IP
			CString strConnClient = _T("");
			UINT port = 0;
			workSocket.GetPeerName(strConnClient, port);
			//�ͻ���������ʾ
			CString strTime;
			strTime.Format(_T("%.3d"), countRow + 1);
			pThreadDlg->m_msgListCtrl.InsertItem(countRow, strTime);
			CString strConnMsg = _T("�ͻ��� ") + strConnClient + _T("������!");
			CTime CurrentTime = CTime::GetCurrentTime();
			strTime.Format(_T("%.2d:%.2d:%.2d"), CurrentTime.GetHour(),  CurrentTime.GetMinute(), CurrentTime.GetSecond());
			pThreadDlg->m_msgListCtrl.SetItemText(countRow, 1, strTime);
			pThreadDlg->m_msgListCtrl.SetItemText(countRow, 2, _T("�ͻ���"));
			pThreadDlg->m_msgListCtrl.SetItemText(countRow, 3, strConnMsg);
			countRow++;

			//Ϊÿ���µ�socket���Ӵ����µĴ����߳�
			SOCKET newSocket = workSocket.Detach();		//�̼߳䴫��socket
			CWinThread* pSocketThread= AfxBeginThread(socketThreadFunc, (LPVOID)newSocket);
			if(NULL == pSocketThread)
			{
				//ʧ����ʾ
				CString strMsg;
				strMsg.Format(_T("������׽������̴߳���ʧ��: %d"), GetLastError());
				AfxMessageBox(strMsg, MB_OK);
			}
			else
			{
				//�ɹ���ʾ
				CString strTime;
				strTime.Format(_T("%.3d"), countRow + 1);
				pThreadDlg->m_msgListCtrl.InsertItem(countRow, strTime);
				CTime CurrentTime = CTime::GetCurrentTime();
				strTime.Format(_T("%.2d:%.2d:%.2d"), CurrentTime.GetHour(),  CurrentTime.GetMinute(), CurrentTime.GetSecond());
				pThreadDlg->m_msgListCtrl.SetItemText(countRow, 1, strTime);
				pThreadDlg->m_msgListCtrl.SetItemText(countRow, 2, _T("�ͻ���"));
				pThreadDlg->m_msgListCtrl.SetItemText(countRow, 3, _T("�ͻ����׽������̴߳����ɹ�!"));
				countRow++;
				//�ͷž����Դ
				//CloseHandle(pSocketThread->m_hThread);
			}
			//�ر�
			workSocket.Close();
		}
	}
	//return 0;
}

//Ϊÿ���µ�socket���Ӵ������̴߳�����
UINT socketThreadFunc(LPVOID pParam)
{
	AfxSocketInit(NULL);	//�����ʼ��
	//����socket
	CSocket serverSocket;
	SOCKET newSocket = (SOCKET)pParam;
	serverSocket.Attach(newSocket);

	//��Ӧ�ͻ���
	//���տͻ��˷�������Ϣ
	TCHAR asscMsg[1024];
	memset(asscMsg, 0, sizeof(asscMsg));
	serverSocket.Receive(asscMsg, sizeof(asscMsg), 0);
	//����Ƿ�ΪC-MOVE����
	LPCTSTR c_move_rq = _T("C-MOVE");
	LPCTSTR actual_rq = asscMsg;
	if(_tcscmp(c_move_rq, actual_rq) == 0)//��
	{
		////�������////
		//��ȡ�ļ�������
		fileMutex.Lock();
		//���ѡ��DCM�ļ����д���
		Sleep(100);
		srand((unsigned)time(NULL));
		int fileIndex = rand() % fileNameVec.size();

		//�����ļ����ͻ��ˣ�����ʽ
		char databuf[1024];
		DWORD dwRead = 0;
		BOOL bRet = FALSE;
		HANDLE hFile=CreateFile(fileNameVec.at(fileIndex), GENERIC_READ, 0, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, 0);
		while (1)
		{
			memset(databuf, 0, 1024); 
			bRet = ReadFile(hFile, databuf, 1024, &dwRead, NULL);
			if(bRet==FALSE)
			{   
				CString strMsg;
				strMsg.Format(_T("��ȡ�ļ�ʧ��: %d"), GetLastError());
				AfxMessageBox(strMsg, MB_OK);
				break;
			}
			if (dwRead==0)
				break;
			serverSocket.Send(databuf, dwRead, 0 );
		}
		CloseHandle(hFile);
		//�ͷ��ļ�������
		fileMutex.Unlock();
	}
	//else
	//	AfxMessageBox(_T("�������ʹ���!"));

	serverSocket.Close();
	return 0;
}