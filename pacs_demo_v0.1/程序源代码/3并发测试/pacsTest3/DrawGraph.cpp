#include "StdAfx.h"
#include "DrawGraph.h"
#include "pacsTest3.h"			//Ӧ�ó���ͷ�ļ�
#include "pacsTest3Dlg.h"	//���Ի���ͷ�ļ�

DrawGraph::DrawGraph(void)
{
}

DrawGraph::~DrawGraph(void)
{
}

//���Ƴ�ʼ���
void DrawGraph::drawFrame()
{
	//��ȡ��ͼ������ָ��
	CpacsTest3Dlg* pMainWnd = (CpacsTest3Dlg*)(AfxGetApp()->GetMainWnd());
	CWnd* pWnd = pMainWnd->GetDlgItem(IDC_GRAPH);
	CRect rect;
	pWnd->GetClientRect(&rect);
	//pWnd->ClientToScreen(&rect);
	// ��ͼ����ʼ����
	x = rect.left;
	y = rect.top;
	// ��ͼ��ʼ��
	xpoint1 = x;
	ypoint1 = y + height;
	xpoint2 = x;
	ypoint2 = y + height;
	// ��ͼ����С
	width = rect.right - x;
	height = rect.bottom - y;
	// ���ƿ̶�������
	int nx = 20;	//����Ϊ20�ȷ�
	int ny = 20;	//����Ϊ20�ȷ�
	xinterval = width / nx; 
	yinterval = height / ny;
	//ˢ�»�ͼ��
	//pWnd->Invalidate();
	//pWnd->UpdateWindow();

	//��ȡ�豸������
	CDC* pDC = pWnd->GetDC();	
	// �������ʶ���
	CPen* pPenBlack = new CPen;
	// ������ɫ���ʣ����ڻ��������ᣩ
	pPenBlack->CreatePen(PS_SOLID, 1, RGB(0,0,0));
	// ѡ���ɫ����
	pDC->SelectObject(pPenBlack);

	//��������
	CFont newFont;
	VERIFY(newFont.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename 

	// ѡ���Զ������壬��������ǰ������
	CFont* oldFont = pDC->SelectObject(&newFont);
	pDC->SetTextColor(RGB(0, 0, 0));	//������ɫ
	pDC->SetTextAlign(TA_CENTER);	//��������

	// ����������
	// ����Y��
	pDC->MoveTo(x, y + height);
	pDC->LineTo(x, y );
	// ����X��
	pDC->MoveTo(x, y + height);
	pDC->LineTo(x + width, y + height);
	// ����������̶�
	// ����X��̶�ֵ
	for (int i = 0; i <= 20; i++)
	{
		CString strTemp;
		strTemp.Format(_T("%d"), i * 5);
		pDC->TextOut(i * xinterval, height + y + 2, strTemp);

		pDC->MoveTo(i * xinterval, height + y);
		pDC->LineTo(i * xinterval, height + y - 5);
	}
	// ����Y��̶�
	for (int i = 1; i <= 100; i++)
	{
		if(i % 5 == 0)
		{
			// ���Y��̶�
			pDC->MoveTo(x, height - i * yinterval / 5);
			pDC->LineTo(x + 8, height - i * yinterval / 5);
			// �ұ�Y��̶�
			pDC->MoveTo(x + width - 8, height - i * yinterval / 5);
			pDC->LineTo(x + width, height - i * yinterval / 5);
		}
		else
		{
			// ���Y��̶�
			pDC->MoveTo(x, height - i * yinterval / 5);
			pDC->LineTo(x + 5, height - i * yinterval / 5);
			// �ұ�Y��̶�
			pDC->MoveTo(x - 5 + width, height - i * yinterval / 5);
			pDC->LineTo(x + width, height - i * yinterval / 5);
		}
	}
	// ����Y��̶�ֵ
	for (int i = 0; i <= 20; i++)
	{
		//��߿̶�
		CString strTemp;
		strTemp.Format(_T("%d%%"), i * 5);	//����ٷֺ�����
		pDC->TextOut(x - 12, y + height - (i * yinterval + 4), strTemp);
		//�ұ߿̶�
		strTemp.Format(_T("%d ms"), i * 500);	//���ʱ�䣺ms
		pDC->TextOut(x + 20 + width , y + height - (i * yinterval + 4), strTemp);
	}

	//������ʾ
	//�������ʶ���
	CPen* pPenBlue = new CPen;
	// ������ɫ���ʣ����ڻ������ߣ�
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0,0,255));
	// ѡ����ɫ����
	pDC->SelectObject(pPenBlue);
	pDC->MoveTo(x + width - 100, y + 20);
	pDC->LineTo(x + width - 80, y + 20);
	pDC->TextOut(x + width - 50, y + 15, _T("����ʱ��"));
	//ˢ�»�ͼ��
	//pWnd->Invalidate();
	//pWnd->UpdateWindow();
	//�������ʶ���
	CPen* pPenRed = new CPen;
	// ������ɫ���ʣ����ڻ������ߣ�
	pPenRed->CreatePen(PS_SOLID, 1, RGB(255,0,0));
	// ѡ���ɫ����
	pDC->SelectObject(pPenRed);
	pDC->MoveTo(x + width - 100, y + 40);
	pDC->LineTo(x + width - 80, y + 40);
	pDC->TextOut(x + width - 50, y + 35, _T("��������"));

	// �ͷ�ͼ��ؼ���DC
	pMainWnd->ReleaseDC(pDC);
	// ɾ���µĻ���
	delete pPenBlack;
	delete pPenBlue;
	delete pPenRed;
}

