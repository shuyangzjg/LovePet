// BMP2TXT.cpp : implementation file
//Modify by �쾰�� 2000.10
//���ܣ�λͼת��ΪASCII�ı�

#include "stdafx.h"
#include "scrgenius.h"
#include "BMP2TXT.h"
#include "AnimEffect.h" //�Ի��򶯻�Ч����ʾ

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBMP2TXT dialog


CBMP2TXT::CBMP2TXT(CWnd* pParent /*=NULL*/)
	: CDialog(CBMP2TXT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBMP2TXT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	//��ʼ��������ֵ
	m_DirPathString = _T(""); //ѡ��λͼ�ļ�������
	m_FilePathString = _T(""); //���Ϊ�ı�����

	m_bSetPalette = TRUE;
    m_lpBMFileHeader = NULL;
    m_lpBMInfoHeader = NULL;
    m_lpLogPalette = NULL;
    m_nBytesPerLine = 0;
    m_pData = NULL;
    m_pFileBuffer = NULL;
}

//��������, �ͷŲ�Ϊ�յ����ݺ͵�ɫ���ڴ�ռ�
CBMP2TXT::~CBMP2TXT()
{
    if (m_lpLogPalette)
    {
        LocalFree(m_lpLogPalette);
    }

    if (m_pFileBuffer)
    {
        LocalFree(m_pFileBuffer);
    }
}

void CBMP2TXT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBMP2TXT)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
	DDX_Text(pDX, IDC_EDIT1, m_DirPathString); //λͼѡ��༭�����
	DDX_Text(pDX, IDC_EDIT2, m_FilePathString);//�ı�����༭�����
	DDX_Control(pDX, IDOK,  m_Ok);
	DDX_Control(pDX, IDCANCEL,  m_Cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBMP2TXT, CDialog)
	//{{AFX_MSG_MAP(CBMP2TXT)
	ON_BN_CLICKED(IDC_DIRBROWSER, OnDirbrowser)
	ON_BN_CLICKED(IDC_FILEBROWSER, OnFilebrowser)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMP2TXT message handlers
//��λͼѡ��Ի���
void CBMP2TXT::OnDirbrowser() 
{
	CFileDialog fileDialog(TRUE,NULL,NULL,NULL,"λͼ�ļ�(*.bmp)|*.bmp||");

	if (fileDialog.DoModal() == IDOK) {
		strOpenBMPName = fileDialog.GetPathName();
		m_DirPathString = strOpenBMPName;
		UpdateData(FALSE);
	}	
}

//���Ϊ�ı��Ի���
void CBMP2TXT::OnFilebrowser() 
{
  CFileDialog fileDialog(FALSE,"*.txt",NULL,NULL,"�ı��ļ�(*.txt)|*.txt||");
	if (fileDialog.DoModal() == IDOK) {
		strSaveTXTName=fileDialog.GetPathName();		
		m_FilePathString = strSaveTXTName;
		
		UpdateData(FALSE);
	}	
}

void CBMP2TXT::OnOK() 
{
  if (strSaveTXTName.IsEmpty() || strOpenBMPName.IsEmpty ())
	{
		AfxMessageBox("\n ��Ǹ,��ѡ��λͼ�ļ��������Ϊ���ı��ļ���! ");
		return;
	}
  
  CString strTemp;
    strTemp="�ı��ļ�:"+strSaveTXTName+"ת���ɹ�!";
	if (!ConvertToText(strOpenBMPName,strSaveTXTName))
	{
		AfxMessageBox("��Ǹ, ת��ʧ��!");
	}
	else
        AfxMessageBox(strTemp);
	
//	CDialog::OnOK();
}

