*********************************************************************
/*
Modified by �쾰�� 2000.9
���ܣ���̬͸��λͼ��ʾ��WAV��Դ�ļ�����
*/

#include "stdafx.h"
#include "TransparentWnd.h"
#include <assert.h>
#include "ScrGenius.h"
#include "ScrGeniusDlg.h"
#include "BCMenu.h"  //���λͼ�˵�
#include <afxpriv.h> //��ӶԻ���ؼ��ĸ�����Ӧ
#include "GetIP.h"
#include "BMP2TXT.H"
#include "About.h"
#include "CustomWavFile.h"
#include "Big2GB.h"
#include "HideInBMP.h"
#include "FileSplit.h"
#include "RegOCX.h"
#include "EDStar.h"
#include "SetTimeShutdown.h"
#include "AnimEffect.h" //�Ի��򶯻�Ч����ʾ

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BCMenu popmenu;  //����λͼ�˵�����

#define WM_TRAYNOTIFY WM_USER + 100  //�Զ���ϵͳ������Ϣ����

CWnd * pFrame  ;//�õ���ǰ����ָ��
CCustomWavFile *pCustomWavFile; //���Ŷ����黰��ָ��

bool bWavPlay ; //�������ű�־λ
bool bDragDrop; //�϶���־λ
CPoint CurPoint; //��¼��ǰ���λ��
bool bShow=false; //������ʾ�������ر�־λ�������ж��Ƿ���д��嶯��
bool bMute=false; //�黰������־λ
bool bEject=true; //�������ǹرչ�����־λ
CString strWavFileName=""; //��lovepet.dat�����ļ��еõ��Զ����黰�ļ�������
CString strTextDescription="";//��������������.
//CString strTextPlay="��������"; //����������ʾ��������Ϣ
//********************************************************************************
//* Constructor
//********************************************************************************

CTransparentWnd::CTransparentWnd()
{
	bMute=false;
	m_iAniSeq=0;
//	m_fontLogo.CreateFont(13, 0, 90, 0, FW_NORMAL, 0, 0,0,0,0,0,0,0, "����");
}


//********************************************************************************
//* Destructor
//********************************************************************************

CTransparentWnd::~CTransparentWnd()
{
}


