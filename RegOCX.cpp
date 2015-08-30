// RegOCX.cpp : implementation file
// 
//���ܣ��ؼ�ע�����Ĵ����࣬�ڴ˼���ע�ᣬע���ؼ��Լ���ʾ����ȹ���
// ���ߣ��쾰�� ��
//���ڣ�2001��7��8��
//

#include "stdafx.h"
#include "scrgenius.h"
#include "RegOCX.h"
#include "AnimEffect.h" //�Ի��򶯻�Ч����ʾ

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegOCX dialog


CRegOCX::CRegOCX(CWnd* pParent /*=NULL*/)
	: CDialog(CRegOCX::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegOCX)
	m_FileName = _T("");
	//}}AFX_DATA_INIT
}


void CRegOCX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegOCX)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_UNREGOCX, m_UnRegOCX);
	DDX_Control(pDX, IDC_SHOWDESKTOP, m_ShowDesktop);
	DDX_Control(pDX, IDC_REGOCX, m_RegOCX);
	DDX_Control(pDX, IDC_BROWSE, m_Browse);
	DDX_Text(pDX, IDC_FILENAME, m_FileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegOCX, CDialog)
	//{{AFX_MSG_MAP(CRegOCX)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_SHOWDESKTOP, OnShowdesktop)
	ON_BN_CLICKED(IDC_REGOCX, OnRegocx)
	ON_BN_CLICKED(IDC_UNREGOCX, OnUnregocx)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegOCX message handlers

//������ҳ�Ҫע���ע���Ŀؼ��ļ�
void CRegOCX::OnBrowse() 
{
   CFileDialog fileDialog( TRUE,"*.OCX",NULL,NULL,"�ؼ��ļ�(*.ocx)|*.ocx|�����ļ�(*.*)|*.*||");
	if (fileDialog.DoModal() == IDOK) {
		m_FileName=fileDialog.GetPathName();	
	
	    UpdateData(FALSE); //�ѱ���ֵ���µ���Ӧ�ؼ���
	}	
	
}

BOOL CRegOCX::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    //��ʼ�����洰����Ϊ0
	m_nWndNum = 0;	

	//�ڴ˽��ļ������ť��Ϊ��ƽ״,��ɫ���Զ��崰��ɫ��ͬ
	m_Browse.SubclassDlgItem(IDC_BROWSE, this);
	m_Browse.SetActiveBgColor(RGB(240,200,90));
	m_Browse.SetInactiveBgColor(RGB(240,200,90));
	m_Browse.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_Browse.SetIcon(IDI_FILEOPEN);
	m_Browse.SetBtnCursor(IDC_HAND); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//��С����ԭ���������д򿪵Ĵ��壬����ʾ������
void CRegOCX::OnShowdesktop() 
{
// Ѱ�������������е����д�������������ʾ�Ĵ洢������pWnd[]��
CWnd *pWnd[MAXNUM_RUNWINDOW], *pWndCur;
int nWndNum = 0;

  //���ؼ�ע�����Ĵ����ᵽ��ǰ
  SetForegroundWindow();

  pWnd[nWndNum]= GetWindow(GW_HWNDFIRST);

  while (pWnd[nWndNum] != NULL)
  {
   pWndCur = pWnd[nWndNum];
   CString str;
   pWndCur->GetWindowText(str);
   //��С�����洰���в�Ӧ��������������
   if (str != "�͸���..." && pWndCur->IsWindowVisible() && ! pWndCur->IsIconic() &&! pWndCur->GetParent() &&pWndCur != this &&pWndCur != GetDesktopWindow() &&! str.IsEmpty() &&str != "Program Manager")
   {
    // �б����д�������һ
    nWndNum++;
    // Ϊ�˰�ȫ�������������Ӧ�����ܴ�����!!
    nWndNum = min(nWndNum, MAXNUM_RUNWINDOW-1);
   }
   pWnd[nWndNum] = pWndCur->GetWindow(GW_HWNDNEXT);
  }

  if (nWndNum)
  {
  // ���������е��������洰����С��(�������)
  for (int i=0; i<nWndNum;++i)
  {
   pWnd[i]->ShowWindow(SW_SHOWMINNOACTIVE);
   m_pWnd[i] = pWnd[i];
  }
  m_nWndNum = nWndNum;
  }
  else
  {
  // �������в�����С�������洰�廹ԭ��ʾ
  for (int i=m_nWndNum-1; i>=0; --i)
  {
   if (::IsWindow(m_pWnd[i]->m_hWnd) &&
   m_pWnd[i]->IsIconic())
   {
    m_pWnd[i]->ShowWindow(SW_RESTORE);
    m_pWnd[i]->UpdateWindow();
   }
  }
  }
  
}

