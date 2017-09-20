#include "MsgLoop.h"
#include <windows.h>
#include <thread>
#include <iostream>

int main(){
	std::thread([](){
		int i = 0;
		std::cin >> i;
		Neko::RegisterMsgFunc(1234, [](unsigned int nMsg, unsigned int wParam, unsigned int lParam, bool& bContinueDispatch)->int{
			printf(""); 
			return 0;
		}, 0);
		Neko::PostMsg(1234, 0, 0);
		Neko::PostMsg(WM_QUIT, 0, 0);
	}).detach();
	Neko::InitMsgLoop(false);
}