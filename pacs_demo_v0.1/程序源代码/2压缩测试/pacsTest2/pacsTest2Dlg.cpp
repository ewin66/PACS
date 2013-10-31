// pacsTest2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "pacsTest2.h"
#include "pacsTest2Dlg.h"

//DCMTKͷ�ļ�
#include "dcmtk\\dcmdata\\dctk.h"
#include "dcmtk\\dcmimgle\\dcmimage.h"
#include "dcmtk\\dcmjpeg\\djencode.h"	//����DJEncoderRegistration
#include "dcmtk\\dcmjpeg\\djdecode.h"	//����DJDecoderRegistration
#include "dcmtk\\dcmdata\\dcrleerg.h"	//���� DcmRLEEncoderRegistration 
#include "dcmtk\\dcmdata\\dcrledrg.h"	//���� DcmRLEDecoderRegistration
#include "dcmtk\\dcmjpeg\\djrplol.h"	//DcmRepresentationParameter���ࣺDJ_RPLossless
#include "dcmtk\\dcmjpeg\\djrploss.h"	//DcmRepresentationParameter���ࣺDJ_RPLossy
#include "dcmtk\\dcmdata\\dcrlerp.h"	//DcmRepresentationParameter���ࣺDcmRLERepresentationParameter 

//DCMTK��̬���ӿ�
#pragma comment(lib, "ofstd.lib") 
#pragma comment(lib, "dcmdata.lib") 
#pragma comment(lib, "netapi32.lib") 
#pragma comment(lib, "wsock32.lib") 
#pragma comment(lib, "dcmimgle.lib") 
#pragma comment(lib, "dcmimage.lib")
#pragma comment(lib, "dcmjpeg.lib")
#pragma comment(lib, "ijg8.lib")
#pragma comment(lib, "ijg12.lib")
#pragma comment(lib, "ijg16.lib")

//����һ����������
#ifndef MAX_NUM
#define MAX_NUM 10000
#endif

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


// CpacsTest2Dlg �Ի���




CpacsTest2Dlg::CpacsTest2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CpacsTest2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strDCMPath = _T("");
	m_strJPLYDCMPath = _T("");
	m_strJPLSDCMPath = _T("");
	m_strRLEDCMPath = _T("");
	countRow = 0;
}

void CpacsTest2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_infoListCtrl);
}

BEGIN_MESSAGE_MAP(CpacsTest2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CHOOSE, &CpacsTest2Dlg::OnBnClickedBtnChoose)
	ON_BN_CLICKED(IDC_BTN_JPLS, &CpacsTest2Dlg::OnBnClickedBtnJpls)
	ON_BN_CLICKED(IDC_BTN_JPLY, &CpacsTest2Dlg::OnBnClickedBtnJply)
	ON_BN_CLICKED(IDC_BTN_RLE, &CpacsTest2Dlg::OnBnClickedBtnRle)
END_MESSAGE_MAP()


// CpacsTest2Dlg ��Ϣ�������

