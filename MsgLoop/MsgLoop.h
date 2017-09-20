#ifndef __MSG_LOOP_H__
#define __MSG_LOOP_H__

typedef void(*pMsgCallback)(unsigned int nMsg, int nResult);
typedef int(*pMsgWorkHandler)(unsigned int nMsg, unsigned int wParam, unsigned int lParam, bool& bContinueDispatch);
namespace Neko{
	int InitMsgLoop(bool bAsync);
	int PostMsg(unsigned int nMsg, unsigned int wParam, unsigned int lParam);
	int UninitMsgLoop();
	int RegisterMsgFunc(unsigned int nMsg, pMsgWorkHandler pHandler, pMsgCallback pCallback);
	int UnregisterMsgFunc(unsigned int nMsg);
}

#endif