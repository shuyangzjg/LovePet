#if !defined(AFX_FILESPLIT_H__3FB0B940_0C24_11D5_9005_E172D79D66C0__INCLUDED_)
#define AFX_FILESPLIT_H__3FB0B940_0C24_11D5_9005_E172D79D66C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileSplit.h : header file
//
//Modify by �쾰�� ��2001.2.13
//���ܣ��ļ��ָ�Ϊ���ָ�ʽ�Ĵ�С��������ϲ���һ��
//
#include "FlatComboBox.h" //��ƽ��Ͽ���ʾ��
#include "BtnST.h" //ƽ�水ť��
#include "EllepticalButton.h" //��Բ��ť��
#include "GradientProgressCtrl.h" //����ɫ��������
/////////////////////////////////////////////////////////////////////////////
// CFileSplit dialog

class CFileSplit : public CDialog
{
// Construction
public:
	CFileSplit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileSplit)
	enum { IDD = IDD_FILESPLIT };
	CFlatComboBox	m_FileSize; //��ʾΪ��ƽ��Ͽ�
	CGradientProgressCtrl  pProgress;
    CEllepticalButton m_buttMerge;  
	CEllepticalButton m_buttExit;
	CEllepticalButton m_buttSplit;
	CButtonST m_Browse;
    CButtonST m_Browse2;
	CButton	m_RadioSplit;
	CEdit	m_edit;
	CString	m_parts;
	CString	m_path;
	CString	m_targetpath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileSplit)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_split;
	CString m_filename;
	CFile m_SourceFile;
	HICON m_hIcon;
	int SplitMe();   //�ָ��ļ�
	int MergeMe();  //�ϲ��ļ�
	UINT GetSplitFileSize(); //��ȡ�ļ��ָ��С

	// Generated message map functions
	//{{AFX_MSG(CFileSplit)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	afx_msg void OnBrowse2();
	afx_msg void OnRadio4();
	afx_msg void OnRadio3();
	afx_msg void OnOk();
	afx_msg void OnButtonMerge();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESPLIT_H__3FB0B940_0C24_11D5_9005_E172D79D66C0__INCLUDED_)
