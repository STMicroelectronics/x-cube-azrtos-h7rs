
## <b>Nx_MQTT_Client_wifi Application Description</b>

This application provides an example of Azure RTOS NetX/NetXDuo stack usage.
It shows how to exchange data between client and server using MQTT protocol in an encrypted mode supporting TLS v1.2.

The main entry function tx_application_define() is called by ThreadX during kernel start, at this stage, all NetX resources are created.

 + A <i>NX_PACKET_POOL</i> is allocated
 + A <i>NX_IP</i> instance using that pool is initialized
 + The <i>ARP</i>, <i>ICMP</i>, <i>UDP</i> and <i>TCP</i> protocols are enabled for the <i>NX_IP</i> instance
 + A <i>DHCP client is created.</i>

The application then creates 3 threads with the different priorities:

 + **NxAppThread** (priority 10, PreemptionThreshold 10) : created with the <i>TX_AUTO_START</i> flag to start automatically.
 + **AppMQTTClientThread** (priority 3, PreemptionThreshold 3) : created with the <i>TX_DONT_START</i> flag to be started later.
 + **AppSNTPThread** (priority 5, PreemptionThreshold 5) : created with the <i>TX_DONT_START</i> flag to be started later.

The **NxAppThread** starts and performs the following actions:

  + Creates a dns_client with USER_DNS_ADDRESS (default value defined IP_ADDRESS(1, 1, 1, 1)) in app_netxduo.h.
  + Starts the DHCP client
  + Waits for the IP address resolution
  + Resumes the **AppSNTPThread**

The **AppSNTPThread**, once started:

  + It connects to an SNTP server to get the current time.
  + Resumes the **AppMQTTClientThread**

The **AppMQTTClientThread**, once started:

  + Creates an mqtt_client
  + Connects mqtt_client to the online MQTT broker; connection with server will be secure and a **tls_setup_callback** will set TLS parameters.
    By default MQTT_PORT for encrypted mode is 8883.

          refer to note 4 below, to know how to setup an x509 certificate.

  + mqtt_client subscribes on a predefined topic TOPIC_NAME "Temperature" with a Quality Of Service QOS0.
  + Depending on the number of message "NB_MESSAGE" defined by user, mqtt_client will publish a random number generated by RNG. If NB_MESSAGE = 0, it means that number of messages is infinite.
  + mqtt_client will get messages from the MQTT broker and print them.

#### <b> Expected success behavior</b>

 + The board IP address is printed on the HyperTerminal
 + Connection's information are printed on the HyperTerminal (broker's name, mqtt port, topic and messages received)

 ```
MQTT client connected to broker < test.mosquitto.org > at PORT 8883 :
Message 1 received: TOPIC = Temperature, MESSAGE = 34
Message 2 received: TOPIC = Temperature, MESSAGE = 35
Message 3 received: TOPIC = Temperature, MESSAGE = 35
Message 4 received: TOPIC = Temperature, MESSAGE = 34
Message 5 received: TOPIC = Temperature, MESSAGE = 33
client disconnected from broker < test.mosquitto.org >.
 ```
 + Green LED is toggling after successfully receiving all messages.

#### <b> Error behaviors</b>

+ The red LED is toggling to indicate any error that has occurred.

#### <b> Assumptions if any</b>

- The application is using the DHCP to acquire IP address, thus a DHCP server should be reachable by the board in the network used to test the application.

#### <b>Known limitations</b>

* Default NX_DNS_MAX_RETRIES value in "nx_user.h" may not work with some MQTT servers, they should be tuned. For example:

    #define NX_DNS_MAX_RETRIES  10

* Since NetXDuo does not support proxy, mqtt_client should be connected directly to the server.

### <b>Notes</b>

 1.  This application runs from the external flash memory. It is launched from a first boot stage and inherits from this boot project configuration (caches, MPU regions [region 0 and 1], system clock at 600 MHz and external memory interface at the highest speed). 
      Note that the boot part is automatically downloaded from the IDE environment via the board boot binary under Binary/Boot_XIP.hex file.

 2.  It is recommended to enable the cache and maintain its coherence:
      - Depending on the use case it is also possible to configure the cache attributes using the MPU.
      - Please refer to the **AN4838** "Managing memory protection unit (MPU) in STM32 MCUs".
      - Please refer to the **AN4839** "Level 1 cache on STM32F7 Series and STM32H7 Series"

 3. To make an encrypted connection with MQTT server, user should follow these steps to add an x509 certificate to the _mqtt\_client_ and use it to ensure server's authentication :
      - download certificate authority CA (in this application "mosquitto.org.der" downloaded from [test.mosquitto](https://test.mosquitto.org))
      - convert certificate downloaded by executing the following cmd from the file downloaded path :

                xxd.exe -i mosquitto.org.der > mosquitto.cert.h

      - add the converted file under the application : NetXDuo/Nx_MQTT_Client/NetXDuo/App
      - configure MOSQUITTO_CERT_FILE with your certificate name.

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
       } >RAM_D1 AT> RAM_D1
	```

       The simplest way to provide memory for ThreadX is to define a new section, see ._threadx_heap above.
       In the example above the ThreadX heap size is set to 64KBytes.
       The ._threadx_heap must be located between the .bss and the ._user_heap_stack sections in the linker script.
       Caution: Make sure that ThreadX does not need more than the provided heap memory (64KBytes in this example).
       Read more in STM32CubeIDE User Guide, chapter: "Linker script".

    + The "tx_initialize_low_level.S" should be also modified to enable the "USE_DYNAMIC_MEMORY_ALLOCATION" flag.

### <b>Keywords</b>

RTOS, Network, ThreadX, NetXDuo, WIFI, MQTT, DNS, TLS, MXCHIP, UART

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

###  <b>How to use it ?</b>

 To configure STM32CubeIDE Debug Configuration, you must do the following :
    1. Add the adequate external loader (MX66UW1G45G_STM32H7S78-DK.stldr file) in Project->Debugger Configuration
    2. Add in the startup the Boot_XIP.elf file in Project->Debugger Configuration
    3. Move up the application in the startup

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Edit the file app_netxduo.h : define the USER_DNS_ADDRESS (if necessary), the MQTT_BROKER_NAME and NB_MESSAGE.
 - Rebuild all files and load your image into target memory
 - Run the application