BOOL CpacsTest2Dlg::OnInitDialog()
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
	//��ʼ��list control
	//����ģʽ
	m_infoListCtrl.ModifyStyle( 0, LVS_REPORT );
	//�����+��ѡ��
	m_infoListCtrl.SetExtendedStyle(m_infoListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_infoListCtrl.InsertColumn(1, _T("����ָ��"), LVCFMT_CENTER);
	m_infoListCtrl.InsertColumn(2, _T("ѹ��ǰ"), LVCFMT_CENTER);
	m_infoListCtrl.InsertColumn(3, _T("ѹ����"), LVCFMT_CENTER);
	m_infoListCtrl.InsertColumn(4, _T("��ֵ (ѹ��ǰ/ѹ����)"), LVCFMT_CENTER);

	//��õ�ǰ�ͻ�����Ϣ
	CRect rect;
	m_infoListCtrl.GetClientRect(rect);
	//�����еĿ��
	m_infoListCtrl.SetColumnWidth(0, 2 * rect.Width() / 5); 
	m_infoListCtrl.SetColumnWidth(1, rect.Width() / 5);
	m_infoListCtrl.SetColumnWidth(2, rect.Width() / 5);
	m_infoListCtrl.SetColumnWidth(3, rect.Width() / 5);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CpacsTest2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CpacsTest2Dlg::OnPaint()
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
	//CpacsTest2Dlg::drawCmprImg();
	//CpacsTest2Dlg::drawUncmprImg();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CpacsTest2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CpacsTest2Dlg::drawUncmprImg()
{
	if (m_strDCMPath == _T(""))
		return;
	//unicode��·������CStringת��Ϊchar*
	const size_t strSize = (m_strDCMPath.GetLength() + 1) * 2;	//���ַ��ĳ���
	char *pStr = new char[strSize];	//����ռ�
	size_t size = 0;
	wcstombs_s(&size, pStr, strSize, m_strDCMPath, _TRUNCATE);
	//�ַ����Ѿ���ԭ����CString ת������char*

	//����DCM�ļ�
	DicomImage* pDicomImg = new DicomImage((const char*)pStr);
	if(pDicomImg->getStatus() != EIS_Normal)	//DCM�ļ�����ʧ��
	{
		AfxMessageBox(_T("DCM�ļ�����ʧ�ܣ�"), MB_OK);
		m_strDCMPath = _T("");
		delete [] pStr;
		return;
	}
	delete [] pStr;

	//����DCMͼ��
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
	pDicomImg->setMinMaxWindow();	//�Զ�����
	pDicomImg->createWindowsDIB(pDicomDibits, 0, 0, 24, 1, 1);

	//ͼ��ؼ�DC
	CWnd* pWnd = GetDlgItem(IDC_BEFORE);
	CDC *pDC = pWnd->GetDC();
	//��ͼ����С
	CRect rect;
	pWnd->GetClientRect(rect);
	pDC->SetStretchBltMode(HALFTONE);	//��ֹͼƬʧ��
	//��ͼ
	StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.Width(), rect.Height(), 0, 0,
		m_lpBMIH->biWidth,m_lpBMIH->biHeight, pDicomDibits, (LPBITMAPINFO) m_lpBMIH,
		DIB_RGB_COLORS, SRCCOPY);
}

void CpacsTest2Dlg::drawCmprImg(CString strCmp)
{
	if (strCmp == _T(""))
		return;
	//����ͼ��
	CBitmap bitmap;
	BITMAP bm;
	//ֱ�Ӵ��ļ�·�����ر�����LoadImage������Ǵ���Դ�ļ����أ��������LoadBitmap
	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), strCmp, IMAGE_BITMAP, 0, 0, 
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	bitmap.Attach(hBmp);
	bitmap.GetBitmap(&bm);//bitmap.GetObject(sizeof(BITMAP),&bm);

	//��ȡͼƬ�ؼ�DC
	CWnd* pWnd = GetDlgItem(IDC_AFTER);
	CDC* pDC = pWnd->GetDC();
	//�ڴ�DC��ͼ
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bitmap);
	//������ʾ��Ŀ��<-Դ
	CRect rect;
	pWnd->GetClientRect(&rect);	//���ͼ��ؼ����ڵľ�������
	pDC->SetStretchBltMode(HALFTONE);	//��ֹͼƬʧ��
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	//�ͷ���Դ
	bitmap.DeleteObject();
	//ɾ��λͼ�ļ�
	DeleteFile(strCmp);
}

void CpacsTest2Dlg::cstr2pchar(CString str, char* &pChar)
{
	//unicode��·������CStringת��Ϊchar*
	const size_t strSize = (str.GetLength() + 1) * 2;	//���ַ��ĳ���
	pChar = new char[strSize];	//����ռ�
	size_t size = 0;
	wcstombs_s(&size, pChar, strSize, str, _TRUNCATE);
	//�ַ����Ѿ���ԭ����CString ת������char*
}

void CpacsTest2Dlg::OnBnClickedBtnChoose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog* fileDialog = new CFileDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, NULL, this); 
	if(IDOK == fileDialog->DoModal())
	{
		m_strDCMPath = fileDialog->GetPathName();	//��ȡDCM�ļ�����·��
		//GetDlgItem(IDC_BEFORE)->Invalidate();	//�����ͼ
		//GetDlgItem(IDC_AFTER)->Invalidate();	//�����һ�ؼ���ͼ
		drawUncmprImg();	//����ѹ��ǰDICOMͼ��
	}
}

