//********************************************************************************

/*
Modified by �쾰�� 2000.9
���ܣ���̬͸��λͼ��ʾ��WAV��Դ�ļ�����
*/

#if !defined(AFX_TRANSPARENTWND_H__INCLUDED_)
#define AFX_TRANSPARENTWND_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "NTray.h"  // ����ϵͳ����֧��ͷ�ļ�
#include "HyperLink.h" //���볬����ͷ�ļ�

class CTransparentWnd : public CWnd
{

public:
	CTransparentWnd();
    int xcounter,ycounter;
	bool GetMuteFlag(); //��ȡ�Ƿ����黰��־λ
	bool GetShowPetFlag(); //��ȡ�Ƿ���ʾ����ı�־λ
	void CreateTransparent(LPCTSTR pTitle, RECT &rect);
	void SetupRegion(CDC *pDC); //͸��λͼ
	void StartBitmap(void);//��ʼͼ����ʾ 
	void Reset(void); //��ֹʱͼ����ʾ
	void DoSet(void);  //�ı������
	void DoWalk(void); //��̬ͼ����ʾ
	void Idle(void);//��������ʱ��ʾ��ͼ��
    void SoundPlay(void);	//WAV��Ƶ�ļ�����
	void WinShutdown(UINT ShutdownFlag);  //Windows NT/98�¹ػ���ע������
	void WinAnimation(bool ShowFlag); //������ʾ/���صĶ���Ч��
	CString GetTextDescription(); //��ȡ�û��Զ��Ƶ�����������Ϣ
	CBitmap m_bmpDraw;
	int m_iAniSeq;
//	CFont m_fontLogo;
	CPoint m_ptDest;
	int m_iLastDirection;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransparentWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTransparentWnd();

protected:
	HICON			m_hIcon[1];  //ϵͳ����֧�����ñ���
	CTrayNotifyIcon m_TrayIcon;

	// Generated message map functions
protected:
	//{{AFX_MSG(CTransparentWnd)
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShow();
	afx_msg void OnRegister();
	afx_msg void OnShutdown();
	afx_msg void OnEject();
	afx_msg void OnExit();
	afx_msg void OnFileSendMail();
	afx_msg void OnMute();
	afx_msg void OnUpdateMute(CCmdUI* pCmdUI);
	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	afx_msg void OnGetip();
	afx_msg void OnBmp2txt();
	afx_msg void OnHelp();
	afx_msg void OnCustomwavfile();
	afx_msg void OnBig2gb();
	afx_msg void OnHideinbmp();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnEdstar();
	afx_msg void OnRegocx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