BEGIN_MESSAGE_MAP(CTransparentWnd, CWnd)
	//{{AFX_MSG_MAP(CTransparentWnd)
	ON_MESSAGE(WM_TRAYNOTIFY, OnTrayNotification)  //�Զ���ϵͳ������Ϣ���� 
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOVING()
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDR_SHOW, OnShow)
	ON_COMMAND(IDR_REGISTER, OnRegister)
	ON_COMMAND(IDR_SHUTDOWN, OnShutdown)
	ON_COMMAND(IDR_EJECT, OnEject)
	ON_COMMAND(IDR_EXIT, OnExit)
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_COMMAND(IDR_MUTE, OnMute)
	ON_UPDATE_COMMAND_UI(IDR_MUTE, OnUpdateMute)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_COMMAND(IDR_GETIP, OnGetip)
	ON_COMMAND(IDR_BMP2TXT, OnBmp2txt)
	ON_COMMAND(IDR_HELP, OnHelp)
	ON_COMMAND(IDR_CUSTOMWAVFILE, OnCustomwavfile)
	ON_COMMAND(IDR_BIG2GB, OnBig2gb)
	ON_COMMAND(IDR_HIDEINBMP, OnHideinbmp)
	ON_WM_MOUSEMOVE()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDR_EDSTAR, OnEdstar)
	ON_COMMAND(IDR_REGOCX, OnRegocx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CTransparentWnd::OnKickIdle(WPARAM wparam, LPARAM lparam)
	{
		UpdateDialogControls(this, false);
		return 0;
	}
//********************************************************************************
//* CreateTransparent()
//*
//* Creates the main application window transparent
//********************************************************************************
void CTransparentWnd::CreateTransparent(LPCTSTR pTitle, RECT &rect)
{
	// ����һ�����ش���
	CreateEx(   0,  
		AfxRegisterWndClass(0,AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
				pTitle,
				WS_POPUP ,
				rect,
				NULL,
				NULL,
			    NULL);

   StartBitmap(); //��ʾ��ʼõ��ͼ��

}

//********************************************************************************
//* SetupRegion()
//*
//* Set the Window Region for transparancy outside the mask region
//********************************************************************************
void CTransparentWnd::SetupRegion(CDC *pDC)
{
	CDC					memDC;
	CBitmap			&cBitmap=m_bmpDraw;
	CBitmap*		pOldMemBmp = NULL;
	COLORREF		col,colMask;
	CRect				cRect;
	int					x, y;
	CRgn				wndRgn, rgnTemp;

	GetWindowRect(&cRect);
	CPoint ptOrg=cRect.TopLeft();

	BITMAP bmInfo;
	cBitmap.GetObject(sizeof(bmInfo),&bmInfo);
	CRect rcNewWnd=CRect(ptOrg,CSize(bmInfo.bmWidth,bmInfo.bmHeight));

	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);
	colMask=memDC.GetPixel(0,0);

	wndRgn.CreateRectRgn(0, 0, rcNewWnd.Width(), rcNewWnd.Height());
	for(x=0; x<=rcNewWnd.Width(); x++)
	{
		for(y=0; y<=rcNewWnd.Height(); y++)
		{
			col = memDC.GetPixel(x, y);
			if(col == colMask)
			{
				rgnTemp.CreateRectRgn(x, y, x+1, y+1);
				wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_XOR);
				rgnTemp.DeleteObject();	
			}
		}
	}
	if (pOldMemBmp) memDC.SelectObject(pOldMemBmp);
	SetWindowRgn((HRGN)wndRgn, TRUE);
	MoveWindow(rcNewWnd);
}

