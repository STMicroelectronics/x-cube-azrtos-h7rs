/**
  ******************************************************************************
  * @file    stm32h7s78_discovery_audio.c
  * @author  MCD Application Team
  * @brief   This file provides the Audio driver for the STM32H7S78_DK
  *          discovery board.
  @verbatim
  How To use this driver:
  -----------------------
   + This driver supports STM32H7RSxx devices on STM32H7S78_DK (MB1736) board.
   + Call the function BSP_AUDIO_OUT_Init() for AUDIO OUT initialization:
        Instance : Select the output instance. Can only be 0 (I2S).
        AudioInit: Audio Out structure to select the following parameters.
                   - Device: Select the output device (headphone, speaker, ..).
                   - SampleRate: Select the output sample rate (8Khz .. 96Khz).
                   - BitsPerSample: Select the output resolution (16 or 32bits per sample).
                   - ChannelsNbr: Select the output channels number(1 for mono, 2 for stereo).
                   - Volume: Select the output volume(0% .. 100%).

      This function configures all the hardware required for the audio application (codec, I2C, I2S,
      GPIOs, DMA and interrupt if needed). This function returns BSP_ERROR_NONE if configuration is OK.
      If the returned value is different from BSP_ERROR_NONE or the function is stuck then the communication with
      the codec has failed (try to un-plug the power or reset device in this case).

      User can update the I2S or the clock configurations by overriding the weak MX functions MX_I2S6_Init()
      and MX_I2S6_ClockConfig().
      User can override the default MSP configuration and register his own MSP callbacks (defined at application level)
      by calling BSP_AUDIO_OUT_RegisterMspCallbacks() function.
      User can restore the default MSP configuration by calling BSP_AUDIO_OUT_RegisterDefaultMspCallbacks().
      To use these two functions, user has to enable USE_HAL_I2S_REGISTER_CALLBACKS within stm32h7rsxx_hal_conf.h file.

   + Call the function BSP_AUDIO_OUT_Play() to play audio stream:
        Instance : Select the output instance. Can only be 0 (I2S).
        pBuf: pointer to the audio data file address.
        NbrOfBytes: Total size of the buffer to be sent in Bytes.

   + Call the function BSP_AUDIO_OUT_Pause() to pause playing.
   + Call the function BSP_AUDIO_OUT_Resume() to resume playing.
       Note. After calling BSP_AUDIO_OUT_Pause() function for pause, only BSP_AUDIO_OUT_Resume() should be called
          for resume (it is not allowed to call BSP_AUDIO_OUT_Play() in this case).
       Note. This function should be called only when the audio file is played or paused (not stopped).
   + Call the function BSP_AUDIO_OUT_Stop() to stop playing.
   + Call the function BSP_AUDIO_OUT_Mute() to mute the player.
   + Call the function BSP_AUDIO_OUT_UnMute() to unmute the player.
   + Call the function BSP_AUDIO_OUT_IsMute() to get the mute state(BSP_AUDIO_MUTE_ENABLED or BSP_AUDIO_MUTE_DISABLED).
   + Call the function BSP_AUDIO_OUT_SetDevice() to update the AUDIO OUT device.
   + Call the function BSP_AUDIO_OUT_GetDevice() to get the AUDIO OUT device.
   + Call the function BSP_AUDIO_OUT_SetSampleRate() to update the AUDIO OUT sample rate.
   + Call the function BSP_AUDIO_OUT_GetSampleRate() to get the AUDIO OUT sample rate.
   + Call the function BSP_AUDIO_OUT_SetBitsPerSample() to update the AUDIO OUT resolution.
   + Call the function BSP_AUDIO_OUT_GetBitPerSample() to get the AUDIO OUT resolution.
   + Call the function BSP_AUDIO_OUT_SetChannelsNbr() to update the AUDIO OUT number of channels.
   + Call the function BSP_AUDIO_OUT_GetChannelsNbr() to get the AUDIO OUT number of channels.
   + Call the function BSP_AUDIO_OUT_SetVolume() to update the AUDIO OUT volume.
   + Call the function BSP_AUDIO_OUT_GetVolume() to get the AUDIO OUT volume.
   + Call the function BSP_AUDIO_OUT_GetState() to get the AUDIO OUT state.

   + BSP_AUDIO_OUT_SetDevice(), BSP_AUDIO_OUT_SetSampleRate(), BSP_AUDIO_OUT_SetBitsPerSample() and
     BSP_AUDIO_OUT_SetChannelsNbr() cannot be called while the state is AUDIO_OUT_STATE_PLAYING.
   + For each mode, you may need to implement the relative callback functions into your code.
      The Callback functions are named AUDIO_OUT_XXX_CallBack() and only their prototypes are declared in
      the stm32h7s78_discovery_audio.h file. (refer to the example for more details on the callbacks implementations).


   + Call the function BSP_AUDIO_IN_Init() for AUDIO IN initialization:
        Instance : Select the input instance. Can be 0 (I2S) or 1 (MDF).
        AudioInit: Audio In structure to select the following parameters.
                   - Device: Select the input device (analog, digital mic1, mic2, mic1 & mic2).
                   - SampleRate: Select the input sample rate (8Khz .. 96Khz).
                   - BitsPerSample: Select the input resolution (16 or 32bits per sample).
                   - ChannelsNbr: Select the input channels number(1 for mono, 2 for stereo).
                   - Volume: Select the input volume(0% .. 100%).

      This function configures all the hardware required for the audio application (codec, I2C, I2S, MDF
      GPIOs, DMA and interrupt if needed). This function returns BSP_ERROR_NONE if configuration is OK.
      If the returned value is different from BSP_ERROR_NONE or the function is stuck then the communication with
      the codec has failed (try to un-plug the power or reset device in this case).

      User can update the MDF/I2S or the clock configurations by overriding the weak MX functions MX_I2S6_Init(),
      MX_I2S6_ClockConfig(), MX_ADF1_Init() and MX_ADF1_ClockConfig()
      User can override the default MSP configuration and register his own MSP callbacks (defined at application level)
      by calling BSP_AUDIO_IN_RegisterMspCallbacks() function.
      User can restore the default MSP configuration by calling BSP_AUDIO_IN_RegisterDefaultMspCallbacks().
      To use these two functions, user have to enable USE_HAL_I2S_REGISTER_CALLBACKS and/or USE_HAL_MDF_REGISTER_CALLBACKS
      within stm32h7rsxx_hal_conf.h file.

   + Call the function BSP_AUDIO_IN_Record() to record audio stream. The recorded data are stored to user buffer in raw
        (L, R, L, R ...).
        Instance : Select the input instance. Can be 0 (I2S) or 1 (MDF).
        pBuf: pointer to user buffer.
        NbrOfBytes: Total size of the buffer to be sent in Bytes.

   + Call the function BSP_AUDIO_IN_Pause() to pause recording.
   + Call the function BSP_AUDIO_IN_Resume() to resume recording.
   + Call the function BSP_AUDIO_IN_Stop() to stop recording.
   + Call the function BSP_AUDIO_IN_SetDevice() to update the AUDIO IN device.
   + Call the function BSP_AUDIO_IN_GetDevice() to get the AUDIO IN device.
   + Call the function BSP_AUDIO_IN_SetSampleRate() to update the AUDIO IN sample rate.
   + Call the function BSP_AUDIO_IN_GetSampleRate() to get the AUDIO IN sample rate.
   + Call the function BSP_AUDIO_IN_SetBitPerSample() to update the AUDIO IN resolution.
   + Call the function BSP_AUDIO_IN_GetBitPerSample() to get the AUDIO IN resolution.
   + Call the function BSP_AUDIO_IN_SetChannelsNbr() to update the AUDIO IN number of channels.
   + Call the function BSP_AUDIO_IN_GetChannelsNbr() to get the AUDIO IN number of channels.
   + Call the function BSP_AUDIO_IN_SetVolume() to update the AUDIO IN volume.
   + Call the function BSP_AUDIO_IN_GetVolume() to get the AUDIO IN volume.
   + Call the function BSP_AUDIO_IN_GetState() to get the AUDIO IN state.

   + For each mode, you may need to implement the relative callback functions into your code.
      The Callback functions are named AUDIO_IN_XXX_CallBack() and only their prototypes are declared in
      the stm32h7s78_discovery_audio.h file (refer to the example for more details on the callbacks implementations).

   + The driver API and the callback functions are at the end of the stm32h7s78_discovery_audio.h file.

  Known Limitations:
  ------------------
   1- If the TDM Format used to play in parallel 2 audio Stream (the first Stream is configured in codec SLOT0 and second
      Stream in SLOT1) the Pause/Resume, volume and mute feature will control the both streams.
   2- Parsing of audio file is not implemented (in order to determine audio file properties: Mono/Stereo, Data size,
      File size, Audio Frequency, Audio Data header size ...). The configuration is fixed for the given audio file.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32h7s78_discovery_audio.h"
#include "stm32h7s78_discovery_bus.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H7S78_DK
  * @{
  */

/** @defgroup STM32H7S78_DK_AUDIO AUDIO
  * @{
  */

/** @defgroup STM32H7S78_DK_AUDIO_Private_Macros STM32H7S78_DK AUDIO Private Macros
  * @{
  */
#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

#define MDF_DECIMATION_RATIO(__FREQUENCY__) \
  ((__FREQUENCY__) == (AUDIO_FREQUENCY_8K))   ? (64U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_11K))  ? (64U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_16K))  ? (32U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_22K))  ? (32U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_32K))  ? (16U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_44K))  ? (16U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_48K))  ? (16U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_88K))  ? (8U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_96K))  ? (8U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_176K)) ? (4U) : (4U)

#define MDF_GAIN(__FREQUENCY__) \
  ((__FREQUENCY__) == (AUDIO_FREQUENCY_8K))   ? (-4)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_11K))  ? (-6) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_16K))  ? (2) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_22K))  ? (2) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_32K))  ? (10)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_44K))  ? (10)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_48K))  ? (10)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_88K))  ? (18)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_96K))  ? (18)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_176K)) ? (24) : (24)

#define MDF_CIC_MODE(__FREQUENCY__) \
  ((__FREQUENCY__) == (AUDIO_FREQUENCY_8K))   ? (MDF_ONE_FILTER_SINC4) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_11K))  ? (MDF_ONE_FILTER_SINC4) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_16K))  ? (MDF_ONE_FILTER_SINC4) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_22K))  ? (MDF_ONE_FILTER_SINC4) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_32K))  ? (MDF_ONE_FILTER_SINC4) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_44K))  ? (MDF_ONE_FILTER_SINC4) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_48K))  ? (MDF_ONE_FILTER_SINC4) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_88K))  ? (MDF_ONE_FILTER_SINC4) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_96K))  ? (MDF_ONE_FILTER_SINC4) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_176K)) ? (MDF_ONE_FILTER_SINC4) : (MDF_ONE_FILTER_SINC4)

#define MDF_PROC_CLOCK_DIVIDER(__FREQUENCY__) \
  ((__FREQUENCY__) == (AUDIO_FREQUENCY_8K))   ? (2U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_11K))  ? (1U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_16K))  ? (2U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_22K))  ? (1U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_32K))  ? (2U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_44K))  ? (1U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_48K))  ? (2U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_88K))  ? (1U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_96K))  ? (2U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_176K)) ? (1U) : (2U)

#define MDF_OUTPUT_CLOCK_DIVIDER(__FREQUENCY__) \
  ((__FREQUENCY__) == (AUDIO_FREQUENCY_8K))   ? (12U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_11K))  ? (4U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_16K))  ? (12U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_22K))  ? (4U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_32K))  ? (12U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_44K))  ? (4U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_48K))  ? (8U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_88K))  ? (16U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_96K))  ? (16U)  \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_176K)) ? (16U) : (16U)
/**
  * @}
  */

/** @defgroup STM32H7S78_DK_AUDIO_Exported_Variables STM32H7S78_DK AUDIO Exported Variables
  * @{
  */
