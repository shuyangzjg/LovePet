// CustomWavFile.cpp : implementation file
//
//Modify by �쾰�� 2000.10
//���ܣ��Զ����黰������

#include "stdafx.h"
#include "scrgenius.h"
#include "CustomWavFile.h"
#include "AnimEffect.h" //�Ի��򶯻�Ч����ʾ

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString strOpenWavName="";  //�黰�ļ���
CString strWavFile=""; //����·�����黰�ļ���
bool bOriginWavPlay=true;   //�Ƿ񲥷�ԭʼ���黰��־λ
CString strPath=""; //��ִ�г������ڵ�·��
/////////////////////////////////////////////////////////////////////////////
// CCustomWavFile dialog


CCustomWavFile::CCustomWavFile(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomWavFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomWavFile)
	m_TextDescription = _T("");
	m_FilePathString1 = _T("");
	//}}AFX_DATA_INIT
    bOriginWavPlay=true; //�Ƿ�ָ�ԭʼ�黰�ļ��ĳ�ʼֵΪ��
}


void CCustomWavFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomWavFile)
	DDX_Control(pDX, IDC_EDIT2, m_TextDescriptionEdit);
	DDX_Text(pDX, IDC_EDIT2, m_TextDescription);
	DDV_MaxChars(pDX, m_TextDescription, 40);
	DDX_Text(pDX, IDC_EDIT1, m_FilePathString1); //�黰�ļ�һ�༭�����
    DDX_Control(pDX, IDOK,  m_OK);
	DDX_Control(pDX, IDCANCEL,  m_Cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomWavFile, CDialog)
	//{{AFX_MSG_MAP(CCustomWavFile)
	ON_BN_CLICKED(IDC_FILEBROWSER1, OnFilebrowser1)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomWavFile message handlers

void CCustomWavFile::OnFilebrowser1() 
{
  CFileDialog fileDialog(TRUE,NULL,NULL,NULL,"�黰�ļ�(*.wav)|*.wav||");

	if (fileDialog.DoModal() == IDOK) {
		strOpenWavName = fileDialog.GetPathName(); //��·��
		strWavFile=fileDialog.GetFileName(); //����·��
		m_FilePathString1 = strOpenWavName;
		UpdateData(FALSE);
	}	
}

void CCustomWavFile::OnOK() 
{
  m_TextDescriptionEdit.GetWindowText(m_TextDescription); //���ַ�ֵ
   if (strOpenWavName.IsEmpty() && m_TextDescription =="")
	{
		AfxMessageBox("\n ��Ǹ,�黰�ļ��������ֲ���ͬʱΪ��!\r\n �����밴�ָ���ť���ָ�ԭʼ�黰. ");
		return;
	}

   bOriginWavPlay=false; //���Ŷ��Ƶ��黰.


  if(strWavFile!=""||m_TextDescription!="")
  {
   //��һ�����ļ������浱ǰ�ļ���������·�������ڶ������г���ʱ�Զ�����
   CStdioFile TxtFile;

    //�����黰�����ļ�������·����Ϊ��ִ�г�������·��
    if (!TxtFile.Open(strPath+ "\\lovepet.dat", CFile::modeCreate | CFile::modeWrite | CFile::typeText))
    {
		CString strErrorMessage;
		strErrorMessage = "�����黰�����ļ�: "+strPath+"lovepet.dat ʧ�� !";
		AfxMessageBox(strErrorMessage);
    }
	else
	{
    //д���黰�ļ���������·������Ĭ�ϵ�ǰ·������һ�𷢸��Է��������ֱ��
    if(strWavFile!="" && m_TextDescription!="")
	{
	 TxtFile.WriteString(strWavFile);
	 TxtFile.WriteString ("$$$");
	 TxtFile.WriteString (m_TextDescription);
	}
	else if(m_TextDescription!="")
	{
     TxtFile.WriteString ("$$$");
	 TxtFile.WriteString (m_TextDescription);
	}
	else
	{
     TxtFile.WriteString(strWavFile);
	 TxtFile.WriteString ("$$$");
	}

	TxtFile.Close();
	}
  }

	CDialog::OnOK();
}

//��ȡѡ�е��黰�ļ���
CString CCustomWavFile::GetWaveFile()
{
	CString sWaveFile="";
	
	if(strOpenWavName!="")
		sWaveFile=strOpenWavName;

	return sWaveFile;
}

//��ȡ�Ƿ񲥷Ŷ����黰�ļ���־
bool CCustomWavFile::GetOriginPlayFlag ()
{
	bool bOriginWavePlayFlag=true;
	
	if(!bOriginWavPlay)
		bOriginWavePlayFlag=bOriginWavPlay;

	return bOriginWavePlayFlag;

}	

void CCustomWavFile::OnCancel() 
{
  CStdioFile TxtFile;

  //�黰�����ļ����ڣ���ɾ����
  if (TxtFile.Open(strPath+"\\lovepet.dat", CFile::modeRead|CFile::typeText))
    {
	
	TxtFile.Close();
	//�ڻָ�ԭʼ�黰ʱ��ɾ���Զ����黰�����ļ�
    TxtFile.Remove(strPath+"\\lovepet.dat");
    }
   	

	CDialog::OnCancel();
}

BOOL CCustomWavFile::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    //�˴���ȡ����������·��,����ȫ�ֱ���strPath��
	GetModuleFileName(NULL,strPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	strPath.ReleaseBuffer ();
    int nPos;
	nPos=strPath.ReverseFind ('\\');
	strPath=strPath.Left (nPos);

   //�ڴ˽��ļ������ť��Ϊ��ƽ״,��ɫ���Զ��崰��ɫ��ͬ
	m_SourceFile.SubclassDlgItem(IDC_FILEBROWSER1, this);
	m_SourceFile.SetActiveBgColor(RGB(240,200,90));
	m_SourceFile.SetInactiveBgColor(RGB(240,200,90));
	m_SourceFile.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_SourceFile.SetIcon(IDI_FILEOPEN);
//	m_SourceFile.SetFlat(FALSE); //����ƽ��ʾ
	m_SourceFile.SetBtnCursor(IDC_HAND); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCustomWavFile::DestroyWindow() 
{
	CRect rc4;  //�Ի��򶯻���ʾЧ��
    AnimEffect anim4;

   //�ڴ˼���Ի��򶯻��ر�Ч��
	GetWindowRect(rc4 );
	rc4.DeflateRect(2,2);//��̬������ʾ
    anim4.DrawWireRects(rc4, CRect(rc4.CenterPoint().x-10,rc4.CenterPoint().y-10,rc4.CenterPoint().x+10,rc4.CenterPoint().y+10 ), 10);
//	anim4.Close(rc4); ////�Ի��򶯻�Ч����ʾ�ر�
//  Sleep(50); //�ӻ�50�����ر�
	
	return CDialog::DestroyWindow();
}

void CCustomWavFile::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
   CRect rc4;  //�Ի��򶯻���ʾЧ��
   AnimEffect anim4;


	//�ڴ˼���Ի��򶯻���Ч��
	GetWindowRect(rc4 );
    anim4.Open( rc4 ); //�Ի��򶯻�Ч����ʾ��

}
