// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� APIHOOK_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// APIHOOK_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef APIHOOK_EXPORTS
#define APIHOOK_API __declspec(dllexport)
#else
#define APIHOOK_API __declspec(dllimport)
#endif
#include <WinSock2.h>
#include <Windows.h>
//
//// �����Ǵ� ApiHook.dll ������
//class APIHOOK_API CApiHook {
//public:
//	CApiHook(void);
//	// TODO: �ڴ�������ķ�����
//};
//
//extern APIHOOK_API int nApiHook;
//
//APIHOOK_API int fnApiHook(void);


extern "C" APIHOOK_API void SetAPIHook(void* pTrueFunc, void* pFakeFunc);
extern "C" APIHOOK_API void SetAPIUnHook(void* pTrueFunc);
extern "C" APIHOOK_API void Test();
extern "C" APIHOOK_API void AddHook(char* pFunctionName);
extern "C" APIHOOK_API void RemoveHook(char* pFunctionName);