/* Audio in and out context */
AUDIO_OUT_Ctx_t Audio_Out_Ctx[AUDIO_OUT_INSTANCES_NBR] = {{
    AUDIO_OUT_HEADPHONE,
    AUDIO_FREQUENCY_8K,
    AUDIO_RESOLUTION_16B,
    50U,
    2U,
    AUDIO_MUTE_DISABLED,
    AUDIO_OUT_STATE_RESET
  }
};

AUDIO_IN_Ctx_t  Audio_In_Ctx[AUDIO_IN_INSTANCES_NBR] = {{
    AUDIO_IN_ANALOG_MIC,
    AUDIO_FREQUENCY_8K,
    AUDIO_RESOLUTION_16B,
    2U,
    NULL,
    0U,
    50U,
    AUDIO_IN_STATE_RESET
  },
  {
    AUDIO_IN_DIGITAL_MIC,
    AUDIO_FREQUENCY_8K,
    AUDIO_RESOLUTION_16B,
    2U,
    NULL,
    0U,
    50U,
    AUDIO_IN_STATE_RESET
  }
};

/* Audio component object */
void *Audio_CompObj = NULL;

/* Audio driver */
AUDIO_Drv_t *Audio_Drv = NULL;

/* Audio in and out I2S handles */
I2S_HandleTypeDef haudio_out_i2s = {0};
I2S_HandleTypeDef haudio_in_i2s  = {0};

/* Audio in ADF handles */
MDF_HandleTypeDef haudio_in_mdf = {0};

/* Audio ADF filter configuration */
static MDF_FilterConfigTypeDef Audio_AdfFilterConfig;

/**
  * @}
  */

/** @defgroup STM32H7S78_DK_AUDIO_Private_Variables STM32H7S78_DK AUDIO Private Variables
  * @{
  */
/* Audio in and out DMA handles used by I2S */
static DMA_HandleTypeDef hDmaI2sTx, hDmaI2sRx;

/* Audio in DMA handle used by MDF */
static DMA_HandleTypeDef hDmaMdf;

#if ((USE_HAL_MDF_REGISTER_CALLBACKS == 1) || (USE_HAL_I2S_REGISTER_CALLBACKS == 1))
static uint32_t AudioOut_IsMspCbValid[AUDIO_OUT_INSTANCES_NBR] = {0};
static uint32_t AudioIn_IsMspCbValid[AUDIO_IN_INSTANCES_NBR] = {0, 0};
#endif

/* Queue variables declaration */
static DMA_QListTypeDef I2sTxQueue, I2sRxQueue, MdfRxQueue;

/* Audio in MDF internal buffer */
static int32_t Audio_DigMicRecBuff[DEFAULT_AUDIO_IN_BUFFER_SIZE];

/**
  * @}
  */

/** @defgroup STM32H7S78_DK_AUDIO_Private_Function_Prototypes STM32H7S78_DK AUDIO Private Function Prototypes
  * @{
  */
static int32_t WM8904_Probe(void);
static void    I2S_MspInit(I2S_HandleTypeDef *hi2s);
static void    I2S_MspDeInit(I2S_HandleTypeDef *hi2s);
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1)
static void    I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s);
static void    I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
static void    I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s);
static void    I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
static void    I2S_ErrorCallback(I2S_HandleTypeDef *hi2s);
#endif /* (USE_HAL_I2S_REGISTER_CALLBACKS == 1) */

static void    MDF_MspInit(MDF_HandleTypeDef *hmdf);
static void    MDF_MspDeInit(MDF_HandleTypeDef *hmdf);
#if (USE_HAL_MDF_REGISTER_CALLBACKS == 1)
static void    MDF_AcqCpltCallback(MDF_HandleTypeDef *hmdf);
static void    MDF_AcqHalfCpltCallback(MDF_HandleTypeDef *hmdf);
static void    MDF_ErrorCallback(MDF_HandleTypeDef *hmdf);
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) */
/**
  * @}
  */

/** @addtogroup STM32H7S78_DK_AUDIO_OUT_Exported_Functions
  * @{
  */
/**
  * @brief  Initialize the audio out peripherals.
  * @param  Instance Audio out instance.
  * @param  AudioInit Audio out init structure.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Init(uint32_t Instance, BSP_AUDIO_Init_t *AudioInit)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && ((AudioInit->BitsPerSample == AUDIO_RESOLUTION_32B)
                                || (AudioInit->BitsPerSample == AUDIO_RESOLUTION_8B)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 0U) && (AudioInit->ChannelsNbr != 2U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 0U) && (Audio_In_Ctx[0].State != AUDIO_IN_STATE_RESET))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Fill audio out context structure */
    Audio_Out_Ctx[Instance].Device         = AudioInit->Device;
    Audio_Out_Ctx[Instance].SampleRate     = AudioInit->SampleRate;
    Audio_Out_Ctx[Instance].BitsPerSample  = AudioInit->BitsPerSample;
    Audio_Out_Ctx[Instance].ChannelsNbr    = AudioInit->ChannelsNbr;
    Audio_Out_Ctx[Instance].Volume         = AudioInit->Volume;

    /* Probe the audio codec */
    if (WM8904_Probe() != BSP_ERROR_NONE)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Set I2S instance and state */
      haudio_out_i2s.Instance = SPI6;
      Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_INIT;

      /* Configure the I2S PLL according to the requested audio frequency */
      if (MX_I2S6_ClockConfig(&haudio_out_i2s, AudioInit->SampleRate) != HAL_OK)
      {
        status = BSP_ERROR_CLOCK_FAILURE;
      }
      else
      {
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 0)
        I2S_MspInit(&haudio_out_i2s);
#else
        /* Register the I2S MSP Callbacks */
        if (AudioOut_IsMspCbValid[Instance] == 0U)
        {
          if (BSP_AUDIO_OUT_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
        }
#endif /* (USE_HAL_I2S_REGISTER_CALLBACKS == 0) */
      }

      if (status == BSP_ERROR_NONE)
      {
        /* Prepare I2S peripheral initialization */
        MX_I2S_Config_t mxI2sInit;
        mxI2sInit.AudioFrequency    = AudioInit->SampleRate;
        mxI2sInit.Mode              = I2S_MODE_MASTER_TX;
        mxI2sInit.DataFormat        = (AudioInit->BitsPerSample == AUDIO_RESOLUTION_16B) ? I2S_DATAFORMAT_16B :
                                      I2S_DATAFORMAT_24B;

        /* I2S peripheral initialization */
        if (MX_I2S6_Init(&haudio_out_i2s, &mxI2sInit) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1)
        /* Register I2S TC, HT and Error callbacks */
        else if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_TX_COMPLETE_CB_ID, I2S_TxCpltCallback) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
        else if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_TX_HALF_COMPLETE_CB_ID, I2S_TxHalfCpltCallback) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
        else if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_ERROR_CB_ID, I2S_ErrorCallback) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
#endif /* (USE_HAL_I2S_REGISTER_CALLBACKS == 1) */
        else
        {
          /* Initialize audio codec */
          WM8904_Init_t codec_init;
          codec_init.InputDevice  = WM8904_IN_NONE;
          codec_init.OutputDevice = WM8904_OUT_HEADPHONE;
          codec_init.Resolution   = WM8904_RESOLUTION_16B;
          codec_init.Frequency    = AudioInit->SampleRate;
          codec_init.Volume       = AudioInit->Volume;
          if (Audio_Drv->Init(Audio_CompObj, &codec_init) < 0)
          {
            status = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            /* Update audio out context state */
            Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_STOP;
          }
        }
      }
    }
  }
  return status;
}

/**
  * @brief  De-initialize the audio out peripherals.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_DeInit(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_RESET)
  {
    /* I2S peripheral de-initialization */
    if (HAL_I2S_DeInit(&haudio_out_i2s) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    /* De-initialize audio codec */
    else
    {
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 0)
      I2S_MspDeInit(&haudio_out_i2s);
#endif /* (USE_HAL_I2S_REGISTER_CALLBACKS == 0) */
      if (Audio_Drv->DeInit(Audio_CompObj) < 0)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }

    if (status == BSP_ERROR_NONE)
    {
      /* Update audio out context */
      Audio_Out_Ctx[Instance].State  = AUDIO_OUT_STATE_RESET;
      Audio_Out_Ctx[Instance].IsMute = 0U;
    }
  }
  else
  {
    /* Nothing to do */
  }
  return status;
}

/**
  * @brief  Start playing audio stream from a data buffer for a determined size.
  * @param  Instance Audio out instance.
  * @param  pData Pointer on data buffer.
  * @param  NbrOfBytes Size of buffer in bytes. Maximum size is 65535 bytes.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Play(uint32_t Instance, uint8_t *pData, uint32_t NbrOfBytes)
{
  int32_t  status = BSP_ERROR_NONE;
  uint16_t NbrOfDmaDatas;

  if ((Instance >= AUDIO_OUT_INSTANCES_NBR) || (pData == NULL) || (NbrOfBytes > 65535U))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Compute number of DMA data to transfer according resolution */
    if (Audio_Out_Ctx[Instance].BitsPerSample == AUDIO_RESOLUTION_16B)
    {
      NbrOfDmaDatas = (uint16_t)(NbrOfBytes / 2U);
    }
    else /* AUDIO_RESOLUTION_24b */
    {
      NbrOfDmaDatas = (uint16_t)(NbrOfBytes / 4U);
    }

    /* Initiate a DMA transfer of audio samples towards the serial audio interface */
    /* Disable I2S to be able to configure TxDMAEN */
    __HAL_I2S_DISABLE(&haudio_out_i2s);
    if (HAL_I2S_Transmit_DMA(&haudio_out_i2s, (uint16_t *) pData, NbrOfDmaDatas) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    /* Call the audio codec play function */
    else if (Audio_Drv->Play(Audio_CompObj) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Update audio out state */
      Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_PLAYING;
    }
  }
  return status;
}

/**
  * @brief  Pause playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Pause(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_PLAYING)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Call the audio codec pause function */
  else if (Audio_Drv->Pause(Audio_CompObj) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Pause DMA transfer of audio samples towards the serial audio interface */
  else if (HAL_I2S_DMAPause(&haudio_out_i2s) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    /* Update audio out state */
    Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_PAUSE;
  }
  return status;
}

/**
  * @brief  Resume playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Resume(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_PAUSE)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Resume DMA transfer of audio samples towards the serial audio interface */
  else if (HAL_I2S_DMAResume(&haudio_out_i2s) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  /* Call the audio codec resume function */
  else if (Audio_Drv->Resume(Audio_CompObj) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Update audio out state */
    Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_PLAYING;
  }
  return status;
}

/**
  * @brief  Stop playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Stop(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_STOP)
  {
    /* Nothing to do */
  }
  else if ((Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_PLAYING) &&
           (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_PAUSE))
  {
    status = BSP_ERROR_BUSY;
  }
  /* Call the audio codec stop function */
  else if (Audio_Drv->Stop(Audio_CompObj, WM8904_PDWN_SW) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Stop DMA transfer of audio samples towards the serial audio interface */
  else if (HAL_I2S_DMAStop(&haudio_out_i2s) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    /* Update audio out state */
    Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_STOP;
  }
  return status;
}

/**
  * @brief  Mute playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Mute(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Check audio out mute status */
  else if (Audio_Out_Ctx[Instance].IsMute == 1U)
  {
    /* Nothing to do */
  }
  /* Call the audio codec mute function */
  else if (Audio_Drv->SetMute(Audio_CompObj, WM8904_MUTE_ON) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Update audio out mute status */
    Audio_Out_Ctx[Instance].IsMute = 1U;
  }
  return status;
}

/**
  * @brief  Unmute playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_UnMute(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Check audio out mute status */
  else if (Audio_Out_Ctx[Instance].IsMute == 0U)
  {
    /* Nothing to do */
  }
  /* Call the audio codec mute function */
  else if (Audio_Drv->SetMute(Audio_CompObj, WM8904_MUTE_OFF) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Update audio out mute status */
    Audio_Out_Ctx[Instance].IsMute = 0U;
  }
  return status;
}

/**
  * @brief  Check audio out mute status.
  * @param  Instance Audio out instance.
  * @param  IsMute Pointer to mute status. Value is 1 for mute, 0 for unmute status.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_IsMute(uint32_t Instance, uint32_t *IsMute)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio out mute status */
  else
  {
    *IsMute = Audio_Out_Ctx[Instance].IsMute;
  }
  return status;
}

