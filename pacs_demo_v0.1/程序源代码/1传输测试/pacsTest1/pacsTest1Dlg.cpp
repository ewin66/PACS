// pacsTest1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "pacsTest1.h"
#include "pacsTest1Dlg.h"	//���Ի���ͷ�ļ�
#include "ConfDlg.h"	//�������öԻ���ͷ�ļ�
#include "Protocol.h"	//Э��ͷ�ļ�
#include "afxmt.h"	//CMutex������ļ�
#include "Shlwapi.h"	//����Ŀ¼���ͷ�ļ�

//DCMTKͷ�ļ�
#include "dcmtk\\dcmdata\\dctk.h"
#include "dcmtk\\dcmimgle\\dcmimage.h"

//DCMTK��̬���ӿ�
#pragma comment(lib, "ofstd.lib") 
#pragma comment(lib, "dcmdata.lib") 
#pragma comment(lib, "netapi32.lib") 
#pragma comment(lib, "wsock32.lib") 
#pragma comment(lib, "dcmimgle.lib") 
#pragma comment(lib, "dcmimage.lib")

//ȫ�ֱ�������
unsigned int totalImgNum = 0;	//�����ͼƬ����Ŀ
unsigned int succImgNum = 0;	//����ɹ���ͼƬ��Ŀ
unsigned int succThreadNum = 0;	//�ѽ������߳���Ŀ
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


// CpacsTest1Dlg �Ի���




CpacsTest1Dlg::CpacsTest1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CpacsTest1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strDCMPath = _T(".\\Client");
	m_strIP = _T("");
}

void CpacsTest1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO_LIST1, m_infoListCtrl1);
	DDX_Control(pDX, IDC_INFO_LIST2, m_infoListCtrl2);
	DDX_Control(pDX, IDC_INFO_LIST3, m_infoListCtrl3);
}

BEGIN_MESSAGE_MAP(CpacsTest1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CONFIGURE, &CpacsTest1Dlg::OnBnClickedBtnConfigure)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CpacsTest1Dlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_TEST, &CpacsTest1Dlg::OnBnClickedBtnTest)
END_MESSAGE_MAP()


// CpacsTest1Dlg ��Ϣ�������

BOOL CpacsTest1Dlg::OnInitDialog()
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

	//��ʼ����Ϣ��ʾ�б�ؼ�
	initListCtrl(&m_infoListCtrl1);
	initListCtrl(&m_infoListCtrl2);
	initListCtrl(&m_infoListCtrl3);
	//����ͼ����ʾ�ؼ�����Ϣ�б�ؼ�
	rstImgAndListCtrl();

	//�ж�DCM�ļ�Ŀ¼�Ƿ����
	if (!PathIsDirectory(m_strDCMPath))
		CreateDirectory(m_strDCMPath, NULL);	//�������򴴽�

	//��������ǰ�޷�����
	GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CpacsTest1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CpacsTest1Dlg::OnPaint()
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
HCURSOR CpacsTest1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��ʼ��ͼ����Ϣ�б�ؼ�
void CpacsTest1Dlg::initListCtrl(CListCtrl* listCtrl)
{
	//����ģʽ
	listCtrl->ModifyStyle( 0, LVS_REPORT );
	//�����+��ѡ��
	listCtrl->SetExtendedStyle(listCtrl->GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	listCtrl->InsertColumn(1, _T("[���, Ԫ�غ�]"), LVCFMT_CENTER);
	listCtrl->InsertColumn(2, _T("��������"), LVCFMT_CENTER);
	listCtrl->InsertColumn(3, _T("ֵ"), LVCFMT_CENTER);
	//��õ�ǰ�ͻ�����Ϣ
	CRect rect;
	listCtrl->GetClientRect(rect);
	//�����еĿ��
	listCtrl->SetColumnWidth(0, rect.Width() / 3 + 5); 
	listCtrl->SetColumnWidth(1, rect.Width() / 3);
	listCtrl->SetColumnWidth(2, rect.Width() / 3);
}

//����ͼ����ʾ�ؼ�����Ϣ�б�ؼ�
void CpacsTest1Dlg::rstImgAndListCtrl()
{
	//�����ͼ�ؼ�����
	GetDlgItem(IDC_SHOW_IMG1)->Invalidate();
	GetDlgItem(IDC_SHOW_IMG2)->Invalidate();
	GetDlgItem(IDC_SHOW_IMG3)->Invalidate();
	//����б�ؼ�����
	m_infoListCtrl1.DeleteAllItems();
	m_infoListCtrl2.DeleteAllItems();
	m_infoListCtrl3.DeleteAllItems();
}

//�����������
void CpacsTest1Dlg::OnBnClickedBtnConfigure()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CConfDlg confDlg;
	if (IDOK == confDlg.DoModal())
	{
		m_strIP = confDlg.m_strIPConf;
		GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
	}
}

