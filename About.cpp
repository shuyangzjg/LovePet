// About.cpp : implementation file
//
//Modify by �쾰�� 2000.9
//���ܣ������Ի���ʵ��

#include "stdafx.h"
#include "scrgenius.h"
#include "About.h"
#include "AnimEffect.h" //�Ի��򶯻�Ч����ʾ

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAbout dialog


CAbout::CAbout(CWnd* pParent /*=NULL*/)
	: CDialog(CAbout::IDD, pParent)
{
	CString str;
	str="���ԣ�\r\n  ��ʥΪһ����ɫ�����������������⴫���͸��ơ���������ллʹ�ã�лл֧�֣�~O~ \r\n\r\n���ܼ�飺\r\n  ��ʥ3.0�棬�����֡����С�ʵ������һ�����ܶ�࣬�������һ�Ա�֪!����������Ŀ�е���(��)��ﰮ��������件�����ǾͰ�ȫ����ʽ��İ�ʥ�͸���(��)��һ�о��ڲ����У���������(��)���������϶����������黰(ע:����ʵʱ�϶�ʱ���Ϊ��Ůͷ��)������ʵ�ù�����Ҫ�У�����ʵʱ�϶���ϵͳ��ͼ���ݲ˵���������ʾ/���س�����������黰����ͣ�黰���š�����ͨ--�����廥ת����ȡ�Ǻ����롢λͼת��Ϊ�ı����ļ����ص�λͼ�С��ļ��ָ���ϲ����ؼ�ע����(��)����ʾ/�ָ�����(��)���ļ��ӽ���(��)����ʱ�ػ�(��)������������(��)�������ʼ�����������ϵͳ����ȵȡ�\r\n\r\n���ߺ󻰣�\r\n  ���Ÿ����������������ʥ�Ѳ��ڽ����������֡���������У���־����ȫ��λ��ʵ�ù��������չ�����ǣ�ͬ����������Ҳ��Ҫ�û��Ļ���֧�֡���ˣ��������ʹ���������������ʲô�õ�������飬��д�Ÿ��ң�jz_x@china.com�����һ�����һ���汾���޸Ļ������书�ܣ����������ܱ�֤���᲻�ϵõ��������£����ϳɳ�������Ļ�����ֻ���ڳ�Ĭ������������汾�Ϳ��ܻ��������հ�!";
//	str=str+"\r\n  �汾";
	//{{AFX_DATA_INIT(CAbout)
	m_edit = _T(str);
	//}}AFX_DATA_INIT
}


void CAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbout)
	DDX_Text(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_MAIL, m_Mail); //��ǩ�����ӱ���
	DDX_Control(pDX, IDOK,  m_Button);	
	DDX_Control(pDX, IDC_HELPFILE,  m_ButtonHelp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbout, CDialog)
	//{{AFX_MSG_MAP(CAbout)
	ON_BN_CLICKED(IDC_HELPFILE, OnHelpfile)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbout message handlers

BOOL CAbout::OnInitDialog() 
{
	CDialog::OnInitDialog();

	  //�����ǩ�ĳ���
    m_Mail.SetURL(_T("mailto:jz_x@china.com"));
	m_Mail.SetUnderline(FALSE);	
     
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//�򿪰�ʥ�����ļ�
void CAbout::OnHelpfile() 
{
  HINSTANCE result =ShellExecute(NULL, _T("open"), "lovepet.chm", NULL,NULL, SW_SHOW);
  
  if((UINT)result <= HINSTANCE_ERROR)
    AfxMessageBox("\n ��Ǹ,�����ļ�lovePet.chm�����ڻ��ڵ�ǰ·���£�");
}


BOOL CAbout::DestroyWindow() 
{

	CRect rc;  //�Ի��򶯻���ʾЧ��
    AnimEffect anim;	

	//�ڴ˼���Ի��򶯻��ر�Ч��
	GetWindowRect(rc );
	rc.DeflateRect(2,2);//��̬������ʾ
    anim.DrawWireRects(rc, CRect(rc.CenterPoint().x-10,rc.CenterPoint().y-10,rc.CenterPoint().x+10,rc.CenterPoint().y+10 ), 10);
//	anim.Close(rc); ////�Ի��򶯻�Ч����ʾ�ر� 
//	Sleep(50); //�ӻ�50�����ر�

	return CDialog::DestroyWindow();
}

void CAbout::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	CRect rc;  //�Ի��򶯻���ʾЧ��
    AnimEffect anim;
	
    //�ڴ˼���Ի��򶯻���Ч��
	GetWindowRect(rc );
    anim.Open( rc ); //�Ի��򶯻�Ч����ʾ��
	
}

void CAbout::OnDestroy() 
{
	CDialog::OnDestroy();
	
}