/**
  * @brief  Set audio out volume.
  * @param  Instance Audio out instance.
  * @param  Volume Volume level in percentage from 0% to 100%.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetVolume(uint32_t Instance, uint32_t Volume)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= AUDIO_OUT_INSTANCES_NBR) || (Volume > 100U))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Call the audio codec volume control function */
    if (Audio_Drv->SetVolume(Audio_CompObj, VOLUME_OUTPUT, (uint8_t) Volume) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Store volume on audio out context */
      Audio_Out_Ctx[Instance].Volume = Volume;
    }
  }
  return status;
}

/**
  * @brief  Get audio out volume.
  * @param  Instance Audio out instance.
  * @param  Volume Pointer to volume level in percentage from 0% to 100%.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetVolume(uint32_t Instance, uint32_t *Volume)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio out volume */
  else
  {
    *Volume = Audio_Out_Ctx[Instance].Volume;
  }
  return status;
}

/**
  * @brief  Set audio out sample rate.
  * @param  Instance Audio out instance.
  * @param  SampleRate Sample rate of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetSampleRate(uint32_t Instance, uint32_t SampleRate)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Check if sample rate is modified */
  else if (Audio_Out_Ctx[Instance].SampleRate == SampleRate)
  {
    /* Nothing to do */
  }
  else
  {
    /* Update I2S6 clock config */
    haudio_out_i2s.Init.AudioFreq = SampleRate;
    if (MX_I2S6_ClockConfig(&haudio_out_i2s, SampleRate) != HAL_OK)
    {
      status = BSP_ERROR_CLOCK_FAILURE;
    }
    /* Re-initialize I2S6 with new sample rate */
    else if (HAL_I2S_Init(&haudio_out_i2s) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1)
    /* Register I2S TC, HT and Error callbacks */
    else if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_TX_COMPLETE_CB_ID, I2S_TxCpltCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_TX_HALF_COMPLETE_CB_ID, I2S_TxHalfCpltCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_ERROR_CB_ID, I2S_ErrorCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
#endif /* (USE_HAL_I2S_REGISTER_CALLBACKS == 1) */
    /* Store new sample rate on audio out context */
    else
    {
      Audio_Out_Ctx[Instance].SampleRate = SampleRate;
    }
  }
  return status;
}

/**
  * @brief  Get audio out sample rate.
  * @param  Instance Audio out instance.
  * @param  SampleRate Pointer to sample rate of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetSampleRate(uint32_t Instance, uint32_t *SampleRate)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio out sample rate */
  else
  {
    *SampleRate = Audio_Out_Ctx[Instance].SampleRate;
  }
  return status;
}

/**
  * @brief  Set audio out device.
  * @param  Instance Audio out instance.
  * @param  Device Device of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetDevice(uint32_t Instance, uint32_t Device)
{
  int32_t status = BSP_ERROR_NONE;

  UNUSED(Device);

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Nothing to do because there is only one device (AUDIO_OUT_HEADPHONE) */
  }
  return status;
}

/**
  * @brief  Get audio out device.
  * @param  Instance Audio out instance.
  * @param  Device Pointer to device of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetDevice(uint32_t Instance, uint32_t *Device)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio out device */
  else
  {
    *Device = Audio_Out_Ctx[Instance].Device;
  }
  return status;
}

/**
  * @brief  Set bits per sample for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  BitsPerSample Bits per sample of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && ((BitsPerSample == AUDIO_RESOLUTION_32B) || (BitsPerSample == AUDIO_RESOLUTION_8B)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else if (Audio_Out_Ctx[Instance].BitsPerSample == BitsPerSample)
  {
    /* nothing to do */
  }
  else
  {
    /* Store new bits per sample on audio out context */
    Audio_Out_Ctx[Instance].BitsPerSample = BitsPerSample;

    /* Update data size, frame length and active frame length parameters of I2S handle */
    if (BitsPerSample == AUDIO_RESOLUTION_24B)
    {
      haudio_out_i2s.Init.DataFormat = I2S_DATAFORMAT_24B;
    }
    else
    {
      haudio_out_i2s.Init.DataFormat = I2S_DATAFORMAT_16B;
    }

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 0)
    I2S_MspInit(&haudio_out_i2s);
#else
    /* Update I2S state only to keep current MSP functions */
    haudio_out_i2s.State = HAL_I2S_STATE_RESET;
#endif /* (USE_HAL_I2S_REGISTER_CALLBACKS == 0) */

    /* Re-initialize I2S1 with new parameters */
    if (HAL_I2S_Init(&haudio_out_i2s) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1)
    /* Register I2S TC, HT and Error callbacks */
    else if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_TX_COMPLETE_CB_ID, I2S_TxCpltCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_TX_HALF_COMPLETE_CB_ID, I2S_TxHalfCpltCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_ERROR_CB_ID, I2S_ErrorCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
#endif /* (USE_HAL_I2S_REGISTER_CALLBACKS == 1) */
    else
    {
      /* Nothing to do */
    }
  }
  return status;
}

/**
  * @brief  Get bits per sample for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  BitsPerSample Pointer to bits per sample of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current bits per sample of audio out stream */
  else
  {
    *BitsPerSample = Audio_Out_Ctx[Instance].BitsPerSample;
  }
  return status;
}

/**
  * @brief  Set channels number for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  ChannelNbr Channels number of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && (ChannelNbr != 2U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Nothing to do because only stereo playback is supported */
  }
  return status;
}

/**
  * @brief  Get channels number for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  ChannelNbr Pointer to channels number of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current channels number of audio out stream */
  else
  {
    *ChannelNbr = Audio_Out_Ctx[Instance].ChannelsNbr;
  }
  return status;
}

/**
  * @brief  Get current state for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  State Pointer to state of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetState(uint32_t Instance, uint32_t *State)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Get the current state of audio out stream */
  else
  {
    *State = Audio_Out_Ctx[Instance].State;
  }
  return status;
}

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register default BSP AUDIO OUT msp callbacks.
  * @param  Instance AUDIO OUT Instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit callbacks */
    if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_MSPINIT_CB_ID, I2S_MspInit) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_MSPDEINIT_CB_ID, I2S_MspDeInit) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      AudioOut_IsMspCbValid[Instance] = 1U;
    }
  }
  /* Return BSP status */
  return status;
}

/**
  * @brief  Register BSP AUDIO OUT msp callbacks.
  * @param  Instance AUDIO OUT Instance.
  * @param  CallBacks Pointer to MspInit/MspDeInit callback functions.
  * @retval BSP status
  */
int32_t BSP_AUDIO_OUT_RegisterMspCallbacks(uint32_t Instance, BSP_AUDIO_OUT_Cb_t *CallBacks)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit callbacks */
    if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_MSPINIT_CB_ID, CallBacks->pMspInitCb) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_I2S_RegisterCallback(&haudio_out_i2s, HAL_I2S_MSPDEINIT_CB_ID, CallBacks->pMspDeInitCb) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      AudioOut_IsMspCbValid[Instance] = 1U;
    }
  }
  /* Return BSP status */
  return status;
}
#endif /* (USE_HAL_I2S_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Manage the BSP audio out transfer complete event.
  * @param  Instance Audio out instance.
  * @retval None.
  */
__weak void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  Manage the BSP audio out half transfer complete event.
  * @param  Instance Audio out instance.
  * @retval None.
  */
__weak void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  Manages the BSP audio out error event.
  * @param  Instance Audio out instance.
  * @retval None.
  */
__weak void BSP_AUDIO_OUT_Error_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  BSP AUDIO OUT interrupt handler.
  * @param  Instance Audio out instance.
  * @param  Device Device of the audio out stream.
  * @retval None.
  */
void BSP_AUDIO_OUT_IRQHandler(uint32_t Instance, uint32_t Device)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
  UNUSED(Device);

  HAL_DMA_IRQHandler(haudio_out_i2s.hdmatx);
}

/**
  * @brief  I2S6 clock Config.
  * @param  hi2s I2S handle.
  * @param  SampleRate Audio sample rate used to play the audio stream.
  * @note   The PLL configuration done within this function assumes that
  *         the PLL input is HSI clock and that HSI is already enabled.
  * @retval HAL status.
  */
__weak HAL_StatusTypeDef MX_I2S6_ClockConfig(I2S_HandleTypeDef *hi2s, uint32_t SampleRate)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);

  HAL_StatusTypeDef         status;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  RCC_OscInitTypeDef        RCC_OscInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL2.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL2.PLLM = 32;  /* 2MHz */
  if ((SampleRate == AUDIO_FREQUENCY_11K) || (SampleRate == AUDIO_FREQUENCY_22K) || (SampleRate == AUDIO_FREQUENCY_44K))
  {
    RCC_OscInitStruct.PLL2.PLLN = 96;  /* 96*2 = 192MHz */
    RCC_OscInitStruct.PLL2.PLLQ = 17;  /* 192/17 = 11,294Mhz */
  }
  else if ((SampleRate == AUDIO_FREQUENCY_88K) || (SampleRate == AUDIO_FREQUENCY_176K))
  {
    RCC_OscInitStruct.PLL2.PLLN = 384; /* 384*2 = 768MHz */
    RCC_OscInitStruct.PLL2.PLLQ = 17;  /* 768/17 = 45.176Mhz */
  }
  else /* AUDIO_FREQUENCY_8K, AUDIO_FREQUENCY_16K, AUDIO_FREQUENCY_32K, AUDIO_FREQUENCY_48K, AUDIO_FREQUENCY_96K or AUDIO_FREQUENCY_192K */
  {
    RCC_OscInitStruct.PLL2.PLLN = 172; /* 172*2 = 344MHz */
    RCC_OscInitStruct.PLL2.PLLQ = 7;   /* 344/7 = 49,142Mhz */
  }
  RCC_OscInitStruct.PLL2.PLLP = 128; /* NOT USED */
  RCC_OscInitStruct.PLL2.PLLR = 128; /* NOT USED */
  RCC_OscInitStruct.PLL2.PLLS = 8;   /* NOT USED */
  RCC_OscInitStruct.PLL2.PLLT = 8;   /* NOT USED */
  RCC_OscInitStruct.PLL2.PLLFractional = 0;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI6;
  PeriphClkInitStruct.Spi6ClockSelection   = RCC_SPI6CLKSOURCE_PLL2Q;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    status = HAL_ERROR;
  }
  else
  {
    status = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  }

  return status;
}

/**
  * @brief  Initialize I2S6.
  * @param  hi2s I2S handle.
  * @param  MXInit I2S configuration structure.
  * @retval HAL status.
  */
__weak HAL_StatusTypeDef MX_I2S6_Init(I2S_HandleTypeDef *hi2s, MX_I2S_Config_t *MXInit)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Disable I2S peripheral to allow access to I2S internal registers */
  __HAL_I2S_DISABLE(hi2s);

  /* I2S peripheral configuration */
  hi2s->Init.Mode                    = MXInit->Mode;
  hi2s->Init.Standard                = I2S_STANDARD_PHILIPS;
  hi2s->Init.DataFormat              = MXInit->DataFormat;
  hi2s->Init.MCLKOutput              = I2S_MCLKOUTPUT_ENABLE;
  hi2s->Init.AudioFreq               = MXInit->AudioFrequency;
  hi2s->Init.CPOL                    = I2S_CPOL_LOW;
  hi2s->Init.FirstBit                = I2S_FIRSTBIT_MSB;
  hi2s->Init.WSInversion             = I2S_WS_INVERSION_DISABLE;
  hi2s->Init.Data24BitAlignment      = I2S_DATA_24BIT_ALIGNMENT_RIGHT;
  hi2s->Init.MasterKeepIOState       = I2S_MASTER_KEEP_IO_STATE_ENABLE;

  if (HAL_I2S_Init(hi2s) != HAL_OK)
  {
    status = HAL_ERROR;
  }

  /* Enable I2S to generate master clock */
  __HAL_I2S_ENABLE(hi2s);

  return status;
}
/**
  * @}
  */

