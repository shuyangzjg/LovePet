// HideInBMP.cpp : implementation file
//
//Modify by �쾰�� 2000.11
//���ܣ��������ļ����ص�λͼ�У����ɷ������

#include "stdafx.h"
#include "scrgenius.h"
#include "HideInBMP.h"
#include "AnimEffect.h" //�Ի��򶯻�Ч����ʾ


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//����������bmp��RGB�ĵ�2,1,3λ
//10lλͼ������ļ���ƫ��
//28l���ص�λ��
/////////////////////////////////////////////////////////////////////////////
// HideInBMP dialog


HideInBMP::HideInBMP(CWnd* pParent /*=NULL*/)
	: CDialog(HideInBMP::IDD, pParent)
{
	//{{AFX_DATA_INIT(HideInBMP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	//��ʼ��������ֵ
	m_DirPathString = _T(""); //ѡ��λͼ�ļ�������
	for(int i=0;i<8;i++)
	{
		mask[i]=1<<i;
	}
}


void HideInBMP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HideInBMP)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress1);
   	DDX_Control(pDX, IDC_HIDEFILE,  m_HideFile);
	DDX_Control(pDX, IDOK,  m_Ok);
	DDX_Control(pDX, IDCANCEL,  m_Cancel);
	DDX_Text(pDX, IDC_EDIT1, m_DirPathString); //λͼѡ��༭�����
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HideInBMP, CDialog)
	//{{AFX_MSG_MAP(HideInBMP)
	ON_BN_CLICKED(IDC_DIRBROWSER, OnDirbrowser)
	ON_BN_CLICKED(IDC_HIDEFILE, OnHideFile)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HideInBMP message handlers
//��λͼ�Ի���
void HideInBMP::OnDirbrowser() 
{
CFileDialog fileDialog(TRUE,NULL,NULL,NULL,"λͼ�ļ�(*.bmp)|*.bmp||");

	if (fileDialog.DoModal() == IDOK) {
		strOpenBMPName = fileDialog.GetPathName();
		m_DirPathString = strOpenBMPName;
		UpdateData(FALSE);
	}	
}

//�����ļ���λͼ
void HideInBMP::OnHideFile() 
{
	CString FileName,FileName1,FileTitle,FileExt;
	unsigned int file_len=0,pic_offset=0,bmp_len=0,len=0;
	state='r';
	number=0;
	if (strOpenBMPName.IsEmpty ())
	{
		AfxMessageBox("\n ��Ǹ,����ѡ��Ҫ���ص���λͼ�ļ�! ");
		return;
	}
	FileName=strOpenBMPName;
	FileName.ReleaseBuffer();
	CFileDialog m_Dialog1(TRUE,NULL,"*.*",NULL,"�����ļ�(*.*)||");
	if(m_Dialog1.DoModal()!=IDOK)
    {
	    AfxMessageBox("\n ��Ǹ,��ѡ����Ҫ�������ص��ļ���! ");
		return;
	}
	RedrawWindow(); //�ָ������ǵĴ���
	FileName1=m_Dialog1.GetPathName();
	FileName1.ReleaseBuffer();
	try
	{
        m_File1.Open(FileName,CFile::modeReadWrite|CFile::typeBinary);
	    m_File1.Seek(28l,0);
		m_File1.Read(&byte_buf,1);
		if(byte_buf!=24) //�Ƿ�Ϊ24λ
		{
			m_File1.Close();
			MessageBox("��ѡ���λͼ�ļ�����24λ,������ѡ��!","����",MB_OK|MB_ICONSTOP);
			return;
		}
		m_File1.Seek(10l,0);
		bmp_len=m_File1.GetLength();
		m_File1.Read(&pic_offset,1);
		bmp_len-=pic_offset-1;
		m_File2.Open(FileName1,CFile::modeRead|CFile::typeBinary);
		file_len=m_File2.GetLength();
		if(file_len>=bmp_len/4-45)
		{
			m_File1.Close();
			m_File2.Close();
			MessageBox("��ǫ����ѡ���λͼ�ļ����������������ļ�!","����",MB_OK|MB_ICONSTOP);
			return;
		}
		m_File1.Seek(pic_offset,0);
		int len1,len2,len3,len4;
		len1=file_len&0xff;
		len2=(file_len>>8)&0xff;
		len3=(file_len>>16)&0xff;
		len4=(file_len>>24)&0xff;
		Put(len1);
		Put(len2);
		Put(len3);
		Put(len4);
		char *filetitle,*fileext;
		FileTitle=m_Dialog1.GetFileTitle();
		FileExt=m_Dialog1.GetFileExt();
		FileTitle.ReleaseBuffer();
		FileTitle.ReleaseBuffer();
		filetitle=FileTitle.GetBuffer(FileTitle.GetLength());
		fileext=FileExt.GetBuffer(FileExt.GetLength());
		for(int i=0;i<8;i++)
		{
			if(i<FileTitle.GetLength())
			{
				Put(*filetitle);
				filetitle++;
			}
			else
			{
				char temp='*';
				Put(temp);
			}
		}
		for(i=0;i<3;i++)
		{
			if(i<FileExt.GetLength())
			{
				Put(*fileext);
				fileext++;
			}
			else
			{
				char temp='*';
				Put(temp);
			}
		}
	
		//���ý���ɫ������
        m_Progress1.SetPos(1);

		while(len<file_len)
		{
			m_File2.Read(&byte_buf,1);
			Put(byte_buf);
			len++;
			
			//�ı��ļ����ؽ�������ʾ(��С����)
		    if(len>=file_len)
		      m_Progress1.SetPos(10);
		    else if((len%(file_len/9+1))==0)
			  m_Progress1.StepIt();
		}
		m_File2.Close();
		m_File1.Close();
		CString ok;
		ok.Format("�ѳɹ����ļ�%s���ص�λͼ%s��!",FileName1,FileName);
		MessageBox(ok,"��ʾ",MB_OK);
	}
	catch( CFileException* fe )
	{
		switch(fe->m_cause)
		{
		case CFileException::fileNotFound:
			MessageBox("�ļ�û���ҵ���������ѡ��!","����",MB_OK|MB_ICONERROR);
		default:
			MessageBox("�ļ���������������ѡ��!","����",MB_OK|MB_ICONERROR);
		}
		m_File1.Close(); //�ر�����򿪵��ļ�
		fe->Delete();
	}		
}

