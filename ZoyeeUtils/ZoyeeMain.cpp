#include "zoyeeUtils.h"
#include <Windows.h>

class CThreadTest : public ZoyeeUtils::CTask{
public:
	CThreadTest(int nId);
	void Run();
private:
	int m_nId;
};

CThreadTest::CThreadTest( int nId )
{
	m_nId = nId;
}

void CThreadTest::Run()
{
	for (int i = 0; i < 10; i++){
		printf("[%d]\n", m_nId);
		Sleep(500);
	}
}

ZoyeeUtils::emCopyFileRes CopyFileCallback(long lTotalFileSize, long lTotalBytesTransferred){
	printf("%08ld / %08ld\n", lTotalBytesTransferred, lTotalFileSize);
	return ZoyeeUtils::emContinue;
}

void OnRecv(char* pBuff, int nLen, ZoyeeUtils::RecvType type, ZoyeeUtils::ISocket* pSocket);

#include <time.h>
#include <typeinfo>
#include <map>

#include "AutoLockMacro.h"
#include "AutoLockClass.h"
#include "DebugView.h"

class Base{
public:
	void DoWork();
	IMPLEMENT_LOCKING(Base);
	_KEY_;
};

void Base::DoWork(){
	AUTOLOCK;
	_LOCK_;
}

int main(){	
	std::vector<std::string> vec;
	vec.push_back("yz-xiaopiplayer.exe");
	ZoyeeUtils::CDebugView view(vec);
	view.Start();
	//HWND hwnd = FindWindowA(NULL, "小皮引擎领航版-1");
	//SendMessageA(hwnd, WM_SYSCOMMAND, SC_RESTORE, NULL);
	//return 0;

	//std::map<int, int> _map;
	//_map[0] = 0;
	//_map[1] = 1;
	//for (auto iter = begin(_map); iter != end(_map); iter++){
	//	printf("%d", iter->first);
	//}
	//ZoyeeUtils::ISocket* pIOCP = ZoyeeUtils::SocketFactory::MakeSocket(ZoyeeUtils::em_IOCP, OnRecv);
	//pIOCP->Init("127.0.0.1", 6666);
	//while(1){
	//	Sleep(1000);
	//}
	//printf("");

	/*char szTime[32];
	sprintf(szTime, "%I64d", time(0));

	std::string str1 = "abc_id_game.jpg";
	str1 = Change(str1);
*/


	//ZoyeeUtils::CWMI wmi;
	//wmi.Init();
	//std::string str = wmi.GetSystemInfo(ZoyeeUtils::CWMI::Win32_NetworkAdapter);
	//const char* pSrc = "1234|4444";
	//ZoyeeUtils::CRegex regex(pSrc, "\\d{1,}");
	//std::vector<std::string> vec = regex.GetSearch();

	//ZoyeeUtils::CRegister reg(ZoyeeUtils::CRegister::em_local_machine, "Software\\7-Zip", true);
	//reg.EnumValue();
	//reg.EnumSubKey();
	/*reg.GetValue("Path");
	reg.SetValue("Fuck", "223344", false);*/
	//reg.DeleteKey("Hello");

	//ZoyeeUtils::ISocket* pClient = ZoyeeUtils::SocketFactory::MakeSocket(ZoyeeUtils::em_client, (void*)OnRecv);
	//pClient->Init("127.0.0.1", 4444);
	//while(1){
	//	getchar();
	//	pClient->Send("Hello", 6, 1, true);
	//}


	//ZoyeeUtils::ISocket* pServer = ZoyeeUtils::SocketFactory::MakeSocket(ZoyeeUtils::em_server, (void*)OnRecv);
	//pServer->Init("127.0.0.1", 4399);
	//while(1){
	//	Sleep(10);
	//}
	//char* pSrc = "Hello";
	//pSrc[0] = 'Z';
	//strcpy(pSrc, "HelloWorld");
	//ZoyeeUtils::ProcessSnapshot::GetProcessInfo("UCBrowser.exe");
	//char sz[16] = "脸超级大";
	//ZoyeeUtils::CTextCode::GBK2UTF8(sz);
	//ZoyeeUtils::CTextCode::UTF82GBK(sz);
	//ZoyeeUtils::CStdStringEx  str(10);
	//ZoyeeUtils::CStdStringEx strCopy("AABBDDBB");
	//strCopy.Replace("B", "0000");
	//std::string strreplace = ZoyeeUtils::CStdStringEx::Replace("Hello", "e", "EEEE");
	//ZoyeeUtils::CStdStringEx str("[%s][%d]", "Hello", 100);

	//ZoyeeUtils::CProcess pro("x:/zutil.exe", "%s", "Hello");
	//std::string strOutput;
	//HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	//SetEvent(hEvent);
	//pro.StartEx(CmdOut, strOutput, hEvent);

//	std::string str = wmi.GetSystemInfo(ZoyeeUtils::CWMI::Win32_OperatingSystem);
	//str = wmi.ExecWQL("select *   from Win32_NetworkAdapter where PhysicalAdapter  = TRUE");
	//
	//printf("");
	//std::cout << str.c_str();
	//std::string str64 = ZoyeeUtils::CTextCode::Base64Encode("Hello", strlen("Hello"));
	//str64 = ZoyeeUtils::CTextCode::Base64Decode(str64.c_str(), strlen(str64.c_str()));
	//std::string strMd5 = ZoyeeUtils::CTextCode::Md5("Hello", 5);
	//ZoyeeUtils::CThreadPool pool;
	//CThreadTest* pTask;
	//for (int i = 0; i < 1; i++){
	//	pTask = new CThreadTest(i);
	//	pool.AddTask(pTask);
	//}
	//while(1){Sleep(100);};
	//ZoyeeUtils::CFileManange::LoadFile("x:/QtChart.exe");
	//char sz[] = {1, 2, 3, 4, 5};
	//std::string str(sz, 5);
	//printf("%d", ZoyeeUtils::CFileManange::WriteFile("z:/ll.log", ""));
	//std::string strRead = ZoyeeUtils::CFileManange::LoadFile("X:/ll.log");
	//ZoyeeUtils::CFileManange::DelFile("x:/ll.log");
	//ZoyeeUtils::CFileManange::CpyFileEx("x:/tcpudp.zip", "x:/tcpudp2.zip", true, CopyFileCallback);
	//while(1){
	//	Sleep(1000);
	//}
	/*ZoyeeUtils::CFileManange::CreateDir("x:/123/ddd\\df\\1");
	std::list<std::pair<std::string, bool>> lst = ZoyeeUtils::CFileManange::GetDirFiles("X:/bs_input");
	ZoyeeUtils::CFileManange::CheckFileExist("d:/11");
	ZoyeeUtils::CFileManange::DeleteDir("x:/123");*/
	//long lFileSize = ZoyeeUtils::CFileManange::GetFileSize("X:\\vs_temp\\piecesexperise-7e9c8f17\\PiecesExperise.sdf");
}