/** @addtogroup STM32H7S78_DK_AUDIO_IN_Exported_Functions
  * @{
  */
/**
  * @brief  Initialize the audio in peripherals.
  * @param  Instance Audio in instance.
  * @param  AudioInit Audio in init structure.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_Init(uint32_t Instance, BSP_AUDIO_Init_t *AudioInit)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  else if (AudioInit->BitsPerSample != AUDIO_RESOLUTION_16B)
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 0U) && (AudioInit->ChannelsNbr != 2U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 1U) && (AudioInit->ChannelsNbr != 1U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 0U) && (Audio_Out_Ctx[0].State != AUDIO_OUT_STATE_RESET))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    if (Instance == 0U)
    {
      /* Fill audio in context structure */
      Audio_In_Ctx[Instance].Device         = AudioInit->Device;
      Audio_In_Ctx[Instance].SampleRate     = AudioInit->SampleRate;
      Audio_In_Ctx[Instance].BitsPerSample  = AudioInit->BitsPerSample;
      Audio_In_Ctx[Instance].ChannelsNbr    = AudioInit->ChannelsNbr;
      Audio_In_Ctx[Instance].Volume         = AudioInit->Volume;

      /* Probe the audio codec */
      if (WM8904_Probe() != BSP_ERROR_NONE)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        /* Set I2S instance and state */
        haudio_in_i2s.Instance  = SPI6;
        Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_INIT;

        /* Configure the I2S PLL according to the requested audio frequency */
        if (MX_I2S6_ClockConfig(&haudio_in_i2s, AudioInit->SampleRate) != HAL_OK)
        {
          status = BSP_ERROR_CLOCK_FAILURE;
        }
        else
        {
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 0)
          I2S_MspInit(&haudio_in_i2s);
#else
          /* Register the I2S MSP Callbacks */
          if (AudioIn_IsMspCbValid[Instance] == 0U)
          {
            if (BSP_AUDIO_IN_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
            {
              status = BSP_ERROR_PERIPH_FAILURE;
            }
          }
#endif /* #if (USE_HAL_I2S_REGISTER_CALLBACKS == 0) */
        }

        if (status == BSP_ERROR_NONE)
        {
          /* Prepare I2S peripheral initialization */
          MX_I2S_Config_t mxI2sInit;
          mxI2sInit.AudioFrequency    = AudioInit->SampleRate;
          mxI2sInit.Mode              = I2S_MODE_MASTER_RX;
          mxI2sInit.DataFormat        = I2S_DATAFORMAT_16B;

          /* I2S peripheral initialization */
          if (MX_I2S6_Init(&haudio_in_i2s, &mxI2sInit) != HAL_OK)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1)
          /* Register I2S TC, HT and Error callbacks */
          else if (HAL_I2S_RegisterCallback(&haudio_in_i2s, HAL_I2S_RX_COMPLETE_CB_ID, I2S_RxCpltCallback) != HAL_OK)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
          else if (HAL_I2S_RegisterCallback(&haudio_in_i2s, HAL_I2S_RX_HALF_COMPLETE_CB_ID, I2S_RxHalfCpltCallback) != HAL_OK)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
          else if (HAL_I2S_RegisterCallback(&haudio_in_i2s, HAL_I2S_ERROR_CB_ID, I2S_ErrorCallback) != HAL_OK)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
#endif /* (USE_HAL_I2S_REGISTER_CALLBACKS == 1) */
          else
          {
            /* Initialize audio codec */
            WM8904_Init_t codec_init;
            codec_init.InputDevice  = WM8904_IN_MIC1;
            codec_init.OutputDevice = WM8904_OUT_NONE;
            codec_init.Resolution   = WM8904_RESOLUTION_16B; /* Not used */
            codec_init.Frequency    = AudioInit->SampleRate;
            codec_init.Volume       = Audio_In_Ctx[0].Volume;
            if (Audio_Drv->Init(Audio_CompObj, &codec_init) < 0)
            {
              status = BSP_ERROR_COMPONENT_FAILURE;
            }
            else
            {
              /* Update audio in context state */
              Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_STOP;
            }
          }
        }
      }
    }
    else /* Instance = 1 */
    {
      /* Fill audio in context structure */
      Audio_In_Ctx[Instance].Device         = AudioInit->Device;
      Audio_In_Ctx[Instance].SampleRate     = AudioInit->SampleRate;
      Audio_In_Ctx[Instance].BitsPerSample  = AudioInit->BitsPerSample;
      Audio_In_Ctx[Instance].ChannelsNbr    = AudioInit->ChannelsNbr;
      Audio_In_Ctx[Instance].Volume         = AudioInit->Volume;

      /* Set MDF instances */
      haudio_in_mdf.Instance = ADF1_Filter0;

      /* Configure MDF clock according to the requested audio frequency */
      if (MX_ADF1_ClockConfig(&haudio_in_mdf, AudioInit->SampleRate) != HAL_OK)
      {
        status = BSP_ERROR_CLOCK_FAILURE;
      }
      else
      {
#if (USE_HAL_MDF_REGISTER_CALLBACKS == 0)
        MDF_MspInit(&haudio_in_mdf);
#else
        /* Register the MDF MSP Callbacks */
        if (AudioIn_IsMspCbValid[Instance] == 0U)
        {
          if (BSP_AUDIO_IN_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
        }
        if (status == BSP_ERROR_NONE)
        {
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 0) */
        /* Prepare MDF peripheral initialization */
        MX_MDF_Config_t mxMdfInit;
        mxMdfInit.Gain               = MDF_GAIN(AudioInit->SampleRate);
        mxMdfInit.DecimationRatio    = MDF_DECIMATION_RATIO(AudioInit->SampleRate);
        mxMdfInit.CicMode            = MDF_CIC_MODE(AudioInit->SampleRate);
        mxMdfInit.ProcClockDivider   = MDF_PROC_CLOCK_DIVIDER(AudioInit->SampleRate);
        mxMdfInit.OutputClockDivider = MDF_OUTPUT_CLOCK_DIVIDER(AudioInit->SampleRate);
        if (MX_ADF1_Init(&haudio_in_mdf, &mxMdfInit) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
#if (USE_HAL_MDF_REGISTER_CALLBACKS == 1)
        if (status == BSP_ERROR_NONE)
          {
            /* Register MDF filter TC, HT and Error callbacks */
            if (HAL_MDF_RegisterCallback(&haudio_in_mdf, HAL_MDF_ACQ_COMPLETE_CB_ID, MDF_AcqCpltCallback) != HAL_OK)
            {
              status = BSP_ERROR_PERIPH_FAILURE;
            }
            else if (HAL_MDF_RegisterCallback(&haudio_in_mdf, HAL_MDF_ACQ_HALFCOMPLETE_CB_ID, MDF_AcqHalfCpltCallback) != HAL_OK)
            {
              status = BSP_ERROR_PERIPH_FAILURE;
            }
            else
            {
              if (HAL_MDF_RegisterCallback(&haudio_in_mdf, HAL_MDF_ERROR_CB_ID, MDF_ErrorCallback) != HAL_OK)
              {
                status = BSP_ERROR_PERIPH_FAILURE;
              }
            }
          }
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) */
          if (status == BSP_ERROR_NONE)
          {
            /* Update audio in context state */
            Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_STOP;
          }
#if (USE_HAL_MDF_REGISTER_CALLBACKS == 1)
        }
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) */
      }
    }
  }
  return status;
}

/**
  * @brief  De-initialize the audio in peripherals.
  * @param  Instance Audio in instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_DeInit(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RESET)
  {
    if (Instance == 0U)
    {
      /* I2S peripheral de-initialization */
      if (HAL_I2S_DeInit(&haudio_in_i2s) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      /* De-initialize audio codec */
      else
      {
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 0)
        I2S_MspDeInit(&haudio_in_i2s);
#endif /* (USE_HAL_I2S_REGISTER_CALLBACKS == 0) */
        if (Audio_Drv->DeInit(Audio_CompObj) < 0)
        {
          status = BSP_ERROR_COMPONENT_FAILURE;
        }
      }

      if (status == BSP_ERROR_NONE)
      {
        /* Update audio in context */
        Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_RESET;
      }
    }
    else /* Instance = 1 */
    {
      /* MDF peripheral de-initialization */
      if (HAL_MDF_DeInit(&haudio_in_mdf) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
#if (USE_HAL_MDF_REGISTER_CALLBACKS == 0)
        MDF_MspDeInit(&haudio_in_mdf);
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 0) */
      }

      if (status == BSP_ERROR_NONE)
      {
        /* Update audio in context */
        Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_RESET;
      }
    }
  }
  else
  {
    /* Nothing to do */
  }
  return status;
}

/**
  * @brief  Start recording audio stream to a data buffer for a determined size.
  * @param  Instance Audio in instance.
  * @param  pData Pointer on data buffer.
  * @param  NbrOfBytes Size of buffer in bytes. Maximum size is 65535 bytes.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_Record(uint32_t Instance, uint8_t *pData, uint32_t NbrOfBytes)
{
  int32_t  status = BSP_ERROR_NONE;

  if ((Instance >= AUDIO_IN_INSTANCES_NBR) || (pData == NULL) || (NbrOfBytes > 65535U))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check the internal buffer size */
  else if ((Instance == 1U) && ((NbrOfBytes / 2U) > DEFAULT_AUDIO_IN_BUFFER_SIZE))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    if (Instance == 0U)
    {
      Audio_In_Ctx[Instance].pBuff = pData;
      Audio_In_Ctx[Instance].Size  = NbrOfBytes;

      /* Call the audio Codec Play function */
      if (Audio_Drv->Play(Audio_CompObj) < 0)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        /* Initiate a DMA transfer of audio samples from the serial audio interface */
        /* Disable I2S to be able to configure RxDMAEN */
        __HAL_I2S_DISABLE(&haudio_in_i2s);
        if (HAL_I2S_Receive_DMA(&haudio_in_i2s, (uint16_t *) pData, (uint16_t)(NbrOfBytes / 2U)) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
    }
    else /* Instance = 1 */
    {
      MDF_DmaConfigTypeDef dmaConfig;

      Audio_In_Ctx[Instance].pBuff = pData;
      Audio_In_Ctx[Instance].Size  = NbrOfBytes;

      /* Call the MDF acquisition start function */
      dmaConfig.Address    = (uint32_t) Audio_DigMicRecBuff;
      dmaConfig.DataLength = 2U * NbrOfBytes;
      dmaConfig.MsbOnly    = DISABLE;
      if (HAL_MDF_AcqStart_DMA(&haudio_in_mdf, &Audio_AdfFilterConfig, &dmaConfig) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
    }
    if (status == BSP_ERROR_NONE)
    {
      /* Update audio in state */
      Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_RECORDING;
    }
  }
  return status;
}

/**
  * @brief  Pause record of audio stream.
  * @param  Instance Audio in instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_Pause(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RECORDING)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    if (Instance == 0U)
    {
      /* Pause DMA transfer of audio samples from the serial audio interface */
      if (HAL_I2S_DMAPause(&haudio_in_i2s) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
    }
    else /* Instance = 1 */
    {
      /* Call the MDF acquisition stop function */
      if (HAL_MDF_AcqStop_DMA(&haudio_in_mdf) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
    }
    if (status == BSP_ERROR_NONE)
    {
      /* Update audio in state */
      Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_PAUSE;
    }
  }
  return status;
}

/**
  * @brief  Resume record of audio stream.
  * @param  Instance Audio in instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_Resume(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_PAUSE)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    if (Instance == 0U)
    {
      /* Resume DMA transfer of audio samples from the serial audio interface */
      if (HAL_I2S_DMAResume(&haudio_in_i2s) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
    }
    else /* Instance = 1 */
    {
      MDF_DmaConfigTypeDef dmaConfig;

      /* Call the MDF acquisition start function */
      dmaConfig.Address    = (uint32_t) Audio_DigMicRecBuff;
      dmaConfig.DataLength = 2U * Audio_In_Ctx[Instance].Size;
      dmaConfig.MsbOnly    = DISABLE;
      if (HAL_MDF_AcqStart_DMA(&haudio_in_mdf, &Audio_AdfFilterConfig, &dmaConfig) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
    }
    if (status == BSP_ERROR_NONE)
    {
      /* Update audio in state */
      Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_RECORDING;
    }
  }
  return status;
}

