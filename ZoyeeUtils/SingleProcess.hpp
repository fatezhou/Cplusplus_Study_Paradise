#ifndef __ZOYEE_SINGLE_PROCESS_H__
#define __ZOYEE_SINGLE_PROCESS_H__

#include <Windows.h>
//想要进程单例, 在最早的地方调用这个
namespace zoyee{
	class CSingleProcess{
	public:
		CSingleProcess(char* pSingeName, int nWaitTimeout = 10) : nTimeout(nWaitTimeout), hMutex(NULL){
			if (pSingeName){
				hMutex = CreateMutexA(NULL, FALSE, pSingeName);
			}			
		};

		bool IsSingle(){
			if (hMutex == NULL || hMutex == INVALID_HANDLE_VALUE || WaitForSingleObject(hMutex, nTimeout) == WAIT_TIMEOUT){
				return false;
			}
			return true;
		};

		~CSingleProcess(){
			if (hMutex){
				ReleaseMutex(hMutex);
				CloseHandle(hMutex);
			}
		}
	private:
		HANDLE hMutex;
		int nTimeout;
	};
}

#endif