//��ͼ
void DrawGraph::drawGraph(GraphData graphData)
//void CGraphDialog::drawGraph()
{
	//��ȡ��ͼ������ָ��
	CpacsTest3Dlg* pMainWnd = (CpacsTest3Dlg*)(AfxGetApp()->GetMainWnd());
	CWnd* pWnd = pMainWnd->GetDlgItem(IDC_GRAPH);
	//��ȡ�豸������
	CDC* pDC = pWnd->GetDC();	
	//ˢ�»�ͼ��
	//pWnd->Invalidate();
	//pWnd->UpdateWindow();

	//���ƴ���ʱ��仯����
	//�������ʶ���
	CPen* pPenBlue = new CPen;
	// ������ɫ���ʣ����ڻ������ߣ�
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0,0,255));
	// ѡ����ɫ����
	pDC->SelectObject(pPenBlue);
	double spentTime = graphData.efficiency;
	//��ʱ��ӳ�䵽ms�����ͼ
	if (spentTime>=10.0)	//��Ӧʱ�䳬��10S����10S��ͼ
		spentTime = 10.0;
	spentTime *=10;
	//��ͼ
	pDC->MoveTo(xpoint1, ypoint1);
	xpoint1 = x + graphData.clientID * xinterval / 5;
	ypoint1 = y + (100 - spentTime) * yinterval / 5;
	pDC->LineTo(xpoint1, ypoint1);

	//ˢ�»�ͼ��
	//pWnd->Invalidate();
	//pWnd->UpdateWindow();
	//���ƴ����ʣ������ʣ���1MB/sΪ��׼���仯����
	//�������ʶ���
	CPen* pPenRed = new CPen;
	// ������ɫ���ʣ����ڻ������ߣ�
	pPenRed->CreatePen(PS_SOLID, 1, RGB(255,0,0));
	// ѡ���ɫ����
	pDC->SelectObject(pPenRed);
	double transRate = graphData.transRate / 1024;	//��B/s�����kB/s
	//��������ӳ�䵽1MB/s�����ͼ
	if (transRate >= 1024.0)	//�����ٶȳ���1MB/s��1MB/s��ͼ
		transRate = 1024.0;
	transRate = transRate / 10.24;
	//��ͼ
	pDC->MoveTo(xpoint2, ypoint2);
	xpoint2 = x + graphData.clientID * xinterval / 5;
	ypoint2 = y + (100 - transRate) * yinterval / 5;
	pDC->LineTo(xpoint2, ypoint2);

	// �ͷ�ͼ��ؼ���DC
	pMainWnd->ReleaseDC(pDC);
	// ɾ���µĻ���
	delete pPenBlue;
	delete pPenRed;
}