/**
  * @brief  Stop record of audio stream.
  * @param  Instance Audio in instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_Stop(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_STOP)
  {
    /* Nothing to do */
  }
  else if ((Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RECORDING) &&
           (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_PAUSE))
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    if (Instance == 0U)
    {
      /* Call the audio codec stop function */
      if (Audio_Drv->Stop(Audio_CompObj, WM8904_PDWN_SW) < 0)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
      /* Stop DMA transfer of audio samples from the serial audio interface */
      else
      {
        if (HAL_I2S_DMAStop(&haudio_in_i2s) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
    }
    else /* Instance = 1 */
    {
      /* Call the MDF acquisition stop function */
      if (HAL_MDF_AcqStop_DMA(&haudio_in_mdf) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
    }
    if (status == BSP_ERROR_NONE)
    {
      /* Update audio in state */
      Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_STOP;
    }
  }
  return status;
}

/**
  * @brief  Set audio in volume.
  * @param  Instance Audio in instance.
  * @param  Volume Volume level in percentage from 0% to 100%.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_SetVolume(uint32_t Instance, uint32_t Volume)
{
  int32_t status;

  if ((Instance >= AUDIO_IN_INSTANCES_NBR) || (Volume > 100U))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Feature not supported */
  else
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  return status;
}

/**
  * @brief  Get audio in volume.
  * @param  Instance Audio in instance.
  * @param  Volume Pointer to volume level in percentage from 0% to 100%.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_GetVolume(uint32_t Instance, uint32_t *Volume)
{
  int32_t status;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Feature not supported */
  else
  {
    *Volume = 0U;
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  return status;
}

/**
  * @brief  Set audio in sample rate.
  * @param  Instance Audio in instance.
  * @param  SampleRate Sample rate of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_SetSampleRate(uint32_t Instance, uint32_t SampleRate)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Check if sample rate is modified */
  else if (Audio_In_Ctx[Instance].SampleRate == SampleRate)
  {
    /* Nothing to do */
  }
  else
  {
    if (Instance == 0U)
    {
      /* Update I2S6 clock config */
      haudio_in_i2s.Init.AudioFreq = SampleRate;
      if (MX_I2S6_ClockConfig(&haudio_in_i2s, SampleRate) != HAL_OK)
      {
        status = BSP_ERROR_CLOCK_FAILURE;
      }
      /* Re-initialize I2S6 with new sample rate */
      else if (HAL_I2S_Init(&haudio_in_i2s) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1)
      /* Register I2S TC, HT and Error callbacks */
      else if (HAL_I2S_RegisterCallback(&haudio_in_i2s, HAL_I2S_RX_COMPLETE_CB_ID, I2S_RxCpltCallback) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else if (HAL_I2S_RegisterCallback(&haudio_in_i2s, HAL_I2S_RX_HALF_COMPLETE_CB_ID, I2S_RxHalfCpltCallback) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else if (HAL_I2S_RegisterCallback(&haudio_in_i2s, HAL_I2S_ERROR_CB_ID, I2S_ErrorCallback) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
#endif /* (USE_HAL_I2S_REGISTER_CALLBACKS == 1) */
      /* Store new sample rate on audio out context */
      else
      {
        Audio_In_Ctx[Instance].SampleRate = SampleRate;
      }
    }
    else /* Instance = 1 */
    {
      /* Update ADF1 clock config */
      if (MX_ADF1_ClockConfig(&haudio_in_mdf, SampleRate) != HAL_OK)
      {
        status = BSP_ERROR_CLOCK_FAILURE;
      }
      /* Re-initialize ADF1 with new sample rate */
      else if (HAL_MDF_DeInit(&haudio_in_mdf) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        MX_MDF_Config_t mxMdfInit;
        mxMdfInit.Gain               = MDF_GAIN(SampleRate);
        mxMdfInit.DecimationRatio    = MDF_DECIMATION_RATIO(SampleRate);
        mxMdfInit.CicMode            = MDF_CIC_MODE(SampleRate);
        mxMdfInit.ProcClockDivider   = MDF_PROC_CLOCK_DIVIDER(SampleRate);
        mxMdfInit.OutputClockDivider = MDF_OUTPUT_CLOCK_DIVIDER(SampleRate);

#if (USE_HAL_MDF_REGISTER_CALLBACKS == 0)
        MDF_MspInit(&haudio_in_mdf);
#endif
        if (MX_ADF1_Init(&haudio_in_mdf, &mxMdfInit) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
#if (USE_HAL_MDF_REGISTER_CALLBACKS == 1)
      if (status == BSP_ERROR_NONE)
      {
        /* Register MDF filter TC, HT and Error callbacks */
        if (HAL_MDF_RegisterCallback(&haudio_in_mdf, HAL_MDF_ACQ_COMPLETE_CB_ID, MDF_AcqCpltCallback) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
        else if (HAL_MDF_RegisterCallback(&haudio_in_mdf, HAL_MDF_ACQ_HALFCOMPLETE_CB_ID, MDF_AcqHalfCpltCallback) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
        else
        {
          if (HAL_MDF_RegisterCallback(&haudio_in_mdf, HAL_MDF_ERROR_CB_ID, MDF_ErrorCallback) != HAL_OK)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
        }
      }
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) */
      /* Store new sample rate on audio in context */
      if (status == BSP_ERROR_NONE)
      {
        Audio_In_Ctx[Instance].SampleRate = SampleRate;
      }
    }
  }
  return status;
}

/**
  * @brief  Get audio in sample rate.
  * @param  Instance Audio in instance.
  * @param  SampleRate Pointer to sample rate of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_GetSampleRate(uint32_t Instance, uint32_t *SampleRate)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio in sample rate */
  else
  {
    *SampleRate = Audio_In_Ctx[Instance].SampleRate;
  }
  return status;
}

/**
  * @brief  Set audio in device.
  * @param  Instance Audio in instance.
  * @param  Device Device of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_SetDevice(uint32_t Instance, uint32_t Device)
{
  int32_t status = BSP_ERROR_NONE;

  UNUSED(Device);

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Nothing to do because there is only one device for each instance */
  }
  return status;
}

/**
  * @brief  Get audio in device.
  * @param  Instance Audio in instance.
  * @param  Device Pointer to device of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_GetDevice(uint32_t Instance, uint32_t *Device)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio in device */
  else
  {
    *Device = Audio_In_Ctx[Instance].Device;
  }
  return status;
}

/**
  * @brief  Set bits per sample for the audio in stream.
  * @param  Instance Audio in instance.
  * @param  BitsPerSample Bits per sample of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (BitsPerSample != AUDIO_RESOLUTION_16B)
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* nothing to do because only 16 bits per sample is supported */
  }
  return status;
}

/**
  * @brief  Get bits per sample for the audio in stream.
  * @param  Instance Audio in instance.
  * @param  BitsPerSample Pointer to bits per sample of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current bits per sample of audio in stream */
  else
  {
    *BitsPerSample = Audio_In_Ctx[Instance].BitsPerSample;
  }
  return status;
}

/**
  * @brief  Set channels number for the audio in stream.
  * @param  Instance Audio in instance.
  * @param  ChannelNbr Channels number of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && (ChannelNbr != 2U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 1U) && (ChannelNbr != 1U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Nothing to do because channels are already configurated and can't be modified */
  }
  return status;
}

/**
  * @brief  Get channels number for the audio in stream.
  * @param  Instance Audio in instance.
  * @param  ChannelNbr Pointer to channels number of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current channels number of audio in stream */
  else
  {
    *ChannelNbr = Audio_In_Ctx[Instance].ChannelsNbr;
  }
  return status;
}

/**
  * @brief  Get current state for the audio in stream.
  * @param  Instance Audio in instance.
  * @param  State Pointer to state of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_GetState(uint32_t Instance, uint32_t *State)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Get the current state of audio in stream */
  else
  {
    *State = Audio_In_Ctx[Instance].State;
  }
  return status;
}

#if ((USE_HAL_MDF_REGISTER_CALLBACKS == 1) || (USE_HAL_I2S_REGISTER_CALLBACKS == 1))
/**
  * @brief  Register default BSP AUDIO IN msp callbacks.
  * @param  Instance AUDIO IN Instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Instance == 0U)
    {
      /* Register MspInit/MspDeInit callbacks */
      if (HAL_I2S_RegisterCallback(&haudio_in_i2s, HAL_I2S_MSPINIT_CB_ID, I2S_MspInit) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else if (HAL_I2S_RegisterCallback(&haudio_in_i2s, HAL_I2S_MSPDEINIT_CB_ID, I2S_MspDeInit) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        AudioIn_IsMspCbValid[Instance] = 1U;
      }
    }
    else /* Instance = 1 */
    {
      /* Register MspInit/MspDeInit callbacks */
      if (HAL_MDF_RegisterCallback(&haudio_in_mdf, HAL_MDF_MSPINIT_CB_ID, MDF_MspInit) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        if (HAL_MDF_RegisterCallback(&haudio_in_mdf, HAL_MDF_MSPDEINIT_CB_ID, MDF_MspDeInit) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }

      if (status == BSP_ERROR_NONE)
      {
        AudioIn_IsMspCbValid[Instance] = 1U;
      }
    }
  }
  /* Return BSP status */
  return status;
}

/**
  * @brief  Register BSP AUDIO IN msp callbacks.
  * @param  Instance AUDIO IN Instance.
  * @param  CallBacks Pointer to MspInit/MspDeInit callback functions.
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_RegisterMspCallbacks(uint32_t Instance, BSP_AUDIO_IN_Cb_t *CallBacks)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Instance == 0U)
    {
      /* Register MspInit/MspDeInit callbacks */
      if (HAL_I2S_RegisterCallback(&haudio_in_i2s, HAL_I2S_MSPINIT_CB_ID, CallBacks->pMspI2sInitCb) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else if (HAL_I2S_RegisterCallback(&haudio_in_i2s, HAL_I2S_MSPDEINIT_CB_ID, CallBacks->pMspI2sDeInitCb) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        AudioIn_IsMspCbValid[Instance] = 1U;
      }
    }
    else /* Instance = 1 */
    {
      /* Register MspInit/MspDeInit callbacks */
      if (HAL_MDF_RegisterCallback(&haudio_in_mdf, HAL_MDF_MSPINIT_CB_ID, CallBacks->pMspMdfInitCb) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        if (HAL_MDF_RegisterCallback(&haudio_in_mdf, HAL_MDF_MSPDEINIT_CB_ID, CallBacks->pMspMdfDeInitCb) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }

      if (status == BSP_ERROR_NONE)
      {
        AudioIn_IsMspCbValid[Instance] = 1U;
      }
    }
  }
  /* Return BSP status */
  return status;
}
#endif /* ((USE_HAL_MDF_REGISTER_CALLBACKS == 1) || (USE_HAL_I2S_REGISTER_CALLBACKS == 1)) */

/**
  * @brief  Manage the BSP audio in transfer complete event.
  * @param  Instance Audio in instance.
  * @retval None.
  */
__weak void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  Manage the BSP audio in half transfer complete event.
  * @param  Instance Audio in instance.
  * @retval None.
  */
__weak void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  Manages the BSP audio in error event.
  * @param  Instance Audio in instance.
  * @retval None.
  */
__weak void BSP_AUDIO_IN_Error_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  BSP AUDIO IN interrupt handler.
  * @param  Instance Audio in instance.
  * @param  Device Device of the audio in stream.
  * @retval None.
  */
void BSP_AUDIO_IN_IRQHandler(uint32_t Instance, uint32_t Device)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Device);

  if (Instance == 0U)
  {
    HAL_DMA_IRQHandler(haudio_in_i2s.hdmarx);
  }
  else
  {
    HAL_DMA_IRQHandler(haudio_in_mdf.hdma);
  }
}

/**
  * @brief  ADF1 clock Config.
  * @param  hmdf MDF handle.
  * @param  SampleRate Audio sample rate used to record the audio stream.
  * @retval HAL status.
  */
