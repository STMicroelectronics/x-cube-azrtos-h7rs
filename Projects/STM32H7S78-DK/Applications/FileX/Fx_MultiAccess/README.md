
## <b>Fx_MultiAccess Application Description</b>

This application provides an example of Azure RTOS FileX stack usage on STM32H7S78-DK board, it demonstrates the FileX's concurrent file access capabilities. The application is designed to execute file operations on the SD card device, the code provides all required software code for handling SD card I/O operations.

The application starts by calling the ThreadX's initialization routine which creates multithreaded environment for starting the concurrent FileX's two main threads that handle file operations. At this stage, all FileX resources are created, the SDIO driver is initialized and the application creates 3 threads with the same priorities:

  - fx_app_thread  (Prio : 10; PreemptionPrio : 10) used to initialize the SD card driver and opening it as a FileX Media.
  - fx_thread_one  (Prio : 10; PreemptionPrio : 10) used to create, write and read operations for file fx_file_one.
  - fx_thread_two  (Prio : 10; PreemptionPrio : 10) used to create, write and read operations for file fx_file_two.

### <b>Notes</b>
A FAT32 compatible SD card is expected to be used with this example. The program will start file operations without formatting the media, so all user related files are kept.

This application runs from the external flash memory. It is launched from a first boot stage and inherits from this boot project configuration (caches, MPU regions [region 0 and 1], system clock at 600 MHz and external memory interface at the highest speed).
Note that the boot part is automatically downloaded from the IDE environment via the board boot binary under Binary/Boot_XIP.hex file.

#### <b>Expected success behavior</b>

- When an SD card is inserted into the STM32H7S78-DK SD card reader and the board is powered up, the application starts file operations.
- During the start phase, the main thread starts by opening the SD card using FileX media open service. Upon successful opening of the media and its underlying FAT32 file
  system, the main thread starts the two file operations threads, fx_thread_one and fx_thread_two. The two threads then start file operations, each at a different file.
- This demonstrates the concurrent access handling capabilities of FileX, without the need for an external access-aribitration locks.
- Successful operation is marked by a toggling LED_GREEN light.

#### <b>Error behaviors</b>

- On failure, the LED_RED starts toggling while the LED_GREEN is switched OFF.
- Error handler is called at the spot where the error occurred.

#### <b>Assumptions if any</b>

Hotplug is not implemented for this application, the SD card is expected to be inserted before application start.

#### <b>Known limitations</b>

None

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it, by updating the "TX_TIMER_TICKS_PER_SECOND" define in the "tx_user.h" file. The update should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related call (HAL) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL).
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
- FileX is using data buffers, passed as arguments to fx_media_open(), fx_media_read() and fx_media_write() API it is recommended that these buffers are multiple of sector size and "32 bytes" aligned to avoid cache maintenance issues.

### <b>Keywords</b>

RTOS, ThreadX, FileX, File system, SDMMC, SDIO, FAT32

### <b>Hardware and Software environment</b>

  - This application runs on STM32H7S7L8xx devices.
  - This application has been tested with STMicroelectronics STM32H7S78-DK boards revision: MB1736-H7S7L8-D01
    and can be easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

To configure STM32CubeIDE Debug Configuration, you must do the following :

    1. Add the adequate external loader (MX66UW1G45G_STM32H7S78-DK.stldr file) in Project->Debugger Configuration
    2. Add in the startup the Boot_XIP.elf file in Project->Debugger Configuration and uncheck the "Load Symbols" option
    3. Move up the application in the startup

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
