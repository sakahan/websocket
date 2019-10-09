
// WebSocketServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "sha1.h"
#include "base64.h"
#include "./log/LogWriter.h"


/******** yangshaoguang 2018/04/25 15:30:19 START �� ********/
//����
#include <iostream>  
#include <string>  
#include <fstream>  
//����rapidjson��Ҫͷ�ļ�,rapidjson�ļ��п���������Ŀ¼����������include·�������߼��뵽������  
#include "rapidjson/document.h"  
#include "rapidjson/filestream.h"  
#include "rapidjson/prettywriter.h"  
#include "rapidjson/stringbuffer.h"  
using namespace std;
using namespace rapidjson;  //����rapidjson�����ռ� 
/******** yangshaoguang 2018/04/25 15:30:19  END  �� ********/

#define WM_SOCKET (WM_USER+100)
#define CLIENT_COUNT 20
enum Connect_State
{
	Open = 0,
	Close
};
typedef struct _SocketStruct
{
	SOCKET socket;
	bool b_connect;
	string m_id6d;
	_SocketStruct()
	{
		b_connect = false;
		m_id6d = "";
	}
}MySocketClient;

enum TimerType
{
	NewConnectTimer,
};
// CWebSocketServerDlg �Ի���
class CWebSocketServerDlg : public CDialog
{
// ����
public:
	CWebSocketServerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CWebSocketServerDlg()
	{
		if (log)
		{
			delete log;
			log = NULL;
		}
	}
// �Ի�������
	enum { IDD = IDD_WEBSOCKETSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	BOOL  m_TimerFlag;  //TRUE  ��ʶ��ʱ���Ѿ�����

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_IPCtl;
	CEdit m_PortEdit;
	CEdit m_RecvEdit;
	CEdit m_SendEdit;
	CButton m_sendBtn;
	CListBox m_ListBox;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	CString m_filePath;
	Connect_State netState;
	SOCKET sock; //������
	SOCKET client; //�ͻ���
	MySocketClient clientarr[CLIENT_COUNT];//�ͻ�������
	sockaddr_in addr_client;
	int Count;
	CButton m_ConnectBtn;
	afx_msg LRESULT OnMySocket(WPARAM wParam, LPARAM lParam);
	std::string m_strMsgFromWeb; //yangshg 2018/04/25 15:00:14������ҳ����Ϣ
	void ProcessRecvMsg(std::string & str);
	void ProcessJsonObjRequest(const rapidjson::Value& data);
	CTime time1, time2;
	afx_msg LRESULT MySend(WPARAM wParam, LPARAM lParam);
	char* GetKey(char* handShakeHeader);
	std::string DecodeKey(std::string key);
	std::string m_strLastLongText;
	LogWriter *log;
	CString m_msgShowStr;

	/******** yangshaoguang 2018/02/13 START �� ********/
	//����
public:
	std::string CStringToString(LPCWSTR pwszSrc)
	{
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
		if (nLen <= 0) return std::string("");
		char* pszDst = new char[nLen];
		if (NULL == pszDst) return std::string("");
		WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
		pszDst[nLen - 1] = 0;
		std::string strTemp(pszDst);
		delete[] pszDst;
		return strTemp;
	}
	std::string MyGetFirstNChar(std::string stringTmp, int n) /////yangshg 2018/02/13��ȡһ���ַ�����ǰn���ַ��������ַ���
	{
		std::string str;
		for (int i = 0; i <n; i++)
		{
			str += stringTmp[i];
		}
		return str;
	}
	std::string MyGetLastNChar(std::string stringTmp, int n) /////yangshg 2018/02/13��ȡһ���ַ����ĺ�n���ַ��������ַ���
	{
		std::string str;
		int len = stringTmp.length();
		for (int i = len - n; i < len; i++)
		{
			str += stringTmp[i];
		}
		return str;
	}
	/*******************************************
	*�������ƣ�U2G
	*�������ܣ�UTF8 ----> GB2312
	*����������
	*�� �� ֵ��
	*��    �ڣ�2018/02/27 17:34:02
	*  Name  : yangshaoguang
	********************************************/
	char* U2G(const char* utf8)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[len + 1];
		memset(wstr, 0, len + 1);
		MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
		len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
		char* str = new char[len + 1];
		memset(str, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
		if (wstr) delete[] wstr;
		return str;
	}
	/******** yangshaoguang 2018/02/13  END  �� ********/
	/*******************************************
	*�������ƣ�
	*�������ܣ�ȥ���������ĺ����"-",�Լ��������ŵĵ�һ��0  +860571 ==>  +86571
	*����������
	*�� �� ֵ��
	*��    �ڣ�2018/03/13 09:58:45
	*  Name  : yangshaoguang
	********************************************/
	std::string RemoveFirstZero(std::string stringTmp)
	{
		std::string strNum = stringTmp;
		string tmp;
		size_t pos = strNum.find_first_of('-');
		int i = (int)pos;
		if (i != -1)
		{
			tmp = MyGetFirstNChar(strNum, i);// + MyGetLastNChar(strNum,strNum.length() - i -1);
			tmp += MyGetLastNChar(strNum, strNum.length() - i - 1);
			strNum = tmp;
		}
		if (strNum[3] == '0')
		{
			i = 3;
			tmp = MyGetFirstNChar(strNum, i);// + MyGetLastNChar(strNum,strNum.length() - i -1);
			tmp += MyGetLastNChar(strNum, strNum.length() - i - 1);
			strNum = tmp;
		}
		return strNum;
	}
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SendToClient(string &strMsg);
	string m_strId6d; //�洢�����ϻ�Ϊ�˺ŵ�id6d
};
