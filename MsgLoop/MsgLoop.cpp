#include "MsgLoop.h"
#include <iostream>
#include <windows.h>
#include <map>
#include <thread>
#include <future>

using namespace Neko;

namespace Neko{
	class MsgConfig{
	public:
		MsgConfig(){
			::InitializeCriticalSection(&cs);
		};
		~MsgConfig(){
			::DeleteCriticalSection(&cs);
			for (auto iter = m_msgFunc.begin(); iter != m_msgFunc.end(); iter++){
				delete iter->second;
			}
			m_msgFunc.clear();
		}
		void Lock(){
			EnterCriticalSection(&cs);
		}
		void UnLock(){
			LeaveCriticalSection(&cs);
		}
		int nThreadId;

		class MsgFunc{
		public:
			MsgFunc(){
				pHandler = nullptr;
				pCallback = nullptr;
				nMsg = 0;
			};

			MsgFunc(unsigned int nMsg, pMsgWorkHandler pHandler, pMsgCallback pCallback){
				this->nMsg = nMsg;
				this->pHandler = pHandler;
				this->pCallback = pCallback;
			};
			pMsgWorkHandler pHandler;
			pMsgCallback pCallback;
			unsigned int nMsg;
		};
		std::map<unsigned int, MsgFunc*> m_msgFunc;
		CRITICAL_SECTION cs;
		std::promise<bool> unInitPromise;
	};
	MsgConfig* g_pConfig = nullptr;

	int Loop(MsgConfig* config){
		MSG msg;
		config->nThreadId = GetCurrentThreadId();
		while (GetMessage(&msg, NULL, 0, 0)){
			TranslateMessage(&msg);
			MsgConfig::MsgFunc* pMsgFunc = nullptr;
			config->Lock();
			auto iter = config->m_msgFunc.find(msg.message);
			if (iter != config->m_msgFunc.end()){
				pMsgFunc = iter->second;
			}
			config->UnLock();

			bool bContinue = true;
			int nRes = 0;
			if (pMsgFunc && pMsgFunc->pHandler){
				nRes = pMsgFunc->pHandler(msg.message, msg.wParam, msg.lParam, bContinue);
			}

			if (pMsgFunc && pMsgFunc->pCallback){
				pMsgFunc->pCallback(msg.message, nRes);
			}

			if (bContinue){
				DispatchMessage(&msg);
			}		
		}
		config->unInitPromise.set_value(true);
		return 0;
	}

	int InitMsgLoop(bool bAsync)
	{
		if (g_pConfig == nullptr){
			g_pConfig = new MsgConfig;
		}
		else{
			UninitMsgLoop();
		}

		if (bAsync){
			std::thread(Loop, g_pConfig).detach();
		}
		else{
			Loop(g_pConfig);
		}
		return 0;
	}

	int PostMsg(unsigned int nMsg, unsigned int wParam, unsigned int lParam)
	{
		if (nMsg <= 0){
			return 0;
		}
		return PostThreadMessage(g_pConfig->nThreadId, nMsg, wParam, lParam);
	}

	int UninitMsgLoop()
	{
		if (g_pConfig){
			PostMsg(WM_QUIT, 0, 0);
			try{				
				g_pConfig->unInitPromise.get_future().get();
				g_pConfig->unInitPromise = std::promise<bool>();
				delete g_pConfig;
				g_pConfig = nullptr;
			}
			catch (...){

			}			
		}
		return 0;
	}

	int RegisterMsgFunc(unsigned int nMsg, pMsgWorkHandler pHandler, pMsgCallback pCallback)
	{
		UnregisterMsgFunc(nMsg);
		g_pConfig->Lock();		
		g_pConfig->m_msgFunc.insert(std::make_pair(nMsg, new MsgConfig::MsgFunc(nMsg, pHandler, pCallback)));
		g_pConfig->UnLock();
		return 0;
	}

	int UnregisterMsgFunc(unsigned int nMsg)
	{
		g_pConfig->Lock();
		auto iter = g_pConfig->m_msgFunc.find(nMsg);
		if (iter != g_pConfig->m_msgFunc.end()){
			MsgConfig::MsgFunc* ptr = iter->second;
			delete ptr;
		}
		g_pConfig->UnLock();
		return 0;
	}

}