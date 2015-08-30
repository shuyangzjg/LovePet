// FileSplit.cpp : implementation file
//
//Modify by �쾰�� ��2001.2.13
//���ܣ��ļ��ָ�Ϊ���ָ�ʽ�Ĵ�С��������ϲ���һ��
//
#include "stdafx.h"
#include "scrgenius.h"
#include "FileSplit.h"
#include "DirDialog.h" //Ŀ¼����Ի���
#include "AnimEffect.h" //�Ի��򶯻�Ч����ʾ

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileSplit dialog


CFileSplit::CFileSplit(CWnd* pParent /*=NULL*/)
	: CDialog(CFileSplit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileSplit)
	m_parts = _T("״̬��ʾ");
	m_path = _T("");
	m_targetpath = _T("");
	m_split = TRUE;
	//}}AFX_DATA_INIT
}


void CFileSplit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileSplit)
	DDX_Control(pDX, IDC_COMBOX_FILESIZE, m_FileSize);
	DDX_Control(pDX, IDC_PROGRESS, pProgress);
	DDX_Control(pDX, IDC_BUTTON_MERGE, m_buttMerge);
	DDX_Control(pDX, IDCANCEL, m_buttExit);
	DDX_Control(pDX, IDOK, m_buttSplit);
	DDX_Control(pDX, IDC_BROWSE, m_Browse);
	DDX_Control(pDX, IDC_BROWSE2, m_Browse2);
	DDX_Control(pDX, IDC_RADIO3, m_RadioSplit);
	DDX_Control(pDX, IDC_PATH, m_edit);
	DDX_Text(pDX, IDC_PARTS, m_parts);
	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Text(pDX, IDC_PATH2, m_targetpath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileSplit, CDialog)
	//{{AFX_MSG_MAP(CFileSplit)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_BROWSE2, OnBrowse2)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON_MERGE, OnButtonMerge)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileSplit message handlers

BOOL CFileSplit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_split) 
		m_RadioSplit.SetCheck(1);

	CButton *pButtonMerge = (CButton*)(GetDlgItem(IDC_BUTTON_MERGE));
	pButtonMerge->EnableWindow(FALSE);

	m_path =_T("");
	m_filename =_T("");

	//�ڴ˳�ʼ������ɫ������
	pProgress.SetRange32(1,500);
    pProgress.SetBkColor(RGB(240,200,90));
	pProgress.ShowPercent(true);	
	pProgress.SetPos(500);

	//����ѡ���ļ���С��ƽ������Ͽ�Ĭ�ϳ�ֵΪ��1��
	m_FileSize.SetCurSel(0);

	//�ڴ˽��ļ��������ť��Ϊƽ��״,��ɫ���Զ��崰��ɫ��ͬ
	m_Browse.SubclassDlgItem(IDC_DIRBROWSER, this);
	m_Browse.SetActiveBgColor(RGB(240,200,90));
	m_Browse.SetInactiveBgColor(RGB(240,200,90));
	m_Browse.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_Browse.SetIcon(IDI_FILEOPEN);
//	m_Browse.SetFlat(FALSE); //����ƽ��ʾ
	m_Browse.SetBtnCursor(IDC_HAND); 

	m_Browse2.SubclassDlgItem(IDC_FILEBROWSER, this);
	m_Browse2.SetActiveBgColor(RGB(240,200,90));
	m_Browse2.SetInactiveBgColor(RGB(240,200,90));
	m_Browse2.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_Browse2.SetIcon(IDI_FILEOPEN);
//	m_Browse2.SetFlat(FALSE); //����ƽ��ʾ
	m_Browse2.SetBtnCursor(IDC_HAND);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//ѡ����ָ��ϲ���Դ�ļ�
void CFileSplit::OnBrowse() 
{
	//���÷ָ��ļ�ʱ�Ի�������ʾ���ļ���չ��
	static char BASED_CODE szSplitFilter[] = _T("���ָ��ļ�(*.*)|*.*||");
	//���úϲ��ļ�ʱ�Ի�������ʾ���ļ���չ��
	static char BASED_CODE szMergeFilter[] = _T("���ϲ��ļ�(1_*.*)|1_*.*||");

	CString filter;
	if (!m_split)
		filter = szMergeFilter;
	else 
		filter = szSplitFilter;

	CFileDialog dlg(TRUE,
					NULL, 
					NULL, 
					OFN_HIDEREADONLY	| 
					OFN_OVERWRITEPROMPT | 
					OFN_FILEMUSTEXIST,
					filter,
					0);

	//��ȡԴ�ļ�����·��
	if (dlg.DoModal() == IDOK) {
		m_path = dlg.GetPathName();
		m_filename = dlg.GetFileName();
		UpdateData(FALSE);
	}
	
}

//ѡ��Ŀ���ļ���
void CFileSplit::OnBrowse2() 
{
   CDirDialog	dlg;

	if (dlg.DoBrowse(this) == IDOK) {
		//��ȡĿ��·��
		m_targetpath = dlg.m_strPath;
		UpdateData(FALSE);
	}	
	
}

//��ȡ�û�ѡ���ļ��ķָ��С
UINT  CFileSplit::GetSplitFileSize()
{
  
    switch(m_FileSize.GetCurSel())
	{
	case 0:
		return 1400000;
    case 1:
		return 1200000;
    case 2:
		return 1000000;
	case 3:
		return 800000;
	default:
		return 1400000;
	}
	
}

// �ļ��ϲ����� 
int CFileSplit::MergeMe() 
{
	CWaitCursor wait;
	CFile destFile;
	CFileException ex;

	BYTE buffer[140000];
	DWORD dwRead;
	UINT nCount = 140000;
	UINT newlen = 1400000;
	char buff [20];
	long l = 1;
	CString name;
	CString pref;
	CString newpath;
		
	UpdateData(TRUE);

	//�ж��ļ��Ƿ����
	if (!m_path.IsEmpty()) {
		if (!m_SourceFile.Open(m_path, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary, &ex)) {
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
      		::AfxMessageBox(szError);
			m_edit.SetFocus();
			m_edit.SetSel(0, -1);
			return 1;
		}
		//��ȡ�ļ���
		m_filename = m_path.Right((m_path.GetLength() - m_path.ReverseFind('\\')) - 1);
		//�ر�Դ�ļ�
		m_SourceFile.Close();
	}

	//��ȡ·����
	newpath = m_path.Left(m_path.GetLength() - m_filename.GetLength());

	if (!m_targetpath.IsEmpty()) {
		//�жϺϲ��ļ����Ƿ���ȷ
		if (!m_filename.IsEmpty() && m_filename.Left(2) != _T("1_")) {
			::AfxMessageBox(_T("���ϲ���Դ�ļ�������..."), MB_ICONERROR);
			return 1;
		}
		else if(m_filename.IsEmpty()) {
			MessageBox(_T("��ѡ����ϲ���Դ�ļ�."), _T("�ļ��ָ���"), MB_ICONEXCLAMATION);
			return 1;
		}
		//��ȡ�µ��ļ���
		m_filename = m_filename.Right(m_filename.GetLength() - 2);
		//�ж�ѡ��Ŀ¼δβ�Ƿ�����"\"��
		if(m_targetpath.Right(1)=='\\')
		    m_path = m_targetpath + m_filename;
		else
			m_path = m_targetpath + _T("\\") + m_filename;
		//����Ŀ���ļ�
		if (!destFile.Open(m_path,  
			CFile::modeWrite		| 
			CFile::shareExclusive	| 
			CFile::typeBinary		| 
			CFile::modeCreate, &ex)) {
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			::AfxMessageBox(szError);
			return 0;
		}
	}
	else if(m_path.IsEmpty())
	{//Դ�ļ�ѡ��Ϊ��
		MessageBox(_T("��ѡ����ϲ���Դ�ļ�."), _T("�ļ��ָ���"), MB_ICONEXCLAMATION);
		return 1;
	}
	if (m_targetpath.IsEmpty()) {//Ŀ���ļ�ѡ��Ϊ��
		MessageBox(_T("��ѡ��ϲ���Ҫ���浽��Ŀ���ļ���."), _T("�ļ��ָ���"), MB_ICONEXCLAMATION);
		return 1;
	}
	//��ʼ�ϲ�
	do {
		//ͨ����̬���ӵ����ֱ������õ��ָ��ļ�����������
		pref = _ltoa(l, buff, 10);
		pref += _T("_");
		if (!m_SourceFile.Open(newpath + pref + m_filename,  
			CFile::modeRead		| 
			CFile::shareExclusive	| 
			CFile::typeBinary, &ex)) {
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			destFile.Close();
			m_path = _T("");
			m_filename = _T("");
//			pProgress.SetPos(0);
			newpath = _T("");
//			m_parts = _T("");
			UpdateData(FALSE);

			return 0;
		}
		else
			//����һ������
			name = _T(newpath + pref + m_filename);
		do  {//д��Ŀ���ļ���
			dwRead = m_SourceFile.Read(buffer, nCount);
			destFile.Write(buffer, dwRead);
		}while (dwRead > 0);

		m_SourceFile.Close();

		// ���ý�������ΧΪ0��500.
		pProgress.SetRange(0, 500);
		// ���ý������ƶ�λ��
		for (int i = 0; i < 500; i++)
			pProgress.SetPos(i);
		m_parts = _ltoa(l, buff, 10);
		m_parts += _T("���ļ��Ѻϲ�");
		UpdateData(FALSE);
		l++;
		UpdateWindow();
	}while (l < 500);
		
	return 0;
}

