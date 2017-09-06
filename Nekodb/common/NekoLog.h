#include <string>
#include <queue>
#include <map>
#include <fstream>
#include <cstdarg>
#include <thread>
#include <mutex>
#include <functional>

#ifdef WIN32
#include <windows.h>
#endif

namespace Neko{	
	class CLog{
	public:
		static CLog* GetInstance();
		void LoadConfig();
		void SetWriteFunction(std::function<void(const std::string& strText)> func);
		std::function<void(const std::string& strText)> GetWriteFunction();

		void Debug(const std::string& strModule, const int nLine, char* pFmt, ...);//0
		void Info(const std::string& strModule, const int nLine, char* pFmt, ...);//1
		void Error(const std::string& strModule, const int nLine, char* pFmt, ...);//2

		void SetAsync(bool bAsync);
		bool IsAsync();
		std::string Format(const std::string& strType, const std::string& strTime, const std::string& strModule, const int nLine, char* pFmt, va_list ap);

	private:
		CLog();
		void input(const std::string& strText);
		static void asyncFunction();
		static void defaultWrite(const std::string& strText);
		std::string getLocalTime();
		void Write(const std::string& strText);
		void Push(const std::string& strText);
		std::string Get();

		void SetRunThreadState(bool bRun);
		bool GetRunThreadState();

	private:
		std::fstream m_fstream;
		std::function<void(const std::string& strText)> m_funcWrite;
		std::queue<std::string> m_queue;
		std::mutex m_mutex;

		bool m_bAsync;
		bool m_bRunThreadState;
		int m_nLogLevel;
		std::map<std::string, std::string> ConfigMap;
#ifdef WIN32
		HANDLE hEvent;
#endif
	};
}
