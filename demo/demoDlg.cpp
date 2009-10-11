// demoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "demo.h"
#include "demoDlg.h"
#include ".\demodlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CdemoDlg 对话框
CdemoDlg* pdemoDlg = NULL;

void client1(CAdoClient* pAdoClient, EPACKATTRIB& result)
{
	//清空原有的数据内容
	
	CHeaderCtrl* pheadCtrl = pdemoDlg->m_ctrList1.GetHeaderCtrl();
	if(pheadCtrl)
	{
		pdemoDlg->m_ctrList1.DeleteAllItems();
		int nColumnCount = pheadCtrl->GetItemCount();
		// Delete all of the columns.
		for (int i=0; i < nColumnCount; i++)
		{
			pdemoDlg->m_ctrList1.DeleteColumn(0);
		}
	}
	

	//获取当前adoclient的列，行属性.
	int nFieldCount = pAdoClient->GetFieldsCount();
	int nRecordCount = pAdoClient->GetRecordCount();

	int nIndex = 0;
	while(nIndex < nFieldCount)
	{
		pdemoDlg->m_ctrList1.InsertColumn(nIndex,pAdoClient->GetFieldName(nIndex),
			LVCFMT_LEFT,50);
		nIndex++;
	}
	while(nIndex < nRecordCount)
	{
		/*
		这里就是把所需要的数据显示出来
		*/
		nIndex++;
	}

}

void client2(CAdoClient* pAdoClient, EPACKATTRIB& result)
{

}


CdemoDlg::CdemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CdemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pdemoDlg = this;
}

void CdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(IDC_BTN1,OnBtn1)
	ON_COMMAND(IDC_BTN2,OnBtn2)
END_MESSAGE_MAP()

// CdemoDlg 消息处理程序

BOOL CdemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//每个客户端都需要一个adoclientManager
	m_adoClientManger.SetDBProvider("192.168.1.2",10003);
	
	//设置客户端所要用到的代理服务器
	//m_adoClientManger.SetProxy("192.168.1.100",8080,HTTP);

	//此示例中用到两个adoClient分别设置下它们的管理器指针
	m_adoClient1.setAdoClientManager(&m_adoClientManger);
	m_adoClient2.setAdoClientManager(&m_adoClientManger);

	m_adoClient1.setRecvCallback(client1);
	m_adoClient2.setRecvCallback(client2);
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CdemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CdemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcOrg,rcPos;
	GetClientRect(&rcOrg);
	if(m_ctrList1.GetSafeHwnd())
	{
		rcPos = rcOrg;
		rcPos.InflateRect(-5,-5, 0-(rcPos.right>>1) - 10,-100);
		m_ctrList1.MoveWindow(&rcPos);
	}

	if(m_ctrList2.GetSafeHwnd())
	{
		rcPos = rcOrg;
		rcPos.InflateRect(0-(rcPos.right>>1) - 10,-5, -5,-100);
		m_ctrList2.MoveWindow(&rcPos);
	}
	if(m_ctrlEdit1.GetSafeHwnd())
	{
		rcPos = rcOrg;
		m_ctrlEdit1.MoveWindow(5,rcPos.bottom-90,190,22);
	}
	if(m_ctrlEdit2.GetSafeHwnd())
	{
		rcPos = rcOrg;
		m_ctrlEdit2.MoveWindow((rcPos.right>>1)+10,rcPos.bottom-90,190,22);
	}
	if(m_ctrlBtn1.GetSafeHwnd())
	{
		rcPos = rcOrg;
		m_ctrlBtn1.MoveWindow(5,rcPos.bottom-50,82,22);
	}
	if(m_ctrlBtn2.GetSafeHwnd())
	{
		rcPos = rcOrg;
		m_ctrlBtn2.MoveWindow((rcPos.right>>1)+10,rcPos.bottom-50,82,22);
	}
}

int CdemoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_ctrList1.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT,CRect(0,0,0,0),this,IDC_LIST1);
	m_ctrList2.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT,CRect(0,0,0,0),this,IDC_LIST2);
	m_ctrlEdit1.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_EDIT1);
	m_ctrlEdit2.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_EDIT2);
	m_ctrlBtn1.Create("执行Sql_1",WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BTN1);
	m_ctrlBtn2.Create("执行Sql_2",WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BTN2);

	return 0;
}

void CdemoDlg::OnBtn1()
{
	CString strSql;
	GetDlgItemText(IDC_EDIT1,strSql);
	m_adoClient1.execute(strSql);//执行sql语句
}

void CdemoDlg::OnBtn2()
{
	CString strSql;
	GetDlgItemText(IDC_EDIT2,strSql);
	m_adoClient2.execute(strSql);
}
