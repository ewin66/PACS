// pacsTest3.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CpacsTest3App:
// �йش����ʵ�֣������ pacsTest3.cpp
//

class CpacsTest3App : public CWinApp
{
public:
	CpacsTest3App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CpacsTest3App theApp;