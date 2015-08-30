// SetTimeShutdown.cpp : implementation file
// 
//���ܣ�Windows 9x/NT�¶�ʱ�������ػ�,���������,��������������
// ���ߣ��쾰�� ��
//���ڣ�2001��7��20��
//
#include "stdafx.h"
#include "scrgenius.h"
#include "SetTimeShutdown.h"
#include "AnimEffect.h" //�Ի��򶯻�Ч����ʾ

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//���ÿ���������������ֵλ�ú������½��Ӽ�ֵ(����Ϊ������)
const TCHAR gcszAutoRunKey[]= _T( "Software\\Microsoft\\windows\\CurrentVersion\\Run" );
const TCHAR	gcszWindowClass[] = _T("lovePet");
/////////////////////////////////////////////////////////////////////////////
// CSetTimeShutdown dialog


CSetTimeShutdown::CSetTimeShutdown(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTimeShutdown::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTimeShutdown)
	m_sNowTime = _T("");
	m_Time = CTime(2001,7,21,20,06,0);
	//}}AFX_DATA_INIT
}


void CSetTimeShutdown::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTimeShutdown)
	DDX_Control(pDX, IDC_REBOOT, m_ReBoot);
	DDX_Control(pDX, IDC_SHUTDOWN, m_ShutDownOnce);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_BUTTON_SHUTDOWN, m_ShutDown);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_Timer);
	DDX_Text(pDX, IDC_STATIC_NOW, m_sNowTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_Time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTimeShutdown, CDialog)
	//{{AFX_MSG_MAP(CSetTimeShutdown)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SHUTDOWN, OnButtonShutdown)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_SHUTDOWN, OnShutdown)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_REBOOT, OnReboot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTimeShutdown message handlers

BOOL CSetTimeShutdown::OnInitDialog() 
{
	CDialog::OnInitDialog();	

    //������ʱ����һ���Ӹ���һ��
    SetTimer(0,1000,NULL);	

	bSetTime=false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//����ʱ�����Զ�����
BOOL StartUpAutoRun(BOOL bIsAdd)
{
	HKEY hKey;
	LONG lRet, lRet2;
	DWORD dwLength, dwDataType;
	TCHAR szItemValue[MAX_PATH], szPrevValue[MAX_PATH];
	TCHAR szBuffer[MAX_PATH];

	// �õ�����ȫ·����
	GetModuleFileName( NULL, szItemValue, MAX_PATH );


	// ��ע����
		lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE, gcszAutoRunKey,
			0, KEY_READ | KEY_WRITE, &hKey );
		if( lRet != ERROR_SUCCESS )
			return FALSE;

	// ��ѯ�Զ�������Ŀ�Ƿ����
		dwLength = sizeof( szBuffer );
		lRet = RegQueryValueEx( hKey, gcszWindowClass, 
			NULL, &dwDataType, (LPBYTE)szBuffer, &dwLength );

	// ���
		if( bIsAdd == TRUE ) 
		{
	// �Զ�������Ŀ������
			if( lRet != ERROR_SUCCESS )
				lRet2 = RegSetValueEx( hKey, gcszWindowClass,
				0, REG_SZ, (LPBYTE)szItemValue, strlen( szItemValue ) );
			else 
			{
	// ����, �Ƚ϶����Ƿ���ͬ
				dwLength = sizeof( szPrevValue );
				lRet2 = RegQueryValueEx( hKey, gcszWindowClass, 
					0, &dwDataType,(LPBYTE)szPrevValue, &dwLength );
	// ����ͬ���滻
				if( lstrcmp( szItemValue, szPrevValue ) ) 
				{
					lRet2 = RegDeleteValue( hKey, gcszWindowClass );
					lRet2 = RegSetValueEx( hKey, gcszWindowClass, 
						0, REG_SZ,(LPBYTE)szItemValue, strlen( szItemValue ) );
				}
			}
		}
	// ɾ��
		else 
	// �Զ�������Ŀ�Ѵ�����ɾ��
			if( lRet == ERROR_SUCCESS )
				lRet2 = RegDeleteValue( hKey, gcszWindowClass );

	// �ر�ע����
		RegCloseKey( hKey );

		if( lRet2 != ERROR_SUCCESS )
			return FALSE;


	return TRUE;
}