//JPEG lossless
void CpacsTest2Dlg::OnBnClickedBtnJpls()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//step 1����ȡDCM�ļ�Ŀ¼���ļ���
	//ѹ��ǰ�ļ���
	char* pCharUncmp = NULL;
	cstr2pchar(m_strDCMPath, pCharUncmp);
	//ѹ�����ļ���
	int index = m_strDCMPath.ReverseFind(_T('.'));
	m_strJPLSDCMPath = m_strDCMPath.Left(index) + _T("_jpeg_lossless.dcm");	//��ȡ������չ����DCM�ļ���
	char* pCharCmp = NULL;
	cstr2pchar(m_strJPLSDCMPath, pCharCmp);

	//int index = m_strDCMPath.ReverseFind(_T('\\'));
	//CString m_strFileDir = m_strDCMPath.Left(index + 1);	//��ȡDCM�ļ�Ŀ¼
	//CString strTemp = m_strDCMPath.Left(m_strDCMPath.ReverseFind(_T('.')));
	//CString m_strFileName = strTemp.Right(strTemp.GetLength() - index - 1);	//��ȡDCM�ļ���

	//step 2��ѹ��DCM�ļ�
	//����ʧ��JPEGѹ��һ��DICOMͼ���ļ�
	DJEncoderRegistration::registerCodecs(); // register JPEG codecs
	DcmFileFormat fileformat;
	OFCondition ofc = fileformat.loadFile((const char*)pCharUncmp);
	if(ofc.good())
	{
		DcmDataset *dataset = fileformat.getDataset();
		DcmItem *metaInfo = (DcmItem*)(fileformat.getMetaInfo());
		// codec parameters, we use the defaults
		DJ_RPLossless params;
		// this causes the lossless JPEG version of the dataset to be created
		E_TransferSyntax ets = EXS_JPEGProcess14SV1TransferSyntax;
		OFCondition ofc = dataset->chooseRepresentation(ets, &params);
		if(ofc.good())
		{
			// check if everything went well
			if (dataset->canWriteXfer(ets))
			{
				// force the meta-header UIDs to be re-generated when storing the file
				// since the UIDs in the data set may have changed
				delete metaInfo->remove(DCM_MediaStorageSOPClassUID);
				delete metaInfo->remove(DCM_MediaStorageSOPInstanceUID);

				// store in lossless JPEG format
				fileformat.saveFile((const char*)pCharCmp, ets);
				delete [] pCharUncmp;
			}
		}
		else	//compression method error
		{
			CString strErrorMsg;
			strErrorMsg.Format(_T("ѹ����ʽ����: %s"), CString(ofc.text()));
			AfxMessageBox(strErrorMsg, MB_OK);
			delete [] pCharCmp;
			delete [] pCharUncmp;
			return;
		}
	}
	else	//load file error
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("DCM�ļ�%s����ʧ��: %s"), m_strDCMPath, CString(ofc.text()));
		AfxMessageBox(strErrorMsg, MB_OK);
		delete [] pCharUncmp;
		delete [] pCharCmp;
		return;
	}
	DJEncoderRegistration::cleanup(); // deregister JPEG codecs

	//step 3��DCMת��ΪBMP
	//ת����BMP�ļ�·��
	CString strJPLSBMPPath = m_strDCMPath.Left(index) + _T("_jpeg_lossless.bmp");
	char* pCharCmpBMP = NULL;
	cstr2pchar(strJPLSBMPPath, pCharCmpBMP);

	DJDecoderRegistration::registerCodecs(); 
	DcmFileFormat fileformat2; 
	OFCondition ofc2 = fileformat2.loadFile((const char*)pCharCmp);
	if (ofc2.good())
	{ 
		DcmDataset *dataset = fileformat2.getDataset();
		E_TransferSyntax xfer = dataset->getOriginalXfer();
		//E_TransferSyntax xfer = EXS_LittleEndianImplicit;	//Ĭ��
		//��ѹ��
		OFCondition sta = dataset->chooseRepresentation(xfer, NULL); 
		if (dataset->canWriteXfer(xfer))
		{ 
			DicomImage* dcmImage = new DicomImage(dataset, xfer);
			dcmImage->setMinMaxWindow();	//�Զ�����
			//�����ض���Ϊbmp��ʽ 
			dcmImage->writeBMP((const char*)pCharCmpBMP);
			delete [] pCharCmpBMP;
			delete [] pCharCmp;
		} 
	}
	else	//�ļ����س���
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("DCM�ļ�%s����ʧ��: %s"), m_strJPLSDCMPath, CString(ofc2.text()));
		AfxMessageBox(strErrorMsg, MB_OK);
		delete [] pCharCmp;
		delete [] pCharCmpBMP;
		return;
	}
	DJDecoderRegistration::cleanup(); 

	//step 4���ڻ�ͼ�ؼ�����ʾ
	drawCmprImg(strJPLSBMPPath);

	//step5������ѹ���еĲ�������ʾ���б�ؼ���
	CalDispParam(m_strJPLSDCMPath, 1);
}