//WAV��Ƶ�ļ�����
void CTransparentWnd::SoundPlay(void)
{
  CString strTemp; 

	if(bMute) //��ѡ�����黰����ʱ����������
	{
	}
	else if(pCustomWavFile->GetOriginPlayFlag() ) //��ѡ�лָ���ť,�򲥷�ԭʼ�黰 
	{
    
	//��ʼ��ʱ���ж��Ƿ����û��Զ��Ƶ��黰�����ļ�(���ڴˣ���Ϊ�˷������黰�ļ��Ƿ����)
	CStdioFile TxtFile;
    //���黰�����ļ�
    if (TxtFile.Open("lovepet.dat", CFile::modeRead|CFile::typeText))
    {
	//�����û��Զ��Ƶ��黰�ļ���������·�������ڵ�ǰ·��Ѱ�Ҳ���������
    //������������Ϣ.
    TxtFile.ReadString (strTemp);
	//�ַ���0���𣬹ʲ���1 ,�������Ӧ�ַ�����Ϣ
	strWavFileName=strTemp.Left(strTemp.Find("$$$"));
	if(strTemp.Find( "$$$")>=0)
	  strTextDescription=strTemp.Right(strTemp.GetLength()-(strTemp.Find("$$$")+3));
	TxtFile.Close();

	//�ж��û��Զ��Ƶ��黰�����ļ��Ƿ��ڵ�ǰ·��
	if(!TxtFile.Open(strWavFileName,CFile::modeRead))
	{
	 //	CString strErrorMessage;
	//	strErrorMessage = "�Զ����黰�ļ�:" + strWavFileName +"�����ڻ򲻺�������(lovepet.exe)��һ��!";
	//	AfxMessageBox(strErrorMessage);
	 
        strWavFileName="" ; //�黰�����ļ������ڣ��������ֵ		
	}
	else
		TxtFile.Close (); //�ļ����ڣ���ر���������Ქ��������
   }
   else
      strWavFileName="" ; //�黰�����ļ������ڣ��������ֵ
	
    if(strWavFileName!="") //�黰���ݲ�Ϊ�ղ����Զ����黰�ļ������򲥷���
	{
		SetTimer(2,5000,NULL); //�����Զ����黰���ŵ�ʱ��Ϊ5��
	   //����API��������
    	sndPlaySound(strWavFileName,SND_ASYNC|SND_NODEFAULT  ); 
	}
	else if(bWavPlay)  //���򣬲���ԭʼ��ʥ�黰һ
	{    //�ȹر�ԭ��������
		PlaySound("IDR_WAVE1",AfxGetResourceHandle(),SND_RESOURCE|SND_PURGE|SND_NODEFAULT  ); 
    	SetTimer(2,10700,NULL); //���õ�һ�β�������ʱ������Ϣ��ʱ��10.7��
	   //��ԴWAV�ļ���ID���˫���ţ�����API��������
    	PlaySound("IDR_WAVE1",AfxGetResourceHandle(),SND_RESOURCE|SND_ASYNC|SND_NODEFAULT  ); 
	}
	else  //���򣬲���ԭʼ��ʥ�黰��
	{
		 //�ȹر�ԭ��������
		PlaySound("IDR_WAVE2",AfxGetResourceHandle(),SND_RESOURCE|SND_PURGE|SND_NODEFAULT  ); 
      	SetTimer(2,12000,NULL); //���õڶ��β�������ʱ������Ϣ��ʱ��12����
	   //��ԴWAV�ļ���ID���˫���ţ�����API��������
    	PlaySound("IDR_WAVE2",AfxGetResourceHandle(),SND_RESOURCE|SND_ASYNC |SND_NODEFAULT  ); 
	}
	}
	else  //���Ŷ����黰
	{
		//�����黰�ļ�һ��Ϊ�գ��򲥷�ԭʼ�黰�ļ�һ
      if(pCustomWavFile->GetWaveFile ().IsEmpty ()) 
	  { //�ȹر�ԭ��������
		PlaySound("IDR_WAVE1",AfxGetResourceHandle(),SND_RESOURCE|SND_PURGE|SND_NODEFAULT  ); 
    	SetTimer(2,10700,NULL); //���õ�һ�β�������ʱ������Ϣ��ʱ��10.7��
	   //��ԴWAV�ļ���ID���˫���ţ�����API��������
    	PlaySound("IDR_WAVE1",AfxGetResourceHandle(),SND_RESOURCE|SND_ASYNC|SND_NODEFAULT  ); 
	  }
	  else //��Ϊ�գ��򲥷��Զ����黰�ļ�
	  {  
    	SetTimer(2,5000,NULL); //�����Զ����黰���ŵ�ʱ��Ϊ5��
	   //����API��������
    	sndPlaySound(pCustomWavFile->GetWaveFile(),SND_ASYNC|SND_NODEFAULT  ); 
	  }

	}
		
}

//��ʼ��ʾõ��ͼ��
void CTransparentWnd::StartBitmap(void)
{
	char szBmp[20];
    CString strTemp; 

    sprintf(szBmp,"ROSE");
	m_bmpDraw.DeleteObject();
	m_bmpDraw.LoadBitmap(szBmp);
	CWindowDC dc(this);
	SetupRegion(&dc);
	Invalidate();

	SoundPlay(); //������Ӧ��WAV�ļ�
}

void CTransparentWnd::Reset(void)
{
    Idle();
	SoundPlay(); //������Ӧ��WAV�ļ�
}

//����WAV�ļ�ʱ��ʾ�Ķ�̬ͼ��
void CTransparentWnd::Idle(void)
{
  char szBmp[20];

    sprintf(szBmp,"WEST1");
	m_bmpDraw.DeleteObject();
	m_bmpDraw.LoadBitmap(szBmp);
	CWindowDC dc(this);
	SetupRegion(&dc);
	Invalidate();
}

