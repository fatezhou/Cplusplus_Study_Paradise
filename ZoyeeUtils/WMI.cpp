#include "WMI.h"

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib,"kernel32.lib")
#include <comutil.h>
#include <iostream>
using namespace std;

ZoyeeUtils::CWMI::CWMI()
{

}

int ZoyeeUtils::CWMI::Init()
{
	HRESULT hres;
	//初始化 COM.
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		cout << "Failed to initialize COM library. "<< "Error code = 0x"<< hex << hres << endl;
		return 1; // Program has failed.
	}

	// 设置进程安全级别

	hres = CoInitializeSecurity(
		NULL,
		-1, // COM negotiates service
		NULL, // Authentication services
		NULL, // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT, // authentication
		RPC_C_IMP_LEVEL_IMPERSONATE, // Impersonation
		NULL, // Authentication info
		EOAC_NONE, // Additional capabilities
		NULL // Reserved
		);

	if (FAILED(hres))
	{
		cout << "Failed to initialize security. "
			<< "Error code = 0x"
			<< hex << hres << endl;
		CoUninitialize();
		return 1; // Program has failed.
	}

	IWbemLocator *pLoc = 0;
	hres = CoCreateInstance(
		CLSID_WbemLocator,        
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *) &pLoc);
	if (FAILED(hres))
	{
		CoUninitialize();
		return -1;       // Program has failed.
	}
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // WMI namespace
		NULL,                    // User name
		NULL,                    // User password
		0,                       // Locale
		NULL,                    // Security flags 
		0,                       // Authority      
		0,                       // Context object
		&pSvc                    // IWbemServices proxy
		);     
	if (FAILED(hres))
	{
		pLoc->Release();    
		CoUninitialize();
		return -2;                // Program has failed.
	}
	return 0;
}

