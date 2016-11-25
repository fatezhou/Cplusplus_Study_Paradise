#ifndef __WMI_H__
#define __WMI_H__

#include <Wbemidl.h>
#include <iostream>
#include <string>
struct IWbemServices;
namespace ZoyeeUtils{
	class CWMI{
	public:
		CWMI();
		~CWMI();
		int Init();
		enum emWmiInfo{
			Win32_SoundDevice,                          // 音频设备  
			Win32_Product,    
			Win32_Registry,                             // 注册表  
			Win32_OnBoardDevice,                        // 主板上的嵌入设备  
			Win32_MotherboardDevice,                    // 母板  
			Win32_BaseBoard,                            // 主板  
			Win32_DMAChannel,                           // DMA通道  
			Win32_Bus,                                  // 总线  
			Win32_BIOS,                                 // BIOS系统  
			Win32_SystemBIOS,  
			Win32_Processor,                            // 处理器  
			Win32_SystemProcesses,                      //   
			Win32_Process,                              // 系统进程  
			Win32_Thread,                               // 系统线程  
			Win32_AssociatedProcessorMemory,            // CACHE  
			Win32_CacheMemory,                          // 二级缓存内存  
			Win32_PhysicalMedia,                        // 物理媒体信息 (硬盘)  
			Win32_LogicalDisk ,                         // 逻辑驱动器  
			Win32_DiskDrive,                            // 磁盘驱动器  
			Win32_MemoryDevice,                         // 内存设备  
			Win32_PhysicalMemoryArray,                  // 物理内存数组  
			Win32_PhysicalMemoryLocation,               // 物理内存位置  
			CIM_NumericSensor,                          // 数字传感器  
			Win32_VoltageProbe,                         // 数字传感器   
			Win32_TemperatureProbe,                     // 温度传感器  
			Win32_CurrentProbe,       
			Win32_OperatingSystem,                      // 操作系统      
			Win32_UserAccount,                          // 用户账号  
			Win32_SerialPort,                           // 串行接口  
			Win32_ParallelPort,                         // 并行接口  
			Win32_SCSIController,                       // 小型计算机系统接口  
			Win32_PortResource,                         // I/O 端口  
			Win32_PNPDevice,                            // 即插即用设备  
			Win32_NetworkAdapter,                       // 网络适配器  
			Win32_NetworkAdapterConfiguration,  
			Win32_NetworkAdapterSetting,  
			Win32_AssociatedBattery,  
			Win32_Battery,                              // 内部电池  
			Win32_PortableBattery,  
			Win32_PowerManagementEvent,  
			Win32_UninterruptiblePowerSupply,  
			Win32_DriverForDevice,  
			Win32_Printer,                              // 打印机  
			Win32_TCPIPPrinterPort,  
			Win32_POTSModem,  
			Win32_DesktopMonitor,                       // 显示器  
			Win32_VideoController,                      // 显卡  
			Win32_CDROMDrive,  
			Win32_Keyboard,                             // 键盘  
			Win32_AutochkSetting,  
			Win32_PointingDevice,                       // 点击设备：鼠标、触摸板  
			Win32_Fan,                                  // 风扇  
			Win32_WMISetting,  
			Win32_TimeZone,       
			Win32_Environment,                          // 环境路径  
			Win32_QuotaSetting,  
			Win32_NetworkProtocol                    // 己安装的网络协议  
		};
		std::string GetSystemInfo(emWmiInfo wmInfo);		
		std::string ExecWQL(std::string strWQL);
	protected:
		std::string GetSelectString(emWmiInfo wmInfo);
	private:
		IWbemServices *pSvc;
		IWbemLocator *pLoc;
	};
}

#endif