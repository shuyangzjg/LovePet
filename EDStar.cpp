// EDStar.cpp : implementation file
//
//���ܣ�ͨ���ļ��ӽ���
// ���ߣ��쾰�� ��
//���ڣ�2001��7��18��
//

#include "stdafx.h"
#include "scrgenius.h"
#include "EDStar.h"
#include "DirDialog.h" //Ŀ¼����Ի���
#include "AnimEffect.h" //�Ի��򶯻�Ч����ʾ

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEDStar dialog


CEDStar::CEDStar(CWnd* pParent /*=NULL*/)
	: CDialog(CEDStar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEDStar)
	m_Password = _T("");
	m_SourcePath = _T("");
	m_TargetPath = _T("");
	//}}AFX_DATA_INIT
}


void CEDStar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEDStar)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_ENCRYPT, m_Encrypt);
	DDX_Control(pDX, IDC_DECRYPT, m_Decrypt);
	DDX_Control(pDX, IDC_BROWSE2, m_Browse2);
	DDX_Control(pDX, IDC_BROWSE1, m_Browse1);
	DDX_Control(pDX, IDC_PATH2, m_TargetPath1);
	DDX_Control(pDX, IDC_PATH1, m_SourcePath1);
	DDX_Control(pDX, IDC_PASSWORD, m_Password1);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDV_MaxChars(pDX, m_Password, 16);
	DDX_Text(pDX, IDC_PATH1, m_SourcePath);
	DDX_Text(pDX, IDC_PATH2, m_TargetPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEDStar, CDialog)
	//{{AFX_MSG_MAP(CEDStar)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BROWSE1, OnBrowse1)
	ON_BN_CLICKED(IDC_BROWSE2, OnBrowse2)
	ON_BN_CLICKED(IDC_ENCRYPT, OnEncrypt)
	ON_BN_CLICKED(IDC_DECRYPT, OnDecrypt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEDStar message handlers

BOOL CEDStar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    //�������ļ�ѡ��·���ľ�̬�������ڲ���ʾ��ʽΪ,
	//����ʾ��·����ʱ��ʡ���м�·����ֻ��ʾ����·����
    m_SourcePath1.SubclassDlgItem (IDC_PATH1, this);
	m_SourcePath1.SetPath (TRUE);
    m_TargetPath1.SubclassDlgItem (IDC_PATH2, this);
    m_TargetPath1.SetPath (TRUE);	

	 //�ڴ˽��ļ��������ť��Ϊƽ��״,��ɫ���Զ��崰��ɫ��ͬ
	m_Browse1.SubclassDlgItem(IDC_BROWSE1, this);
	m_Browse1.SetActiveBgColor(RGB(240,200,90));
	m_Browse1.SetInactiveBgColor(RGB(240,200,90));
	m_Browse1.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_Browse1.SetIcon(IDI_FILEOPEN);
	m_Browse1.SetBtnCursor(IDC_HAND); 

	m_Browse2.SubclassDlgItem(IDC_BROWSE2, this);
	m_Browse2.SetActiveBgColor(RGB(240,200,90));
	m_Browse2.SetInactiveBgColor(RGB(240,200,90));
	m_Browse2.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_Browse2.SetIcon(IDI_FILEOPEN);
	m_Browse2.SetBtnCursor(IDC_HAND);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//�����ļ��㷨����
BOOL CEDStar::EncryptFile(CString filename,char *password)
{
	SetFileAttributes(filename,FILE_ATTRIBUTE_ARCHIVE|FILE_ATTRIBUTE_NORMAL);
	CFile m_file;
	if(m_file.Open(filename,CFile::modeReadWrite|CFile::typeBinary))
	{
		char m_buffer[3001];		
		m_file.Read(m_buffer,3000);
		//�ж��ļ�ͷ�˸��ַ��Ƿ�Ϊָ���ַ���
		//�ǵĻ������Ǽ��ܹ��ģ������ټ����ˡ�
		if(strncmp(m_buffer,"����֮��",8)==0)
		{
			m_file.Close();
			return false;
		}
	
		//ȡͷ3000�ֽڰ�λ����������,
		//����������ļ���δβ����
		int m_len=strlen(password);
		for(int i=0;i<3000;i++)
			m_buffer[i]^=password[i%m_len];
		m_len=m_file.GetLength();		
		m_file.SeekToEnd();
		m_file.Write(m_buffer,3000);

        //�ڼ��ܺ���ļ���ͷ������һ�����ַ���,
		//��������һЩ��Ҫ�����ַ���һ�̶��ַ�
		//��PW�����,�ٽ�������ļ���ͷ����
		memset(m_buffer,0,3001);
		strcpy(m_buffer,"����֮��");
		int *data=(int *)&m_buffer[8];
		data[0]=m_len;
		data[1]=strlen(password);
		strcpy((char *)&m_buffer[2*sizeof(int)+8],password);
		int temp=(2*sizeof(int)+8)+strlen(password);
		char * PW="xujingzhou";
		int m_PWlen=strlen(PW);
		for(int j=8;j<= temp;j++)
			m_buffer[j]^=PW[j%m_PWlen];
		m_file.SeekToBegin();
		if(m_len<3000)		
			m_file.Write(m_buffer,m_len);
		else
		    m_file.Write(m_buffer,3000);

		m_file.Close();
		
		return true;
	}
	return false;
}

//�����ļ��㷨����
BOOL CEDStar::DecryptFile(CString filename,char *password)
{
	SetFileAttributes(filename,FILE_ATTRIBUTE_ARCHIVE|FILE_ATTRIBUTE_NORMAL);
 
	CFile m_file;
	if(m_file.Open(filename,CFile::modeReadWrite|CFile::typeBinary))
	{
		char m_buffer[3001];		
		memset(m_buffer,0,3001);
		m_file.Read(m_buffer,3000);
		//�ж��ļ�ͷ�˸��ַ��Ƿ�Ϊ���ܺ�ָ��������
		//�������ǵĻ������Ǽ��ܹ��ģ����Խ����ˣ�
		//����Ͳ��ܽ��ܡ�
		if(strncmp(m_buffer,"����֮��",8)!=0)
		{
			m_file.Close();
			return FALSE;
		}
		//����������ֺ�����һЩ��Ҫ�ֽڵĳ��ȣ�
        //��������ܺ���ָ������Ƚϣ��ж���
        // ����ͬ��
		int temp=(2*sizeof(int)+8)+strlen(password);
		char * PW="xujingzhou";
		int m_PWlen=strlen(PW);
        for(int j=8;j<=temp;j++)
			m_buffer[j]^=PW[j%m_PWlen];
		int *data=(int *)&m_buffer[8];
		int m_len=data[1];
		if((m_len!=(int)strlen(password)) || (strcmp((char *)&m_buffer[8+2*sizeof(int)],password)!=0))
		{
			m_file.Close();
			return FALSE;
		}

		//�Ӽ��ܺ���ļ�δβ��ȡ��3000�ֽڣ�
		//�����ܺ���д���ļ���ͷ���ȿɡ�
		m_len=data[0];
		long m_pos=m_file.Seek(-3000,CFile::end);
		int  m_number=m_file.Read(m_buffer,3000);
		int  m_passlen=strlen(password);
		for(int i=0;i<3000;i++)
			m_buffer[i]^=password[i%m_passlen];
		m_file.SeekToBegin();
		m_file.Write(m_buffer,3000);
		m_file.SetLength(m_len);
		m_file.Close();

		return TRUE;
	}
	return FALSE;
}

BOOL CEDStar::DestroyWindow() 
{
	CRect rect2;  //�Ի��򶯻���ʾЧ��
    AnimEffect animation2;

	//�ڴ˼���Ի��򶯻��ر�Ч��
	GetWindowRect(rect2);
	rect2.DeflateRect(2,2);//��̬������ʾ
    animation2.DrawWireRects(rect2, CRect(rect2.CenterPoint().x-10,rect2.CenterPoint().y-10,rect2.CenterPoint().x+10,rect2.CenterPoint().y+10 ), 10);
 //	animation2.Close(rect2); ////�Ի��򶯻�Ч����ʾ�ر�
//	Sleep(50); //�ӻ�50�����ر�
	
	return CDialog::DestroyWindow();
}

void CEDStar::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
    CRect rect2;  //�Ի��򶯻���ʾЧ��
    AnimEffect animation2;

	//�ڴ˼���Ի��򶯻���Ч��
	GetWindowRect(rect2 );
    animation2.Open( rect2 ); //�Ի��򶯻�Ч����ʾ��
	
}

