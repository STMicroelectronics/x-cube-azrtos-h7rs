##  <b>Nx_Iperf_wifi Application Description</b>

This application provides an example of Azure RTOS NetXDuo stack usage .
It shows performance when using different modes : TCP_server, UDP_server, TCP_client and UDP_client.
The main entry function tx_application_define() is then called by ThreadX during kernel start, at this stage, all NetXDuo resources are created.

 + A NX_PACKET_POOL **AppPacketPool** is allocated
 + A NX_IP instance **IpInstance** using that pool is initialized
 + A NX_PACKET_POOL **WebServerPool** is allocated
 + The ARP, ICMP and protocols (TCP and UDP) are enabled for the NX_IP instance
 + A DHCP client is created.

The application creates 1 thread :

 + **AppMainThread** (priority 4, PreemptionThreshold 4) : created with the <i>TX_AUTO_START</i> flag to start automatically.

The **AppMainThread** starts and performs the following actions:

  + Starts the DHCP client
  + Waits for the IP address resolution
  + Resumes the *nx_iperf_entry*

The **nx_iperf_entry**, once started:

  + Creates a NetXDuo Iperf demo web page.

The application then creates 4 threads with the same priorities :

   + **thread_tcp_rx_iperf** (priority 1, PreemptionThreshold 1) : created with the <i>TX_AUTO_START</i> flag to start automatically.
   + **thread_tcp_tx_iperf** (priority 1, PreemptionThreshold 1) : created with the <i>TX_AUTO_START</i> flag to start automatically.
   + **thread_udp_rx_iperf** (priority 1, PreemptionThreshold 1) : created with the <i>TX_AUTO_START</i> flag to start automatically.
   + **thread_udp_tx_iperf** (priority 1, PreemptionThreshold 1) : created with the <i>TX_AUTO_START</i> flag to start automatically.

####  <b>Expected success behavior</b>

 + The board IP address is printed on the HyperTerminal
 + When the web HTTP server is successfully started, then the user can test the performance on the web browser after entering the url http://@IP.
 + To execute each Iperf test you must do the following steps and have the expected result in this link https://github.com/eclipse-threadx/rtos-docs/blob/ac6725870ff53d0926782a9ae246fded0eae0af3/rtos-docs/netx-duo/netx-duo-iperf/chapter3.md .

#### <b>Error behaviors</b>

+ The red LED is toggling to indicate any error that have occurred.

#### <b>Assumptions if any</b>

- The Application is using the DHCP to acquire IP address, thus a DHCP server should be reachable by the board in the network used to test the application.

#### <b>Known limitations</b>

  - The packet pool is not optimized. It can be less than that by reducing NX_PACKET_POOL_SIZE in file "app_netxduo.h" and NX_APP_MEM_POOL_SIZE in file "app_azure_rtos_config.h". This update can decrease NetXDuo performance.

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
   This require changes in the linker files to expose this memory location.
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
       } >RAM_D1 AT> RAM_D1
	```

       The simplest way to provide memory for ThreadX is to define a new section, see ._threadx_heap above.
       In the example above the ThreadX heap size is set to 64KBytes.
       The ._threadx_heap must be located between the .bss and the ._user_heap_stack sections in the linker script.
       Caution: Make sure that ThreadX does not need more than the provided heap memory (64KBytes in this example).
       Read more in STM32CubeIDE User Guide, chapter: "Linker script".

    + The "tx_initialize_low_level.S" should be also modified to enable the "USE_DYNAMIC_MEMORY_ALLOCATION" flag.

### <b>Keywords</b>

RTOS, Network, ThreadX, NetXDuo, WiFi, Station mode, Iperf, UART, MXCHIP, SPI, STMod+

### <b>Hardware and Software environment</b>

  - This application runs on STM32H7S7L8xx devices.
  - This application has been tested with STMicroelectronics STM32H7S78-DK boards revision: MB1736-H7S7L8-D01
    and can be easily tailored to any other supported device and development board.
  - This application uses UART4 to display logs, the hyperterminal configuration is as follows:
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - Stop Bit = 1
      - Parity = none
      - Flow control = None

### <b>How to use it ?</b>

To configure STM32CubeIDE Debug Configuration, you must do the following :

    1. Add the adequate external loader (MX66UW1G45G_STM32H7S78-DK.stldr file) in Project->Debugger Configuration
    2. Add in the startup the Boot_XIP.elf file in Project->Debugger Configuration
    3. Move up the application in the startup

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
 - Run the [iperf tool] by following steps described in this link https://github.com/eclipse-threadx/rtos-docs/blob/ac6725870ff53d0926782a9ae246fded0eae0af3/rtos-docs/netx-duo/netx-duo-iperf/chapter3.md .
