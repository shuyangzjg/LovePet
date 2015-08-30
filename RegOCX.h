#if !defined(AFX_REGOCX_H__A5FBFEE0_7DAC_11D5_9008_E230DBA088C1__INCLUDED_)
#define AFX_REGOCX_H__A5FBFEE0_7DAC_11D5_9008_E230DBA088C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegOCX.h : header file
// 
//���ܣ��ؼ�ע�����Ĵ����࣬�ڴ˼���ע�ᣬע���ؼ��Լ���ʾ����ȹ���
// ���ߣ��쾰�� ��
//���ڣ�2001��7��8��
//

/////////////////////////////////////////////////////////////////////////////
// CRegOCX dialog

#include "BtnST.h" //ƽ�水ť��
#include "EllepticalButton.h" //��Բ��ť��

//����������д�����
#define MAXNUM_RUNWINDOW 256

class CRegOCX : public CDialog
{
// Construction
public:
	CRegOCX(CWnd* pParent = NULL);   // standard constructor

	//��С����ԭ�������д���ָ�����鼰��Ŀ
	CWnd * m_pWnd[MAXNUM_RUNWINDOW];
    int m_nWndNum;

// Dialog Data
	//{{AFX_DATA(CRegOCX)
	enum { IDD = IDD_REGOCX };
	//������Բ��ť
	CEllepticalButton	m_Cancel;
	CEllepticalButton	m_UnRegOCX;
	CEllepticalButton	m_ShowDesktop;
	CEllepticalButton	m_RegOCX;
	//����ƽ�水ť
	CButtonST	m_Browse;
	CString	m_FileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegOCX)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegOCX)
	afx_msg void OnBrowse();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowdesktop();
	afx_msg void OnRegocx();
	afx_msg void OnUnregocx();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGOCX_H__A5FBFEE0_7DAC_11D5_9008_E230DBA088C1__INCLUDED_)