//ѡ��ӽ����ļ��ĶԻ���
void CEDStar::OnBrowse1() 
{
    //���öԻ����������ʾ
	static char BASED_CODE filter[] = _T("�����ļ�(*.*)|*.*||");

	CFileDialog dlg(TRUE,
					NULL, 
					NULL, 
					OFN_HIDEREADONLY	| 
					OFN_OVERWRITEPROMPT | 
					OFN_FILEMUSTEXIST,
					filter,
					0);

	//�õ��ӽ��ܵ�Դ�ļ���m_SourcePath
	//����ʾ�����Ի������Ӧ·�����С�
	if (dlg.DoModal() == IDOK) {
		m_SourcePath = dlg.GetPathName();
		m_Filename=dlg.GetFileName ();
	    UpdateData(FALSE);
	}	
}

//��ʾѡ��ӽ��ܺ󱣴�·���Ի���
void CEDStar::OnBrowse2() 
{
    CDirDialog	dlg;
	if (dlg.DoBrowse(this) == IDOK) {
		//�õ�Ŀ��·����m_TargetPath
		//����ʾ�����Ի������Ӧ·�����С�
		m_TargetPath = dlg.m_strPath;
		UpdateData(FALSE);
	}
	
}

//�����ļ����ܲ���
void CEDStar::OnEncrypt() 
{
    UpdateData(true); //���Ի����и��ؼ�ֵ������Ӧ����
	if(m_SourcePath == "" || m_TargetPath == "" || m_Password == "")
	{
		MessageBox("����ѡ������ļ��ͼ��ܺ�ı���·����\nȻ�����ü�������ȿɣ�","��ʾ");
		return ;
	}

	//��Դ�ļ����Ƶ�Ҫ�����Ŀ��·�����ٽ��м��ܲ���
	CString tempPath;
    if(m_TargetPath.Right (1)=='\\')
       tempPath=m_TargetPath+m_Filename;
	else
       tempPath=m_TargetPath+'\\'+m_Filename;
	CopyFile(m_SourcePath,tempPath,false);

	if(EncryptFile(tempPath,m_Password.GetBuffer (0)))
	{
		MessageBox("�ļ����ܳɹ���","��ʾ");
		//�����Ӧ����ֵ�����������ӡ����ܰ�ť
       m_SourcePath = "";
	   m_Password ="";
       UpdateData(false); //���ı�ı���ֵ������Ӧ�ؼ�
	}
	else
	{
		MessageBox("�ļ�����ʧ�ܣ�","��ʾ");
		if(tempPath != m_SourcePath)
		   DeleteFile(tempPath);  //·����ͬʱ��ɾ���Ѹ����ļ�
	}

}

