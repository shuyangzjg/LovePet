// ScrGenius.cpp : Defines the class behaviors for the application.
//
//������ƣ���ʥ
//���ߣ��쾰��
//���ܣ��ؼ�ע��������ʾ/�ָ����桢�ļ��ӽ��ܡ���ʱ�ػ���
//      �����廥ת����ȡ�Ǻ����롢BMPת�ı����ļ����ص�λͼ��
//      �ļ��ָ�������������ϵͳ�����(������...)
//
#include "stdafx.h"
#include "ScrGenius.h"
#include "ScrGeniusDlg.h"
#include "TransparentWnd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScrGeniusApp

BEGIN_MESSAGE_MAP(CScrGeniusApp, CWinApp)
	//{{AFX_MSG_MAP(CScrGeniusApp)	
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrGeniusApp construction

CScrGeniusApp::CScrGeniusApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CScrGeniusApp object

CScrGeniusApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CScrGeniusApp initialization

BOOL CScrGeniusApp::InitInstance()
{
	srand(time(NULL));
    
	//�û��������жϳ����Ƿ��������У������˳�
    HANDLE m_hMutex=CreateMutex(NULL,TRUE, m_pszAppName); 
    if(GetLastError()==ERROR_ALREADY_EXISTS) { return FALSE; }
    
	//���öԻ��򱳾����ı���ɫ
	SetDialogBkColor(RGB(240,200,90),RGB(0,0,0));

	CTransparentWnd* pFrame = new CTransparentWnd;
	m_pMainWnd = pFrame;
    
	// create and load the frame with its resources
	CRect rect(0, 0, 100, 100);
	pFrame->CreateTransparent("�͸���...", rect);//IDB_MASK, IDB_BACK);
	pFrame->CenterWindow();  //��ʼ��ʾ����λ��
	pFrame->ShowWindow(SW_SHOW);
	
	return TRUE;
}