//��λͼ�з�����ļ�
void HideInBMP::OnOK() 
{
	try
	{
	  if (strOpenBMPName.IsEmpty ())
	  {
		AfxMessageBox("\n ��Ǹ,����ѡ��Ҫ�����ļ������λͼ�ļ�! ");
		return;
	  }
		CString FilePath,FileTitle,FileExt;
		state='r';
    	number=0;
		FilePath=strOpenBMPName;
		FilePath.ReleaseBuffer();
		unsigned int bmp_len=0,pic_offset=0,len=0,file_len=0;
		char tag;
		m_File1.Open(FilePath,CFile::modeRead|CFile::typeBinary);
		m_File1.Seek(28l,0);
		m_File1.Read(&tag,1);
		if(tag!=24) //�Ƿ�24λλͼ
		{
			m_File1.Close();
			MessageBox("��ѡ���λͼ�ļ�����24λ��������ѡ��!","����",MB_OK|MB_ICONSTOP);
			return;			
		}
        m_File1.Seek(10l,0);
		m_File1.Read(&pic_offset,1);
		m_File1.Seek(pic_offset,0);
		int len1,len2,len3,len4;
		len1=Get();
		len2=Get();
		len3=Get();
		len4=Get();
		file_len=len1+(len2<<8)+(len3<<16)+(len4<<24);
		for(int i=0;i<8;i++)
		{
			char temp;
			temp=Get();
			if(temp!='*')
				FileTitle+=temp;
		}
		for(i=0;i<3;i++)
		{
			char temp;
			temp=Get();
			if(temp!='*')
				FileExt+=temp;
		}
		FilePath=strOpenBMPName;
		int le=FilePath.GetLength();
		for(i=0;i<=le-1;i++)
		{
			if(FilePath.Right(1)!="\\") FilePath.Delete(le-1-i,1);
			else break;
		}		
		FilePath+=FileTitle;
		FilePath+=".";
		FilePath+=FileExt;
		
		//���ý���ɫ������
        m_Progress1.SetPos(1);

		m_File2.Open(FilePath,CFile::modeCreate|CFile::typeBinary|CFile::modeWrite);
		while(len<file_len)
		{
			char temp;
			temp=Get();
			m_File2.Write(&temp,1);
			len++;

			//�ı��ļ������������ʾ
		    if(len>=file_len)
		      m_Progress1.SetPos(10);
		    else if((len%(file_len/9+1))==0)
			  m_Progress1.StepIt();
		}
		m_File1.Close();
		m_File2.Close();
		CString String;
		String.Format("�ѳɹ����ļ�%s��λͼ��ȡ��������λ��Ϊλͼ���ڵ�·��!",FilePath);
		MessageBox(String,"��ʾ",MB_OK);
	}
	catch( CFileException* fe )
	{

		switch(fe->m_cause)
		{
		case CFileException::fileNotFound:
			MessageBox("�ļ�û���ҵ���������ѡ��!","����",MB_OK|MB_ICONERROR);
		default:
			MessageBox("�ļ���������,������ѡ��!","����",MB_OK|MB_ICONERROR);
		}
		m_File1.Close(); //�ر�����򿪵��ļ�
		fe->Delete();
	}	
	return;
	
//	CDialog::OnOK();
}