//����ѡ�ؼ�����ϵͳע��
void CRegOCX::OnRegocx() 
{
//ActiveX�ؼ���·�����ļ���
  LPCTSTR pszDllName;

  //��ѡ�ؼ����Ƿ�Ϊ��
  if(m_FileName!="")
    pszDllName=m_FileName;
  else
  {
	MessageBox("����ѡ������Ҫע��Ŀؼ����ƣ�","��ʾ",MB_OK);
	return;
  }

  //װ��ActiveX�ؼ�
  HINSTANCE hLib = LoadLibrary(pszDllName);
  if (hLib < (HINSTANCE)HINSTANCE_ERROR)
  {
       MessageBox("��������ÿؼ��ļ�!","����",MB_OK);
       return;
  }
  //��ȡע�ắ��DllRegisterServer��ַ
  FARPROC lpDllEntryPoint; 
  lpDllEntryPoint = GetProcAddress(hLib,_T("DllRegisterServer"));
  //����ע�ắ��DllRegisterServer
  if(lpDllEntryPoint!=NULL)
  {
       if(FAILED((*lpDllEntryPoint)()))
       {
            MessageBox("�ؼ�ע��ʧ�ܣ�","����",MB_OK);
            FreeLibrary(hLib);
            return;
       }
       MessageBox("�ؼ�ע��ɹ�","��ʾ",MB_OK);
  }
  else
  MessageBox("�ؼ�ע��ʧ�ܣ�","����",MB_OK);
		
}

//����ע��ؼ�����ϵͳע��
void CRegOCX::OnUnregocx() 
{
//ActiveX�ؼ���·�����ļ���
  LPCTSTR pszDllName;

  //��ѡ�ؼ����Ƿ�Ϊ��
  if(m_FileName!="")
    pszDllName=m_FileName;
  else
  {
	MessageBox("����ѡ������Ҫע���Ŀؼ����ƣ�","��ʾ",MB_OK);
	return;
  }

  //װ��ActiveX�ؼ�
  HINSTANCE hLib = LoadLibrary(pszDllName);
  if (hLib < (HINSTANCE)HINSTANCE_ERROR)
  {
       MessageBox("��������ÿؼ��ļ�!","����",MB_OK);
       return;
  }
  //��ȡע������DllUnregisterServerr��ַ
  FARPROC lpDllEntryPoint; 
  lpDllEntryPoint = GetProcAddress(hLib,_T("DllUnregisterServer"));
  //����ע������DllUnRegisterServer
  if(lpDllEntryPoint!=NULL)
  {
       if(FAILED((*lpDllEntryPoint)()))
       {
            MessageBox("�ؼ�ע��ʧ�ܣ�","����",MB_OK);
            FreeLibrary(hLib);
            return;
       }
       MessageBox("�ؼ�ע���ɹ�","��ʾ",MB_OK);
  }
  else
  MessageBox("�ؼ�ע��ʧ�ܣ�","����",MB_OK);
  
}


BOOL CRegOCX::DestroyWindow() 
{
   CRect rect1;  //�Ի��򶯻���ʾЧ��
   AnimEffect animation1;

	//�ڴ˼���Ի��򶯻��ر�Ч��
	GetWindowRect(rect1);
	rect1.DeflateRect(2,2);//��̬������ʾ
    animation1.DrawWireRects(rect1, CRect(rect1.CenterPoint().x-10,rect1.CenterPoint().y-10,rect1.CenterPoint().x+10,rect1.CenterPoint().y+10 ), 10);
//	animation1.Close(rect1); ////�Ի��򶯻�Ч����ʾ�ر�
//	Sleep(50); //�ӻ�50�����ر�
	
	
	return CDialog::DestroyWindow();
}

void CRegOCX::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
   CRect rect1;  //�Ի��򶯻���ʾЧ��
   AnimEffect animation1;

	//�ڴ˼���Ի��򶯻���Ч��
	GetWindowRect(rect1 );
    animation1.Open( rect1 ); //�Ի��򶯻�Ч����ʾ��		
}