//�����ļ����ܲ���
void CEDStar::OnDecrypt() 
{
	UpdateData(true); //���Ի����и��ؼ�ֵ������Ӧ����
    if(m_SourcePath == "" || m_TargetPath == "" || m_Password == "")
	{
		MessageBox("����ѡ������ļ��ͽ��ܺ�ı���·����\nȻ�����ý�������ȿɣ�","��ʾ");
		return ;
	}
    
	//��Դ�ļ����Ƶ�Ҫ�����Ŀ��·�����ٽ��н��ܲ���
	CString tempPath;
    if(m_TargetPath.Right (1)=='\\')
       tempPath=m_TargetPath+m_Filename;
	else
       tempPath=m_TargetPath+'\\'+m_Filename;
	CopyFile(m_SourcePath,tempPath,false);

	if(DecryptFile(tempPath,m_Password.GetBuffer (0)))
	{
		MessageBox("�ļ����ܳɹ���","��ʾ");
		//�����Ӧ����ֵ�����������ӡ����ܰ�ť
        m_SourcePath ="";
	    m_Password ="";
        UpdateData(false); //���ı�ı���ֵ������Ӧ�ؼ�
	}
	else
	{
		MessageBox("�ļ�����ʧ�ܣ�","��ʾ");
		if(tempPath != m_SourcePath)
		   DeleteFile(tempPath);  //·����ͬʱ��ɾ���Ѹ����ļ�
	}

}