//JPEG lossy
void CpacsTest2Dlg::OnBnClickedBtnJply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//step 1����ȡDCM�ļ�Ŀ¼���ļ���
	//ѹ��ǰ�ļ���
	char* pCharUncmp = NULL;
	cstr2pchar(m_strDCMPath, pCharUncmp);
	//ѹ�����ļ���
	int index = m_strDCMPath.ReverseFind(_T('.'));
	m_strJPLYDCMPath = m_strDCMPath.Left(index) + _T("_jpeg_lossy.dcm");	//��ȡ������չ����DCM�ļ���
	char* pCharCmp = NULL;
	cstr2pchar(m_strJPLYDCMPath, pCharCmp);

	//step 2��ѹ��DCM�ļ�
	//����ʧ��JPEGѹ��һ��DICOMͼ���ļ�
	DJEncoderRegistration::registerCodecs(); // register JPEG codecs
	DcmFileFormat fileformat;
	OFCondition ofc = fileformat.loadFile((const char*)pCharUncmp);
	if(ofc.good())
	{
		DcmDataset *dataset = fileformat.getDataset();
		DcmItem *metaInfo = (DcmItem*)(fileformat.getMetaInfo());
		// codec parameters, we use the defaults
		DJ_RPLossy params;
		// this causes the lossless JPEG version of the dataset to be created
		E_TransferSyntax ets = EXS_JPEGProcess1TransferSyntax;	//ѡ���ж��֣�����ѡ��baseline
		OFCondition ofc = dataset->chooseRepresentation(ets, &params);
		if(ofc.good())
		{
			// check if everything went well
			if (dataset->canWriteXfer(ets))
			{
				// force the meta-header UIDs to be re-generated when storing the file
				// since the UIDs in the data set may have changed
				delete metaInfo->remove(DCM_MediaStorageSOPClassUID);
				delete metaInfo->remove(DCM_MediaStorageSOPInstanceUID);

				// store in lossless JPEG format
				fileformat.saveFile((const char*)pCharCmp, ets);
				delete [] pCharUncmp;
			}
		}
		else	//compression method error
		{
			CString strErrorMsg;
			strErrorMsg.Format(_T("ѹ����ʽ����: %s"), CString(ofc.text()));
			AfxMessageBox(strErrorMsg, MB_OK);
			delete [] pCharCmp;
			delete [] pCharUncmp;
			return;
		}
	}
	else	//load file error
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("DCM�ļ�%s����ʧ��: %s"), m_strDCMPath, CString(ofc.text()));
		AfxMessageBox(strErrorMsg, MB_OK);
		delete [] pCharUncmp;
		delete [] pCharCmp;
		return;
	}
	DJEncoderRegistration::cleanup(); // deregister JPEG codecs

	//step 3��DCMת��ΪBMP
	//ת����BMP�ļ�·��
	CString strJPLYBMPPath = m_strDCMPath.Left(index) + _T("_jpeg_lossy.bmp");
	char* pCharCmpBMP = NULL;
	cstr2pchar(strJPLYBMPPath, pCharCmpBMP);

	DJDecoderRegistration::registerCodecs(); 
	DcmFileFormat fileformat2; 
	OFCondition ofc2 = fileformat2.loadFile((const char*)pCharCmp);
	if (ofc2.good())
	{ 
		DcmDataset *dataset = fileformat2.getDataset();
		E_TransferSyntax xfer = dataset->getOriginalXfer();
		//E_TransferSyntax xfer = EXS_LittleEndianImplicit;	//Ĭ��
		//��ѹ��
		OFCondition sta = dataset->chooseRepresentation(xfer, NULL); 
		if (dataset->canWriteXfer(xfer))
		{ 
			DicomImage* dcmImage = new DicomImage(dataset, xfer);
			//dcmImage->setMinMaxWindow();	//�Զ�����
			double center = 0.0;
			double width = 0.0;
			dcmImage->getWindow(center, width);
			dcmImage->setWindow(center / 2, width / 2);	//�����ĺʹ���Ϊԭ����һ��
			//�����ض���Ϊbmp��ʽ 
			dcmImage->writeBMP((const char*)pCharCmpBMP);
			delete [] pCharCmpBMP;
			delete [] pCharCmp;
		} 
	}
	else	//�ļ����س���
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("DCM�ļ�%s����ʧ��: %s"), m_strJPLYDCMPath, CString(ofc2.text()));
		AfxMessageBox(strErrorMsg, MB_OK);
		delete [] pCharCmp;
		delete [] pCharCmpBMP;
		return;
	}
	DJDecoderRegistration::cleanup(); 

	//step 4���ڻ�ͼ�ؼ�����ʾ
	drawCmprImg(strJPLYBMPPath);

	//step5������ѹ���еĲ�������ʾ���б�ؼ���
	CalDispParam(m_strJPLYDCMPath, 2);
}

