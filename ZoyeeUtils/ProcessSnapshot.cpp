#include "ProcessSnapshot.h"
using namespace ZoyeeUtils;

#include <Windows.h>
#include <Windows.h>  
#include <tlhelp32.h>  

typedef enum _SYSTEM_INFORMATION_CLASS     
{     
	SystemBasicInformation,                 //  0 Y N     
	SystemProcessorInformation,             //  1 Y N     
	SystemPerformanceInformation,           //  2 Y N     
	SystemTimeOfDayInformation,             //  3 Y N     
	SystemNotImplemented1,                  //  4 Y N     
	SystemProcessesAndThreadsInformation,   //  5 Y N     
	SystemCallCounts,                       //  6 Y N     
	SystemConfigurationInformation,         //  7 Y N     
	SystemProcessorTimes,                   //  8 Y N     
	SystemGlobalFlag,                       //  9 Y Y     
	SystemNotImplemented2,                  // 10 Y N     
	SystemModuleInformation,                // 11 Y N     
	SystemLockInformation,                  // 12 Y N     
	SystemNotImplemented3,                  // 13 Y N     
	SystemNotImplemented4,                  // 14 Y N     
	SystemNotImplemented5,                  // 15 Y N     
	SystemHandleInformation,                // 16 Y N     
	SystemObjectInformation,                // 17 Y N     
	SystemPagefileInformation,              // 18 Y N     
	SystemInstructionEmulationCounts,       // 19 Y N     
	SystemInvalidInfoClass1,                // 20     
	SystemCacheInformation,                 // 21 Y Y     
	SystemPoolTagInformation,               // 22 Y N     
	SystemProcessorStatistics,              // 23 Y N     
	SystemDpcInformation,                   // 24 Y Y     
	SystemNotImplemented6,                  // 25 Y N     
	SystemLoadImage,                        // 26 N Y     
	SystemUnloadImage,                      // 27 N Y     
	SystemTimeAdjustment,                   // 28 Y Y     
	SystemNotImplemented7,                  // 29 Y N     
	SystemNotImplemented8,                  // 30 Y N     
	SystemNotImplemented9,                  // 31 Y N     
	SystemCrashDumpInformation,             // 32 Y N     
	SystemExceptionInformation,             // 33 Y N     
	SystemCrashDumpStateInformation,        // 34 Y Y/N     
	SystemKernelDebuggerInformation,        // 35 Y N     
	SystemContextSwitchInformation,         // 36 Y N     
	SystemRegistryQuotaInformation,         // 37 Y Y     
	SystemLoadAndCallImage,                 // 38 N Y     
	SystemPrioritySeparation,               // 39 N Y     
	SystemNotImplemented10,                 // 40 Y N     
	SystemNotImplemented11,                 // 41 Y N     
	SystemInvalidInfoClass2,                // 42     
	SystemInvalidInfoClass3,                // 43     
	SystemTimeZoneInformation,              // 44 Y N     
	SystemLookasideInformation,             // 45 Y N     
	SystemSetTimeSlipEvent,                 // 46 N Y     
	SystemCreateSession,                    // 47 N Y     
	SystemDeleteSession,                    // 48 N Y     
	SystemInvalidInfoClass4,                // 49     
	SystemRangeStartInformation,            // 50 Y N     
	SystemVerifierInformation,              // 51 Y Y     
	SystemAddVerifier,                      // 52 N Y     
	SystemSessionProcessesInformation       // 53 Y N     
} SYSTEM_INFORMATION_CLASS;  

NTSTATUS WINAPI ZwQuerySystemInformation(  
	SYSTEM_INFORMATION_CLASS SystemInformationClass,  
	PVOID SystemInformation,  
	ULONG SystemInformationLength,  
	PULONG ReturnLength);  

typedef struct _LSA_UNICODE_STRING  
{  
	USHORT  Length;  
	USHORT  MaximumLength;  
	PWSTR   Buffer;  
}LSA_UNICODE_STRING,*PLSA_UNICODE_STRING;  
typedef LSA_UNICODE_STRING UNICODE_STRING, *PUNICODE_STRING;  

typedef LONG KPRIORITY;  

