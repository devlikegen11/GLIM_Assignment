#include "CGLIMIMAGE.h"
#pragma once


class CGlimProjectDlg : public CDialogEx
{
private:
	CImage m_image;
public:
	CGlimProjectDlg(CWnd* pParent = nullptr);	
	CGLIMIMAGE* m_pGlimimage;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIMPROJECT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);


protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void UpdateDisplay();
	void Initimage();
	void Draw();
	int m_SttX;
	int m_SttY;
	int m_EndX;
	int m_EndY;
	void drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray);
	bool isInCIrcle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	afx_msg void OnBnClickedDraw();
	afx_msg void OnBnClickedAction();
	afx_msg void OnBnClickedOpen(); 
	int nRadius;
	void Action();
	void EClimit();
	void OnDestroy();
	void ActionDraw();
	void CreateFolder();
};