//�ļ��ָ��
int CFileSplit::SplitMe() 
{
	CWaitCursor wait;
	// Ŀ���ļ�
	CFile destFile;
	// �ļ��������Ϣ����
	CFileException ex;

	DWORD dwRead;
	UINT newlen;
	char buff [20];
	char b [20];
	long l = 1;
	CString name;
	UINT len = 0;
//	CGradientProgressCtrl *pProgress = (CProgressCtrl*) GetDlgItem(IDC_PROGRESS);

	UpdateData(TRUE);

	//��ȡ�ļ��ָ��Ĵ�С���������Ӧ������ֵ
    newlen=GetSplitFileSize();
	UINT nCount = newlen/10;
    BYTE buffer[140000];

	//�ж��Ƿ���ѡ��Դ�ļ����Ƿ��ܴ򿪣����ò����ļ���С
	if (!m_path.IsEmpty()) {
		if (!m_SourceFile.Open(m_path, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary, &ex)) {
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
      		::AfxMessageBox(szError);
			m_edit.SetFocus();
			m_edit.SetSel(0, -1);
			return 1;
		}
		len = m_SourceFile.GetLength();
	}
	else {
		MessageBox(_T("��ѡ����ָ��Դ�ļ�."), _T("�ļ��ָ���"), MB_ICONEXCLAMATION);
		return 1;
	}
	if (m_targetpath.IsEmpty()) {
		MessageBox(_T("��ѡ��ָ�󱣴浽��Ŀ���ļ���."), _T("�ļ��ָ���"), MB_ICONEXCLAMATION);
		return 1;
	}
	//�ж�Դ�ļ���С�Ƿ񹻷ָ�Ĵ�С
	if (len < newlen) {
		CString length = _itoa(len, b, 10);
		MessageBox(_T("�ļ�����Ϊ " + length + " �ֽڣ�����ָ���ķָ��С�� û�б�Ҫ�ٽ��зָ�."), _T("�ļ��ָ���"), MB_ICONEXCLAMATION);
		m_SourceFile.Close();
		m_path = _T("");
		m_filename = _T("");
		UpdateData(FALSE);
		return 1;
	}

	//��ʼ�ָ�
	do {
		//��̬�ı�ָ��ı����ļ���
		name = _ltoa(l, buff, 10);
		name += _T("_");
		CString newpath; 

		//�ж�ѡ��Ŀ¼δβ�Ƿ�����"\"��
		if(m_targetpath.Right(1)=='\\')
			newpath = m_targetpath;
		else
			newpath = m_targetpath + _T("\\");
		if (!destFile.Open(newpath + name + m_SourceFile.GetFileName(),  
			CFile::modeWrite		| 
			CFile::shareExclusive	| 
			CFile::typeBinary		|
			CFile::modeCreate, &ex)) {
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			::AfxMessageBox(szError);
			m_SourceFile.Close();
			return 1;
		}
		do {
			dwRead = m_SourceFile.Read(buffer, nCount);
			destFile.Write(buffer, dwRead);
		}//����СС��ָ���ָ��Сʱ������
		while (dwRead > 0 && destFile.GetLength() < newlen); 
		destFile.Close();
		// ���ý�������Χ
		pProgress.SetRange(0, len /newlen*10);
		// ���ý������ƶ�λ��
		pProgress.SetPos(l);
		m_parts = _ltoa(l , buff, 10);
		m_parts += _T("���ļ�����");
		UpdateData(FALSE);
		l++;
		UpdateWindow();
	}
	while (dwRead > 0);
		// �ر�Դ�ļ�
		m_SourceFile.Close();

	m_path = _T("");
	m_filename = _T("");
	pProgress.SetPos(len /newlen*10);
//	m_parts = _T("");
	UpdateData(FALSE);

	return 0;
}

