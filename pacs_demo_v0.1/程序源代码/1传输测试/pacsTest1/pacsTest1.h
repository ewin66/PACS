// pacsTest1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CpacsTest1App:
// �йش����ʵ�֣������ pacsTest1.cpp
//

class CpacsTest1App : public CWinApp
{
public:
	CpacsTest1App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
// �Զ������
public:
	
};

extern CpacsTest1App theApp;