//��ֹͨ��esc�����رնԻ���
BOOL CpacsTest1Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)   
	{   
		pMsg->wParam = 0;
	}    
	return CDialog::PreTranslateMessage(pMsg);
}
//��ֹͨ���س����رնԻ���
void CpacsTest1Dlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	//CDialog::OnOK();
}
//�˳���ť
void CpacsTest1Dlg::OnBnClickedBtnExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}

//�������
void CpacsTest1Dlg::OnBnClickedBtnTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�����ͻ����߳�
	CWinThread* m_pThread1 = AfxBeginThread(socketThreadFunc1, (LPVOID)this);
	CWinThread* m_pThread2 = AfxBeginThread(socketThreadFunc2, (LPVOID)this);
	CWinThread* m_pThread3 = AfxBeginThread(socketThreadFunc3, (LPVOID)this);
	//ʧ����ʾ
	unsigned int countFail = 0;
	if(NULL == m_pThread1)
	{
		AfxMessageBox(_T("�ͻ����׽������߳�#1����ʧ��!"), MB_OK);
		countFail ++;
	}
	if(NULL == m_pThread2)
	{
		AfxMessageBox(_T("�ͻ����׽������߳�#2����ʧ��!"), MB_OK);
		countFail ++;
	}
	if(NULL == m_pThread3)
	{
		AfxMessageBox(_T("�ͻ����׽������߳�#3����ʧ��!"), MB_OK);
		countFail ++;
	}
	//�����̴߳�����ʧ��
	if (countFail == DCM_NUM)
		return;
	////�ȴ��߳̽�����10s��ʱ
	//WaitForSingleObject(m_pThread1->m_hThread, 10000);
	//WaitForSingleObject(m_pThread2->m_hThread, 10000);
	//WaitForSingleObject(m_pThread3->m_hThread, 10000);
	////�ͷž����Դ
	//CloseHandle(m_pThread1->m_hThread);
	//CloseHandle(m_pThread2->m_hThread);
	//CloseHandle(m_pThread3->m_hThread);
	////�ͷ��ڴ�
	//delete m_pThread1;
	//m_pThread1 = NULL;
	//delete m_pThread2;
	//m_pThread2 = NULL;
	//delete m_pThread3;
	//m_pThread3 = NULL;

	//����ͼ����б�
	rstImgAndListCtrl();
	//������߳�����0
	succThreadNum = 0;
	//��ֹ���ε��
	GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(FALSE);
}