void CTransparentWnd::DoSet(void)
{
	int iSW=GetSystemMetrics(SM_CXFULLSCREEN);
	int iSH=GetSystemMetrics(SM_CYFULLSCREEN);

	KillTimer(1);
	SetTimer(2,600,NULL);  //����ͼ��ı���ʱ��

}

//��ȡ�û��Զ������������ַ���
CString CTransparentWnd::GetTextDescription()
{
  return strTextDescription;
}

void CTransparentWnd::DoWalk(void)
{
	int iSW=GetSystemMetrics(SM_CXFULLSCREEN);
	int iSH=GetSystemMetrics(SM_CYFULLSCREEN);

	CRect rcW;
	GetWindowRect(rcW);

	SetTimer(2,600,NULL);  //�ָ�ԭͼ��ı�ʱ�ļ��ʱ��

	if(rcW.left<50) xcounter=10;
	if(rcW.top<50) ycounter=10;
	if( rcW.left <50) m_iLastDirection=1;  //�����ƶ�
	
	if(rcW.left>iSW-50) xcounter=-10;
	if(rcW.top>iSH-50) ycounter=-10;
    if(rcW.left >iSW-50) m_iLastDirection=0; //�����ƶ�
	
	CPoint ptOffset(xcounter,ycounter);
 	rcW+=ptOffset;
//	MoveWindow(rcW);  //�ƶ�����

	char szBmp[20];
  
	if(bDragDrop) //����ʵʱ�϶�ʱ��ʾ��ͼ��	 
      sprintf(szBmp,"GRIL");
    else if(m_iLastDirection==1) //���������ƶ�ʱ��ʾ��ͼ��
	{
	  if((m_iAniSeq%24)>16) //ͼ��̬�Ķ�һ��
	    sprintf(szBmp,"WALK%d",m_iAniSeq%8+5); //����ʱͼ��(��˸�)
	  else
	  {
		sprintf(szBmp,"WALK%d",m_iAniSeq%4+1); //��ʱͼ��
		MoveWindow(rcW);//Ϊ�˶�ͼ��ʱ���ƶ�����(ǰ�ĸ�)
	  }
	}
	else//���������ƶ�ʱ��ʾ��ͼ��
	{
      if((m_iAniSeq%24)>16)
        sprintf(szBmp,"WALK%d",m_iAniSeq%8+17); //����ʱͼ��(��˸�)
	  else
	  {
		sprintf(szBmp,"WALK%d",m_iAniSeq%4+13); //��ʱͼ��
	    MoveWindow(rcW);//Ϊ�˶�ͼ��ʱ���ƶ�����(ǰ�ĸ�)
	  }
	}

	m_bmpDraw.DeleteObject();
	m_bmpDraw.LoadBitmap(szBmp);
	CWindowDC dc(this);
	SetupRegion(&dc);
	Invalidate();
}


//********************************************************************************
//* CTransparentWnd message handlers
//********************************************************************************

void CTransparentWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);

    SetTimer(3,60,NULL);  //�϶�ʱͬʱ��ʾ����ļ�����3
	//����ƶ�������ʱ�ı����ָ��
	::SetCursor(AfxGetApp()->LoadCursor(IDC_DROP));
	SetCapture(); //�������ȫ����׽
    CurPoint=point; //���°���ʱ���������

	//ʵ���ޱ����϶�
//	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y)); 
}

int CTransparentWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

     	//ϵͳ����̬��ʾ��ͼ���ļ�
	m_hIcon[0]=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    
	//����ϵͳ��ͼ��
	if (!m_TrayIcon.Create(this, 
				IDR_POPUP, 
				_T("��ʥV3.0"), 
				m_hIcon, 
				1, //һ��ͼ��
				1000, //��ʱ1��仯
				WM_TRAYNOTIFY))
	{
		AfxMessageBox(_T("���󣺴���ϵͳͼ��ʧ�ܣ�"), MB_OK | MB_ICONSTOP);
		return -1;
	}

	xcounter=10,ycounter=10;
	m_iLastDirection=1;  //�����ܶ��ķ����־λ����ʼΪ����
	SetTimer(1,3000,NULL);  //��ʼʱ��õ��ͼ����ʾʱ��3��
	bWavPlay=true;  //������ʼ���ű�־λ��ʼΪ��
	bDragDrop=false;  //�����϶���־��ʼΪ��
	bShow=true; //������ʾ������־λ��ʼΪ�棬������ϵͳ��
	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); //����������ǰ��

	return 0;
}