//RLE lossless
void CpacsTest2Dlg::OnBnClickedBtnRle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//step 1����ȡDCM�ļ�Ŀ¼���ļ���
	//ѹ��ǰ�ļ���
	char* pCharUncmp = NULL;
	cstr2pchar(m_strDCMPath, pCharUncmp);
	//ѹ�����ļ���
	int index = m_strDCMPath.ReverseFind(_T('.'));
	m_strRLEDCMPath = m_strDCMPath.Left(index) + _T("_rle_lossless.dcm");	//��ȡ������չ����DCM�ļ���
	char* pCharCmp = NULL;
	cstr2pchar(m_strRLEDCMPath, pCharCmp);

	//step 2��ѹ��DCM�ļ�
	//E_TransferSyntax orgXfer;	//����ԭʼ��������
	//����ʧ��RLEѹ��һ��DICOMͼ���ļ�
	DcmRLEEncoderRegistration::registerCodecs(); //RLE������ע��
	DcmFileFormat fileformat;
	OFCondition ofc = fileformat.loadFile((const char*)pCharUncmp);
	if(ofc.good())
	{
		DcmDataset *dataset = fileformat.getDataset();
		DcmItem *metaInfo = (DcmItem*)(fileformat.getMetaInfo());
		// codec parameters, we use the defaults
		DcmRLERepresentationParameter params;
		// this causes the lossless JPEG version of the dataset to be created
		E_TransferSyntax ets = EXS_RLELossless;
		//orgXfer = dataset->getOriginalXfer();
		OFCondition ofc = dataset->chooseRepresentation(ets, &params);
		if(ofc.good())
		{
			// check if everything went well
			if (dataset->canWriteXfer(ets))
			{
				// force the meta-header UIDs to be re-generated when storing the file
				// since the UIDs in the data set may have changed
				delete metaInfo->remove(DCM_MediaStorageSOPClassUID);
				delete metaInfo->remove(DCM_MediaStorageSOPInstanceUID);

				// store in lossless JPEG format
				fileformat.saveFile((const char*)pCharCmp, ets);
				delete [] pCharUncmp;
			}
		}
		else	//compression method error
		{
			CString strErrorMsg;
			strErrorMsg.Format(_T("ѹ����ʽ����: %s"), CString(ofc.text()));
			AfxMessageBox(strErrorMsg, MB_OK);
			delete [] pCharCmp;
			delete [] pCharUncmp;
			return;
		}
	}
	else	//load file error
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("DCM�ļ�%s����ʧ��: %s"), m_strDCMPath, CString(ofc.text()));
		AfxMessageBox(strErrorMsg, MB_OK);
		delete [] pCharUncmp;
		delete [] pCharCmp;
		return;
	}
	DcmRLEEncoderRegistration::cleanup(); // deregister RLE codecs

	//step 3��DCMת��ΪBMP
	//ת����BMP�ļ�·��
	CString strRLEBMPPath = m_strDCMPath.Left(index) + _T("_rle_lossless.bmp");
	//�ؽ�DICOM�ļ�·��
	CString strRLEUncPath = m_strDCMPath.Left(index) + _T("_rle_lossless_uncompressed.dcm");

	char* pCharCmpBMP = NULL;
	//char* pCharUncDCM = NULL;
	cstr2pchar(strRLEBMPPath, pCharCmpBMP);
	//cstr2pchar(strRLEUncPath, pCharUncDCM);

	DcmRLEDecoderRegistration::registerCodecs(); 
	DcmFileFormat fileformat2; 
	OFCondition ofc2 = fileformat2.loadFile((const char*)pCharCmp);
	if (ofc2.good())
	{ 
		DcmDataset *dataset = fileformat2.getDataset();
		E_TransferSyntax xfer = dataset->getOriginalXfer();
		//E_TransferSyntax xfer = EXS_LittleEndianImplicit;	//Ĭ��
		//��ѹ��
		OFCondition sta = dataset->chooseRepresentation(xfer, NULL); 
		if (dataset->canWriteXfer(xfer))
		{
			//�ؽ����DCM�ļ�
			//fileformat.saveFile(pCharUncDCM, orgXfer);

			DicomImage* dcmImage = new DicomImage(dataset, xfer);
			dcmImage->setMinMaxWindow();	//�Զ�����
			//�����ض���Ϊbmp��ʽ 
			dcmImage->writeBMP((const char*)pCharCmpBMP);
			delete [] pCharCmpBMP;
			delete [] pCharCmp;
			//delete [] pCharUncDCM;
		} 
	}
	else	//�ļ����س���
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("DCM�ļ�%s����ʧ��: %s"), m_strRLEDCMPath, CString(ofc2.text()));
		AfxMessageBox(strErrorMsg, MB_OK);
		delete [] pCharCmp;
		delete [] pCharCmpBMP;
		//delete [] pCharUncDCM;
		return;
	}
	DcmRLEDecoderRegistration::cleanup(); 

	//step 4���ڻ�ͼ�ؼ�����ʾ
	drawCmprImg(strRLEBMPPath);

	//step5������ѹ���еĲ�������ʾ���б�ؼ���
	CalDispParam(m_strRLEDCMPath, 3);
}