typedef enum _THREAD_STATE  
{  
	StateInitialized,  
	StateReady,  
	StateRunning,  
	StateStandby,  
	StateTerminated,  
	StateWait,  
	StateTransition,  
	StateUnknown  
}THREAD_STATE;  

typedef enum _KWAIT_REASON  
{  
	Executive,  
	FreePage,  
	PageIn,  
	PoolAllocation,  
	DelayExecution,  
	Suspended,  
	UserRequest,  
	WrExecutive,  
	WrFreePage,  
	WrPageIn,  
	WrPoolAllocation,  
	WrDelayExecution,  
	WrSuspended,  
	WrUserRequest,  
	WrEventPair,  
	WrQueue,  
	WrLpcReceive,  
	WrLpcReply,  
	WrVertualMemory,  
	WrPageOut,  
	WrRendezvous,  
	Spare2,  
	Spare3,  
	Spare4,  
	Spare5,  
	Spare6,  
	WrKernel  
}KWAIT_REASON;  

typedef struct _CLIENT_ID  
{  
	HANDLE UniqueProcess;  
	HANDLE UniqueThread;  
}CLIENT_ID;  
typedef CLIENT_ID *PCLIENT_ID;  

typedef LONG KPRIORITY;  

typedef struct _VM_COUNTERS  
{  
	ULONG PeakVirtualSize;  
	ULONG VirtualSize;  
	ULONG PageFaultCount;  
	ULONG PeakWorkingSetSize;  
	ULONG WorkingSetSize;  
	ULONG QuotaPeakPagedPoolUsage;  
	ULONG QuotaPagedPoolUsage;  
	ULONG QuotaPeakNonPagedPoolUsage;  
	ULONG QuotaNonPagedPoolUsage;  
	ULONG PagefileUsage;  
	ULONG PeakPagefileUsage;  
}VM_COUNTERS,*PVM_COUNTERS;  

typedef struct _SYSTEM_THREADS  
{  
	LARGE_INTEGER KernelTime;  
	LARGE_INTEGER UserTime;  
	LARGE_INTEGER CreateTime;  
	ULONG         WaitTime;  
	PVOID         StartAddress;  
	CLIENT_ID     ClientId;  
	KPRIORITY     Priority;  
	KPRIORITY     BasePriority;  
	ULONG         ContextSwitchCount;  
	THREAD_STATE  State;  
	KWAIT_REASON  WaitReason;  
}SYSTEM_THREADS,*PSYSTEM_THREADS;  

typedef struct _SYSTEM_PROCESSES  
{  
	ULONG          NextEntryDelta;  
	ULONG          ThreadCount;  
	ULONG          Reserved1[6];  
	LARGE_INTEGER  CreateTime;  
	LARGE_INTEGER  UserTime;  
	LARGE_INTEGER  KernelTime;  
	UNICODE_STRING ProcessName;  
	KPRIORITY      BasePriority;  
	ULONG          ProcessId;  
	ULONG          InheritedFromProcessId;  
	ULONG          HandleCount;  
	ULONG          Reserved2[2];  
	VM_COUNTERS    VmCounters;  
	IO_COUNTERS    IoCounters;  
	SYSTEM_THREADS Threads[1];  
}SYSTEM_PROCESSES,*PSYSTEM_PROCESSES;  

#define NT_SUCCESS(Status)((NTSTATUS)(Status)>=0)  
#define STATUS_SUCCESS 0x00000000  
#define STATUS_UNSUCCESSFUL 0xC0000001  
#define STATUS_NOT_IMPLEMENTED 0xC0000002  
#define STATUS_INFO_LENGTH_MISMATCH 0xC0000004  
#define STATUS_INVALID_PARAMETER 0xC000000D  
#define STATUS_ACCESS_DENIED 0xC0000022  
#define STATUS_BUFFER_TOO_SMALL 0xC0000023  
#define OBJ_KERNEL_HANDLE 0x00000200  

typedef NTSTATUS (WINAPI *PFN_NTQUERYSYSTEMINFORMATION)(SYSTEM_INFORMATION_CLASS,PVOID, ULONG, PULONG);  

ProcessSnapshot::ProcessSnapshot(void)
{
}


ProcessSnapshot::~ProcessSnapshot(void)
{
}