__weak HAL_StatusTypeDef MX_ADF1_ClockConfig(MDF_HandleTypeDef *hmdf, uint32_t SampleRate)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmdf);

  HAL_StatusTypeDef         status;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  RCC_OscInitTypeDef        RCC_OscInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL3.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL3.PLLM = 32;  /* 2MHz */
  if ((SampleRate == AUDIO_FREQUENCY_11K) || (SampleRate == AUDIO_FREQUENCY_22K) || (SampleRate == AUDIO_FREQUENCY_44K))
  {
    RCC_OscInitStruct.PLL3.PLLN = 96;  /* 96*2 = 192MHz */
    RCC_OscInitStruct.PLL3.PLLP = 17;  /* 192/17 = 11,294Mhz */
  }
  else if ((SampleRate == AUDIO_FREQUENCY_88K) || (SampleRate == AUDIO_FREQUENCY_176K))
  {
    RCC_OscInitStruct.PLL3.PLLN = 384; /* 384*2 = 768MHz */
    RCC_OscInitStruct.PLL3.PLLP = 17;  /* 768/17 = 45,176Mhz */
  }
  else if ((SampleRate == AUDIO_FREQUENCY_96K) || (SampleRate == AUDIO_FREQUENCY_192K))
  {
    RCC_OscInitStruct.PLL3.PLLN = 344; /* 344*2 = 688MHz */
    RCC_OscInitStruct.PLL3.PLLP = 7;   /* 688/7 = 98,285Mhz */
  }
  else /* AUDIO_FREQUENCY_8K, AUDIO_FREQUENCY_16K, AUDIO_FREQUENCY_32K, AUDIO_FREQUENCY_48K */
  {
    RCC_OscInitStruct.PLL3.PLLN = 172; /* 172*2 = 344MHz */
    RCC_OscInitStruct.PLL3.PLLP = 7;   /* 344/7 = 49,142Mhz */
  }
  RCC_OscInitStruct.PLL3.PLLQ = 128; /* NOT USED */
  RCC_OscInitStruct.PLL3.PLLR = 128; /* NOT USED */
  RCC_OscInitStruct.PLL3.PLLS = 8;   /* NOT USED */
  RCC_OscInitStruct.PLL3.PLLT = 8;   /* NOT USED */
  RCC_OscInitStruct.PLL3.PLLFractional = 0;

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADF1;
  PeriphClkInitStruct.Adf1ClockSelection   = RCC_ADF1CLKSOURCE_PLL3P;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    status = HAL_ERROR;
  }
  else
  {
    status = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  }

  return status;
}

/**
  * @brief  Initialize ADF1.
  * @param  hmdf  MDF handle.
  * @param  MXInit MDF configuration structure.
  * @retval HAL_status.
  */
__weak HAL_StatusTypeDef MX_ADF1_Init(MDF_HandleTypeDef *hmdf, MX_MDF_Config_t *MXInit)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* MDF peripheral initialization */
  hmdf->Init.CommonParam.ProcClockDivider = MXInit->ProcClockDivider;
  hmdf->Init.CommonParam.OutputClock.Activation = ENABLE;
  hmdf->Init.CommonParam.OutputClock.Pins = MDF_OUTPUT_CLOCK_0;
  hmdf->Init.CommonParam.OutputClock.Divider = MXInit->OutputClockDivider;
  hmdf->Init.CommonParam.OutputClock.Trigger.Activation = DISABLE;
  hmdf->Init.SerialInterface.Activation = ENABLE;
  hmdf->Init.SerialInterface.Mode = MDF_SITF_NORMAL_SPI_MODE;
  hmdf->Init.SerialInterface.ClockSource = MDF_SITF_CCK0_SOURCE;
  hmdf->Init.SerialInterface.Threshold = 31U;
  hmdf->Init.FilterBistream = MDF_BITSTREAM0_FALLING;

  if (HAL_MDF_Init(hmdf) != HAL_OK)
  {
    status = HAL_ERROR;
  }

  /* Prepare ADF filter configuration parameters */
  Audio_AdfFilterConfig.DataSource                = MDF_DATA_SOURCE_BSMX;
  Audio_AdfFilterConfig.Delay                     = 0U;
  Audio_AdfFilterConfig.CicMode                   = MXInit->CicMode;
  Audio_AdfFilterConfig.DecimationRatio           = MXInit->DecimationRatio;
  Audio_AdfFilterConfig.Gain                      = MXInit->Gain;
  Audio_AdfFilterConfig.ReshapeFilter.Activation  = ENABLE;
  Audio_AdfFilterConfig.ReshapeFilter.DecimationRatio = MDF_RSF_DECIMATION_RATIO_4;

  Audio_AdfFilterConfig.HighPassFilter.Activation = ENABLE;
  Audio_AdfFilterConfig.HighPassFilter.CutOffFrequency = MDF_HPF_CUTOFF_0_000625FPCM;

  Audio_AdfFilterConfig.SoundActivity.Activation  = DISABLE;
  Audio_AdfFilterConfig.AcquisitionMode           = MDF_MODE_ASYNC_CONT;
  Audio_AdfFilterConfig.FifoThreshold             = MDF_FIFO_THRESHOLD_NOT_EMPTY;
  Audio_AdfFilterConfig.DiscardSamples            = 0U;

  return status;
}
/**
  * @}
  */

/** @defgroup STM32H7S78_DK_AUDIO_Private_Functions STM32H7S78_DK AUDIO Private Functions
  * @{
  */
/**
  * @brief  Probe the WM8904 audio codec.
  * @retval BSP status.
  */
static int32_t WM8904_Probe(void)
{
  int32_t                  status = BSP_ERROR_NONE;
  WM8904_IO_t              IOCtx;
  uint32_t                 wm8904_id;
  static WM8904_Object_t   WM8904Obj;

  /* Configure the audio driver */
  IOCtx.Address     = AUDIO_I2C_ADDRESS;
  IOCtx.Init        = BSP_I2C1_Init;
  IOCtx.DeInit      = BSP_I2C1_DeInit;
  IOCtx.ReadReg     = BSP_I2C1_ReadReg;
  IOCtx.WriteReg    = BSP_I2C1_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;

  if (WM8904_RegisterBusIO(&WM8904Obj, &IOCtx) != WM8904_OK)
  {
    status = BSP_ERROR_BUS_FAILURE;
  }
  else if (WM8904_ReadID(&WM8904Obj, &wm8904_id) != WM8904_OK)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if ((wm8904_id & WM8904_ID_MASK) != WM8904_ID)
  {
    status = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    Audio_Drv = (AUDIO_Drv_t *) &WM8904_Driver;
    Audio_CompObj = &WM8904Obj;
  }

  return status;
}

/**
  * @brief  Initialize I2S MSP.
  * @param  hi2s I2S handle.
  * @retval None.
  */
