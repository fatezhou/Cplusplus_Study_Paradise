#ifndef __CRASH_DEBUGER_H__
#define __CRASH_DEBUGER_H__

typedef void(*pCallbackUserData)();

namespace ZoyeeUtils{
	void InitDebuger(pCallbackUserData pFunction = nullptr);
}


#endif