//�ͻ���socket�̺߳���1
UINT socketThreadFunc1(LPVOID pParam)
{
	//��ȡ���Ի���ָ��
	CpacsTest1Dlg* pThreadDlg = (CpacsTest1Dlg*)pParam;
	ASSERT(pThreadDlg);
	//��ʼ��socket������Ĳ���
	AfxSocketInit(NULL);

	//��Ǳ������жϽ��յ��ļ��Ƿ�ͬʱ���㣺1.����ɹ���2.�洢����DICOM��׼��
	unsigned int uiSucc = 0;

	//step 1�������ͻ����׽���
	CSocket clientSocket;
	//�������ԣ�ͬһ���˿�
	if(!clientSocket.Create(0, SOCK_STREAM))	//��0��ɹ�
	{
		CString str;
		str.Format(_T("�ͻ����׽��ִ���ʧ��: %d"), clientSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		clientSocket.Close();
		pThreadDlg->GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
		return -1;
	}
	//�����������
	if(!clientSocket.Connect(pThreadDlg->m_strIP, PRE_AGREED_PORT)) //��0��ɹ�
	{
		CString str;
		str.Format(_T("�ͻ����׽�������ʧ��: %d"), clientSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		clientSocket.Close();
		pThreadDlg->GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
		return -1;
	}
	//��������
	LPCTSTR c_move_req = _T("C-MOVE");
	if(!clientSocket.Send(c_move_req, _tcslen(c_move_req) * 2, 0))	//�ֽڳ���
	{
		CString str;
		str.Format(_T("�ͻ����׽��ַ���ʧ��: %d"), clientSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		clientSocket.Close();
		pThreadDlg->GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
		return -1;
	}

	//step 2��ȷ��д���ļ���
	CString fileName = _T("");
	//��ȡ���ݻ�����
	dataMutex.Lock();
	//ͼƬ������
	totalImgNum++;
	//д���ļ���
	fileName.Format(_T("%s\\retrieval%d.dcm"), pThreadDlg->m_strDCMPath, totalImgNum);
	//�ͷ����ݻ�����
	dataMutex.Unlock();

	//step 3�����շ���˷�����Ϣ
	TCHAR databuf[1024];
	//DWORD dwTotalWrite = 0;	//�ܵ�д���ַ�����
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
		//dwTotalWrite += dwWrite;	//�ܵ�д���ַ�����
		if(read <= 0)
		{
			//��Ǳ���������ɹ���1
			uiSucc ++;	
			break;
		}
		if(bRet == FALSE)
		{
			CString strMsg;
			strMsg.Format(_T("д���ļ�ʧ��: %d"), GetLastError());
			AfxMessageBox(strMsg, MB_OK);
			break;
		} 
	}
	//�ر��ļ����
	CloseHandle(hFile);
	//����ͨ�Ž������ر�socket
	clientSocket.Close();

	//�ļ�����CStringת��Ϊ��const��char*
	const size_t strSize = (fileName.GetLength() + 1)*2;	//���ַ��ĳ���
	char *pStr = new char[strSize];	//����ռ�
	size_t size = 0;
	wcstombs_s(&size, pStr, strSize, fileName, _TRUNCATE);

	//step 4����ȡDCM�ļ���Ϣ
	OFCondition status;
	DcmFileFormat fileformat;
	status = fileformat.loadFile((const char*)pStr, EXS_Unknown, EGL_noChange, DCM_MaxReadLength);
	int countRow = 0;	//�б�ؼ��м���
	if (status.good())
	{
		//�ļ�ͷ��metainfo
		DcmMetaInfo *metainfo = fileformat.getMetaInfo();
		DcmObject* pDO = metainfo->nextInContainer(NULL);
		if (pDO != NULL)
		{
			//��Ǳ������洢����DICOM��׼��1
			uiSucc ++;
			do 
			{
				DcmTag dt = pDO->getTag();	//��ȡtag
				OFString value;
				metainfo->findAndGetOFString(dt.getXTag(), value);	//����tagkey��ȡtag��value
				//��ʾ��list control
				CString strTag;
				strTag.Format(_T("[%.4x, %.4x]"), dt.getGTag(), dt.getETag());
				pThreadDlg->m_infoListCtrl1.InsertItem(countRow, strTag);	//����ţ�Ԫ�غţ�
				CString strTagName(dt.getTagName());
				pThreadDlg->m_infoListCtrl1.SetItemText(countRow, 1, strTagName);	//tag������
				CString strTagValue(value.c_str());
				pThreadDlg->m_infoListCtrl1.SetItemText(countRow, 2, strTagValue);	//tag��ֵ��
				//��һ��
				pDO = metainfo->nextInContainer(pDO);
				countRow ++;
			} while (pDO != NULL);
		}
		else	//metainfoΪ��
		{
			pThreadDlg->m_infoListCtrl1.InsertItem(countRow, _T("NULL"));	//NULL
			pThreadDlg->m_infoListCtrl1.SetItemText(countRow, 1, _T("NULL"));	//NULL
			pThreadDlg->m_infoListCtrl1.SetItemText(countRow, 2, _T("NULL"));	//NULL
			countRow ++;
		}

		//�ļ�ͷ��dataset
		DcmDataset *dataset = fileformat.getDataset();
		pDO = dataset->nextInContainer(NULL);
		if (pDO != NULL)
		{	
			do 
			{
				DcmTag dt = pDO->getTag();	//��ȡtag
				OFString value;
				dataset->findAndGetOFString(dt.getXTag(), value);	//����tagkey��ȡtag��value
				//��ʾ��list control
				CString strTag;
				strTag.Format(_T("[%.4x, %.4x]"), dt.getGTag(), dt.getETag());
				pThreadDlg->m_infoListCtrl1.InsertItem(countRow, strTag);	//����ţ�Ԫ�غţ�
				CString strTagName(dt.getTagName());
				pThreadDlg->m_infoListCtrl1.SetItemText(countRow, 1, strTagName);	//tag������
				CString strTagValue(value.c_str());
				pThreadDlg->m_infoListCtrl1.SetItemText(countRow, 2, strTagValue);	//tag��ֵ��
				//��һ��
				pDO = dataset->nextInContainer(pDO);
				countRow ++;
			} while (pDO != NULL);
		}
	}
	else	//�޷����ļ�
	{
		pThreadDlg->m_infoListCtrl1.InsertItem(countRow, _T("�޷�"));
		pThreadDlg->m_infoListCtrl1.SetItemText(countRow, 1, _T("��"));
		pThreadDlg->m_infoListCtrl1.SetItemText(countRow, 2, _T("�ļ���"));
		countRow ++;
	}

	//step 5����ʾDCM�ļ�
	//�Ƿ�ͬʱ���㣺1.����ɹ���2.�洢����DICOM��׼
	if (uiSucc == 2)	//�������ͼ
	{
		//��ȡͼ������
		DicomImage* pDicomImg = new DicomImage((const char*)pStr);
		LPBITMAPINFOHEADER m_lpBMIH;
		m_lpBMIH = (LPBITMAPINFOHEADER) new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
		m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
		m_lpBMIH->biWidth = pDicomImg->getWidth();
		m_lpBMIH->biHeight = pDicomImg->getHeight();
		m_lpBMIH->biPlanes = 1;
		m_lpBMIH->biBitCount = 24;
		m_lpBMIH->biCompression = BI_RGB;
		m_lpBMIH->biSizeImage = 0;
		m_lpBMIH->biXPelsPerMeter = 0;
		m_lpBMIH->biYPelsPerMeter = 0;
		void* pDicomDibits;
		pDicomImg->setMinMaxWindow();
		pDicomImg->createWindowsDIB(pDicomDibits, 0, 0, 24, 1, 1);
		//���ͼ��ؼ��ľ��
		CWnd *pWnd = pThreadDlg->GetDlgItem(IDC_SHOW_IMG1);
		//���ͼ��ؼ���DC
		CDC *pDC = pWnd->GetDC();
		//��ȡͼƬ�ؼ��Ŀ�͸�
		CRect rect;
		pWnd->GetClientRect(&rect);
		pDC->SetStretchBltMode(HALFTONE);	//��ֹͼƬʧ��
		//��ͼ
		StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.Width(), rect.Height(), 0, 0,
			m_lpBMIH->biWidth,m_lpBMIH->biHeight, pDicomDibits, (LPBITMAPINFO) m_lpBMIH,
			DIB_RGB_COLORS, SRCCOPY);
	}
	delete [] pStr;

	//step 6���ж��Ƿ���Խ�����һ�δ������
	//��ȡ���ݻ�����
	dataMutex.Lock();
	//���Գɹ���ͼƬ��Ŀ����
	if (uiSucc == 2)
		succImgNum ++;	
	//������߳���Ŀ����
	succThreadNum ++;
	//��������
	if (succThreadNum == DCM_NUM)
	{
		//����ť���
		pThreadDlg->GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
		//������ʾ
		pThreadDlg->SetDlgItemInt(IDC_EDIT_SUCCESS, succImgNum);
		pThreadDlg->SetDlgItemInt(IDC_EDIT_FAIL, totalImgNum - succImgNum);
		CString strRatio;
		strRatio.Format(_T("%.2f %%"), (double)succImgNum / totalImgNum * 100);
		pThreadDlg->SetDlgItemText(IDC_EDIT_RATIO, strRatio);
	}
	//�ͷ����ݻ�����
	dataMutex.Unlock();
	return 0;
}

//�ͻ���socket�̺߳���2
UINT socketThreadFunc2(LPVOID pParam)
{
	//��ȡ���Ի���ָ��
	CpacsTest1Dlg* pThreadDlg = (CpacsTest1Dlg*)pParam;
	ASSERT(pThreadDlg);
	//��ʼ��socket������Ĳ���
	AfxSocketInit(NULL);

	//��Ǳ������жϽ��յ��ļ��Ƿ�ͬʱ���㣺1.����ɹ���2.�洢����DICOM��׼��
	unsigned int uiSucc = 0;

	//step 1�������ͻ����׽���
	CSocket clientSocket;
	//�������ԣ�ͬһ���˿�
	if(!clientSocket.Create(0, SOCK_STREAM))	//��0��ɹ�
	{
		CString str;
		str.Format(_T("�ͻ����׽��ִ���ʧ��: %d"), clientSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		clientSocket.Close();
		pThreadDlg->GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
		return -1;
	}
	//�����������
	if(!clientSocket.Connect(pThreadDlg->m_strIP, PRE_AGREED_PORT)) //��0��ɹ�
	{
		CString str;
		str.Format(_T("�ͻ����׽�������ʧ��: %d"), clientSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		clientSocket.Close();
		pThreadDlg->GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
		return -1;
	}
	//��������
	LPCTSTR c_move_req = _T("C-MOVE");
	if(!clientSocket.Send(c_move_req, _tcslen(c_move_req) * 2, 0))	//�ֽڳ���
	{
		CString str;
		str.Format(_T("�ͻ����׽��ַ���ʧ��: %d"), clientSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		clientSocket.Close();
		pThreadDlg->GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
		return -1;
	}

	//step 2��ȷ��д���ļ���
	CString fileName = _T("");
	//��ȡ���ݻ�����
	dataMutex.Lock();
	//ͼƬ������
	totalImgNum++;
	//д���ļ���
	fileName.Format(_T("%s\\retrieval%d.dcm"), pThreadDlg->m_strDCMPath, totalImgNum);
	//�ͷ����ݻ�����
	dataMutex.Unlock();

	//step 3�����շ���˷�����Ϣ
	TCHAR databuf[1024];
	//DWORD dwTotalWrite = 0;	//�ܵ�д���ַ�����
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
		//dwTotalWrite += dwWrite;	//�ܵ�д���ַ�����
		if(read <= 0)
		{
			//��Ǳ���������ɹ���1
			uiSucc ++;	
			break;
		}
		if(bRet == FALSE)
		{
			CString strMsg;
			strMsg.Format(_T("д���ļ�ʧ��: %d"), GetLastError());
			AfxMessageBox(strMsg, MB_OK);
			break;
		} 
	}
	//�ر��ļ����
	CloseHandle(hFile);
	//����ͨ�Ž������ر�socket
	clientSocket.Close();

	//�ļ�����CStringת��Ϊ��const��char*
	const size_t strSize = (fileName.GetLength() + 1)*2;	//���ַ��ĳ���
	char *pStr = new char[strSize];	//����ռ�
	size_t size = 0;
	wcstombs_s(&size, pStr, strSize, fileName, _TRUNCATE);

	//step 4����ȡDCM�ļ���Ϣ
	OFCondition status;
	DcmFileFormat fileformat;
	status = fileformat.loadFile((const char*)pStr, EXS_Unknown, EGL_noChange, DCM_MaxReadLength);
	int countRow = 0;	//�б�ؼ��м���
	if (status.good())
	{
		//�ļ�ͷ��metainfo
		DcmMetaInfo *metainfo = fileformat.getMetaInfo();
		DcmObject* pDO = metainfo->nextInContainer(NULL);
		if (pDO != NULL)
		{
			//��Ǳ������洢����DICOM��׼��1
			uiSucc ++;
			do 
			{
				DcmTag dt = pDO->getTag();	//��ȡtag
				OFString value;
				metainfo->findAndGetOFString(dt.getXTag(), value);	//����tagkey��ȡtag��value
				//��ʾ��list control
				CString strTag;
				strTag.Format(_T("[%.4x, %.4x]"), dt.getGTag(), dt.getETag());
				pThreadDlg->m_infoListCtrl2.InsertItem(countRow, strTag);	//����ţ�Ԫ�غţ�
				CString strTagName(dt.getTagName());
				pThreadDlg->m_infoListCtrl2.SetItemText(countRow, 1, strTagName);	//tag������
				CString strTagValue(value.c_str());
				pThreadDlg->m_infoListCtrl2.SetItemText(countRow, 2, strTagValue);	//tag��ֵ��
				//��һ��
				pDO = metainfo->nextInContainer(pDO);
				countRow ++;
			} while (pDO != NULL);
		}
		else	//metainfoΪ��
		{
			pThreadDlg->m_infoListCtrl2.InsertItem(countRow, _T("NULL"));	//NULL
			pThreadDlg->m_infoListCtrl2.SetItemText(countRow, 1, _T("NULL"));	//NULL
			pThreadDlg->m_infoListCtrl2.SetItemText(countRow, 2, _T("NULL"));	//NULL
			countRow ++;
		}

		//�ļ�ͷ��dataset
		DcmDataset *dataset = fileformat.getDataset();
		pDO = dataset->nextInContainer(NULL);
		if (pDO != NULL)
		{	
			do 
			{
				DcmTag dt = pDO->getTag();	//��ȡtag
				OFString value;
				dataset->findAndGetOFString(dt.getXTag(), value);	//����tagkey��ȡtag��value
				//��ʾ��list control
				CString strTag;
				strTag.Format(_T("[%.4x, %.4x]"), dt.getGTag(), dt.getETag());
				pThreadDlg->m_infoListCtrl2.InsertItem(countRow, strTag);	//����ţ�Ԫ�غţ�
				CString strTagName(dt.getTagName());
				pThreadDlg->m_infoListCtrl2.SetItemText(countRow, 1, strTagName);	//tag������
				CString strTagValue(value.c_str());
				pThreadDlg->m_infoListCtrl2.SetItemText(countRow, 2, strTagValue);	//tag��ֵ��
				//��һ��
				pDO = dataset->nextInContainer(pDO);
				countRow ++;
			} while (pDO != NULL);
		}
	}
	else	//�޷����ļ�
	{
		pThreadDlg->m_infoListCtrl2.InsertItem(countRow, _T("�޷�"));
		pThreadDlg->m_infoListCtrl2.SetItemText(countRow, 1, _T("��"));
		pThreadDlg->m_infoListCtrl2.SetItemText(countRow, 2, _T("�ļ���"));
		countRow ++;
	}

	//step 5����ʾDCM�ļ�
	//�Ƿ�ͬʱ���㣺1.����ɹ���2.�洢����DICOM��׼
	if (uiSucc == 2)	//�������ͼ
	{
		//��ȡͼ������
		DicomImage* pDicomImg = new DicomImage((const char*)pStr);
		LPBITMAPINFOHEADER m_lpBMIH;
		m_lpBMIH = (LPBITMAPINFOHEADER) new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
		m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
		m_lpBMIH->biWidth = pDicomImg->getWidth();
		m_lpBMIH->biHeight = pDicomImg->getHeight();
		m_lpBMIH->biPlanes = 1;
		m_lpBMIH->biBitCount = 24;
		m_lpBMIH->biCompression = BI_RGB;
		m_lpBMIH->biSizeImage = 0;
		m_lpBMIH->biXPelsPerMeter = 0;
		m_lpBMIH->biYPelsPerMeter = 0;
		void* pDicomDibits;
		pDicomImg->setMinMaxWindow();
		pDicomImg->createWindowsDIB(pDicomDibits, 0, 0, 24, 1, 1);
		//���ͼ��ؼ��ľ��
		CWnd *pWnd = pThreadDlg->GetDlgItem(IDC_SHOW_IMG2);
		//���ͼ��ؼ���DC
		CDC *pDC = pWnd->GetDC();
		//��ȡͼƬ�ؼ��Ŀ�͸�
		CRect rect;
		pWnd->GetClientRect(&rect);
		pDC->SetStretchBltMode(HALFTONE);	//��ֹͼƬʧ��
		//��ͼ
		StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.Width(), rect.Height(), 0, 0,
			m_lpBMIH->biWidth,m_lpBMIH->biHeight, pDicomDibits, (LPBITMAPINFO) m_lpBMIH,
			DIB_RGB_COLORS, SRCCOPY);
	}
	delete [] pStr;

	//step 6���ж��Ƿ���Խ�����һ�δ������
	//��ȡ���ݻ�����
	dataMutex.Lock();
	//���Գɹ���ͼƬ��Ŀ����
	if (uiSucc == 2)
		succImgNum ++;	
	//������߳���Ŀ����
	succThreadNum ++;
	//��������
	if (succThreadNum == DCM_NUM)
	{
		//����ť���
		pThreadDlg->GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
		//������ʾ
		pThreadDlg->SetDlgItemInt(IDC_EDIT_SUCCESS, succImgNum);
		pThreadDlg->SetDlgItemInt(IDC_EDIT_FAIL, totalImgNum - succImgNum);
		CString strRatio;
		strRatio.Format(_T("%.2f %%"), (double)succImgNum / totalImgNum * 100);
		pThreadDlg->SetDlgItemText(IDC_EDIT_RATIO, strRatio);
	}
	//�ͷ����ݻ�����
	dataMutex.Unlock();
	return 0;
}

//�ͻ���socket�̺߳���3
UINT socketThreadFunc3(LPVOID pParam)
{
	//��ȡ���Ի���ָ��
	CpacsTest1Dlg* pThreadDlg = (CpacsTest1Dlg*)pParam;
	ASSERT(pThreadDlg);
	//��ʼ��socket������Ĳ���
	AfxSocketInit(NULL);

	//��Ǳ������жϽ��յ��ļ��Ƿ�ͬʱ���㣺1.����ɹ���2.�洢����DICOM��׼��
	unsigned int uiSucc = 0;

	//step 1�������ͻ����׽���
	CSocket clientSocket;
	//�������ԣ�ͬһ���˿�
	if(!clientSocket.Create(0, SOCK_STREAM))	//��0��ɹ�
	{
		CString str;
		str.Format(_T("�ͻ����׽��ִ���ʧ��: %d"), clientSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		clientSocket.Close();
		pThreadDlg->GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
		return -1;
	}
	//�����������
	if(!clientSocket.Connect(pThreadDlg->m_strIP, PRE_AGREED_PORT)) //��0��ɹ�
	{
		CString str;
		str.Format(_T("�ͻ����׽�������ʧ��: %d"), clientSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		clientSocket.Close();
		pThreadDlg->GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
		return -1;
	}
	//��������
	LPCTSTR c_move_req = _T("C-MOVE");
	if(!clientSocket.Send(c_move_req, _tcslen(c_move_req) * 2, 0))	//�ֽڳ���
	{
		CString str;
		str.Format(_T("�ͻ����׽��ַ���ʧ��: %d"), clientSocket.GetLastError());
		AfxMessageBox(str, MB_OK);
		clientSocket.Close();
		pThreadDlg->GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
		return -1;
	}

	//step 2��ȷ��д���ļ���
	CString fileName = _T("");
	//��ȡ���ݻ�����
	dataMutex.Lock();
	//ͼƬ������
	totalImgNum++;
	//д���ļ���
	fileName.Format(_T("%s\\retrieval%d.dcm"), pThreadDlg->m_strDCMPath, totalImgNum);
	//�ͷ����ݻ�����
	dataMutex.Unlock();

	//step 3�����շ���˷�����Ϣ
	TCHAR databuf[1024];
	//DWORD dwTotalWrite = 0;	//�ܵ�д���ַ�����
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
		//dwTotalWrite += dwWrite;	//�ܵ�д���ַ�����
		if(read <= 0)
		{
			//��Ǳ���������ɹ���1
			uiSucc ++;	
			break;
		}
		if(bRet == FALSE)
		{
			CString strMsg;
			strMsg.Format(_T("д���ļ�ʧ��: %d"), GetLastError());
			AfxMessageBox(strMsg, MB_OK);
			break;
		} 
	}
	//�ر��ļ����
	CloseHandle(hFile);
	//����ͨ�Ž������ر�socket
	clientSocket.Close();

	//�ļ�����CStringת��Ϊ��const��char*
	const size_t strSize = (fileName.GetLength() + 1)*2;	//���ַ��ĳ���
	char *pStr = new char[strSize];	//����ռ�
	size_t size = 0;
	wcstombs_s(&size, pStr, strSize, fileName, _TRUNCATE);

	//step 4����ȡDCM�ļ���Ϣ
	OFCondition status;
	DcmFileFormat fileformat;
	status = fileformat.loadFile((const char*)pStr, EXS_Unknown, EGL_noChange, DCM_MaxReadLength);
	int countRow = 0;	//�б�ؼ��м���
	if (status.good())
	{
		//�ļ�ͷ��metainfo
		DcmMetaInfo *metainfo = fileformat.getMetaInfo();
		DcmObject* pDO = metainfo->nextInContainer(NULL);
		if (pDO != NULL)
		{
			//��Ǳ������洢����DICOM��׼��1
			uiSucc ++;
			do 
			{
				DcmTag dt = pDO->getTag();	//��ȡtag
				OFString value;
				metainfo->findAndGetOFString(dt.getXTag(), value);	//����tagkey��ȡtag��value
				//��ʾ��list control
				CString strTag;
				strTag.Format(_T("[%.4x, %.4x]"), dt.getGTag(), dt.getETag());
				pThreadDlg->m_infoListCtrl3.InsertItem(countRow, strTag);	//����ţ�Ԫ�غţ�
				CString strTagName(dt.getTagName());
				pThreadDlg->m_infoListCtrl3.SetItemText(countRow, 1, strTagName);	//tag������
				CString strTagValue(value.c_str());
				pThreadDlg->m_infoListCtrl3.SetItemText(countRow, 2, strTagValue);	//tag��ֵ��
				//��һ��
				pDO = metainfo->nextInContainer(pDO);
				countRow ++;
			} while (pDO != NULL);
		}
		else	//metainfoΪ��
		{
			pThreadDlg->m_infoListCtrl3.InsertItem(countRow, _T("NULL"));	//NULL
			pThreadDlg->m_infoListCtrl3.SetItemText(countRow, 1, _T("NULL"));	//NULL
			pThreadDlg->m_infoListCtrl3.SetItemText(countRow, 2, _T("NULL"));	//NULL
			countRow ++;
		}

		//�ļ�ͷ��dataset
		DcmDataset *dataset = fileformat.getDataset();
		pDO = dataset->nextInContainer(NULL);
		if (pDO != NULL)
		{	
			do 
			{
				DcmTag dt = pDO->getTag();	//��ȡtag
				OFString value;
				dataset->findAndGetOFString(dt.getXTag(), value);	//����tagkey��ȡtag��value
				//��ʾ��list control
				CString strTag;
				strTag.Format(_T("[%.4x, %.4x]"), dt.getGTag(), dt.getETag());
				pThreadDlg->m_infoListCtrl3.InsertItem(countRow, strTag);	//����ţ�Ԫ�غţ�
				CString strTagName(dt.getTagName());
				pThreadDlg->m_infoListCtrl3.SetItemText(countRow, 1, strTagName);	//tag������
				CString strTagValue(value.c_str());
				pThreadDlg->m_infoListCtrl3.SetItemText(countRow, 2, strTagValue);	//tag��ֵ��
				//��һ��
				pDO = dataset->nextInContainer(pDO);
				countRow ++;
			} while (pDO != NULL);
		}
	}
	else	//�޷����ļ�
	{
		pThreadDlg->m_infoListCtrl3.InsertItem(countRow, _T("�޷�"));
		pThreadDlg->m_infoListCtrl3.SetItemText(countRow, 1, _T("��"));
		pThreadDlg->m_infoListCtrl3.SetItemText(countRow, 2, _T("�ļ���"));
		countRow ++;
	}

	//step 5����ʾDCM�ļ�
	//�Ƿ�ͬʱ���㣺1.����ɹ���2.�洢����DICOM��׼
	if (uiSucc == 2)	//�������ͼ
	{
		//��ȡͼ������
		DicomImage* pDicomImg = new DicomImage((const char*)pStr);
		LPBITMAPINFOHEADER m_lpBMIH;
		m_lpBMIH = (LPBITMAPINFOHEADER) new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
		m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
		m_lpBMIH->biWidth = pDicomImg->getWidth();
		m_lpBMIH->biHeight = pDicomImg->getHeight();
		m_lpBMIH->biPlanes = 1;
		m_lpBMIH->biBitCount = 24;
		m_lpBMIH->biCompression = BI_RGB;
		m_lpBMIH->biSizeImage = 0;
		m_lpBMIH->biXPelsPerMeter = 0;
		m_lpBMIH->biYPelsPerMeter = 0;
		void* pDicomDibits;
		pDicomImg->setMinMaxWindow();
		pDicomImg->createWindowsDIB(pDicomDibits, 0, 0, 24, 1, 1);
		//���ͼ��ؼ��ľ��
		CWnd *pWnd = pThreadDlg->GetDlgItem(IDC_SHOW_IMG3);
		//���ͼ��ؼ���DC
		CDC *pDC = pWnd->GetDC();
		//��ȡͼƬ�ؼ��Ŀ�͸�
		CRect rect;
		pWnd->GetClientRect(&rect);
		pDC->SetStretchBltMode(HALFTONE);	//��ֹͼƬʧ��
		//��ͼ
		StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.Width(), rect.Height(), 0, 0,
			m_lpBMIH->biWidth,m_lpBMIH->biHeight, pDicomDibits, (LPBITMAPINFO) m_lpBMIH,
			DIB_RGB_COLORS, SRCCOPY);
	}
	delete [] pStr;

	//step 6���ж��Ƿ���Խ�����һ�δ������
	//��ȡ���ݻ�����
	dataMutex.Lock();
	//���Գɹ���ͼƬ��Ŀ����
	if (uiSucc == 2)
		succImgNum ++;	
	//������߳���Ŀ����
	succThreadNum ++;
	//��������
	if (succThreadNum == DCM_NUM)
	{
		//����ť���
		pThreadDlg->GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
		pThreadDlg->GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
		//������ʾ
		pThreadDlg->SetDlgItemInt(IDC_EDIT_SUCCESS, succImgNum);
		pThreadDlg->SetDlgItemInt(IDC_EDIT_FAIL, totalImgNum - succImgNum);
		CString strRatio;
		strRatio.Format(_T("%.2f %%"), (double)succImgNum / totalImgNum * 100);
		pThreadDlg->SetDlgItemText(IDC_EDIT_RATIO, strRatio);
	}
	//�ͷ����ݻ�����
	dataMutex.Unlock();
	return 0;
}