BOOL CTransparentWnd::OnEraseBkgnd(CDC* pDC) 
{
	CRect	rect;
	GetWindowRect(&rect);

	CDC memDC;
	CBitmap			&cBitmap=m_bmpDraw;;
	CBitmap*		pOldMemBmp = NULL;
	CFont* pOldMemFont=NULL;

	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);
/*	//�����Ϸ���ʾ������Ϣ
	pOldMemFont = memDC.SelectObject(&m_fontLogo);
	CString szT=strTextPlay;
	int iLen=szT.GetLength ();
	CString strTemp=szT.Left (2);
	szT=szT.Right (iLen-2)+strTemp;
	CRect rcD(3,2,16,33);
	memDC.SetBkMode(TRANSPARENT);
	memDC.DrawText(szT,rcD,DT_LEFT|DT_WORDBREAK);
*/
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	if (pOldMemBmp) memDC.SelectObject( pOldMemBmp );
//	if (pOldMemFont) memDC.SelectObject( pOldMemFont );

	return TRUE;
}

void CTransparentWnd::OnPaint()
{
	CPaintDC dc(this);

}


void CTransparentWnd::OnTimer(UINT nIDEvent) 
{
  POINT point;
  CRect rcW;

	GetWindowRect(rcW);

	switch(nIDEvent)
	{
	case(1)://judge
		DoSet();
	
		break;
	case(2)://walk
		if(m_iAniSeq%24)  //���ܻ��Ǿ�ֹ��������
			DoWalk();
        else if(m_iAniSeq%59) //�����黰
		{   
		    bWavPlay=!bWavPlay;
			Reset();						
		}
		
		break; 
    case(3):
		bDragDrop=true;  //�����϶���־Ϊ��
        ::GetCursorPos(&point);  //ʵ���϶�ʱ��������ƶ�
		MoveWindow(point.x-CurPoint.x ,point.y-CurPoint.y ,rcW.Width(),rcW.Height() ,true); 

		break;
	default:
		break;
	}
	m_iAniSeq++;  //ͼ��ѭ����ʾ�������Ӽ���λ
	if(m_iAniSeq>60) m_iAniSeq=0;

	CWnd::OnTimer(nIDEvent);
}

void CTransparentWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}

	//�رճ���Ի���
void CTransparentWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonDown(nFlags, point);

