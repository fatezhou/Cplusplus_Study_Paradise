#include "MsgLoop.h"
#include <windows.h>
#include <thread>
#include <iostream>

int main(){
	std::thread([](){
		int i = 0;
		while (1){
			std::cin >> i;
			Neko::RegisterMsgFunc(i,
				[](unsigned int nMsg, unsigned int wParam, unsigned int lParam, bool& bContinueDispatch)->int{
				printf("msg:%d handler\n", nMsg);
				return 0;
			},
				[](unsigned int nMsg, int nResult){
				printf("msg:%d nRes:%d\n", nMsg, nResult);
			});
			Neko::PostMsg(i, 0, 0);
		}
		
		//Neko::PostMsg(WM_QUIT, 0, 0);
		//Neko::UninitMsgLoop();
	}).detach();
	Neko::InitMsgLoop(false);
}