// demoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "demo.h"
#include "demoDlg.h"
#include ".\demodlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CdemoDlg �Ի���
CdemoDlg* pdemoDlg = NULL;

void client1(CAdoClient* pAdoClient, EPACKATTRIB& result)
{
	//���ԭ�е���������
	
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
	

	//��ȡ��ǰadoclient���У�������.
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
		������ǰ�����Ҫ��������ʾ����
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

// CdemoDlg ��Ϣ�������

BOOL CdemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//ÿ���ͻ��˶���Ҫһ��adoclientManager
	m_adoClientManger.SetDBProvider("192.168.1.2",10003);
	
	//���ÿͻ�����Ҫ�õ��Ĵ��������
	//m_adoClientManger.SetProxy("192.168.1.100",8080,HTTP);

	//��ʾ�����õ�����adoClient�ֱ����������ǵĹ�����ָ��
	m_adoClient1.setAdoClientManager(&m_adoClientManger);
	m_adoClient2.setAdoClientManager(&m_adoClientManger);

	m_adoClient1.setRecvCallback(client1);
	m_adoClient2.setRecvCallback(client2);
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CdemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CdemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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

	// TODO:  �ڴ������ר�õĴ�������
	m_ctrList1.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT,CRect(0,0,0,0),this,IDC_LIST1);
	m_ctrList2.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT,CRect(0,0,0,0),this,IDC_LIST2);
	m_ctrlEdit1.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_EDIT1);
	m_ctrlEdit2.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_EDIT2);
	m_ctrlBtn1.Create("ִ��Sql_1",WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BTN1);
	m_ctrlBtn2.Create("ִ��Sql_2",WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BTN2);

	return 0;
}

void CdemoDlg::OnBtn1()
{
	CString strSql;
	GetDlgItemText(IDC_EDIT1,strSql);
	m_adoClient1.execute(strSql);//ִ��sql���
}

void CdemoDlg::OnBtn2()
{
	CString strSql;
	GetDlgItemText(IDC_EDIT2,strSql);
	m_adoClient2.execute(strSql);
}
