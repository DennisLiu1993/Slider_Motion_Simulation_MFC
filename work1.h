
// work1.h: PROJECT_NAME 應用程式的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 包含此檔案前先包含 'stdafx.h'"
#endif

#include "resource.h"		// 主要符號


// Cwork1App:
// 查看 work1.cpp 以了解此類別的實作
//

class Cwork1App : public CWinApp
{
public:
	Cwork1App();

// 覆寫
public:
	virtual BOOL InitInstance();

// 程式碼實作

	DECLARE_MESSAGE_MAP()
};

extern Cwork1App theApp;
