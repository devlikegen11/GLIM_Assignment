
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
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
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
	ON_BN_CLICKED(IDC_DIA_DRAW, &CGlimProjectDlg::OnBnClickedDiaDraw)
	ON_BN_CLICKED(IDC_DIA_Action, &CGlimProjectDlg::OnBnClickedDiaAction)
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
	if (!m_image.IsNull())
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
	nWidth = 650;
	nHeight = 425;
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
	nRadius = (rand() % 20) + 30;
	LimitDraw(nRadius);

	static int nPrevRadius = 0;
	static int nPrevsttX = 0;
	static int nPrevsttY = 0;

	nGray = 180;
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	drawCircle(fm, nPrevsttX, nPrevsttY, nPrevRadius, 40);
	drawCircle(fm, m_SttX, m_SttY, nRadius, nGray);

	nPrevRadius = nRadius;
	nPrevsttX = m_SttX;
	nPrevsttY = m_SttY;

	UpdateDisplay();
}


void CGlimProjectDlg::drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray)
{
	SetImage();
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;
	int nPitch = m_image.GetPitch();
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();

	for (int j = y; j < y + nRadius * 2; j++)
	{
		if (j < 0 || j >= nHeight) continue;

		for (int i = x; i < x + nRadius * 2; i++)
		{
			if (i < 0 || i >= nWidth) continue;

			if (isInCIrcle(i, j, nCenterX, nCenterY, nRadius))
			{
				fm[j * nPitch + i] = nGray;
			}
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
	Draw();
	UpdateData(false);
}

void CGlimProjectDlg::OnBnClickedAction()
{
	UpdateData(true);
	LimitDraw(nRadius);
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
	else
	{
		AfxMessageBox(_T("이미지 로드 실패"));
	}
}

void CGlimProjectDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pGlimimage != nullptr) 
	{
		delete m_pGlimimage;
		m_pGlimimage = nullptr;
	}
}

void CGlimProjectDlg::ActionDraw()
{
	SetImage();

	int steps;
	float xIncrement, yIncrement;
	Increments(steps, xIncrement, yIncrement);

	RunCircle(steps, xIncrement, yIncrement);
}

void CGlimProjectDlg::SetImage()
{
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	memset(fm, 40, nWidth * nHeight);
}

void CGlimProjectDlg::Increments(int& steps, float& xIncrement, float& yIncrement)
{
	int farX = m_EndX - m_SttX;
	int farY = m_EndY - m_SttY;
	steps = max(abs(farX), abs(farY));

	xIncrement = farX / (float)steps;
	yIncrement = farY / (float)steps;
}

void CGlimProjectDlg::RunCircle(int steps, float xIncrement, float yIncrement)
{
	nGray = 180;
	int nPrevsttX = m_SttX;
	int nPrevsttY = m_SttY;
	float x = m_SttX;
	float y = m_SttY;

	unsigned char* fm = (unsigned char*)m_image.GetBits();
	CreateFolder();

	for (int i = 0; i <= steps; i++)
	{
		drawCircle(fm, nPrevsttX, nPrevsttY, nRadius, 40);
		drawCircle(fm, (int)x, (int)y, nRadius, nGray);

		CString basePath = _T("C:\\image\\GlimCircle");
		CString Glim_Imagesave;
		Glim_Imagesave.Format(_T("%s%d.bmp"), basePath, i);
		m_image.Save(Glim_Imagesave);

		nPrevsttX = (int)x;
		nPrevsttY = (int)y;

		x += xIncrement;
		y += yIncrement;

		UpdateDisplay();
		Sleep(10);
	}
}


void CGlimProjectDlg::CreateFolder()
{
	CString folderPath = _T("C:\\image");

	if (GetFileAttributes(folderPath) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectory(folderPath, NULL);
	}
}

void CGlimProjectDlg::OnBnClickedDiaDraw()
{
    UpdateData(true);
    nDiaRadius = (rand() % 21) + 50;;
    LimitDraw(nDiaRadius);

    SetImage();  
    nGray = 180;  
    unsigned char* fm = (unsigned char*)m_image.GetBits();
    
    drawDiamond(fm, m_SttX, m_SttY, nDiaRadius, nGray);

    UpdateDisplay();
    UpdateData(false);
}

void CGlimProjectDlg::drawDiamond(unsigned char* fm, int m_SttX, int m_SttY, int size, int nGray)
{
	int nPitch = m_image.GetPitch();
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int centerX = m_SttX + size;
	int centerY = m_SttY + size;

	int leftX = centerX - size;
	int rightX = centerX + size;
	int topY = centerY - size;
	int bottomY = centerY + size;

	for (int y = topY; y <= bottomY; y++)
	{
		for (int x = leftX; x <= rightX; x++)
		{
			if (x >= 0 && x < nWidth && y >= 0 && y < nHeight &&
				abs(centerX - x) + abs(centerY - y) <= size)
			{
				fm[y * nPitch + x] = nGray;
			}
		}
	}
}

void CGlimProjectDlg::OnBnClickedDiaAction()
{
	UpdateData(true);
	SetImage();
	LimitDraw(nDiaRadius);
	int steps;
	float xIncrement, yIncrement;
	Increments(steps, xIncrement, yIncrement);
	ActionDiamond(steps, xIncrement, yIncrement);

	UpdateData(false);
}

void CGlimProjectDlg::ActionDiamond(int steps, float xIncrement, float yIncrement)
{
	nGray = 180;
	int prevX = m_SttX;
	int prevY = m_SttY;
	float x = m_SttX;
	float y = m_SttY;

	unsigned char* fm = (unsigned char*)m_image.GetBits();
	CreateFolder();

	for (int i = 0; i <= steps; i++)
	{
		drawDiamond(fm, prevX, prevY, nDiaRadius, 40); 
		drawDiamond(fm, (int)x, (int)y, nDiaRadius, nGray); 

		CString basePath = _T("C:\\image\\GlimDiamond_");
		CString Glim_Imagesave;
		Glim_Imagesave.Format(_T("%s%d.bmp"), basePath, i);
		m_image.Save(Glim_Imagesave);

		prevX = (int)x;
		prevY = (int)y;

		x += xIncrement;
		y += yIncrement;

		UpdateDisplay();
		Sleep(10);
	}
}

void CGlimProjectDlg::LimitDraw(int radius)
{
	int maxX = nWidth - (radius * 2);
	int maxY = nHeight - (radius * 2);

	if (m_SttX < radius && m_SttX >= 0)
		m_SttX = m_SttX;
	else if (m_SttX > maxX)
		m_SttX = maxX;
	else if (m_SttX < 0)
		m_SttX = 0;

	if (m_EndX < radius && m_EndX >= 0)
		m_EndX = m_EndX;
	else if (m_EndX > maxX)
		m_EndX = maxX;
	else if (m_EndX < 0)
		m_EndX = 0;

	if (m_SttY < radius && m_SttY >= 0)
		m_SttY = m_SttY;
	else if (m_SttY > maxY)
		m_SttY = maxY;
	else if (m_SttY < 0)
		m_SttY = 0;

	if (m_EndY < radius && m_EndY >= 0)
		m_EndY = m_EndY;
	else if (m_EndY > maxY)
		m_EndY = maxY;
	else if (m_EndY < 0)
		m_EndY = 0;
}