std::list<ProcessSnapshot::ProcessInfo> ZoyeeUtils::ProcessSnapshot::GetProcessInfo( const char* pProcessName /*= nullptr*/ )
{
	std::list<ProcessSnapshot::ProcessInfo> lst;
	
	HMODULE hNtDll = LoadLibraryA("ntdll.dll");  
	PFN_NTQUERYSYSTEMINFORMATION pfnZwQuerySystemInformation = NULL;    // ZwQuerySystemInformation  
	pfnZwQuerySystemInformation = (PFN_NTQUERYSYSTEMINFORMATION) GetProcAddress(hNtDll, "ZwQuerySystemInformation");  
	if (pfnZwQuerySystemInformation == NULL)  
	{  
		DWORD dwErrCode = GetLastError();  
		return lst;  
	}  
	
	PSYSTEM_PROCESSES psp = NULL;   
	DWORD dwNeedSize = 0;  
	NTSTATUS status = pfnZwQuerySystemInformation(SystemProcessesAndThreadsInformation, NULL, 0, &dwNeedSize);  

	if (status == STATUS_INFO_LENGTH_MISMATCH )  
	{     
		BYTE *pBuffer = new BYTE[dwNeedSize];    
		status = pfnZwQuerySystemInformation(SystemProcessesAndThreadsInformation, (PVOID)pBuffer, dwNeedSize, NULL);    
		if (status == STATUS_SUCCESS )    
		{    
			psp = (PSYSTEM_PROCESSES)pBuffer;  
			do {    
				ProcessInfo processInfo = {0};				
				processInfo.lWorkSetMemory = psp->VmCounters.WorkingSetSize;
				processInfo.nHandleCount = psp->HandleCount;
				processInfo.nPid = psp->ProcessId;
				processInfo.nThreadCount = psp->ThreadCount;
				if (psp->ProcessName.Buffer != NULL){
					WideCharToMultiByte(CP_ACP, 0, psp->ProcessName.Buffer, wcslen(psp->ProcessName.Buffer), processInfo.szName, sizeof(processInfo.szName), NULL, NULL);
					processInfo.hHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.nPid);
					if (pProcessName != nullptr){
						if (stricmp(pProcessName, processInfo.szName) == 0){
							lst.push_back(processInfo);
						}
					}else{
						lst.push_back(processInfo);
					}					
				}
				psp = (PSYSTEM_PROCESSES)((ULONG)psp + psp->NextEntryDelta );    
			} while ( psp->NextEntryDelta != 0 );  
		}  
		delete []pBuffer;    
		pBuffer = NULL;   
	}  
	FreeLibrary(hNtDll);

	//PROCESSENTRY32 pe32 = {sizeof(pe32)} ;  
	//HANDLE hProcessShot = CreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS, 0 ) ;  
	//char szBuf[1024] = {0} ;  
	//if ( Process32First ( hProcessShot, &pe32 ) )  
	//{  
	//	do {  
	//		memset ( szBuf, 0, sizeof(szBuf) ) ;  
	//		// 把宽字符的进程名转化为ANSI字符串  
	//		WideCharToMultiByte (CP_ACP, 0, pe32.szExeFile, wcslen(pe32.szExeFile), szBuf, sizeof(szBuf), NULL, NULL );  

	//		//cout << "Process: " << szBuf << endl ;  
	//		//cout << '\t' << "Usage           : " << pe32.cntUsage << endl ;  
	//		//fout << '\t' << "ProcessID       : " << pe32.th32ProcessID << endl ;  
	//		//fout << '\t' << "DefaultHeapID   : " << (ULONG_PTR)pe32.th32DefaultHeapID << endl ;  
	//		//fout << '\t' << "ModuleID        : " << pe32.th32ModuleID << endl ;  
	//		//fout << '\t' << "ThreadNum       : " << pe32.cntThreads   << endl ;  
	//		//fout << '\t' << "ParentProcessID : " << pe32.th32ParentProcessID << endl ;  
	//		//fout << '\t' << "PriClassBase    : " << pe32.pcPriClassBase << endl ;  
	//	}while ( Process32Next ( hProcessShot, &pe32 ) ) ;  
	//}  

	//CloseHandle ( hProcessShot ) ;  
	return lst;
}