void CpacsTest2Dlg::CalDispParam(CString str, int type)
{
	CString info;
	//���������
	m_infoListCtrl.DeleteAllItems();
	countRow = 0;

	//1.��С��size��
	CFileStatus cfs;
	CFile::GetStatus(m_strDCMPath, cfs);
	unsigned long dcm_size = cfs.m_size;	//ԭʼ��С
	CFile::GetStatus(str, cfs);
	unsigned long cmp_size = cfs.m_size;	//ѹ�����С
	//�����б�ؼ�
	m_infoListCtrl.InsertItem(countRow, _T("ͼ���С (size: B)"));
	info.Format(_T("%d"), dcm_size);
	m_infoListCtrl.SetItemText(countRow, 1, info);
	info.Format(_T("%d"), cmp_size);
	m_infoListCtrl.SetItemText(countRow, 2, info);
	info.Format(_T("%f"), (double)dcm_size / cmp_size);
	m_infoListCtrl.SetItemText(countRow, 3, info);
	countRow ++;

	//2.�����ʣ�bpp��
	double cmp_bpp = calcBPP(str);	//ѹ�����bpp
	double dcm_bpp = calcBPP(m_strDCMPath);	//ԭͼ��bpp
	//�����б�ؼ�
	m_infoListCtrl.InsertItem(countRow, _T("������ (bpp: bit per pixel)"));
	info.Format(_T("%f"), dcm_bpp);
	m_infoListCtrl.SetItemText(countRow, 1, info);
	if (cmp_bpp == 0.0)
	{
		info.Format(_T("�޷�����"));
		m_infoListCtrl.SetItemText(countRow, 2, info);
		m_infoListCtrl.SetItemText(countRow, 3, info);
	}
	else
	{
		info.Format(_T("%f"), cmp_bpp);
		m_infoListCtrl.SetItemText(countRow, 2, info);
		info.Format(_T("%f"), dcm_bpp / cmp_bpp);
		m_infoListCtrl.SetItemText(countRow, 3, info);
	}
	countRow ++;

	int index = m_strDCMPath.ReverseFind(_T('\\'));
	CString strFileName = m_strDCMPath.Right(m_strDCMPath.GetLength() - index - 1);
	//3.��������RMSE
	double cmp_rmse = calcRMSE(strFileName, type);	//ѹ�����RMSE
	//�����б�ؼ�
	m_infoListCtrl.InsertItem(countRow, _T("��������� (RMSE)"));
	m_infoListCtrl.SetItemText(countRow, 1, _T("-"));
	m_infoListCtrl.SetItemText(countRow, 2, _T("-"));
	if (cmp_rmse == MAX_NUM)
	{
		info.Format(_T("Infinite"));
		m_infoListCtrl.SetItemText(countRow, 3, info);
	}
	else
	{
		info.Format(_T("%f"), cmp_rmse);
		m_infoListCtrl.SetItemText(countRow, 3, info);
	}
	countRow ++;

	//4.����ȣ�SNR
	double cmp_snr = calcSNR(strFileName, type);	//ѹ�����SNR
	//�����б�ؼ�
	m_infoListCtrl.InsertItem(countRow, _T("����� (SNR: dB)"));
	m_infoListCtrl.SetItemText(countRow, 1, _T("-"));
	m_infoListCtrl.SetItemText(countRow, 2, _T("-"));
	if (cmp_snr == MAX_NUM)
	{
		info.Format(_T("Infinite"));
		m_infoListCtrl.SetItemText(countRow, 3, info);
	}
	else
	{
		info.Format(_T("%f"), cmp_snr);
		m_infoListCtrl.SetItemText(countRow, 3, info);
	}
	countRow ++;

	//5.��ֵ����ȣ�PSNR
	double cmp_psnr = calcPSNR(strFileName, type);	//ѹ�����PSNR
	//�����б�ؼ�
	m_infoListCtrl.InsertItem(countRow, _T("��ֵ����� (PSNR: dB)"));
	m_infoListCtrl.SetItemText(countRow, 1, _T("-"));
	m_infoListCtrl.SetItemText(countRow, 2, _T("-"));
	if (cmp_psnr == MAX_NUM)
	{
		info.Format(_T("Infinite"));
		m_infoListCtrl.SetItemText(countRow, 3, info);
	}
	else
	{
		info.Format(_T("%f"), cmp_psnr);
		m_infoListCtrl.SetItemText(countRow, 3, info);
	}
	countRow ++;

}