//	CScrGeniusDlg dlg;  
//	if(IDOK!=dlg.DoModal())
//		DestroyWindow();

	//����һ�ʱ������ݲ˵�
 /* CMenu menu;
    menu.LoadMenu(IDR_POPUP);
 
  CMenu* pSubMenu = menu.GetSubMenu(0);

    // Make first menu item the default (bold font)
    ::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

    //Display and track the popup menu
    CPoint pos;
    GetCursorPos(&pos);
 //   ::SetForegroundWindow(::FindWindow(NULL,"����Ů�����Ӻ���!"));  
    ::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, ::FindWindow(NULL,"����Ů�����Ӻ���!"), NULL);
  */

	//����Ҽ����ͼ��˵�(��������ʽ�˵�)
  popmenu.LoadMenu(IDR_POPUP);
  popmenu.ModifyODMenu(NULL,IDR_EJECT,IDB_EJECT);
  if(bMute) 
	popmenu.ModifyODMenu(NULL, IDR_MUTE,IDB_CHECKED);
  else
    popmenu.ModifyODMenu(NULL, IDR_MUTE,IDB_CHECK);
  popmenu.ModifyODMenu(NULL, IDR_CUSTOMWAVFILE,IDB_CUSTOM);
  popmenu.ModifyODMenu(NULL, IDR_GETIP,IDB_GETIP);
  popmenu.ModifyODMenu(NULL, IDR_HELP,IDB_HELP);
  popmenu.ModifyODMenu(NULL, IDR_BIG2GB,IDB_BIG2GB);
  popmenu.ModifyODMenu(NULL, IDR_BMP2TXT,IDB_BMP2TXT);
  popmenu.ModifyODMenu(NULL,IDR_EXIT,IDB_EXIT);
  popmenu.ModifyODMenu(NULL,IDR_REGISTER,IDB_REGISTER);
  popmenu.ModifyODMenu(NULL,IDR_SHUTDOWN,IDB_SHUTDOWN);
  popmenu.ModifyODMenu(NULL,IDR_HIDEINBMP,IDB_HIDEINBMP);
  popmenu.ModifyODMenu(NULL,IDR_SHOW,IDB_SHOW);
  popmenu.ModifyODMenu(NULL,IDR_REGOCX,IDB_REGOCX);
  popmenu.ModifyODMenu(NULL,IDR_EDSTAR,IDB_EDSTAR);
  //��ԭ�ʼ�����ID��,��Ϊ�ļ��ָ�ͼ��
  popmenu.ModifyODMenu(NULL,ID_FILE_SEND_MAIL,IDB_FILESPLIT); 
  
  //SK modified for Unicode
 // popmenu.LoadToolbar(IDR_TOOLBAR);
  ClientToScreen(&point);
  BCMenu *psub = (BCMenu *)popmenu.GetSubMenu(0); 
  psub->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
  popmenu.DestroyMenu();
}

void CTransparentWnd::OnMoving(UINT fwSide, LPRECT pRect) 
{
	CWnd::OnMoving(fwSide, pRect);
	
}

void CTransparentWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
    
	bDragDrop=false;  //�����϶���־Ϊ��
	ReleaseCapture();
	KillTimer(3);  //�ر��϶�ʱ��ͼ����ʾ������3 
	// SetCursor(LoadCursor(NULL, IDC_ARROW)); //�ָ����������ʾ
	
	CWnd::OnLButtonUp(nFlags, point);
}

LRESULT CTransparentWnd::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
  //ɾ��ϵͳ��������ͼ��ӳ�亭��
  return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

//��ȡ�����Ƿ���ʾ�ı�־λ
bool CTransparentWnd::GetShowPetFlag ()
{
  bool bShowFlag=true;

       if(!bShow)
		   bShowFlag=bShow;

	   return bShowFlag;
}

//��ʾ�����س���
void CTransparentWnd::OnShow() 
{
	pFrame=AfxGetApp()->GetMainWnd();
	if(!bShow) //�Ƿ���д��嶯��,��������ʾ�򲻽���
	{
		WinAnimation(false); //���嶯����ϵͳ������ 
		pFrame->ShowWindow(SW_SHOW);
	}
    else //�Ƿ���д��嶯��,�����������򲻽���
	{
        pFrame->ShowWindow(SW_HIDE);
	    WinAnimation(true); //���嶯�����뵽ϵͳ���� 
	}
	bShow=!bShow;  //��ʾ���ر�־λȡ��
	
}

//WINDOWS NT/98�°�ȫ�ػ���ע���ĵ��ú���
void CTransparentWnd::WinShutdown(UINT ShutdownFlag)
{
    OSVERSIONINFO oi;
    oi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&oi);
	  //�����NT/2000�������������Ȩ��
     if (oi.dwPlatformId == VER_PLATFORM_WIN32_NT) 
	 {
       HANDLE handle;
       TOKEN_PRIVILEGES tkp;

       OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &handle);
       LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

       tkp.PrivilegeCount = 1;  // one privilege to set    
       tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
       AdjustTokenPrivileges(handle, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	 }

	   //WIN98�¹ػ���ע��ʱ��ֱ�ӵ������溭���ȿ�
		::ExitWindowsEx(ShutdownFlag,0);

}

