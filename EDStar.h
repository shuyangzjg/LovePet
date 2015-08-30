#if !defined(AFX_EDSTAR_H__C3037040_7DB6_11D5_9008_851E79159E58__INCLUDED_)
#define AFX_EDSTAR_H__C3037040_7DB6_11D5_9008_851E79159E58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EDStar.h : header file
//
//
//���ܣ�ͨ���ļ��ӽ���
// ���ߣ��쾰�� ��
//���ڣ�2001��7��18��
//
/////////////////////////////////////////////////////////////////////////////
// CEDStar dialog

#include "StaticFilespec.h" //��չ��̬����
#include "FlatEdit.h" //��ƽ�༭����ʾ��
#include "BtnST.h" //ƽ�水ť��
#include "EllepticalButton.h" //��Բ��ť��

class CEDStar : public CDialog
{
// Construction
public:
	CEDStar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEDStar)
	enum { IDD = IDD_EDSTAR };
	//���ô�½��Բ��ť
	CEllepticalButton	m_Cancel;
	CEllepticalButton	m_Encrypt;
	CEllepticalButton	m_Decrypt;
	//����ƽ�水ť
	CButtonST	m_Browse2;
	CButtonST	m_Browse1;
	//������չ��̬��ǩ
	CStaticFilespec	m_TargetPath1;
	CStaticFilespec	m_SourcePath1;
	//����ƽ��༭��
	CFlatEdit	m_Password1;
	CString	m_Password;
	CString	m_SourcePath;
	CString	m_TargetPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEDStar)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CString m_Filename; //��ԭ�ļ����б����ļ���
	//�����ļ��㷨����
    BOOL EncryptFile(CString filename,char *password);
	//�����ļ��㷨����
    BOOL DecryptFile(CString filename,char *password);

	// Generated message map functions
	//{{AFX_MSG(CEDStar)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBrowse1();
	afx_msg void OnBrowse2();
	afx_msg void OnEncrypt();
	afx_msg void OnDecrypt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDSTAR_H__C3037040_7DB6_11D5_9008_851E79159E58__INCLUDED_)
