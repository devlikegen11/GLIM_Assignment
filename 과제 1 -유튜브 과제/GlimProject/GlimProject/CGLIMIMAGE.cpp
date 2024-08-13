#include "pch.h"
#include "GlimProject.h"
#include "afxdialogex.h"
#include "CGLIMIMAGE.h"
#include <vector>


IMPLEMENT_DYNAMIC(CGLIMIMAGE, CDialogEx)

CGLIMIMAGE::CGLIMIMAGE(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CGLIMIMAGE, pParent)
{

}

CGLIMIMAGE::~CGLIMIMAGE()
{
}

void CGLIMIMAGE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CIRCLE, m_OpenImage);
}


BEGIN_MESSAGE_MAP(CGLIMIMAGE, CDialogEx)

END_MESSAGE_MAP()

 
void CGLIMIMAGE::LoadImage(CString& filePath)
{
	m_image.Destroy();

	if (m_image.Load(filePath) == S_OK)
	{
		CRect rect;
		m_OpenImage.GetClientRect(&rect);
		m_OpenImage.SetWindowPos(nullptr, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE);
		
		CClientDC dc(&m_OpenImage);
		m_image.Draw(dc, 0, 0);
	}
}


void CGLIMIMAGE::Circle()
{
    int nWidth = m_image.GetWidth();
    int nHeight = m_image.GetHeight();
    int nPitch = m_image.GetPitch();
    unsigned char* fm = (unsigned char*)m_image.GetBits();

    std::vector<CPoint> vPoint;

    for (int y = 0; y < nHeight; y++)
    {
        for (int x = 0; x < nWidth; x++)
        {
            int Pixel = fm[y * nPitch + x];

            if (Pixel == 180)
            {
                vPoint.emplace_back(CPoint(x, y));
            }
        }
    }

    if (!vPoint.empty())
    {
        int Max_xPoint = vPoint[0].x;
        int Min_xPoint = vPoint[0].x;
        int Max_yPoint = vPoint[0].y;
        int Min_yPoint = vPoint[0].y;

        for (auto& point : vPoint)
        {
            if (point.x > Max_xPoint)
            {
                Max_xPoint = point.x;
            }

            if (point.x < Min_xPoint)
            {
                Min_xPoint = point.x;
            }

            if (point.y > Max_yPoint)
            {
                Max_yPoint = point.y;
            }

            if (point.y < Min_yPoint)
            {
                Min_yPoint = point.y;
            }
        }

        int CenterX = (Max_xPoint + Min_xPoint) / 2;
        int CenterY = (Max_yPoint + Min_yPoint) / 2;

        MakeX(CenterX, CenterY);
    }
}

void CGLIMIMAGE::MakeX(int X, int Y)
{
    CClientDC dc(this);
    dc.MoveTo(X - 10, Y - 10);
    dc.LineTo(X + 10, Y + 10);
    dc.MoveTo(X - 10, Y + 10);
    dc.LineTo(X + 10, Y - 10);

    CString showTxT;
    showTxT.Format(_T("(%d, %d)"), X, Y);
    dc.TextOutW(X + 15, Y - 5, showTxT);
}