//ע����ǰ�û�
void CTransparentWnd::OnRegister() 
{
  
//	if(MessageBox("���Ƿ����Ҫע����ǰ�û�?","�û�ע��",
//		                         MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON2)==IDOK)
//	{   
	   WinShutdown(EWX_LOGOFF);
	   CTransparentWnd::DestroyWindow();
//	}
	
}

//�رռ����
void CTransparentWnd::OnShutdown() 
{
 //   if(MessageBox("���Ƿ����Ҫ�رռ����?","�ر�ϵͳ",
//		                         MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON2)==IDOK)
								
	   //  WinShutdown(EWX_SHUTDOWN);
	CSetTimeShutdown dlg;
    //�˶Ի���ֻ��ʾһ��
	if( !FindWindow(NULL,"��ʱ�ػ�"))
	     dlg.DoModal();
	else
		::ShowWindow (::FindWindow(NULL,"��ʱ�ػ�"),SW_SHOW);
}

//������ر�CD-ROM
void CTransparentWnd::OnEject() 
{
   if(bEject)
   {
	if(mciSendString("set cdaudio door open",NULL,0,NULL)!=0)
		  MessageBox("����CD-ROM����!");
   }
   else
   {
	 if(mciSendString("set cdaudio door closed",NULL,0,NULL)!=0)
		  MessageBox("�ر�CD-ROM����");
   }
   bEject=!bEject;
	
}

//�رճ���Ի���
void CTransparentWnd::OnExit() 
{
	
	CScrGeniusDlg dlg;  
	
      //ֻ��ʾһ�ιرնԻ���
	if( !FindWindow(NULL,"Ը�����������ճɾ��� -- δ��������(Future Studio)"))
	  if(IDOK!=dlg.DoModal())
	    DestroyWindow();
}

//ԭΪ�ʼ����͹��ܣ��ָ�Ϊ���ļ��ָ���ϲ��Ի���
void CTransparentWnd::OnFileSendMail() 
{
 // CHyperLink m_Mail; 

 //   m_Mail.GotoURL(_T("mailto:jz_x@china.com"), SW_SHOW);

	CFileSplit dlg;  

	  //�˶Ի���ֻ��ʾһ��
	if( !FindWindow(NULL,"�ļ��ָ���"))
	     dlg.DoModal();

}

//��ȡ�����黰�ı�־λ
bool CTransparentWnd::GetMuteFlag ()
{
	bool bMuteFlag=false;

	if(bMute)
		bMuteFlag=bMute;

	return bMuteFlag;
}

//�رջ���黰����
void CTransparentWnd::OnMute() 
{
	bMute=!bMute; //�黰������־ȡ��	
	if(bMute)
	{
	  if(bWavPlay) 
		  //�ر�ԭ�黰����һ
		PlaySound("IDR_WAVE1",AfxGetResourceHandle(),SND_RESOURCE|SND_PURGE|SND_NODEFAULT  ); 
	  else  
		 //�ȹر�ԭ�黰���Ŷ�
		PlaySound("IDR_WAVE2",AfxGetResourceHandle(),SND_RESOURCE|SND_PURGE|SND_NODEFAULT  );  	 

	  SetTimer(2,600,NULL);
	}
}

void CTransparentWnd::OnUpdateMute(CCmdUI* pCmdUI) 
{ 

   pCmdUI->SetCheck (bMute);
	
}

//��ȡ�Ǻ�����Ի���
void CTransparentWnd::OnGetip() 
{
	GetIP dlg;  
	
	//�˶Ի���ֻ��ʾһ��
  if( !FindWindow(NULL,"��ȡ�Ǻ�����"))
	    dlg.DoModal();
	
}

//λͼת��Ϊ�ı��Ի���
void CTransparentWnd::OnBmp2txt() 
{
	CBMP2TXT dlg; 

	//�˶Ի���ֻ��ʾһ��
  if( !FindWindow(NULL,"λͼת��Ϊ�ı�"))
	    dlg.DoModal();
	
}