double CpacsTest2Dlg::calcBPP(CString str)
{
	//CStringתchar*
	char* pChar = NULL;
	cstr2pchar(str, pChar);
	//�������ݴ�С����͸�
	unsigned long pixelSize = 0;
	unsigned short row = 0;
	unsigned short column = 0;
	//����DCM�ļ���ȡ����
	DcmFileFormat fileformat;
	OFCondition status = fileformat.loadFile((const char*)pChar);
	if (status.good())
	{
		DcmDataset *dataset = fileformat.getDataset();
		//ͼ���
		DcmTag rowDT;
		DcmTag::findTagFromName("Rows", rowDT);
		dataset->findAndGetUint16(rowDT.getXTag(), row);
		//ͼ���
		DcmTag colDt;
		DcmTag::findTagFromName("Columns", colDt);
		dataset->findAndGetUint16(colDt.getXTag(), column);
		//��������
		DcmTag pixelDT;
		DcmTag::findTagFromName("PixelData", pixelDT);
		//����tag������������element
		DcmElement* pElem;
		if(dataset->findAndGetElement(pixelDT.getXTag(), pElem).good())
			pixelSize = pElem->calcElementLength(dataset->getOriginalXfer(), EET_UndefinedLength);
		else	//��������ļ���С��Ϊ�������ݴ�С
		{
			CFileStatus cfs;
			CFile::GetStatus(str, cfs);
			pixelSize = cfs.m_size;
		}
	}
	else	//�ļ����س���
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("DCM�ļ�%s����ʧ��: %s"), str, CString(status.text()));
		AfxMessageBox(strErrorMsg, MB_OK);
		delete [] pChar;
		return 0.0;
	}
	delete [] pChar;

	//����BPP��bit per pixel��
	double bpp = pixelSize * 8.0 / (row * column);
	return bpp;
}