//�����ļ���λͼ����
void HideInBMP::Put(char byte_buf)
{
	try{
		m_File1.Read(byte_bufs,5);
		if(state=='r'&&number==0)
		{
			if(byte_buf&mask[0]) byte_bufs[0]|=1;
			else byte_bufs[0]&=0xfe;
			if((byte_buf&mask[1])>>1) byte_bufs[0]|=2;
			else byte_bufs[0]&=0xfd;
			if((byte_buf&mask[2])>>2) byte_bufs[1]|=1;
			else byte_bufs[1]&=0xfe;
			if((byte_buf&mask[3])>>3) byte_bufs[2]|=1;
			else byte_bufs[2]&=0xfe;
			if((byte_buf&mask[4])>>4) byte_bufs[2]|=2;
			else byte_bufs[2]&=0xfd;
			if((byte_buf&mask[5])>>5) byte_bufs[2]|=4;
			else byte_bufs[2]&=0xfb;
			if((byte_buf&mask[6])>>6) byte_bufs[3]|=1;
			else byte_bufs[3]&=0xfe;
			if((byte_buf&mask[7])>>7) byte_bufs[3]|=2;
			else byte_bufs[3]&=0xfd;
			m_File1.Seek(-5l,1);
			m_File1.Write(byte_bufs,5);
			state='g';
			number=0;
			m_File1.Seek(-1l,1);
			return;
		}
		if(state=='g'&&number==0)
		{
			if(byte_buf&mask[0]) byte_bufs[0]|=1;
			else byte_bufs[0]&=0xfe;
			if((byte_buf&mask[1])>>1) byte_bufs[1]|=1;
			else byte_bufs[1]&=0xfe;
			if((byte_buf&mask[2])>>2) byte_bufs[1]|=2;
			else byte_bufs[1]&=0xfd;
			if((byte_buf&mask[3])>>3) byte_bufs[1]|=4;
			else byte_bufs[1]&=0xfb;
			if((byte_buf&mask[4])>>4) byte_bufs[2]|=1;
			else byte_bufs[2]&=0xfe;
			if((byte_buf&mask[5])>>5) byte_bufs[2]|=2;
			else byte_bufs[2]&=0xfd;
			if((byte_buf&mask[6])>>6) byte_bufs[3]|=1;
			else byte_bufs[3]&=0xfe;
			if((byte_buf&mask[7])>>7) byte_bufs[4]|=1;
			else byte_bufs[4]&=0xfe;
			m_File1.Seek(-5l,1);
			m_File1.Write(byte_bufs,5);
			state='b';
			number=1;
			m_File1.Seek(-1l,1);
			return;
		}
		if(state=='b'&&number==1)
		{
			if(byte_buf&mask[0]) byte_bufs[0]|=2;
			else byte_bufs[0]&=0xfd;
			if((byte_buf&mask[1])>>1) byte_bufs[0]|=4;
			else byte_bufs[0]&=0xfb;
			if((byte_buf&mask[2])>>2) byte_bufs[1]|=1;
			else byte_bufs[1]&=0xfe;
			if((byte_buf&mask[3])>>3) byte_bufs[1]|=2;
			else byte_bufs[1]&=0xfd;
			if((byte_buf&mask[4])>>4) byte_bufs[2]|=1;
			else byte_bufs[2]&=0xfe;
			if((byte_buf&mask[5])>>5) byte_bufs[3]|=1;
			else byte_bufs[3]&=0xfe;
			if((byte_buf&mask[6])>>6) byte_bufs[3]|=2;
			else byte_bufs[3]&=0xfd;
			if((byte_buf&mask[7])>>7) byte_bufs[3]|=4;
			else byte_bufs[3]&=0xfb;
			m_File1.Seek(-5l,1);
			m_File1.Write(byte_bufs,5);
			m_File1.Seek(-1l,1);
			state='r';
			number=0;
			return;
		}
	}
	catch( CFileException* fe )
	{
		switch(fe->m_cause)
		{
		case CFileException::fileNotFound:
			MessageBox("û���ҵ��ļ�,�����ܼ���.","����",MB_OK|MB_ICONERROR);
		default:
			MessageBox("�ļ�����,�����ܼ���.","����",MB_OK|MB_ICONERROR);
		}
		fe->Delete();
	}	
	return;
}

