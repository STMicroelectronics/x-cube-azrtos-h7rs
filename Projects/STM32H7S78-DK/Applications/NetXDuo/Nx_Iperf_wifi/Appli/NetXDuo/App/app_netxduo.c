/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_netxduo.c
  * @author  MCD Application Team
  * @brief   NetXDuo applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_netxduo.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_netxduo.h"
#include "app_azure_rtos.h"
#include "msg.h"
#include "nxd_dhcp_client.h"
#include <stdbool.h>
#include <inttypes.h>
#include "mx_wifi_conf.h"
#include "io_pattern/mx_wifi_io.h"

#include "nx_packet.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NX_IPERF_HTTP_STACK_SIZE         2048
#define NX_IPERF_STACK_SIZE              2048
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

static TX_THREAD AppMainThread;
static CHAR AppMainThreadName[] = "App Main thread";

static TX_THREAD AppMain2Thread;
static CHAR AppMain2ThreadName[] = "App Main2 thread";

static __IO bool AppMain2ThreadRunning = true;

static TX_THREAD AppIperfThread;
static CHAR AppIperfThreadName[] = "App Iperf Thread";

TX_BYTE_POOL *AppBytePool = NULL;

static NX_PACKET_POOL AppPacketPool;
static CHAR AppPacketPoolName[] = "Main Packet Pool";

static NX_PACKET_POOL IperfPacketPool;
static CHAR IperfPacketPoolName[] = "Iperf Packet Pool";

static NX_IP IpInstance;
static CHAR IpInstanceName[] = "NetX IP Instance 0";

static NX_DHCP DhcpClient;
static CHAR DhcpClientName[] = "DHCP Client";

static TX_SEMAPHORE DhcpSemaphore;
static CHAR DhcpSemaphoreName[] = "DHCP Semaphore";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static VOID App_Main_Thread_Entry(ULONG thread_input);
static VOID App_Main2_Thread_Entry(ULONG thread_input);

static VOID App_Iperf_Thread_Entry(ULONG thread_input);

static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr);


void nx_iperf_entry(NX_PACKET_POOL *pool_ptr, NX_IP *ip_ptr,
                    UCHAR *http_stack, ULONG http_stack_size,
                    UCHAR *iperf_stack, ULONG iperf_stack_size);
/* USER CODE END PFP */

/**
  * @brief  Application NetXDuo Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_NetXDuo_Init(VOID *memory_ptr)
{
  UINT ret = NX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN MX_NetXDuo_MEM_POOL */
  AppBytePool = byte_pool;
  /* USER CODE END MX_NetXDuo_MEM_POOL */

  /* USER CODE BEGIN 0 */

  /* USER CODE END 0 */

  /* USER CODE BEGIN MX_NetXDuo_Init */
  setbuf(stdout, NULL);

  {
#if defined(MX_WIFI_NETWORK_BYPASS_MODE)
    const char wifi_stack_mode[] = "NetXDuo";
#else
    const char wifi_stack_mode[] = "Network on module";
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */

#if defined(MX_WIFI_USE_SPI_DMA)
    const char wifi_io[] = "SPI-DMA";
#elif defined(MX_WIFI_USE_SPI)
    const char wifi_io[] = "SPI";
#else
    const char wifi_io[] = "UART";
#endif /* MX_WIFI_USE_SPI */
    
  MSG_INFO("\nNx_Iperf_wifi application started.\n");
  
  MSG_INFO("# build: %s-%s, %s, %s %s\n",
             wifi_io,
             "RTOS",
             wifi_stack_mode,
             __TIME__, __DATE__);
  }

#if defined(STM32_THREAD_SAFE_STRATEGY)
  {
    char the_thread_safe_strategy[50] = {""};

    strcat(the_thread_safe_strategy, " STM32_THREAD_SAFE_STRATEGY ");
    sprintf(&the_thread_safe_strategy[strlen(the_thread_safe_strategy)], "(%d)", STM32_THREAD_SAFE_STRATEGY);
    MSG_INFO("\n[%" PRIu32 "] MX_NetXDuo_Init(): %s\n\n", HAL_GetTick(), the_thread_safe_strategy);
  }