void CmdOut(int nPid, int nExitCode, std::string& pBuff){
	printf("[pid:%d][%s]\n", nPid, pBuff.c_str());
}

void OnRecv(char* pBuff, int nLen, ZoyeeUtils::RecvType type, ZoyeeUtils::ISocket* pSocket)
{
	switch(type)
	{
	case ZoyeeUtils::em_on_recv:
		printf("[recv][%s:%d][%s]\n", pSocket->GetIP().c_str(), pSocket->GetPort(), pBuff);
		pSocket->Send("Hello", 5);
		break;
	case ZoyeeUtils::em_on_error:
		printf("[error][%s:%d][%s]\n", pSocket->GetIP().c_str(), pSocket->GetPort(), pBuff);
		break;
	case ZoyeeUtils::em_on_disconnect:
		printf("[disconnect][%s:%d][%s]\n", pSocket->GetIP().c_str(), pSocket->GetPort(), pBuff);
		break;
	case ZoyeeUtils::em_on_connect:
		printf("[connect][%s:%d][%s]\n", pSocket->GetIP().c_str(), pSocket->GetPort(), pBuff);
		break;
	case ZoyeeUtils::em_on_send_ok:
		printf("[send_ok][%s:%d][%s]\n", pSocket->GetIP().c_str(), pSocket->GetPort(), pBuff);
		break;
	case ZoyeeUtils::em_on_send_fail:
		printf("[send_fail][%s:%d][%s]\n", pSocket->GetIP().c_str(), pSocket->GetPort(), pBuff);
		break;
	default:
		printf("[%d][%s]\n", type, pBuff);
		break;
	}	
	return;
}