std::string ZoyeeUtils::CWMI::GetSelectString( emWmiInfo wmInfo )
{
	string strTable;
	switch(wmInfo)
	{
	case Win32_SoundDevice:                          // 音频设备  
		strTable = "Win32_SoundDevice";
		break;
	case Win32_Product:    
		strTable = "Win32_Product";
		break;
	case Win32_Registry:                             // 注册表  
		strTable = "Win32_Registry";
		break;
	case Win32_OnBoardDevice:                        // 主板上的嵌入设备  
		strTable = "Win32_OnBoardDevice";
		break;
	case Win32_MotherboardDevice:                    // 母板  
		strTable  = "Win32_MotherboardDevice";
		break;
	case Win32_BaseBoard:                            // 主板  
		strTable = "Win32_BaseBoard";
		break;
	case Win32_DMAChannel:                           // DMA通道  
		strTable = "Win32_DMAChannel";
		break;
	case Win32_Bus:                                  // 总线  
		strTable = "Win32_Bus";
		break;
	case Win32_BIOS:                                 // BIOS系统  
		strTable = "Win32_BIOS";
		break;
	case Win32_SystemBIOS:  
		strTable = "Win32_SystemBIOS";
		break;
	case Win32_Processor:                            // 处理器  
		strTable = "Win32_Processor";
		break;
	case Win32_SystemProcesses:                      //   
		strTable = "Win32_SystemProcesses";
		break;
	case Win32_Process:                              // 系统进程  
		strTable = "Win32_Process";
		break;
	case Win32_Thread:                               // 系统线程  
		strTable = "Win32_Thread";
		break;
	case Win32_AssociatedProcessorMemory:            // CACHE  
		strTable = "Win32_AssociatedProcessorMemory";
		break;
	case Win32_CacheMemory:                          // 二级缓存内存  
		strTable = "Win32_CacheMemory";
		break;
	case Win32_PhysicalMedia:                        // 物理媒体信息 (硬盘)  
		strTable = "Win32_PhysicalMedia";
		break;
	case Win32_LogicalDisk :                         // 逻辑驱动器  
		strTable = "Win32_LogicalDisk";
		break;
	case Win32_DiskDrive:                            // 磁盘驱动器  
		strTable = "Win32_DiskDrive";
		break;
	case Win32_MemoryDevice:                         // 内存设备  
		strTable = "Win32_MemoryDevice";
		break;
	case Win32_PhysicalMemoryArray:                  // 物理内存数组  
		strTable = "Win32_PhysicalMemoryArray";
		break;
	case Win32_PhysicalMemoryLocation:               // 物理内存位置  
		strTable = "Win32_PhysicalMemoryLocation";
		break;
	case CIM_NumericSensor:                          // 数字传感器  
		strTable = "CIM_NumericSensor";
		break;
	case Win32_VoltageProbe:                         // 数字传感器   
		strTable = "Win32_VoltageProbe";
		break;
	case Win32_TemperatureProbe:                     // 温度传感器  
		strTable = "Win32_TemperatureProbe";
		break;
	case Win32_CurrentProbe:       
		strTable = "Win32_CurrentProbe";
		break;
	case Win32_OperatingSystem:                      // 操作系统      
		strTable = "Win32_OperatingSystem";
		break;
	case Win32_UserAccount:                          // 用户账号  
		strTable = "Win32_UserAccount";
		break;
	case Win32_SerialPort:                           // 串行接口  
		strTable = "Win32_SerialPort";
		break;
	case Win32_ParallelPort:                         // 并行接口  
		strTable = "Win32_ParallelPort";
		break;
	case Win32_SCSIController:                       // 小型计算机系统接口  
		strTable = "Win32_SCSIController";
		break;
	case Win32_PortResource:                         // I/O 端口  
		strTable = "Win32_PortResource";
		break;
	case Win32_PNPDevice:                            // 即插即用设备  
		strTable = "Win32_PNPDevice";
		break;
	case Win32_NetworkAdapter:                       // 网络适配器  
		strTable = "Win32_NetworkAdapter";
		break;
	case Win32_NetworkAdapterConfiguration:  
		strTable = "Win32_NetworkAdapterConfiguration";
		break;
	case Win32_NetworkAdapterSetting:  
		strTable = "Win32_NetworkAdapterSetting";
		break;
	case Win32_AssociatedBattery:  
		strTable = "Win32_AssociatedBattery";
		break;
	case Win32_Battery:                              // 内部电池  
		strTable = "Win32_Battery";
		break;
	case Win32_PortableBattery:  
		strTable = "Win32_PortableBattery";
		break;
	case Win32_PowerManagementEvent:  
		strTable = "Win32_PowerManagementEvent";
		break;
	case Win32_UninterruptiblePowerSupply:  
		strTable = "Win32_UninterruptiblePowerSupply";
		break;
	case Win32_DriverForDevice:  
		strTable = "Win32_DriverForDevice";
		break;
	case Win32_Printer:                              // 打印机  
		strTable = "Win32_Printer";
		break;
	case Win32_TCPIPPrinterPort:  
		strTable = "Win32_TCPIPPrinterPort";
		break;
	case Win32_POTSModem:  
		strTable = "Win32_POTSModem";
		break;
	case Win32_DesktopMonitor:                       // 显示器  
		strTable = "Win32_DesktopMonitor";
		break;
	case Win32_VideoController:                      // 显卡  
		strTable = "Win32_VideoController";
		break;
	case Win32_CDROMDrive:  
		strTable = "Win32_CDROMDrive";
		break;
	case Win32_Keyboard:                             // 键盘  
		strTable = "Win32_Keyboard";
		break;
	case Win32_AutochkSetting:  
		strTable = "Win32_AutochkSetting";
		break;
	case Win32_PointingDevice:                       // 点击设备：鼠标、触摸板  
		strTable = "Win32_PointingDevice";
		break;
	case Win32_Fan:                                  // 风扇  
		strTable = "Win32_Fan";
		break;
	case Win32_WMISetting:  
		strTable = "Win32_WMISetting";
		break;
	case Win32_TimeZone:       
		strTable = "Win32_TimeZone";
		break;
	case Win32_Environment:                          // 环境路径  
		strTable = "Win32_Environment";
		break;
	case Win32_QuotaSetting:  
		strTable = "Win32_QuotaSetting";
		break;
	case Win32_NetworkProtocol:                      // 己安装的网络协议  
		strTable = "Win32_NetworkProtocol";
		break;
	default:
		break;
	}
	char sz[128] = {0};
	sprintf(sz, "SELECT * FROM %s", strTable.c_str());
	return sz;
}