#endif /* STM32_THREAD_SAFE_STRATEGY */
  
  /* Initialize the NetX system. */
  nx_system_initialize();

  /* Allocate the memory for packet_pool.  */
  {
    const ULONG pool_size = NX_PACKET_POOL_SIZE;
    VOID *pool_start;

    if (tx_byte_allocate(byte_pool, &pool_start, pool_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Create the Packet pool to be used for packet allocation */
    ret = nx_packet_pool_create(&AppPacketPool, AppPacketPoolName, PAYLOAD_SIZE, pool_start, pool_size);

    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_packet_pool_create() done\n");
  }

  /* Allocate the memory for Iperf packet_pool.  */
  {
    const ULONG pool_size = NX_PACKET_POOL_SIZE;
    VOID *pool_start;

    if (tx_byte_allocate(byte_pool, &pool_start, pool_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Create the Packet pool to be used for packet allocation */
    ret = nx_packet_pool_create(&IperfPacketPool, IperfPacketPoolName, PAYLOAD_SIZE, pool_start, pool_size);

    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_packet_pool_create() done\n");
  }

  /* Allocate the memory for Ip_Instance */
  {
    const ULONG stack_size = NETX_IP_THREAD_STACK_SIZE;
    VOID *stack_ptr;
    if (tx_byte_allocate(byte_pool, &stack_ptr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    MSG_DEBUG("[%06" PRIu32 "] Calling nx_ip_create()\n", HAL_GetTick());

    /* Create the main NX_IP instance. */
    ret = nx_ip_create(&IpInstance, IpInstanceName, 0, 0,
                       &AppPacketPool, nx_driver_emw3080_entry,
                       stack_ptr, stack_size, NETX_IP_THREAD_PRIORITY);
    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_ip_create() done\n");
  }

  {
    ret = nx_ip_address_change_notify(&IpInstance, ip_address_change_notify_callback, NULL);
    if (ret != NX_SUCCESS)
    {
      Error_Handler();
    }
    MSG_DEBUG("nx_ip_address_change_notify() done\n");
  }

  /* Create the DHCP client. */
  ret = nx_dhcp_create(&DhcpClient, &IpInstance, DhcpClientName);
  if (ret != NX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }
  MSG_DEBUG("nx_dhcp_create() done\n");

  /* Allocate the memory for ARP */
  {
    const ULONG arp_cache_size = ARP_MEMORY_SIZE;
    VOID *arp_cache_memory;

    if (tx_byte_allocate(byte_pool, &arp_cache_memory, arp_cache_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Enable the ARP protocol and provide the ARP cache size for the IP instance */
    ret = nx_arp_enable(&IpInstance, arp_cache_memory, arp_cache_size);

    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_arp_enable() done\n");
  }

  /* Enable the ICMP */
  {
    ret = nx_icmp_enable(&IpInstance);

    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_icmp_enable() done\n");
  }

  /* Enable the UDP protocol required for DNS/DHCP communication. */
  {
    ret = nx_udp_enable(&IpInstance);

    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_udp_enable() done\n");
  }

  /* Enable the TCP protocol. */
  {
    ret = nx_tcp_enable(&IpInstance);

    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_tcp_enable() done\n");
  }

  /* Allocate the memory for the main thread. */
  {
    const ULONG stack_size = MAIN_THREAD_STACK_SIZE;
    VOID *stack_ptr;

    if (tx_byte_allocate(byte_pool, &stack_ptr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Create the main thread */
    ret = tx_thread_create(&AppMainThread, AppMainThreadName, App_Main_Thread_Entry,
                           (ULONG)byte_pool, stack_ptr, stack_size,
                           MAIN_THREAD_PRIORITY, MAIN_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

    if (ret != TX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("tx_thread_create() done\n");
  }

  /* Allocate the memory for main2 thread. */
  {
    const ULONG stack_size = MAIN2_THREAD_STACK_SIZE;
    VOID *stack_ptr;

    if (tx_byte_allocate(byte_pool, &stack_ptr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Create the main2 thread. */
    ret = tx_thread_create(&AppMain2Thread, AppMain2ThreadName, App_Main2_Thread_Entry,
                           (ULONG)byte_pool, stack_ptr, stack_size,
                           MAIN2_THREAD_PRIORITY, MAIN2_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

    if (ret != TX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("tx_thread_create() done\n");
  }

  /* Allocate the memory for Iperf thread. */
  {
    const ULONG stack_size = APP_IPERF_THREAD_STACK_SIZE;
    VOID *stack_ptr;

    if (tx_byte_allocate(byte_pool, &stack_ptr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Create the Iperf thread. */
    ret = tx_thread_create(&AppIperfThread, AppIperfThreadName, App_Iperf_Thread_Entry,
                           (ULONG)byte_pool, stack_ptr, stack_size,
                           APP_IPERF_THREAD_PRIORITY, APP_IPERF_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_DONT_START);

    if (ret != TX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("tx_thread_create() done\n");
  }
  
  /* Set DHCP notification callback */
  tx_semaphore_create(&DhcpSemaphore, DhcpSemaphoreName, 0);
  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/* USER CODE BEGIN 1 */
/**
  * @brief ip address change callback.
  * @param ip_instance: NX_IP instance
  * @param ptr: user data
  * @retval none
  */
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr)
{
  NX_PARAMETER_NOT_USED(ip_instance);
  NX_PARAMETER_NOT_USED(ptr);

  MSG_DEBUG(">\"%s\"\n", ip_instance->nx_ip_name);

  /* Release the semaphore as soon as an IP address is available. */
  tx_semaphore_put(&DhcpSemaphore);
}


/**
  * @brief  Main thread entry.
  * @param thread_input: ULONG user argument used by the thread entry
  * @retval none
  */
static VOID App_Main_Thread_Entry(ULONG thread_input)
{
  UINT ret = NX_SUCCESS;
  ULONG dhcp_ip_address;
  UCHAR dhcp_ip_string[4];

  NX_PARAMETER_NOT_USED(thread_input);

  MSG_DEBUG(">\n");

  ret = nx_ip_address_change_notify(&IpInstance, ip_address_change_notify_callback, NULL);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }
  MSG_DEBUG("nx_ip_address_change_notify() done\n");

  /* Start the DHCP client. */
  ret = nx_dhcp_start(&DhcpClient);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }
  MSG_DEBUG("nx_dhcp_start() done\n");

  /* Wait until an IP address is ready. */
  if (tx_semaphore_get(&DhcpSemaphore, TX_WAIT_FOREVER) != TX_SUCCESS)
  {
    MSG_ERROR("ERROR: Cannot connect the WiFi interface!\n");
    Error_Handler();
  }

  /* Kill our terminator. */
  AppMain2ThreadRunning = false;


  {
    const ULONG status_check_mask = \
    NX_IP_INITIALIZE_DONE | NX_IP_ADDRESS_RESOLVED | NX_IP_LINK_ENABLED | \
    NX_IP_ARP_ENABLED | NX_IP_UDP_ENABLED | NX_IP_TCP_ENABLED | \
    /* NX_IP_IGMP_ENABLED | */ NX_IP_RARP_COMPLETE | NX_IP_INTERFACE_LINK_ENABLED;
    ULONG interface_status = 0;

    ret = nx_ip_interface_status_check(&IpInstance, 0U, status_check_mask, &interface_status, NX_WAIT_FOREVER);
    MSG_INFO("Interface status: %04" PRIX32 "\n", (uint32_t)interface_status);
  }

#ifdef ENABLE_IOT_INFO
  {
    const UINT IpIndex = 0;
    CHAR *interface_name = NULL;
    ULONG ip_address = 0;
    ULONG net_mask = 0;
    ULONG mtu_size = 0;
    ULONG physical_addres_msw = 0;
    ULONG physical_address_lsw = 0;

    _nx_ip_interface_info_get(&IpInstance, IpIndex, &interface_name,
                              &ip_address, &net_mask,
                              &mtu_size,
                              &physical_addres_msw, &physical_address_lsw);

    MSG_INFO("\nIP: \"%s\", MTU: %" PRIu32 "\n", interface_name, (uint32_t)mtu_size);
    MSG_DEBUG("0x%" PRIX32 "%" PRIX32 "\n", physical_addres_msw, physical_address_lsw);
  }
#endif /* ENABLE_IOT_INFO */

  /* Read back IP address and gateway address. */
  {
    ULONG ip_address = 0;
    ULONG net_mask = 0;
    ULONG gateway_address = 0;

    ret = nx_ip_address_get(&IpInstance, &ip_address, &net_mask);
    if (ret != TX_SUCCESS)
    {
      Error_Handler();
    }

    ret = nx_ip_gateway_address_get(&IpInstance, &gateway_address);
    if (ret != TX_SUCCESS)
    {
      Error_Handler();
    }

    MSG_INFO("\n- Network Interface connected: ");
    PRINT_IP_ADDRESS(ip_address);
    MSG_INFO("\n");

    MSG_INFO("Mask: ");
    PRINT_IP_ADDRESS(net_mask);

    MSG_INFO("Gateway: ");
    PRINT_IP_ADDRESS(gateway_address);
  }

  /* Check for Default Gateway IP from DHCP. */
  {
    UINT size = sizeof(dhcp_ip_string);
    ret = nx_dhcp_user_option_retrieve(&DhcpClient, NX_DHCP_OPTION_GATEWAYS,
                                       dhcp_ip_string, &size);
    if (ret)
    {
      Error_Handler();
    }
  }

  /* Get the gateway address of IP instance. */
  ret = nx_ip_gateway_address_get(&IpInstance, &dhcp_ip_address);

  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }
  /* Set the gateway address for IP instance. */
  ret = nx_ip_gateway_address_set(&IpInstance, dhcp_ip_address);
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* The network is correctly initialized, start the Iperf thread. */
  tx_thread_resume(&AppIperfThread);

  /* this thread is not needed any more, we relinquish it */
  tx_thread_relinquish();

  return;
}


/**
  * @brief  Main2 thread entry.
  * @param thread_input: ULONG user argument used by the thread entry
  * @retval none
  */
static VOID App_Main2_Thread_Entry(ULONG thread_input)
{
  const uint32_t ticks_for_1s = TX_TIMER_TICKS_PER_SECOND;
  uint32_t time_out = 0;
  NX_PARAMETER_NOT_USED(thread_input);

  MSG_DEBUG(">\n");

  AppMain2ThreadRunning = true;

  while (AppMain2ThreadRunning)
  {
    tx_thread_sleep(ticks_for_1s);
    time_out++;

    if (time_out == 20)
    {
      MSG_DEBUG("Could not get an IP address!\n");
      MSG_INFO("ERROR: Cannot connect WiFi interface!\n");
      Error_Handler();
    }
  }
  MSG_DEBUG("<\n");
}


/**
  * @brief Iperf thread entry.
  * @param thread_input: ULONG user argument used by the thread entry
  * @retval none
  */
static VOID App_Iperf_Thread_Entry(ULONG thread_input)
{
  TX_BYTE_POOL *const byte_pool = (TX_BYTE_POOL *) thread_input;

  MSG_DEBUG("[%06" PRIu32 "]>\n", HAL_GetTick());

  {
    MSG_INFO(" - Device Name    : %s.\n", wifi_obj_get()->SysInfo.Product_Name);
    MSG_INFO(" - Device ID      : %s.\n", wifi_obj_get()->SysInfo.Product_ID);
    MSG_INFO(" - Device Version : %s.\n", wifi_obj_get()->SysInfo.FW_Rev);
    MSG_INFO(" - MAC address    : %02X.%02X.%02X.%02X.%02X.%02X\n",
             wifi_obj_get()->SysInfo.MAC[0], wifi_obj_get()->SysInfo.MAC[1],
             wifi_obj_get()->SysInfo.MAC[2], wifi_obj_get()->SysInfo.MAC[3],
             wifi_obj_get()->SysInfo.MAC[4], wifi_obj_get()->SysInfo.MAC[5]);
  }


  /* The network is correctly initialized, start the Iperf utility. */


  /* Allocate the memory for HTTP and Iperf stack */
  {
    const ULONG http_stack_size = NX_IPERF_HTTP_STACK_SIZE;
    UCHAR *http_stack;
    const ULONG iperf_stack_size = NX_IPERF_STACK_SIZE;
    UCHAR *iperf_stack;

    if (tx_byte_allocate(byte_pool, (VOID **)&http_stack, http_stack_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      MSG_ERROR("Allocation failed!\n");
      Error_Handler();
    }

    if (tx_byte_allocate(byte_pool, (VOID **)&iperf_stack, iperf_stack_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      MSG_ERROR("Allocation failed!\n");
      Error_Handler();
    }

    MSG_INFO("\n##### Please open a browser window with the Target board's IP address\n\n");

    /* Application body. */
    nx_iperf_entry(&IperfPacketPool, &IpInstance, http_stack, http_stack_size, iperf_stack, iperf_stack_size);
  }
}
/* USER CODE END 1 */
