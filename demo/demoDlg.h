// demoDlg.h : ͷ�ļ�
//

#pragma once
#include "../MidClient/AdoClientManager.h"
#include "../MidClient/AdoClient.h"

#define IDC_LIST1 0x9001
#define IDC_EDIT1 0x9002
#define IDC_BTN1  0x9003

#define IDC_LIST2 0x9004
#define IDC_EDIT2 0x9005
#define IDC_BTN2  0x9006


// CdemoDlg �Ի���
class CdemoDlg : public CDialog
{
// ����
public:
	CdemoDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CListCtrl m_ctrList1;
	CListCtrl m_ctrList2;

// �Ի�������
	enum { IDD = IDD_DEMO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CEdit m_ctrlEdit1;
	CEdit m_ctrlEdit2;
	CButton m_ctrlBtn1;
	CButton m_ctrlBtn2;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBtn1();
	afx_msg void OnBtn2();

protected:
	CAdoClientManager m_adoClientManger;//adoclient������
	CAdoClient m_adoClient1;
	CAdoClient m_adoClient2;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