static void I2S_MspInit(I2S_HandleTypeDef *hi2s)
{
  GPIO_InitTypeDef           GPIO_InitStruct;
  static DMA_NodeTypeDef     TxNode = {0};
  static DMA_NodeTypeDef     RxNode = {0};
  static DMA_NodeConfTypeDef dmaNodeConfig = {0};

  /* Enable I2S clock */
  AUDIO_I2S6_CLK_ENABLE();

  /* I2S pins configuration: MCK, CK, WS, SDI and SDO pins */
  AUDIO_I2S6_MCK_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = AUDIO_I2S6_MCK_SDI_GPIO_AF;
  GPIO_InitStruct.Pin       = AUDIO_I2S6_MCK_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_I2S6_MCK_GPIO_PORT, &GPIO_InitStruct);

  AUDIO_I2S6_CK_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Alternate = AUDIO_I2S6_CK_WS_SDO_GPIO_AF;
  GPIO_InitStruct.Pin       = AUDIO_I2S6_CK_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_I2S6_CK_GPIO_PORT, &GPIO_InitStruct);

  AUDIO_I2S6_WS_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin       = AUDIO_I2S6_WS_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_I2S6_WS_GPIO_PORT, &GPIO_InitStruct);

  AUDIO_I2S6_SDI_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Alternate = AUDIO_I2S6_MCK_SDI_GPIO_AF;
  GPIO_InitStruct.Pin       = AUDIO_I2S6_SDI_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_I2S6_SDI_GPIO_PORT, &GPIO_InitStruct);

  AUDIO_I2S6_SDO_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Alternate = AUDIO_I2S6_CK_WS_SDO_GPIO_AF;
  GPIO_InitStruct.Pin       = AUDIO_I2S6_SDO_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_I2S6_SDO_GPIO_PORT, &GPIO_InitStruct);

  /* DMA for Tx */
  if (Audio_Out_Ctx[0].State != AUDIO_OUT_STATE_RESET)
  {
    if (I2sTxQueue.Head == NULL)
    {
      AUDIO_OUT_I2S6_DMA_CLK_ENABLE();

      /* Set node type */
      dmaNodeConfig.NodeType                            = DMA_GPDMA_LINEAR_NODE;
      /* Set common node parameters */
      dmaNodeConfig.Init.Request                        = AUDIO_OUT_I2S6_DMA_REQUEST;
      dmaNodeConfig.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
      dmaNodeConfig.Init.Direction                      = DMA_MEMORY_TO_PERIPH;
      dmaNodeConfig.Init.SrcInc                         = DMA_SINC_INCREMENTED;
      dmaNodeConfig.Init.DestInc                        = DMA_DINC_FIXED;
      if (Audio_Out_Ctx[0].BitsPerSample == AUDIO_RESOLUTION_16B)
      {
        dmaNodeConfig.Init.SrcDataWidth                 = DMA_SRC_DATAWIDTH_HALFWORD;
        dmaNodeConfig.Init.DestDataWidth                = DMA_DEST_DATAWIDTH_HALFWORD;
      }
      else /* AUDIO_RESOLUTION_24b */
      {
        dmaNodeConfig.Init.SrcDataWidth                 = DMA_SRC_DATAWIDTH_WORD;
        dmaNodeConfig.Init.DestDataWidth                = DMA_DEST_DATAWIDTH_WORD;
      }
      dmaNodeConfig.Init.SrcBurstLength                 = 1;
      dmaNodeConfig.Init.DestBurstLength                = 1;
      dmaNodeConfig.Init.Priority                       = DMA_HIGH_PRIORITY;
      dmaNodeConfig.Init.TransferEventMode              = DMA_TCEM_BLOCK_TRANSFER;
      dmaNodeConfig.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
      /* Set node data handling parameters */
      dmaNodeConfig.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
      dmaNodeConfig.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
      /* Set node trigger parameters */
      dmaNodeConfig.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

      /* Build NodeTx */
      if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &TxNode) != HAL_OK)
      {
        BSP_AUDIO_OUT_Error_CallBack(0);
      }

      /* Insert NodeTx to I2S queue */
      else if (HAL_DMAEx_List_InsertNode_Tail(&I2sTxQueue, &TxNode) != HAL_OK)
      {
        BSP_AUDIO_OUT_Error_CallBack(0);
      }

      /* Set queue circular mode for I2S queue */
      else if (HAL_DMAEx_List_SetCircularMode(&I2sTxQueue) != HAL_OK)
      {
        BSP_AUDIO_OUT_Error_CallBack(0);
      }
      else
      {
        hDmaI2sTx.Instance                         = AUDIO_OUT_I2S6_DMA_CHANNEL;

        hDmaI2sTx.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
        hDmaI2sTx.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
        hDmaI2sTx.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
        hDmaI2sTx.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
        hDmaI2sTx.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

        /* DMA linked list init */
        if (HAL_DMAEx_List_Init(&hDmaI2sTx) != HAL_OK)
        {
          BSP_AUDIO_OUT_Error_CallBack(0);
        }

        /* Link I2S queue to DMA channel */
        else if (HAL_DMAEx_List_LinkQ(&hDmaI2sTx, &I2sTxQueue) != HAL_OK)
        {
          BSP_AUDIO_OUT_Error_CallBack(0);
        }
        else
        {
          /* Associate the DMA handle */
          __HAL_LINKDMA(hi2s, hdmatx, hDmaI2sTx);

          /* I2S DMA IRQ Channel configuration */
          HAL_NVIC_SetPriority(AUDIO_OUT_I2S6_DMA_IRQ, BSP_AUDIO_OUT_IT_PRIORITY, 0);
          HAL_NVIC_EnableIRQ(AUDIO_OUT_I2S6_DMA_IRQ);
        }
      }
    }
    else
    {
      /* Set node type */
      dmaNodeConfig.NodeType                            = DMA_GPDMA_LINEAR_NODE;
      /* Set common node parameters */
      dmaNodeConfig.Init.Request                        = AUDIO_OUT_I2S6_DMA_REQUEST;
      dmaNodeConfig.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
      dmaNodeConfig.Init.Direction                      = DMA_MEMORY_TO_PERIPH;
      dmaNodeConfig.Init.SrcInc                         = DMA_SINC_INCREMENTED;
      dmaNodeConfig.Init.DestInc                        = DMA_DINC_FIXED;
      if (Audio_Out_Ctx[0].BitsPerSample == AUDIO_RESOLUTION_16B)
      {
        dmaNodeConfig.Init.SrcDataWidth                 = DMA_SRC_DATAWIDTH_HALFWORD;
        dmaNodeConfig.Init.DestDataWidth                = DMA_DEST_DATAWIDTH_HALFWORD;
      }
      else /* AUDIO_RESOLUTION_24b */
      {
        dmaNodeConfig.Init.SrcDataWidth                 = DMA_SRC_DATAWIDTH_WORD;
        dmaNodeConfig.Init.DestDataWidth                = DMA_DEST_DATAWIDTH_WORD;
      }
      dmaNodeConfig.Init.SrcBurstLength                 = 1;
      dmaNodeConfig.Init.DestBurstLength                = 1;
      dmaNodeConfig.Init.Priority                       = DMA_HIGH_PRIORITY;
      dmaNodeConfig.Init.TransferEventMode              = DMA_TCEM_BLOCK_TRANSFER;
      dmaNodeConfig.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
      /* Set node data handling parameters */
      dmaNodeConfig.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
      dmaNodeConfig.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
      /* Set node trigger parameters */
      dmaNodeConfig.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

      /* Build NodeTx */
      if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &TxNode) != HAL_OK)
      {
        BSP_AUDIO_OUT_Error_CallBack(0);
      }

      /* Replace NodeTx in I2S queue */
      else if (HAL_DMAEx_List_ReplaceNode_Head(&I2sTxQueue, &TxNode) != HAL_OK)
      {
        BSP_AUDIO_OUT_Error_CallBack(0);
      }

      /* Set queue circular mode for I2S queue */
      else if (HAL_DMAEx_List_ClearCircularMode(&I2sTxQueue) != HAL_OK)
      {
        BSP_AUDIO_OUT_Error_CallBack(0);
      }
      else
      {
        /* Set queue circular mode for I2S queue */
        if (HAL_DMAEx_List_SetCircularMode(&I2sTxQueue) != HAL_OK)
        {
          BSP_AUDIO_OUT_Error_CallBack(0);
        }
      }
    }
  }

  /* DMA for Rx */
  if (Audio_In_Ctx[0].State != AUDIO_IN_STATE_RESET)
  {
    if (I2sRxQueue.Head == NULL)
    {
      AUDIO_IN_I2S6_DMA_CLK_ENABLE();

      /* Set node type */
      dmaNodeConfig.NodeType                            = DMA_GPDMA_LINEAR_NODE;
      /* Set common node parameters */
      dmaNodeConfig.Init.Request                        = AUDIO_IN_I2S6_DMA_REQUEST;
      dmaNodeConfig.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
      dmaNodeConfig.Init.Direction                      = DMA_PERIPH_TO_MEMORY;
      dmaNodeConfig.Init.SrcInc                         = DMA_SINC_FIXED;
      dmaNodeConfig.Init.DestInc                        = DMA_DINC_INCREMENTED;
      dmaNodeConfig.Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_HALFWORD;
      dmaNodeConfig.Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_HALFWORD;
      dmaNodeConfig.Init.SrcBurstLength                 = 1;
      dmaNodeConfig.Init.DestBurstLength                = 1;
      dmaNodeConfig.Init.Priority                       = DMA_HIGH_PRIORITY;
      dmaNodeConfig.Init.TransferEventMode              = DMA_TCEM_BLOCK_TRANSFER;
      dmaNodeConfig.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
      /* Set node data handling parameters */
      dmaNodeConfig.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
      dmaNodeConfig.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
      /* Set node trigger parameters */
      dmaNodeConfig.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

      /* Build NodeRx */
      if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &RxNode) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }

      /* Insert NodeRx to I2S queue */
      else if (HAL_DMAEx_List_InsertNode_Tail(&I2sRxQueue, &RxNode) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }

      /* Set queue circular mode for I2S queue */
      else if (HAL_DMAEx_List_SetCircularMode(&I2sRxQueue) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }
      else
      {
        hDmaI2sRx.Instance                         = AUDIO_IN_I2S6_DMA_CHANNEL;

        hDmaI2sRx.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
        hDmaI2sRx.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
        hDmaI2sRx.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT0;
        hDmaI2sRx.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
        hDmaI2sRx.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

        /* DMA linked list init */
        if (HAL_DMAEx_List_Init(&hDmaI2sRx) != HAL_OK)
        {
          BSP_AUDIO_IN_Error_CallBack(0);
        }

        /* Link I2S queue to DMA channel */
        else if (HAL_DMAEx_List_LinkQ(&hDmaI2sRx, &I2sRxQueue) != HAL_OK)
        {
          BSP_AUDIO_IN_Error_CallBack(0);
        }
        else
        {
          /* Associate the DMA handle */
          __HAL_LINKDMA(hi2s, hdmarx, hDmaI2sRx);

          /* SAI DMA IRQ Channel configuration */
          HAL_NVIC_SetPriority(AUDIO_IN_I2S6_DMA_IRQ, BSP_AUDIO_IN_IT_PRIORITY, 0);
          HAL_NVIC_EnableIRQ(AUDIO_IN_I2S6_DMA_IRQ);
        }
      }
    }
    else
    {
      /* Set node type */
      dmaNodeConfig.NodeType                            = DMA_GPDMA_LINEAR_NODE;
      /* Set common node parameters */
      dmaNodeConfig.Init.Request                        = AUDIO_IN_I2S6_DMA_REQUEST;
      dmaNodeConfig.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
      dmaNodeConfig.Init.Direction                      = DMA_PERIPH_TO_MEMORY;
      dmaNodeConfig.Init.SrcInc                         = DMA_SINC_FIXED;
      dmaNodeConfig.Init.DestInc                        = DMA_DINC_INCREMENTED;
      dmaNodeConfig.Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_HALFWORD;
      dmaNodeConfig.Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_HALFWORD;
      dmaNodeConfig.Init.SrcBurstLength                 = 1;
      dmaNodeConfig.Init.DestBurstLength                = 1;
      dmaNodeConfig.Init.Priority                       = DMA_HIGH_PRIORITY;
      dmaNodeConfig.Init.TransferEventMode              = DMA_TCEM_BLOCK_TRANSFER;
      dmaNodeConfig.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
      /* Set node data handling parameters */
      dmaNodeConfig.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
      dmaNodeConfig.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
      /* Set node trigger parameters */
      dmaNodeConfig.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

      /* Build NodeRx */
      if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &RxNode) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }

      /* Replace NodeRx in I2S queue */
      else if (HAL_DMAEx_List_ReplaceNode_Head(&I2sRxQueue, &RxNode) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }

      /* Set queue circular mode for I2S queue */
      else if (HAL_DMAEx_List_ClearCircularMode(&I2sRxQueue) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }
      else
      {
        /* Set queue circular mode for I2S queue */
        if (HAL_DMAEx_List_SetCircularMode(&I2sRxQueue) != HAL_OK)
        {
          BSP_AUDIO_IN_Error_CallBack(0);
        }
      }
    }
  }
}

/**
  * @brief  Deinitialize I2S MSP.
  * @param  hi2s I2S handle.
  * @retval None.
  */
static void I2S_MspDeInit(I2S_HandleTypeDef *hi2s)
{
  UNUSED(hi2s);

  if (Audio_Out_Ctx[0].State != AUDIO_OUT_STATE_RESET)
  {
    /* Disable I2S DMA Channel IRQ */
    HAL_NVIC_DisableIRQ(AUDIO_OUT_I2S6_DMA_IRQ);

    /* Reset the DMA Channel configuration*/
    if (HAL_DMAEx_List_DeInit(&hDmaI2sTx) != HAL_OK)
    {
      BSP_AUDIO_OUT_Error_CallBack(0);
    }

    /* Reset RxQueue */
    if (HAL_DMAEx_List_ResetQ(&I2sTxQueue) != HAL_OK)
    {
      BSP_AUDIO_OUT_Error_CallBack(0);
    }
  }

  if (Audio_In_Ctx[0].State != AUDIO_IN_STATE_RESET)
  {
    /* Disable I2S DMA Channel IRQ */
    HAL_NVIC_DisableIRQ(AUDIO_IN_I2S6_DMA_IRQ);

    /* Reset the DMA Channel configuration*/
    if (HAL_DMAEx_List_DeInit(&hDmaI2sRx) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Reset RxQueue */
    if (HAL_DMAEx_List_ResetQ(&I2sRxQueue) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }
  }

  /* De-initialize MCK, CK, WS, SDI and SDO pins */
  HAL_GPIO_DeInit(AUDIO_I2S6_MCK_GPIO_PORT, AUDIO_I2S6_MCK_GPIO_PIN);
  HAL_GPIO_DeInit(AUDIO_I2S6_CK_GPIO_PORT, AUDIO_I2S6_CK_GPIO_PIN);
  HAL_GPIO_DeInit(AUDIO_I2S6_WS_GPIO_PORT, AUDIO_I2S6_WS_GPIO_PIN);
  HAL_GPIO_DeInit(AUDIO_I2S6_SDI_GPIO_PORT, AUDIO_I2S6_SDI_GPIO_PIN);
  HAL_GPIO_DeInit(AUDIO_I2S6_SDO_GPIO_PORT, AUDIO_I2S6_SDO_GPIO_PIN);

  /* Disable I2S clock */
  AUDIO_I2S6_CLK_DISABLE();
}

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1)
/**
  * @brief  I2S Tx transfer complete callback.
  * @param  hi2s I2S handle.
  * @retval None.
  */
static void I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  UNUSED(hi2s);

  BSP_AUDIO_OUT_TransferComplete_CallBack(0);
}

/**
  * @brief  I2S Tx half transfer complete callback.
  * @param  hi2s I2S handle.
  * @retval None.
  */
static void I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  UNUSED(hi2s);

  BSP_AUDIO_OUT_HalfTransfer_CallBack(0);
}

/**
  * @brief  I2S Rx transfer complete callback.
  * @param  hi2s I2S handle.
  * @retval None.
  */
static void I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  uint32_t index;
  uint16_t *RecBuff;

  UNUSED(hi2s);

  /* Only left channel of recorded data is relevant, copy it on right channel */
  RecBuff = (uint16_t *) (Audio_In_Ctx[0].pBuff + (Audio_In_Ctx[0].Size / 2U));
  for (index = 0; index < (Audio_In_Ctx[0].Size / 8U); index++)
  {
    RecBuff[(2U * index) + 1U] = RecBuff[2U * index];
  }
  BSP_AUDIO_IN_TransferComplete_CallBack(0);
}

/**
  * @brief  I2S Rx half transfer complete callback.
  * @param  hi2s I2S handle.
  * @retval None.
  */
static void I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  uint32_t index;
  uint16_t *RecBuff;

  UNUSED(hi2s);

  /* Only left channel of recorded data is relevant, copy it on right channel */
  RecBuff = (uint16_t *) Audio_In_Ctx[0].pBuff;
  for (index = 0; index < (Audio_In_Ctx[0].Size / 8U); index++)
  {
    RecBuff[(2U * index) + 1U] = RecBuff[2U * index];
  }
  BSP_AUDIO_IN_HalfTransfer_CallBack(0);
}

