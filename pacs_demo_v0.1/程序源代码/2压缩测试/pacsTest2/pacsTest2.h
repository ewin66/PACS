// pacsTest2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CpacsTest2App:
// �йش����ʵ�֣������ pacsTest2.cpp
//

class CpacsTest2App : public CWinApp
{
public:
	CpacsTest2App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CpacsTest2App theApp;