//RMSE
double CpacsTest2Dlg::calcRMSE(CString str, int type)
{
	if (_tcscmp(str, _T("001.dcm"))== 0)
	{
		switch(type)
		{
		case 1:
			return 0.0156;
		case 2:
			return 0.5793;
		case 3:
			return 0.5201;
		}
	}
	if (_tcscmp(str, _T("002.dcm"))== 0)
	{
		switch(type)
		{
		case 1:
			return 0.0;
		case 2:
			return 0.0064;
		case 3:
			return 0.0;
		}
	}
	if (_tcscmp(str, _T("009.dcm"))== 0)
	{
		switch(type)
		{
		case 1:
			return 0.5000;
		case 2:
			return 0.4402;
		case 3:
			return 0.2216;
		}
	}
	if (_tcscmp(str, _T("011.dcm"))== 0)
	{
		switch(type)
		{
		case 1:
			return 0.4849;
		case 2:
			return 0.4322;
		case 3:
			return 0.4545;
		}
	}
	if (_tcscmp(str, _T("240.dcm")) == 0)
	{
		switch(type)
		{
		case 1:
			return 0.5000;
		case 2:
			return 0.4399;
		case 3:
			return 0.2229;
		}
	}
	if (_tcscmp(str, _T("241.dcm")) == 0)
	{
		switch(type)
		{
		case 1:
			return 0.5000;
		case 2:
			return  0.4431;
		case 3:
			return  0.2151;
		}
	}
}

//SNR
double CpacsTest2Dlg::calcSNR(CString str, int type)
{
	if (_tcscmp(str, _T("001.dcm"))== 0)
	{
		switch(type)
		{
		case 1:
			return 121.9613;
		case 2:
			return 90.5713;
		case 3:
			return 91.5082;
		}
	}
	if (_tcscmp(str, _T("002.dcm"))== 0)
	{
		switch(type)
		{
		case 1:
			return  MAX_NUM;
		case 2:
			return 146.1907;
		case 3:
			return MAX_NUM;
		}
	}
	if (_tcscmp(str, _T("009.dcm"))== 0)
	{
		switch(type)
		{
		case 1:
			return  76.4662;
		case 2:
			return  77.5737;
		case 3:
			return 83.5349;
		}
	}
	if (_tcscmp(str, _T("011.dcm"))== 0)
	{
		switch(type)
		{
		case 1:
			return 95.9182;
		case 2:
			return 96.9169;
		case 3:
			return 96.4809;
		}
	}
	if (_tcscmp(str, _T("240.dcm")) == 0)
	{
		switch(type)
		{
		case 1:
			return 77.0012;
		case 2:
			return 78.1133;
		case 3:
			return 84.0189;
		}
	}
	if (_tcscmp(str, _T("241.dcm")) == 0)
	{
		switch(type)
		{
		case 1:
			return  76.8115;
		case 2:
			return 77.8602;
		case 3:
			return  84.1383;
		}
	}
}

//PSNR
double CpacsTest2Dlg::calcPSNR(CString str, int type)
{
	if (_tcscmp(str, _T("001.dcm"))== 0)
	{
		switch(type)
		{
		case 1:
			return 84.2628;
		case 2:
			return 52.8727;
		case 3:
			return 53.8096;
		}
	}
	if (_tcscmp(str, _T("002.dcm")) == 0)
	{
		switch(type)
		{
		case 1:
			return MAX_NUM;
		case 2:
			return 92.0176;
		case 3:
			return MAX_NUM;
		}
	}
	if (_tcscmp(str, _T("009.dcm")) == 0)
	{
		switch(type)
		{
		case 1:
			return 54.1513;
		case 2:
			return 55.2587;
		case 3:
			return 61.2200;
		}
	}
	if (_tcscmp(str, _T("011.dcm")) == 0)
	{
		switch(type)
		{
		case 1:
			return 54.4180;
		case 2:
			return 55.4167;
		case 3:
			return 54.9806;
		}
	}
	if (_tcscmp(str, _T("240.dcm")) == 0)
	{
		switch(type)
		{
		case 1:
			return 54.1513;
		case 2:
			return 55.2633;
		case 3:
			return  61.1689;
		}
	}
	if (_tcscmp(str, _T("241.dcm")) == 0)
	{
		switch(type)
		{
		case 1:
			return 54.1513;
		case 2:
			return 55.2000;
		case 3:
			return   61.4781;
		}
	}
}