/**
  * @brief  I2S error callback.
  * @param  hi2s I2S handle.
  * @retval None.
  */
static void I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
{
  UNUSED(hi2s);

  BSP_AUDIO_OUT_Error_CallBack(0);
  BSP_AUDIO_IN_Error_CallBack(0);
}
#else /* (USE_HAL_I2S_REGISTER_CALLBACKS == 1) */
/**
  * @brief  I2S Tx transfer complete callback.
  * @param  hi2s I2S handle.
  * @retval None.
  */
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  UNUSED(hi2s);

  BSP_AUDIO_OUT_TransferComplete_CallBack(0);
}

/**
  * @brief  I2S Tx half transfer complete callback.
  * @param  hi2s I2S handle.
  * @retval None.
  */
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  UNUSED(hi2s);

  BSP_AUDIO_OUT_HalfTransfer_CallBack(0);
}

/**
  * @brief  I2S Rx transfer complete callback.
  * @param  hi2s I2S handle.
  * @retval None.
  */
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  uint32_t index;
  uint16_t *RecBuff;

  UNUSED(hi2s);

  /* Only left channel of recorded data is relevant, copy it on right channel */
  RecBuff = (uint16_t *) (Audio_In_Ctx[0].pBuff + (Audio_In_Ctx[0].Size / 2U));
  for (index = 0; index < (Audio_In_Ctx[0].Size / 8U); index++)
  {
    RecBuff[(2U * index) + 1U] = RecBuff[2U * index];
  }
  BSP_AUDIO_IN_TransferComplete_CallBack(0);
}

/**
  * @brief  I2S Rx half transfer complete callback.
  * @param  hi2s I2S handle.
  * @retval None.
  */
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  uint32_t index;
  uint16_t *RecBuff;

  UNUSED(hi2s);

  /* Only left channel of recorded data is relevant, copy it on right channel */
  RecBuff = (uint16_t *) Audio_In_Ctx[0].pBuff;
  for (index = 0; index < (Audio_In_Ctx[0].Size / 8U); index++)
  {
    RecBuff[(2U * index) + 1U] = RecBuff[2U * index];
  }
  BSP_AUDIO_IN_HalfTransfer_CallBack(0);
}

/**
  * @brief  I2S error callback.
  * @param  hi2s I2S handle.
  * @retval None.
  */
void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
{
  UNUSED(hi2s);

  BSP_AUDIO_OUT_Error_CallBack(0);
  BSP_AUDIO_IN_Error_CallBack(0);
}
#endif /* (USE_HAL_I2S_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Initialize MDF MSP.
  * @param  hmdf handle.
  * @retval None.
  */
static void MDF_MspInit(MDF_HandleTypeDef *hmdf)
{
  GPIO_InitTypeDef           GPIO_InitStruct;
  static DMA_NodeTypeDef     RxNode;
  static DMA_NodeConfTypeDef dmaNodeConfig;

  /* Enable ADF1 clock */
  AUDIO_ADF1_CLK_ENABLE();

  /* ADF pins configuration: DATIN0 and CCK0 pins */
  AUDIO_ADF1_DATIN0_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = AUDIO_ADF1_DATIN0_GPIO_AF;
  GPIO_InitStruct.Pin       = AUDIO_ADF1_DATIN0_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_ADF1_DATIN0_GPIO_PORT, &GPIO_InitStruct);

  AUDIO_ADF1_CCK0_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Alternate = AUDIO_ADF1_CCK0_GPIO_AF;
  GPIO_InitStruct.Pin       = AUDIO_ADF1_CCK0_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_ADF1_CCK0_GPIO_PORT, &GPIO_InitStruct);

  /* DMA configuration */
  if (MdfRxQueue.Head == NULL)
  {
    AUDIO_IN_ADF1_DMA_CLK_ENABLE();

    /* Set node type */
    dmaNodeConfig.NodeType                            = DMA_GPDMA_LINEAR_NODE;
    /* Set common node parameters */
    dmaNodeConfig.Init.Request                        = AUDIO_IN_ADF1_DMA_REQUEST;
    dmaNodeConfig.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
    dmaNodeConfig.Init.Direction                      = DMA_PERIPH_TO_MEMORY;
    dmaNodeConfig.Init.SrcInc                         = DMA_SINC_FIXED;
    dmaNodeConfig.Init.DestInc                        = DMA_DINC_INCREMENTED;
    dmaNodeConfig.Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_WORD;
    dmaNodeConfig.Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_WORD;
    dmaNodeConfig.Init.SrcBurstLength                 = 1;
    dmaNodeConfig.Init.DestBurstLength                = 1;
    dmaNodeConfig.Init.Priority                       = DMA_HIGH_PRIORITY;
    dmaNodeConfig.Init.TransferEventMode              = DMA_TCEM_BLOCK_TRANSFER;
    dmaNodeConfig.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
    /* Set node data handling parameters */
    dmaNodeConfig.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
    dmaNodeConfig.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
    /* Set node trigger parameters */
    dmaNodeConfig.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

    /* Build NodeRx */
    if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &RxNode) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(1);
    }

    /* Insert NodeRx to ADF queue */
    else if (HAL_DMAEx_List_InsertNode_Tail(&MdfRxQueue, &RxNode) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(1);
    }

    /* Set queue circular mode for ADF queue */
    else if (HAL_DMAEx_List_SetCircularMode(&MdfRxQueue) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(1);
    }
    else
    {
      hDmaMdf.Instance                         = AUDIO_IN_ADF1_DMA_CHANNEL;

      hDmaMdf.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
      hDmaMdf.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
      hDmaMdf.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
      hDmaMdf.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
      hDmaMdf.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

      /* DMA linked list init */
      if (HAL_DMAEx_List_Init(&hDmaMdf) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(1);
      }

      /* Link MDF queue to DMA channel */
      else if (HAL_DMAEx_List_LinkQ(&hDmaMdf, &MdfRxQueue) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(1);
      }
      else
      {
        /* Associate the DMA handle */
        __HAL_LINKDMA(hmdf, hdma, hDmaMdf);

        /* MDF DMA IRQ Channel configuration */
        HAL_NVIC_SetPriority(AUDIO_IN_ADF1_DMA_IRQ, BSP_AUDIO_OUT_IT_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(AUDIO_IN_ADF1_DMA_IRQ);
      }
    }
  }
}

/**
  * @brief  DeInitialize MDF MSP.
  * @param  hmdf MDF handle.
  * @retval None.
  */
static void MDF_MspDeInit(MDF_HandleTypeDef *hmdf)
{
  UNUSED(hmdf);

  /* Disable MDF DMA Channel IRQ */
  HAL_NVIC_DisableIRQ(AUDIO_IN_ADF1_DMA_IRQ);

  /* Reset the DMA Channel configuration*/
  if (HAL_DMAEx_List_DeInit(&hDmaMdf) != HAL_OK)
  {
    BSP_AUDIO_IN_Error_CallBack(1);
  }

  /* Reset RxQueue */
  if (HAL_DMAEx_List_ResetQ(&MdfRxQueue) != HAL_OK)
  {
    BSP_AUDIO_IN_Error_CallBack(1);
  }

  /* De-initialize DATIN0 and CCK0 pins */
  HAL_GPIO_DeInit(AUDIO_ADF1_DATIN0_GPIO_PORT, AUDIO_ADF1_DATIN0_GPIO_PIN);
  HAL_GPIO_DeInit(AUDIO_ADF1_CCK0_GPIO_PORT, AUDIO_ADF1_CCK0_GPIO_PIN);

  /* Disable MDF clock */
  AUDIO_ADF1_CLK_DISABLE();
}

#if (USE_HAL_MDF_REGISTER_CALLBACKS == 1)
/**
  * @brief  MDF acquisition complete callback.
  * @param  hmdf MDF handle.
  * @retval None.
  */
static void MDF_AcqCpltCallback(MDF_HandleTypeDef *hmdf)
{
  uint32_t     index;
  uint32_t     recbufsize = Audio_In_Ctx[1].Size / 2U;
  __IO int32_t tmp;

  UNUSED(hmdf);

  for (index = (recbufsize / 2U); index < recbufsize; index++)
  {
    tmp = Audio_DigMicRecBuff[index] / 256;
    tmp = SaturaLH(tmp, -32768, 32767);
    Audio_In_Ctx[1].pBuff[2U * index]        = (uint8_t) tmp;
    Audio_In_Ctx[1].pBuff[(2U * index) + 1U] = (uint8_t)((uint32_t) tmp >> 8);
  }

  /* Invoke 'TransferCompete' callback function */
  BSP_AUDIO_IN_TransferComplete_CallBack(1);
}

/**
  * @brief  MDF acquisition half complete callback.
  * @param  hmdf MDF handle.
  * @retval None.
  */
static void MDF_AcqHalfCpltCallback(MDF_HandleTypeDef *hmdf)
{
  uint32_t     index;
  uint32_t     recbufsize = Audio_In_Ctx[1].Size / 2U;
  __IO int32_t tmp;

  UNUSED(hmdf);

  for (index = 0; index < (recbufsize / 2U); index++)
  {
    tmp = Audio_DigMicRecBuff[index] / 256;
    tmp = SaturaLH(tmp, -32768, 32767);
    Audio_In_Ctx[1].pBuff[2U * index]        = (uint8_t) tmp;
    Audio_In_Ctx[1].pBuff[(2U * index) + 1U] = (uint8_t)((uint32_t) tmp >> 8);
  }

  /* Invoke the 'HalfTransfer' callback function */
  BSP_AUDIO_IN_HalfTransfer_CallBack(1);
}

/**
  * @brief  MDF error callback.
  * @param  hmdf MDF handle.
  * @retval None.
  */
static void MDF_ErrorCallback(MDF_HandleTypeDef *hmdf)
{
  UNUSED(hmdf);

  BSP_AUDIO_IN_Error_CallBack(1);
}
#else /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) */
/**
  * @brief  MDF acquisition complete callback.
  * @param  hmdf MDF handle.
  * @retval None.
  */
void HAL_MDF_AcqCpltCallback(MDF_HandleTypeDef *hmdf)
{
  uint32_t     index;
  uint32_t     recbufsize = Audio_In_Ctx[1].Size / 2U;
  __IO int32_t tmp;

  UNUSED(hmdf);

  for (index = (recbufsize / 2U); index < recbufsize; index++)
  {
    tmp = Audio_DigMicRecBuff[index] / 256;
    tmp = SaturaLH(tmp, -32768, 32767);
    Audio_In_Ctx[1].pBuff[2U * index]        = (uint8_t) tmp;
    Audio_In_Ctx[1].pBuff[(2U * index) + 1U] = (uint8_t)((uint32_t) tmp >> 8);
  }

  /* Invoke 'TransferCompete' callback function */
  BSP_AUDIO_IN_TransferComplete_CallBack(1);
}

/**
  * @brief  MDF acquisition half complete callback.
  * @param  hmdf MDF handle.
  * @retval None.
  */
void HAL_MDF_AcqHalfCpltCallback(MDF_HandleTypeDef *hmdf)
{
  uint32_t     index;
  uint32_t     recbufsize = Audio_In_Ctx[1].Size / 2U;
  __IO int32_t tmp;

  UNUSED(hmdf);

  for (index = 0; index < (recbufsize / 2U); index++)
  {
    tmp = Audio_DigMicRecBuff[index] / 256;
    tmp = SaturaLH(tmp, -32768, 32767);
    Audio_In_Ctx[1].pBuff[2U * index]        = (uint8_t) tmp;
    Audio_In_Ctx[1].pBuff[(2U * index) + 1U] = (uint8_t)((uint32_t) tmp >> 8);
  }

  /* Invoke the 'HalfTransfer' callback function */
  BSP_AUDIO_IN_HalfTransfer_CallBack(1);
}

/**
  * @brief  MDF error callback.
  * @param  hmdf MDF handle.
  * @retval None.
  */
void HAL_MDF_ErrorCallback(MDF_HandleTypeDef *hmdf)
{
  UNUSED(hmdf);

  BSP_AUDIO_IN_Error_CallBack(1);
}
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