//WINDOWS NT/98�°�ȫ�ػ���ע���ĵ��ú���
void WinShutdown(UINT ShutdownFlag)
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

void CSetTimeShutdown::OnTimer(UINT nIDEvent) 
{
  int hour,min,sec;
  int yhour,ymin,ysec;
    
    CTime nowtime = CTime::GetCurrentTime();
	m_sNowTime = nowtime.Format(_T("%Y-%m-%d %H:%M:%S"));

	UpdateData(FALSE);

	if(bSetTime)
	{
	hour = nowtime.GetHour();
	min	 = nowtime.GetMinute();
	sec  = nowtime.GetSecond();

	yhour = m_Time.GetHour();
	ymin  = m_Time.GetMinute();
	ysec  = m_Time.GetSecond();

	//���ʱ�䵽����ػ�
	if(hour == yhour && min == ymin )
	    WinShutdown(EWX_SHUTDOWN);
	}

	
	CDialog::OnTimer(nIDEvent);
}

//��ʱ�ػ���ť����ʱ
void CSetTimeShutdown::OnButtonShutdown() 
{
	if(m_sNowTime.IsEmpty())
	  return;

    bSetTime=true;

	ShowWindow(SW_HIDE);//���ظô���

	//�ж��Ƿ�ѡ�п����������������Ӧ����
	int Status;
	CButton * check1 = ( CButton * )GetDlgItem( IDC_CHECK1 );
	Status=check1->GetCheck();
	if (Status==1) //����Ӽ�
	   StartUpAutoRun(true);
	else  //ɾ���Ӽ�
	   StartUpAutoRun(false);
}

void CSetTimeShutdown::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
    UpdateData(true);	//���¿ؼ���ʾ

	*pResult = 0;
}

//�����������ػ���ťʱ�������Ϲػ�
void CSetTimeShutdown::OnShutdown() 
{
    //�ж��Ƿ�ѡ�п����������������Ӧ����
	int Status;
	CButton * check1 = ( CButton * )GetDlgItem( IDC_CHECK1 );
	Status=check1->GetCheck();
	if (Status==1) //����Ӽ�
	   StartUpAutoRun(true);
	else  //ɾ���Ӽ�
	   StartUpAutoRun(false);
   
	//�ػ�
   WinShutdown(EWX_SHUTDOWN);
}

BOOL CSetTimeShutdown::DestroyWindow() 
{
   CRect rect3;  //�Ի��򶯻���ʾЧ��
   AnimEffect animation3;

	//�ڴ˼���Ի��򶯻��ر�Ч��
	GetWindowRect(rect3);
	rect3.DeflateRect(2,2);//��̬������ʾ
    animation3.DrawWireRects(rect3, CRect(rect3.CenterPoint().x-10,rect3.CenterPoint().y-10,rect3.CenterPoint().x+10,rect3.CenterPoint().y+10 ), 10);
//	animation3.Close(rect3); ////�Ի��򶯻�Ч����ʾ�ر�
//	Sleep(50); //�ӻ�50�����ر�

	return CDialog::DestroyWindow();
}

void CSetTimeShutdown::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	CRect rect3;  //�Ի��򶯻���ʾЧ��
    AnimEffect animation3;

	//�ڴ˼���Ի��򶯻���Ч��
	GetWindowRect(rect3 );
    animation3.Open( rect3 ); //�Ի��򶯻�Ч����ʾ��
	
}

//�رոöԻ���
void CSetTimeShutdown::OnCancel() 
{
    //�ж��Ƿ�ѡ�п����������������Ӧ����
	int Status;
	CButton * check1 = ( CButton * )GetDlgItem( IDC_CHECK1 );
	Status=check1->GetCheck();
	if (Status==1) //����Ӽ�
	   StartUpAutoRun(true);
	else  //ɾ���Ӽ�
	   StartUpAutoRun(false);
	
	CDialog::OnCancel();
}

//�������������
void CSetTimeShutdown::OnReboot() 
{
   //�ж��Ƿ�ѡ�п����������������Ӧ����
	int Status;
	CButton * check1 = ( CButton * )GetDlgItem( IDC_CHECK1 );
	Status=check1->GetCheck();
	if (Status==1) //����Ӽ�
	   StartUpAutoRun(true);
	else  //ɾ���Ӽ�
	   StartUpAutoRun(false);
   
	//��������
   WinShutdown(EWX_REBOOT);
	
}
