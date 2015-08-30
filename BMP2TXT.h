#if !defined(AFX_BMP2TXT_H__5BEFA200_C968_11D4_9000_C3A311B28F6A__INCLUDED_)
#define AFX_BMP2TXT_H__5BEFA200_C968_11D4_9000_C3A311B28F6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BMP2TXT.h : header file
//
#include "BtnST.h" //ƽ�水ť��
#include "EllepticalButton.h" //��Բ��ť��
#include "GradientProgressCtrl.h" //����ɫ��������
/////////////////////////////////////////////////////////////////////////////
// CBMP2TXT dialog

class CBMP2TXT : public CDialog
{
// Construction
public:
	CBMP2TXT(CWnd* pParent = NULL);   // standard constructor
    ~CBMP2TXT();

	CString strOpenBMPName;
    CString strSaveTXTName;
	CString	m_DirPathString;
	CString	m_FilePathString;

	LPBITMAPFILEHEADER	m_lpBMFileHeader; //λͼ�ļ�ͷ
	LPBITMAPINFOHEADER	m_lpBMInfoHeader; //λͼ��Ϣͷ

	BOOL Read(CString strBMPName); //��ȡλͼ�ļ�
	BOOL ConvertToText(CString strBMPName,CString strTXTName);//ת��λͼΪASCII
private:
	char *m_pFileBuffer;

//	LPBITMAPFILEHEADER	m_lpBMFileHeader;
//	LPBITMAPINFOHEADER	m_lpBMInfoHeader;
	char				*m_pData;
	LPLOGPALETTE		m_lpLogPalette;
	BOOL m_bSetPalette;

	int m_nBytesPerLine;
// Dialog Data
	//{{AFX_DATA(CBMP2TXT)
	enum { IDD = IDD_BMP2TXT };
	CGradientProgressCtrl	m_ProgressBar;
	//}}AFX_DATA

	CButtonST m_SourceFile,m_DescFile; //����ƽ�水ť
	//������Բ��ť
	CEllepticalButton m_Ok,m_Cancel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMP2TXT)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBMP2TXT)
	afx_msg void OnDirbrowser();
	afx_msg void OnFilebrowser();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMP2TXT_H__5BEFA200_C968_11D4_9000_C3A311B28F6A__INCLUDED_)