//��ȡ BMP�ļ�����, ����ָ�븳ֵ�ͼ����߼���ɫ��
//�ݲ�֧�� JPEG �� PNG��ʽ
BOOL CBMP2TXT::Read(CString strBMPName)
{
    CFile File;
    //��ֻ����ʽ���ļ�
    BOOL bResult = File.Open(strBMPName, CFile::modeRead);
	if (!bResult)
	{
		CString strErrorMessage;
		strErrorMessage = "���ļ�:" + strBMPName + "ʧ�� !";
		AfxMessageBox(strErrorMessage);
		return FALSE;
	}

    //ȡ���ļ�����
    int nFileLength = File.GetLength();

    //���ļ���������ռ�
    m_pFileBuffer = (char *)LocalAlloc(LPTR, nFileLength);
    if (!m_pFileBuffer)
    {
		AfxMessageBox("���������ڴ�ռ�ʧ�� !");
        return FALSE;
    }

    //��ȡ�ļ���������
    int nBytesRead = File.Read(m_pFileBuffer, nFileLength);
    if (nBytesRead != nFileLength)
    {
		AfxMessageBox("��ȡ�ļ�����ʧ�� !");
        return FALSE;
    }

    //�ļ�ͷָ�븳ֵ
    m_lpBMFileHeader = (LPBITMAPFILEHEADER)m_pFileBuffer;

    //�ж��ļ�����
    if (m_lpBMFileHeader->bfType != 0x4d42) // 'BM'
    {
		CString strErrorMessage;
		strErrorMessage = "�ļ�:" + strBMPName + "������Ч��λͼ�ļ� !";
		AfxMessageBox(strErrorMessage);
        return FALSE;
    }

    //��Ϣͷָ�븳ֵ
    m_lpBMInfoHeader = (LPBITMAPINFOHEADER)(m_pFileBuffer + sizeof(BITMAPFILEHEADER));
    
    //����ÿ��ռ�õ��ֽ��� (m_lpBMInfoHeader��biSizeImage��Ա��ʱΪ�ղ�����)
    //m_nBytesPerLine = m_lpBMInfoHeader->biSizeImage / m_lpBMInfoHeader->biHeight;
    m_nBytesPerLine = m_lpBMInfoHeader->biWidth * m_lpBMInfoHeader->biBitCount / 8;
    if (m_nBytesPerLine % 4 != 0)
        m_nBytesPerLine = (m_nBytesPerLine / 4 + 1) * 4;

    //����ָ�븳ֵ
    m_pData = m_pFileBuffer + m_lpBMFileHeader->bfOffBits;

    //Ϊ��ɫ������ռ�
    m_lpLogPalette = (LPLOGPALETTE)LocalAlloc(LPTR, sizeof(LOGPALETTE) + 256 * sizeof(PALETTEENTRY));
    m_lpLogPalette->palVersion = 0x300;

    //�ж��Ƿ���ʹ�õ�ɫ��
    switch (m_lpBMInfoHeader->biBitCount)
    {
    case 0: //JPEG �� PNG ��ʽ
        m_bSetPalette = FALSE;
        break;
        
    case 1:
        m_lpLogPalette->palNumEntries = 2;
        m_bSetPalette = TRUE;
        break;
        
    case 4:
        m_lpLogPalette->palNumEntries = 16;
        m_bSetPalette = TRUE;
        break;
        
    case 8:
        m_lpLogPalette->palNumEntries = 256;
        m_bSetPalette = TRUE;
        break;
        
    case 16:
    case 24:
    case 32:
        m_bSetPalette = FALSE;
        break;
        
    default:
		AfxMessageBox("�ļ�ɫ��������ʶ�� !");
        return FALSE;
    }
        
    //������ʱ�ռ��Դ����ɫ��
    char *pPalette = m_pFileBuffer + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    if (!pPalette)
	{
		AfxMessageBox("���������ڴ�ռ�ʧ�� !");
        return FALSE;
	}
        
    RGBQUAD rgbQuad[256];
    //��ɫ�̸�ֵ
    memcpy(rgbQuad, pPalette, sizeof(PALETTEENTRY) * m_lpLogPalette->palNumEntries);
    for (int i = 0; i < m_lpLogPalette->palNumEntries; i ++)
    {                                               
        m_lpLogPalette->palPalEntry[i].peBlue = rgbQuad[i].rgbBlue;
        m_lpLogPalette->palPalEntry[i].peGreen = rgbQuad[i].rgbGreen;
        m_lpLogPalette->palPalEntry[i].peRed = rgbQuad[i].rgbRed;
        m_lpLogPalette->palPalEntry[i].peFlags = rgbQuad[i].rgbReserved;
    }

    return TRUE;
}


