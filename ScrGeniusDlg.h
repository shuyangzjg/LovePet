// ScrGeniusDlg.h : header file
//

#if !defined(AFX_ScrGeniusDLG_H__D363F3E7_49F3_11D3_8660_8A601C3D7832__INCLUDED_)
#define AFX_ScrGeniusDLG_H__D363F3E7_49F3_11D3_8660_8A601C3D7832__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BtnST.h"  //ƽ�水ť��
//#include "EllepticalButton.h" //��Բ��ť��
/////////////////////////////////////////////////////////////////////////////
// CScrGeniusDlg dialog

class CScrGeniusDlg : public CDialog
{
// Construction
public:
	CScrGeniusDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CScrGeniusDlg)
	enum { IDD = IDD_ScrGenius_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

    CButtonST m_Button1,m_Button2; //�����ƽ��ť����
	//������Բ��ť
//	CEllepticalButton m_Button1, m_Button2;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrGeniusDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	//����3D����
    void SetLogoFont(CString Name, int nHeight = -16, int nWeight = FW_BOLD, BYTE bItalic = TRUE, BYTE bUnderline = FALSE);
	CFont m_fontLogo;
	// Generated message map functions
	//{{AFX_MSG(CScrGeniusDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnPaint();
//	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ScrGeniusDLG_H__D363F3E7_49F3_11D3_8660_8A601C3D7832__INCLUDED_)
