
## <b>Nx_WebServer Application Description</b>

This application provides an example of Azure RTOS NetX Duo stack usage on STM32H7S78-DK board, it shows how to develop Web HTTP server based application.
The application is designed to load files and dyncamic web pages stored in SD card using a Web HTTP server, the code provides all required features to build a compliant Web HTTP Server.
The main entry function tx_application_define() is called by ThreadX during kernel start, at this stage, all NetX and FileX resources are created.

 + A NX_PACKET_POOL **NxAppPool** is allocated
 + A NX_IP instance **NetXDuoEthIpInstance** using that pool is initialized
 + A NX_PACKET_POOL **WebServerPool** is allocated
 + A NX_WEB_HTTP_SERVER **HTTPServer** instance using that pool is initialized
 + The ARP, ICMP and protocols (TCP and UDP) are enabled for the NX_IP instance
 + A DHCP client is created.

The application then creates 2 threads with different priorities:

 + **NxAppThread** (priority 10, PreemtionThreashold 10) : created with the TX_AUTO_START flag to start automatically.
 + **AppServerThread** (priority 5, PreemtionThreashold 5) : created with the TX_DONT_START flag to be started later.
 + **LedThread** (priority 15, PreemtionThreashold 15) : created with the TX_DONT_START flag to be started later.

The **NxAppThread** starts and performs the following actions:

  + Starts the DHCP client
  + Waits for the IP address resolution
  + Resumes the **AppServerThread**

The **AppServerThread**, once started:

  + Fx_media_open.
  + Starts HTTP server.
  + Each command coming from client (browser) is treated on the callback webserver_request_notify_callback.

The **LedThread**, once resumed from the dashboard:

  + Green LED is toggling & message is printed on HyperTerminal.

#### <b>Expected success behavior</b>

 + The board IP address "IP@" is printed on the HyperTerminal
 + Home page is well displayed on the browser after entering the url: http://IP@/about.html
 + Dashboard is well displayed on the browser after entering the url: http://IP@/dashboard.html
 + On dashboard, You can command green LED and watch some infos about threadx and netxduo.

#### <b>Error behaviors</b>

If the WEB HTTP server is not successfully started, the green LED stays OFF and the red LED toggles every 1 second.
In case of other errors, the Web HTTP server does not operate as designed (Files stored in the SD card are not loaded in the web browser).

#### <b>Assumptions if any</b>

The uSD card must be plugged before starting the application.
The board must be in a DHCP Ethernet network.

#### <b>Known limitations</b>

Hotplug is not implemented for this example, that is, the SD card is expected to be inserted before application running.

### <b>Notes</b>

 1.  This application runs from the external flash memory. It is launched from a first boot stage and inherits from this boot project configuration (caches, MPU regions [region 0 and 1], system clock at 600 MHz and external memory interface at the highest speed).
      Note that the boot part is automatically downloaded from the IDE environment via the board boot binary under Binary/Boot_XIP.hex file.

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it, by updating the "TX_TIMER_TICKS_PER_SECOND" define in the "tx_user.h" file. The update should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL or BSP).
 - Using dynamic memory allocation requires to apply some changes to the linker file.
   ThreadX needs to pass a pointer to the first free memory location in RAM to the tx_application_define() function,
   using the "first_unused_memory" argument.
   This requires changes in the linker files to expose this memory location.
    + For EWARM add the following section into the .icf file:
     ```
     place in RAM_region    { last section FREE_MEM };
     ```
    + For MDK-ARM:
    ```
    either define the RW_IRAM1 region in the ".sct" file
    or modify the line below in "tx_initialize_low_level.S to match the memory region being used
        LDR r1, =|Image$$RW_IRAM1$$ZI$$Limit|
    ```
    + For STM32CubeIDE add the following section into the .ld file:
    ```
    ._threadx_heap :
      {
         . = ALIGN(8);
         __RAM_segment_used_end__ = .;
         . = . + 64K;
         . = ALIGN(8);
       } >RAM AT> RAM
    ```

       The simplest way to provide memory for ThreadX is to define a new section, see ._threadx_heap above.
       In the example above the ThreadX heap size is set to 64KBytes.
       The ._threadx_heap must be located between the .bss and the ._user_heap_stack sections in the linker script.
       Caution: Make sure that ThreadX does not need more than the provided heap memory (64KBytes in this example).
       Read more in STM32CubeIDE User Guide, chapter: "Linker script".

    + The "tx_initialize_low_level.S" should be also modified to enable the "USE_DYNAMIC_MEMORY_ALLOCATION" flag.

#### <b>FileX/LevelX usage hints</b>

