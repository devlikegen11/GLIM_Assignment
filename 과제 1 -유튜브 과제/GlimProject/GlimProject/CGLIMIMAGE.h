#pragma once
#include "afxdialogex.h"

class CGLIMIMAGE : public CDialogEx
{
	DECLARE_DYNAMIC(CGLIMIMAGE)

public:
	CGLIMIMAGE(CWnd* pParent = nullptr); 
	virtual ~CGLIMIMAGE();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CGLIMIMAGE };
#endif
private:
	CStatic m_OpenImage;
	CImage m_image;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()

public:
	void LoadImage(CString &filePath);
	void Circle();
	void MakeX(int X, int Y);
};
