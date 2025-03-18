
# <b>Ux_Device_HID Application Description</b>

This application provides an example of Azure RTOS USBX stack usage on NUCLEO-H7S3L8 board,
it shows how to develop USB Device Human Interface "HID" mouse based application.

The application is designed to emulate a USB HID mouse device, the code provides all required device descriptors framework
and associated class descriptor report to build a compliant USB HID mouse device.

At the beginning ThreadX calls the entry function tx_application_define(), at this stage, all USBx resources
are initialized, the HID class driver is registered and the application creates 2 threads with the same priorities :

  - app_ux_device_thread_entry (Prio : 10; PreemptionPrio : 10) used to initialize USB_OTG HAL PCD driver and start the device.
  - usbx_hid_thread_entry (Prio : 20; PreemptionPrio : 20) used to send HID reports to move automatically the PC host machine cursor.

The thread usbx_app_thread_entry is responsible to start or stop the USB device.
At Run mode the thread will be waiting on message queue form USB_PD interface, when the USB device is plugged to host PC
a callback in USB_PD interface will send a message to usbx_app_thread_entry to start the USB device.
By the same way when the USB device is unplugged, a callback in USB_PD interface will send a message to usbx_app_thread_entry to stop the USB device.

To customize the HID application by sending the mouse position step by step every 10ms:
  - For each 10ms, the application calls the GetPointerData() API to update the mouse position (x, y) and send
    the report buffer through the ux_device_class_hid_event_set() API.

#### <b>Expected success behavior</b>

When plugged to PC host, the NUCLEO-H7S3L8 must be properly enumerated as a USB HID mouse device.
During the enumeration phase, device provides host with the requested descriptors (device, configuration, string).
Those descriptors are used by host driver to identify the device capabilities.
Once the NUCLEO-H7S3L8 USB device successfully completed the enumeration phase, the device sends a HID report after a user button press.
Each report sent should move the mouse cursor by one step on host side.

#### <b>Error behaviors</b>

Host PC shows that USB device does not operate as designed (mouse enumeration failed, the mouse pointer doesn't move).

#### <b>Assumptions if any</b>

User is familiar with USB 2.0 "Universal Serial BUS" specification and HID class specification.

#### <b>Known limitations</b>

The remote wakeup feature is not yet implemented (used to bring the USB suspended bus back to the active condition).

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

### <b>Keywords</b>

RTOS, ThreadX, USBXDevice, USB_OTG, Full Speed, HID, Mouse, USBPD.

### <b>Hardware and Software environment</b>

  - This application runs on STM32H7S3L8xx devices.
  - This application has been tested with STMicroelectronics NUCLEO-H7S3L8 boards revision MB1737-H7S3L8-B02
    and can be easily tailored to any other supported device and development board.
  - NUCLEO-H7S3L8 board set-up:
    - Connect the NUCLEO-H7S3L8 board CN1 to the PC through "TYPE-C" to "Standard A" cable.
    - Press the Tamper push-button to move the cursor.

  - Note User shall remove the jumpers JP3 and JP4.

### <b>How to use it ?</b>

To configure STM32CubeIDE Debug Configuration, you must do the following :

    1. Add the adequate external loader (MX25UW25645G_NUCLEO-H7S3L8.stldr file) in Project->Debugger Configuration
    2. Add in the startup the Boot_XIP.elf file in Project->Debugger Configuration and uncheck the "Load Symbols" option
    3. Move up the application in the startup

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application