void CTransparentWnd::OnHelp() 
{
	CAbout dlg; 

	//�˶Ի���ֻ��ʾһ��
  if( !FindWindow(NULL,"����"))
	    dlg.DoModal();
	
}

//����ʾ���嶯��Ч��ǰ����Ѱ��ϵͳ��λ��
BOOL CALLBACK FindTrayWnd(HWND hwnd, LPARAM lParam)
{
    TCHAR szClassName[256];
    GetClassName(hwnd, szClassName, 255);

    // �Ƚϴ�������
    if (_tcscmp(szClassName, _T("TrayNotifyWnd")) == 0)
    {
        CRect *pRect = (CRect*) lParam;
        ::GetWindowRect(hwnd, pRect);
        return TRUE;
    }

    // ���ҵ�ʱ�Ӵ���ʱ��ʾ���Խ�����
    if (_tcscmp(szClassName, _T("TrayClockWClass")) == 0)
    {
        CRect *pRect = (CRect*) lParam;
        CRect rectClock;
        ::GetWindowRect(hwnd, rectClock);
        pRect->right = rectClock.left;
        return FALSE;
    }
 
    return TRUE;
}

//��ʾ���ڶ���Ч���ĺ���
void CTransparentWnd::WinAnimation(bool ShowFlag) 
{
    CRect rect(0,0,0,0);

    // �������̴��� 
    CWnd* pWnd = FindWindow("Shell_TrayWnd", NULL);
    if (pWnd)
    {
        pWnd->GetWindowRect(rect);
		EnumChildWindows(pWnd->m_hWnd, FindTrayWnd, (LPARAM)&rect);
		//rect Ϊ����������
		CRect rcWnd;
		GetWindowRect(rcWnd);
		if(ShowFlag) //���廬��ϵͳ��
		  DrawAnimatedRects(GetSafeHwnd(),IDANI_CAPTION,rcWnd,rect);
		else   //�����ϵͳ������
		  DrawAnimatedRects(GetSafeHwnd(),IDANI_CAPTION,rect,rcWnd);
    }
}


void CTransparentWnd::OnCustomwavfile() 
{
  CCustomWavFile dlg; 

	//�˶Ի���ֻ��ʾһ��
  if( !FindWindow(NULL,"���������黰"))
	    dlg.DoModal();
	
}

void CTransparentWnd::OnBig2gb() 
{
	Big2GB dlg; 

	//�˶Ի���ֻ��ʾһ��
  if( !FindWindow(NULL,"����ͨ--�����廥ת"))
	    dlg.DoModal();
	
}

void CTransparentWnd::OnHideinbmp() 
{
  HideInBMP dlg;

   	//�˶Ի���ֻ��ʾһ��
  if( !FindWindow(NULL,"���ļ����ص�λͼ��"))
	  dlg.DoModal();
	
}

void CTransparentWnd::OnMouseMove(UINT nFlags, CPoint point) 
{	
	CWnd::OnMouseMove(nFlags, point);
}

void CTransparentWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
    CRect rc0;  //�Ի��򶯻���ʾЧ��
    AnimEffect anim0;

	GetWindowRect(rc0);
	anim0.Open (rc0);  //��ʾ�Ի���̬�򿪶���Ч��	
}

//�ļ��ӽ��ܶԻ���
void CTransparentWnd::OnEdstar() 
{
	CEDStar dlg;

   	//�˶Ի���ֻ��ʾһ��
  if( !FindWindow(NULL,"����֮�������"))
	  dlg.DoModal();
	
}

//�ؼ�ע�����Ի���
void CTransparentWnd::OnRegocx() 
{
  CRegOCX dlg;

   	//�˶Ի���ֻ��ʾһ��
  if( !FindWindow(NULL,"�ؼ�ע����"))
	  dlg.DoModal();
}