// ������ BMP�ļ�ת��Ϊ�ı��ļ�, ��֧�� 256ɫ��ʽ����ʸ�ʽ
BOOL CBMP2TXT::ConvertToText(CString strBMPName,CString strTXTName)
{
    //�ַ���ɫ��
    BYTE CharPalette[16];
    CharPalette[0] = '#';
    CharPalette[1] = 'M';
    CharPalette[2] = '@';
    CharPalette[3] = 'H';
    CharPalette[4] = 'X';
    CharPalette[5] = '$';
    CharPalette[6] = '%';
    CharPalette[7] = '+';
    CharPalette[8] = '/';
    CharPalette[9] = ';';
    CharPalette[10] = ':';
    CharPalette[11] = '=';
    CharPalette[12] = '-';
    CharPalette[13] = ',';
    CharPalette[14] = '.';
    CharPalette[15] = ' ';
    

    CStdioFile TxtFile;

    //�����ı��ļ�
    if (!TxtFile.Open(strTXTName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
    {
		CString strErrorMessage;
		strErrorMessage = "�����ı��ļ�:" + strTXTName + "ʧ�� !";
		AfxMessageBox(strErrorMessage);
        return FALSE;
    }

    //��ȡ BMP�ļ�����
    if (!Read(strBMPName))
    {
        return FALSE;
    }

    //���ý���ɫ������
    m_ProgressBar.SetPos(1);

    //ȡ�� BMP����ָ��
    BYTE *pData = (BYTE *)m_pData;
    BYTE *pLine = pData;


    for (int i = m_lpBMInfoHeader->biHeight - 1; i >= 0; i--)
    {
        //����ÿ�е�����ָ��
        pLine = pData + i * m_nBytesPerLine;
        CString strLineText;
        for (int j = 0; j < m_lpBMInfoHeader->biWidth; j++)
        {
            int nRed, nGreen, nBlue, nValue;

            //���㵱ǰ���ص� RGB��������ֵ
            switch (m_lpBMInfoHeader->biBitCount)
            {
            case 24:
                nRed = *pLine++;
                nGreen = *pLine++;
                nBlue = *pLine++;
                break;

            case 8:
                nRed = m_lpLogPalette->palPalEntry[*pLine].peRed;
                nGreen = m_lpLogPalette->palPalEntry[*pLine].peGreen;
                nBlue = m_lpLogPalette->palPalEntry[*pLine].peBlue;
                pLine++;
                break;

            default:
				AfxMessageBox("��Ǹ, ��֧�� 8λ��24λλͼ�ļ� !");
                return FALSE;
            }

            //����Ҷ�ֵ
            nValue = (nRed * 30 + nGreen * 59 + nBlue * 11) / 100;

            //ת�����ı�
            strLineText += CharPalette[nValue / 16];
        }

        //д���ı��ļ�
        TxtFile.WriteString(strLineText);
        TxtFile.WriteString("\n");

		//�ı��ļ�ת����������ʾ(�ɴ�С)
		if(i<=0)
		  m_ProgressBar.SetPos(10);
		else if((i%(m_lpBMInfoHeader->biHeight/8+1))==0)
		  m_ProgressBar.StepIt();
		  
    }

    TxtFile.Close();

    return TRUE;
}

BOOL CBMP2TXT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   //�ڴ˽��ļ��������ť��Ϊƽ��״,��ɫ���Զ��崰��ɫ��ͬ
	m_SourceFile.SubclassDlgItem(IDC_DIRBROWSER, this);
	m_SourceFile.SetActiveBgColor(RGB(240,200,90));
	m_SourceFile.SetInactiveBgColor(RGB(240,200,90));
	m_SourceFile.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_SourceFile.SetIcon(IDI_FILEOPEN);
//	m_SourceFile.SetFlat(FALSE); //����ƽ��ʾ
	m_SourceFile.SetBtnCursor(IDC_HAND); 

	m_DescFile.SubclassDlgItem(IDC_FILEBROWSER, this);
	m_DescFile.SetActiveBgColor(RGB(240,200,90));
	m_DescFile.SetInactiveBgColor(RGB(240,200,90));
	m_DescFile.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_DescFile.SetIcon(IDI_FILEOPEN);
//	m_DescFile.SetFlat(FALSE); //����ƽ��ʾ
	m_DescFile.SetBtnCursor(IDC_HAND);
	
	 //�ڴ˳�ʼ������ɫ������
	    m_ProgressBar.SetRange32(1,10);
		m_ProgressBar.SetBkColor(RGB(240,200,90));
		m_ProgressBar.ShowPercent(true);
		m_ProgressBar.SetStep(1);		
		m_ProgressBar.SetPos(10);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBMP2TXT::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
   CRect rc3;  //�Ի��򶯻���ʾЧ��
   AnimEffect anim3;

	//�ڴ˼���Ի��򶯻���Ч��
	GetWindowRect(rc3 );
    anim3.Open( rc3 ); //�Ի��򶯻�Ч����ʾ��

}

BOOL CBMP2TXT::DestroyWindow() 
{
   CRect rc3;  //�Ի��򶯻���ʾЧ��
   AnimEffect anim3;

	//�ڴ˼���Ի��򶯻��ر�Ч��
	GetWindowRect(rc3 );
	rc3.DeflateRect(2,2);//��̬������ʾ
    anim3.DrawWireRects(rc3, CRect(rc3.CenterPoint().x-10,rc3.CenterPoint().y-10,rc3.CenterPoint().x+10,rc3.CenterPoint().y+10 ), 10);
//	anim3.Close(rc3); ////�Ի��򶯻�Ч����ʾ�ر�
//	Sleep(50); //�ӻ�50�����ر�

	return CDialog::DestroyWindow();
}
