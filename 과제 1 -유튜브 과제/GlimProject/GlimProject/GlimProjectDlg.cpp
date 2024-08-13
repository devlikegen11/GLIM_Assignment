
#include "pch.h"
#include "framework.h"
#include "GlimProject.h"
#include "GlimProjectDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



CGlimProjectDlg::CGlimProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIMPROJECT_DIALOG, pParent)
	, m_SttX(0)
	, m_SttY(0)
	, m_EndX(0)
	, m_EndY(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGlimProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STARTX, m_SttX);
	DDX_Text(pDX, IDC_STARTY, m_SttY);
	DDX_Text(pDX, IDC_ENDX, m_EndX);
	DDX_Text(pDX, IDC_ENDY, m_EndY);

}

BEGIN_MESSAGE_MAP(CGlimProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DRAW, &CGlimProjectDlg::OnBnClickedDraw)
	ON_BN_CLICKED(IDC_Action, &CGlimProjectDlg::OnBnClickedAction)
	ON_BN_CLICKED(IDC_Open, &CGlimProjectDlg::OnBnClickedOpen)
END_MESSAGE_MAP()


BOOL CGlimProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	Initimage();
	m_pGlimimage = new CGLIMIMAGE;
	m_pGlimimage->Create(IDD_CGLIMIMAGE, this);

	return TRUE;  
}


void CGlimProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CGlimProjectDlg::OnPaint()
{
	CPaintDC dc(this);
	if (m_image)
	{
		m_image.Draw(dc, 0, 50);
	}
}

HCURSOR CGlimProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGlimProjectDlg::UpdateDisplay()
{
	CClientDC dc(this); 
	m_image.Draw(dc, 0, 50);  
}

void CGlimProjectDlg::Initimage()
{
	int nWidth = 650;
	int nHeight = 425;
	int nBpp = 8;

	m_image.Create(nWidth, -nHeight, nBpp);
	if (nBpp == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_image.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	memset(fm, 40, nWidth * nHeight);
}

void CGlimProjectDlg::Draw()
{
	int nsttX = m_SttX;
	int nsttY = m_SttY;
	static int nPrevRadius = 0; 
	static int nPrevsttX = 0;
	static int nPrevsttY = 0;

	int nGray = 180;  
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();
	nRadius = rand() % 50; 
	unsigned char* fm = (unsigned char*)m_image.GetBits();  

	drawCircle(fm, nPrevsttX, nPrevsttY, nPrevRadius, 40);  
	drawCircle(fm, nsttX, nsttY, nRadius, nGray);

	nPrevRadius = nRadius;
	nPrevsttX = nsttX;
	nPrevsttY = nsttY;

	UpdateDisplay();  
}


void CGlimProjectDlg::drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray)
{
	int nCenterX = x + nRadius;  
	int nCenterY = y + nRadius;  
	int nPitch = m_image.GetPitch();

	for (int j = y; j < y + nRadius * 2; j++) 
	{
		for (int i = x; i < x + nRadius * 2; i++) 
		{
			if (isInCIrcle(i, j, nCenterX, nCenterY, nRadius))  
				fm[j * nPitch + i] = nGray;  
		}
	}
}

bool CGlimProjectDlg::isInCIrcle(int i, int j, int nCenterX, int nCenterY, int nRadius)  
{
	bool bRet = false;  
	double dX = i - nCenterX;  
	double dY = j - nCenterY;  
	double dDist = dX * dX + dY * dY;  

	if (dDist < nRadius * nRadius)  
	{
		bRet = true;  
	}

	return bRet; 
}

void CGlimProjectDlg::OnBnClickedDraw()
{
	UpdateData(true);
	EClimit();
	Draw();
	UpdateData(false);
}

void CGlimProjectDlg::OnBnClickedAction()
{
	UpdateData(true);
	EClimit();
	Action();
	UpdateData(false);
}

void CGlimProjectDlg::Action()
{
	ActionDraw();
	m_SttX = m_EndX;
	m_SttY = m_EndY;
	UpdateDisplay();
}

void CGlimProjectDlg::EClimit()	
{
	if (m_SttX == 0)
		m_SttX = 0;
	else if (m_SttX >= 550)
		m_SttX = 550;
	else if (m_SttX < 0)
		m_SttX = 550;

	if (m_EndX == 0)
		m_EndX = 0;
	else if (m_EndX >= 550)
		m_EndX = 550;
	else if (m_EndX < 0)
		m_EndX = 550;

	if (m_SttY == 0)
		m_SttY = 0;
	else if (m_SttY >= 320)
		m_SttY = 320;
	else if (m_SttY < 0)
		m_SttY = 320;

	if (m_EndY == 0)
		m_EndY = 0;
	else if (m_EndY >= 320) 
		m_EndY = 320;
	else if (m_EndY < 0)
		m_EndY = 320;
}


void CGlimProjectDlg::OnBnClickedOpen()
{
	m_pGlimimage->ShowWindow(SW_SHOW);
	CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_FILEMUSTEXIST, _T("Image Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();
		m_pGlimimage->LoadImage(filePath);
		m_pGlimimage->Circle();
	}
}

void CGlimProjectDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pGlimimage)
	{
		delete m_pGlimimage;
		m_pGlimimage = nullptr;
	}
}

void CAboutDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

}

void CGlimProjectDlg::ActionDraw()
{
	int nsttX = m_SttX;
	int nsttY = m_SttY;
	static int nPrevRadius = 0;
	static int nPrevsttX = m_SttX;
	static int nPrevsttY = m_SttY;

	int nGray = 180;
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	memset(fm, 40, nWidth * nHeight);

	int farX = m_EndX - nsttX;
	int farY = m_EndY - nsttY;
	int steps = max(abs(farX), abs(farY));

	float xIncrement = farX / (float)steps;
	float yIncrement = farY / (float)steps;

	float x = nsttX;
	float y = nsttY;
	CreateFolder();

	for (int i = 0; i <= steps; i++)
	{
		drawCircle(fm, nPrevsttX, nPrevsttY, nRadius, 40);
		drawCircle(fm, (int)x, (int)y, nRadius, nGray);
		CString Glim_Imagesave;
		Glim_Imagesave.Format(_T("C:\\image\\GlimImage_%d.bmp"), i);
		m_image.Save(Glim_Imagesave);
		nPrevsttX = (int)x;
		nPrevsttY = (int)y;
		nPrevRadius = nRadius;

		x += xIncrement;
		y += yIncrement;

		UpdateDisplay();
		Sleep(10);

		drawCircle(fm, (int)x, (int)y, nRadius, nGray);
	}
}

void CGlimProjectDlg::CreateFolder()
{
	LPCTSTR folderPath = _T("C:\\image");

	if (GetFileAttributes(folderPath) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectory(folderPath, NULL);
	}

}