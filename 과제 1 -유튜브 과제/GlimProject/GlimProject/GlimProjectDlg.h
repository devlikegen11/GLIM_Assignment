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
	int nWidth;
	int nHeight;
	int nDiaRadius;
	int nGray;
	void drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray);
	bool isInCIrcle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	afx_msg void OnBnClickedDraw();
	afx_msg void OnBnClickedAction();
	afx_msg void OnBnClickedOpen(); 
	int nRadius;
	void Action();
	void OnDestroy();
	void ActionDraw();
	void CreateFolder();
	afx_msg void OnBnClickedDiaDraw();
	afx_msg void OnBnClickedDiaAction();
	void SetImage();
	void Increments(int& steps, float& xIncrement, float& yIncrement);
	void RunCircle(int steps, float xIncrement, float yIncrement);
	void drawDiamond(unsigned char* fm, int centerX, int centerY, int size, int nGray);
	void ActionDiamond(int steps, float xIncrement, float yIncrement);
	void LimitDraw(int radius);
};