- FileX SD driver is using the DMA, thus the DTCM (0x20000000) memory should not be used by the application, as it is not accessible by the SD DMA.
- When calling the fx_media_format() API, it is highly recommended to understand all the parameters used by the API to correctly generate a valid filesystem.
- FileX is using data buffers, passed as arguments to fx_media_open(), fx_media_read() and fx_media_write() APIs, it is recommended that these buffers are multiple of sector size and "32 bytes" aligned to avoid cache maintenance issues.

#### <b>NetX Duo usage hints</b>

- The ETH TX And RX descriptors are accessed by the CPU and the ETH DMA IP, thus they should not be allocated into the DTCM RAM "0x20000000".
- Make sure to allocate them into a "Non-Cacheable" memory region to always ensure data coherency between the CPU and ETH DMA.
- Depending on the application scenario, the total TX and RX descriptors may need to be increased by updating respectively the "ETH_TX_DESC_CNT" and "ETH_RX_DESC_CNT" in the "stm32h7rsxx_hal_conf.h", to guarantee the application's correct behavior, but this will cost extra memory to allocate.
- The NetXDuo application needs to allocate the <b> <i> NX_PACKET </i> </b> pool in a dedicated section that is configured as "Cacheable Write-through". Below is an example of the section declaration for different IDEs.
   + For EWARM ".icf" file
   ```
   define symbol __ICFEDIT_region_NXDATA_start__  = 0x24020100;
   define symbol __ICFEDIT_region_NXDATA_end__   = 0x240240FF;
   define symbol __ICFEDIT_region_NXDATA_POOL_start__  = 0x24024100;
   define symbol __ICFEDIT_region_NXDATA_POOL_end__   = 0x2403FFFF;
   define region NXApp_region = mem:[from __ICFEDIT_region_NXDATA_start__ to __ICFEDIT_region_NXDATA_end__];
   define region NXApp_region_pool = mem:[from __ICFEDIT_region_NXDATA_POOL_start__ to __ICFEDIT_region_NXDATA_POOL_end__];
   place in NXApp_region { section .NxServerPoolSection};
   place in NXApp_region_pool { section .NetXPoolSection};
   ```
   + For MDK-ARM
   ```
  RW_NXServerPoolSection 0x24020100 0x4000  {
  *(.NxServerPoolSection)
  }

   RW_NXDriverSection 0x24024100 0x1BF00  {
  *(.NetXPoolSection)
  }
   ```
   + For STM32CubeIDE ".ld" file
   ```
.nx_data 0x24020100 (NOLOAD):
 {
    . = ABSOLUTE(0x24020100);
    *(.NxServerPoolSection)
    . = ABSOLUTE(0x24024100);
    *(.NetXPoolSection)
 } >RAM AT> FLASH
   ```

  This section is then used in the <code> app_azure_rtos.c</code> file to force the <code>nx_byte_pool_buffer</code> allocation.

```

/* USER CODE BEGIN NX_Pool_Buffer */

#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma location = ".NetXPoolSection"

#elif defined ( __CC_ARM ) || defined(__ARMCC_VERSION) /* ARM Compiler 5/6 */
__attribute__((section(".NetXPoolSection")))

#elif defined ( __GNUC__ ) /* GNU Compiler */
__attribute__((section(".NetXPoolSection")))
#endif

/* USER CODE END NX_Pool_Buffer */
__ALIGN_BEGIN static UCHAR nx_byte_pool_buffer[NX_APP_MEM_POOL_SIZE] __ALIGN_END;
static TX_BYTE_POOL nx_app_byte_pool;
```
For more details about the MPU configuration please refer to the [AN4838](https://www.st.com/resource/en/application_note/dm00272912-managing-memory-protection-unit-in-stm32-mcus-stmicroelectronics.pdf)

### <b>Keywords</b>

RTOS, ThreadX, Network, NetxDuo, Web HTTP Server, FileX, File ,SDMMC, UART

### <b>Hardware and Software environment</b>

  - This application runs on STM32H7S7L8xx devices.
  - This application has been tested with STMicroelectronics STM32H7S78-DK boards revision: MB1736-H7S7L8-D01
    and can be easily tailored to any other supported device and development board.
  - This application uses UART4 to display logs, the hyperterminal configuration is as follows:
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - Stop Bit = 1
      - Parity = None
      - Flow control = None

  - On STM32H7S78-DK board, the JP6 pin [2-3] must be ON.

### <b>How to use it ?</b>

To configure STM32CubeIDE Debug Configuration, you must do the following :
    1. Add the adequate external loader (MX66UW1G45G_STM32H7S78-DK.stldr file) in Project->Debugger Configuration and uncheck the "Load Symbols" option
    2. Add in the startup the Boot_XIP.elf file in Project->Debugger Configuration
    3. Move up the application in the startup

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