std::string ZoyeeUtils::CWMI::GetSystemInfo( emWmiInfo wmInfo )
{
	HRESULT hres;
	hres	= CoSetProxyBlanket(
		pSvc,
		// the proxy to set 
		RPC_C_AUTHN_WINNT,            // authentication service
		RPC_C_AUTHZ_NONE,             // authorization service
		NULL,                         // Server principal name
		RPC_C_AUTHN_LEVEL_CALL,       // authentication level
		RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
		NULL,                         // client identity
		EOAC_NONE                     // proxy capabilities    
		);

	if (FAILED(hres))
	{
		return "";               // Program has failed.
	}
	IEnumWbemClassObject* pEnumerator = NULL;     

	hres = pSvc->ExecQuery(     
		_bstr_t("WQL"),   
		_bstr_t(GetSelectString(wmInfo).c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,      
		NULL,     
		&pEnumerator);   
	IWbemClassObject* pclsObj = NULL;     
	ULONG uReturn = 0;     
	std::string strvtProBSTR;
	while (pEnumerator)     
	{     
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);     
		if ((0 == uReturn) || (0 == pclsObj))     
		{     
			break;     
		}     
		VARIANT vtProp; 
		// Get the value of the Name property      
		VariantInit(&vtProp);   
		vtProp.bstrVal = 0;// NULL   
		hr = pclsObj->GetObjectText(0, &vtProp.bstrVal);  
		if (vtProp.bstrVal != 0)     
		{     
			/*wcout.imbue(locale(locale(""), "" ,LC_CTYPE));  
			wcout << L"硬件信息: " << vtProp.bstrVal << endl;     */
			strvtProBSTR += _com_util::ConvertBSTRToString(vtProp.bstrVal);
		}     
		else     
		{     
			
		}  
		VariantClear(&vtProp);     
	}       
	if (pEnumerator)     
	{     
		pEnumerator->Release();     
	}     	
	if (pclsObj){
		pclsObj->Release();
	}  
	return strvtProBSTR;
}

ZoyeeUtils::CWMI::~CWMI()
{
	pSvc->Release();
	pLoc->Release();    
	CoUninitialize();
}

std::string ZoyeeUtils::CWMI::ExecWQL( std::string strWQL )
{
	HRESULT hres;
	hres	= CoSetProxyBlanket(
		pSvc,
		// the proxy to set 
		RPC_C_AUTHN_WINNT,            // authentication service
		RPC_C_AUTHZ_NONE,             // authorization service
		NULL,                         // Server principal name
		RPC_C_AUTHN_LEVEL_CALL,       // authentication level
		RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
		NULL,                         // client identity
		EOAC_NONE                     // proxy capabilities    
		);

	if (FAILED(hres))
	{
		return "";               // Program has failed.
	}
	IEnumWbemClassObject* pEnumerator = NULL;     

	hres = pSvc->ExecQuery(     
		_bstr_t("WQL"),   
		_bstr_t(strWQL.c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,      
		NULL,     
		&pEnumerator);   
	IWbemClassObject* pclsObj = NULL;     
	ULONG uReturn = 0;     
	std::string strvtProBSTR;
	while (pEnumerator)     
	{     
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);     
		if ((0 == uReturn) || (0 == pclsObj))     
		{     
			break;     
		}     
		VARIANT vtProp; 
		// Get the value of the Name property      
		VariantInit(&vtProp);   
		vtProp.bstrVal = 0;// NULL   
		hr = pclsObj->GetObjectText(0, &vtProp.bstrVal);  
		if (vtProp.bstrVal != 0)     
		{     
			/*wcout.imbue(locale(locale(""), "" ,LC_CTYPE));  
			wcout << L"硬件信息: " << vtProp.bstrVal << endl;     */
			strvtProBSTR += _com_util::ConvertBSTRToString(vtProp.bstrVal);
		}     
		else     
		{     
			
		}  
		VariantClear(&vtProp);     
	}       
	if (pEnumerator)     
	{     
		pEnumerator->Release();     
	}     	
	if (pclsObj){
		pclsObj->Release();
	}  
	return strvtProBSTR;
}