//��λͼ�з�����ļ�����
int HideInBMP::Get()
{
	try
	{
		m_File1.Read(&byte_bufs,5);
		byte_buf=0;
		if(state=='r'&&number==0)
		{
			if(byte_bufs[0]&1==1)      byte_buf|=mask[0];
			if((byte_bufs[0]>>1)&1==1) byte_buf|=mask[1];
			if(byte_bufs[1]&1==1)      byte_buf|=mask[2];
			if(byte_bufs[2]&1==1)      byte_buf|=mask[3];
			if((byte_bufs[2]>>1)&1==1) byte_buf|=mask[4];
			if((byte_bufs[2]>>2)&1==1) byte_buf|=mask[5];
			if(byte_bufs[3]&1==1)      byte_buf|=mask[6];
			if((byte_bufs[3]>>1)&1==1) byte_buf|=mask[7];
			m_File1.Seek(-1l,1);
			state='g';
			number=0;
			return byte_buf;
		}
		if(state=='g'&&number==0)
		{
			if(byte_bufs[0]&1==1)      byte_buf|=mask[0];
			if(byte_bufs[1]&1==1)      byte_buf|=mask[1];
			if((byte_bufs[1]>>1)&1==1) byte_buf|=mask[2];
			if((byte_bufs[1]>>2)&1==1) byte_buf|=mask[3];
			if(byte_bufs[2]&1==1)      byte_buf|=mask[4];
			if((byte_bufs[2]>>1)&1==1) byte_buf|=mask[5];
			if(byte_bufs[3]&1==1)      byte_buf|=mask[6];
			if(byte_bufs[4]&1==1)      byte_buf|=mask[7];
			m_File1.Seek(-1l,1);
			state='b';
			number=1;
			return byte_buf;
		}
		if(state=='b'&&number==1)
		{
			if((byte_bufs[0]>>1)&1==1) byte_buf|=mask[0];
			if((byte_bufs[0]>>2)&1==1) byte_buf|=mask[1];
			if(byte_bufs[1]&1==1)      byte_buf|=mask[2];
			if((byte_bufs[1]>>1)&1==1) byte_buf|=mask[3];
			if(byte_bufs[2]&1==1)      byte_buf|=mask[4];
			if(byte_bufs[3]&1==1)      byte_buf|=mask[5];
			if((byte_bufs[3]>>1)&1==1) byte_buf|=mask[6];
			if((byte_bufs[3]>>2)&1==1) byte_buf|=mask[7];
			m_File1.Seek(-1l,1);
			state='r';
			number=0;
			return byte_buf;
		}
	}
	catch( CFileException* fe )
	{
		switch(fe->m_cause)
		{
		case CFileException::fileNotFound:
			MessageBox("û���ҵ��ļ�,�����ܼ���.","����",MB_OK|MB_ICONERROR);
		default:
			MessageBox("�ļ�����,�����ܼ���.","����",MB_OK|MB_ICONERROR);
		}
		fe->Delete();
	}	    
	return 0;
}

BOOL HideInBMP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   //�ڴ˽��ļ������ť��Ϊ��ƽ״,��ɫ���Զ��崰��ɫ��ͬ
	m_SourceFile.SubclassDlgItem(IDC_DIRBROWSER, this);
	m_SourceFile.SetActiveBgColor(RGB(240,200,90));
	m_SourceFile.SetInactiveBgColor(RGB(240,200,90));
	m_SourceFile.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_SourceFile.SetIcon(IDI_FILEOPEN);
//	m_SourceFile.SetFlat(FALSE); //����ƽ��ʾ
	m_SourceFile.SetBtnCursor(IDC_HAND); 

	    //�ڴ˳�ʼ������ɫ������
	    m_Progress1.SetRange32(1,10);
		m_Progress1.ShowPercent(true);
		m_Progress1.SetBkColor(RGB(240,200,90));
		m_Progress1.SetStep(1);      
		m_Progress1.SetPos(10);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL HideInBMP::DestroyWindow() 
{
	CRect rc6;  //�Ի��򶯻���ʾЧ��
    AnimEffect anim6;

   //�ڴ˼���Ի��򶯻��ر�Ч��
	GetWindowRect(rc6 );
	rc6.DeflateRect(2,2);//��̬������ʾ
    anim6.DrawWireRects(rc6, CRect(rc6.CenterPoint().x-10,rc6.CenterPoint().y-10,rc6.CenterPoint().x+10,rc6.CenterPoint().y+10 ), 10);
//	anim6.Close(rc6); ////�Ի��򶯻�Ч����ʾ�ر� 
//	Sleep(50); //�ӻ�50�����ر�
	
	return CDialog::DestroyWindow();
}

void HideInBMP::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	

    CRect rc6;  //�Ի��򶯻���ʾЧ��
    AnimEffect anim6;

	//�ڴ˼���Ի��򶯻���Ч��
	
 	GetWindowRect(rc6 );
    anim6.Open( rc6 ); //�Ի��򶯻�Ч����ʾ��

}