//��ʼ�ָ��ļ�
void CFileSplit::OnOk() 
{
	 //�ָ��ļ�
	int error = SplitMe();

	//�жϽ���Ƿ�ɹ�
	if (!error) 
		MessageBox(_T("�ļ��ѱ��ɹ��ָ�!"), _T("�ļ��ָ���"), MB_ICONEXCLAMATION);
		
	
}

//��ʼ�ϲ��ļ�
void CFileSplit::OnButtonMerge() 
{
	//�ϲ��ļ�
	int error = MergeMe();

	//�жϽ���Ƿ�ɹ�
	if (!error) 
		MessageBox(_T("�ļ��ϲ��ɹ�!"), _T("�ļ��ָ���"), MB_ICONEXCLAMATION);
	
	
}

//ѡ���ļ��ϲ���ѡ��
void CFileSplit::OnRadio4() 
{
  	CButton *pButtonMerge = (CButton*)(GetDlgItem(IDC_BUTTON_MERGE));
	CButton *pButtonSplit = (CButton*)(GetDlgItem(IDOK));
	
	//��ѡ��ϲ��ļ�ʱ����ѡ���ļ���С����λ��Ϊ��Ч״̬
	m_FileSize.EnableWindow(false); 
	//����������ť��״̬,����ϲ���ť������ָť
	pButtonSplit->EnableWindow(FALSE);
	pButtonMerge->EnableWindow(TRUE);
	m_split = FALSE;
	m_path = _T("");
	UpdateData(FALSE);	
}

//ѡ���ļ��ָѡ��
void CFileSplit::OnRadio3() 
{
	CButton *pButtonMerge = (CButton*)(GetDlgItem(IDC_BUTTON_MERGE));
	CButton *pButtonSplit = (CButton*)(GetDlgItem(IDOK));
	
	//��ѡ��ָ��ļ�ʱ���ָ�ѡ���ļ���С����λ��Ϊ��Ч״̬
	m_FileSize.EnableWindow(true); 
	//����������ť��״̬,����ָť������ϲ���ť
	pButtonSplit->EnableWindow(TRUE);
	pButtonMerge->EnableWindow(FALSE);
	m_split = TRUE;
	m_path = _T("");
	UpdateData(FALSE);	
}

BOOL CFileSplit::DestroyWindow() 
{
   CRect rc0;  //�Ի��򶯻���ʾЧ��
   AnimEffect anim0;

	//�ڴ˼���Ի��򶯻��ر�Ч��
	GetWindowRect(rc0);
	rc0.DeflateRect(2,2);//��̬������ʾ
    anim0.DrawWireRects(rc0, CRect(rc0.CenterPoint().x-10,rc0.CenterPoint().y-10,rc0.CenterPoint().x+10,rc0.CenterPoint().y+10 ), 10);
//	anim0.Close(rc0); ////�Ի��򶯻�Ч����ʾ�ر�
//	Sleep(50); //�ӻ�50�����ر�
	
	return CDialog::DestroyWindow();
}

void CFileSplit::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
   CRect rc0;  //�Ի��򶯻���ʾЧ��
   AnimEffect anim0;

	//�ڴ˼���Ի��򶯻���Ч��
	GetWindowRect(rc0 );
    anim0.Open( rc0 ); //�Ի��򶯻�Ч����ʾ��	
}
