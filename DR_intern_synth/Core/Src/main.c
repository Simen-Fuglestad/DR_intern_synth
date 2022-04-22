/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "wave_gen.h"
#include "usb_host.h"
#include "wavetable.h"
#include "timer_utils.h"
#include "input_handler.h"
//#include "DSP_utils.h"
#include "MY_CS43L22.h"
#include "filter.h"
#include "modulator.h"
#include "mixer.h"
#include "usbh_MIDI.h"
#include "MIDI_application.h"
//#include "oscillator.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define REF_V_ANALOG 2.90
#define REF_V_DIGITAL 0xFFF
#define N_SAMPLES 255
#define ADC1_N_CHANNELS 3 //active channels on adc 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s3;
DMA_HandleTypeDef hdma_spi3_tx;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

uint16_t output_wave[MAX_SAMPLE_SIZE];


#define I2S_OUT_N MAX_SAMPLE_SIZE
uint16_t i2s_out[I2S_OUT_N];

uint16_t out_wave_sine[I2S_OUT_N];
uint16_t out_wave_square[I2S_OUT_N];
uint16_t out_wave_tri[I2S_OUT_N];
uint16_t out_wave_saw[I2S_OUT_N];

float in_wave_index = 0;
//float index_step = pow(OCTAVE_STEP, 12*4 + SEMITONE_C);
float index_step = 0;

uint16_t i2s_it_index = 0;

bool i2s_tx_cplt = false;
bool i2s_tx_half = false;

bool key_pressed = false;

bool i2s_tx_rdy = false;
bool i2s_next_out_rdy = false;

extern uint8_t* MIDI_RX_Buffer;

waveshape_enum current_wave_out = SINE;
bool cycle_waveshape_flag = false;

uint16_t dma_adc_inputs[ADC1_N_CHANNELS];

float f_base;

bool timer_updated = false;
note_t nf_map_440hz[N_OCTAVES * N_SEMITONES];

uint16_t debounce_cnt = 0;
uint32_t debounce_limit = 0x1F;
bool debounce_flag = false;

float fc_hp;
float fc_lp;

float ns; //samples per period

float ns_a4 = I2S_SAMPLE_RATE/440;
float ns_c4 = I2S_SAMPLE_RATE/261.63;
float ns_g4 = I2S_SAMPLE_RATE/392.00;

filter_t filters[0xFF];

filter_f* filter_functions[0x0F];
filter_t filter_RC_lowpass;
filter_t filter_RC_highpass;

am_modulator_t mod1;
am_modulator_t mod2;
am_modulator_t mod3;
uint8_t chord_progress = 0;

uint16_t buff_sz = 18;
uint16_t adc_dma_buff[18];

bool input_active;
int input_key;

USBH_HandleTypeDef hUSBHost; /* USB Host handle */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2S3_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

waveshape_enum cycle_waveshape (waveshape_enum wave) {
	if (wave == SAWTOOTH) {
		wave = SINE;
	} else {
		wave++;
	}
	return wave;
}


/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	//	float output = 0;
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_I2C1_Init();
	MX_I2S3_Init();
	MX_SPI1_Init();
	MX_TIM2_Init();
	MX_ADC1_Init();
	MX_TIM3_Init();
	MX_USART2_UART_Init();
	MX_USB_HOST_Init();
	/* USER CODE BEGIN 2 */

	CS43_Init(hi2c1, MODE_I2S);
	CS43_Enable_RightLeft(CS43_RIGHT_LEFT);
	CS43_SetVolume(5);
	CS43_Start();

	nf_map_init_440(nf_map_440hz);

	f_base = nf_get_f440hz(SEMITONE_C, 0, nf_map_440hz); //use as basis for all subsequent waves

	ns = round(2*I2S_SAMPLE_RATE/f_base);

	wavetable_create(SINE, out_wave_sine, REF_V_DIGITAL, I2S_OUT_N, 1);
	wavetable_create(SQUARE, out_wave_square, REF_V_DIGITAL, I2S_OUT_N, 1);
	wavetable_create(TRIANGLE, out_wave_tri, REF_V_DIGITAL, I2S_OUT_N, 1);
	wavetable_create(SAWTOOTH, out_wave_saw, REF_V_DIGITAL, I2S_OUT_N, 1);

	am_mod_init(&mod1, out_wave_sine, SEMITONE_E, 4);
	am_mod_init(&mod2, out_wave_sine, SEMITONE_G, 4);
	am_mod_init(&mod3, out_wave_sine, SEMITONE_B, 4);

	fc_lp = mixer_get_filter_fc_low();
	fc_hp = mixer_get_filter_fc_high();
	filter_lowpass_RC_init(&filter_RC_lowpass, fc_lp, 1);
	filter_highpass_RC_init(&filter_RC_highpass, fc_hp, 1);

	//  filters[0] = filter_RC_lowpass;
	filters[0] = filter_RC_highpass;
	//  filter_functions[0] = filter_lowpass_RC_get_next;
	filter_functions[0] = filter_highpass_RC_get_next;


	HAL_StatusTypeDef tx_init_status = HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*)i2s_out, I2S_OUT_N);
	//	HAL_I2S_Transmit_DMA(&hi2s3, out_wave_sine, ns);

	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);

	//  HAL_ADC_Start_DMA(&hadc1, adc_dma_buff, 10);
	//  mixer_init(&hadc1);
	//  mixer_DMA_start(&htim3);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */


	while (1)
	{
		MIDI_Application();
		/* USER CODE END WHILE */
		MX_USB_HOST_Process();

		/* USER CODE BEGIN 3 */

		input_key = MIDI_get_key_pressed();
		if (input_key) {
//			HAL_I2S_DMAResume(&hi2s3);
			float exponent = (float)input_key - 69.0f;
			exponent = exponent/12;
			float input_f = 440.0f * powf(2.0f, exponent);
			index_step = input_f/f_base;
			input_active = true;
//			debounce_flag = true;
		} else {
			input_active = false;
		}

		if (input_active) {
			//filters go here
		} else if (MIDI_get_key_released()){
//			uint16_t current_val = out_wave_sine[(uint16_t)in_wave_index];
//			for (uint16_t i = 0; i < I2S_OUT_N; i++) {
//				i2s_out[i] = 0;
//			}
//			in_wave_index = 0;
			index_step = 0;
//			HAL_I2S_DMAPause(&hi2s3);
		}

		if (i2s_tx_half) {
			HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_SET);
			in_wave_index = wavetable_outwave_update(i2s_out, 0, I2S_OUT_N/2, out_wave_tri, in_wave_index, index_step);
			i2s_tx_half = false;
		}

		if (i2s_tx_cplt) {
			HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);
			in_wave_index = wavetable_outwave_update(i2s_out, I2S_OUT_N/2, I2S_OUT_N, out_wave_tri, in_wave_index, index_step);
			i2s_tx_cplt = false;
		}





		//		if (MIDI_RX_Buffer[2] == 69) {
		//			index_step = pow(OCTAVE_STEP, 12*4 + SEMITONE_A);
		//		}

		//		if (i2s_tx_cplt) {
		//			uint16_t out_val = out_wave_sine[(uint16_t)floor(out_index_f)];
		//    	out_val = am_modulate(&mod1, out_val, ns);
		//    	out_val = am_modulate(&mod2, out_val, ns);
		//    	out_val = am_modulate(&mod3, out_val, ns);
		//    	out_val = filter_apply(filter_functions, filters, out_val, 1);
		//			i2s_out[0] = out_val;
		//			i2s_out[1] = out_val;
		//			out_index_f = out_index_f + index_step;
		//			if (out_index_f >= ns) {
		//				out_index_f = out_index_f - ns;
		//			}
		//
		//			i2s_tx_cplt = false;
		//		}

		if (debounce_flag) {
			debounce_cnt++;
			if (debounce_cnt >= debounce_limit) {
				debounce_flag = false;
				debounce_cnt = 0;
			}
		}

		if (key_pressed) {
			key_pressed = false;
			if(chord_progress == 0) {
				index_step = pow(OCTAVE_STEP, 12*4 + SEMITONE_F);
				//				float wavetable_outwave_update(i2s_out, I2S_OUT_N, out_wave_sine, 0, index_step);
				//				wavetable_create(SINE, i2s_out, REF_V_DIGITAL, ns_a4, 1);
				chord_progress = 1;
			} else {
				index_step = pow(OCTAVE_STEP, 12*8 + SEMITONE_C);
				chord_progress = 0;
				//				wavetable_create(SINE, i2s_out, REF_V_DIGITAL, ns_c4, 1);
			}


			//update buffer
		}


		if (cycle_waveshape_flag) {

			debounce_flag = true;
			//    	filter_lowpass_RC_set_R(&filters[0], filters[0].R[0]+50); //test lp
			//    	filter_highpass_RC_set_R(&filters[1], filters[0].R[0]-50); //test hp
			//    	filter_highpass_RC_set_R(&filters[0], filters[1]);

			//    	if (tone == B) {
			//    		tone = C;
			//    		octave++;
			//    		if (octave > 8) {
			//    			octave = 0;
			//    		}
			//    	} else {
			//    		tone++;
			//    	}

			//    	if (tone == C) {
			//    		tone = B;
			//    		octave--;
			//    		if (octave < 0) {
			//    			octave = 8;
			//    		}
			//    	} else {
			//    		tone--;
			//    	}
			if (chord_progress == 0) {
				//				wavetable_create_sine(out_wave_sine, REF_V_DIGITAL, ns_a4, 1);
				//				HAL_I2S_DMAStop(&hi2s3);
				//				HAL_I2S_Transmit_DMA(&hi2s3, out_wave_sine, ns_a4);
				//				index_step = pow(OCTAVE_STEP, 12*4 + SEMITONE_F);

				//				wavetable_set_f(out_wave_sine, index_step, ns);
				//				memcpy(output_wave, out_wave_square, ns * sizeof(uint16_t));
				//
				//				am_mod_set_tone(&mod1, SEMITONE_A, 4);
				//				am_mod_set_tone(&mod2, SEMITONE_C, 5);
				//				am_mod_set_tone(&mod3, SEMITONE_E, 5);
			}
			else if (chord_progress == 1) {
				//				wavetable_create_sine(out_wave_sine, REF_V_DIGITAL, ns_c4, 1);
				//				HAL_I2S_DMAStop(&hi2s3);
				//				HAL_I2S_Transmit_DMA(&hi2s3, out_wave_sine, ns_c4);
				//				wavetable_create_sine(output_wave, REF_V_DIGITAL, ns_c4, ns, 1);
				//				index_step = pow(OCTAVE_STEP, 12*4 + SEMITONE_D);
				//				wavetable_set_f(out_wave_sine, index_step, ns);

				//				index_step = pow(OCTAVE_STEP, 12*4 + SEMITONE_D);
				//				am_mod_set_tone(&mod1, SEMITONE_F, 4);
				//				am_mod_set_tone(&mod2, SEMITONE_A, 4);
				//				am_mod_set_tone(&mod3, SEMITONE_C, 5);
			}
			else if (chord_progress == 2) {
				//				wavetable_create_sine(out_wave_sine, REF_V_DIGITAL, ns_g4, 1);
				//				HAL_I2S_DMAStop(&hi2s3);
				//				HAL_I2S_Transmit_DMA(&hi2s3, out_wave_sine, ns_g4);
				//				memcpy(output_wave, out_wave_sine, ns * sizeof(uint16_t));
				//				wavetable_create_sine(output_wave, REF_V_DIGITAL, ns_g4, ns, 1);
				//				index_step = pow(OCTAVE_STEP, 12*4 + SEMITONE_C);
				//				wavetable_set_f(out_wave_sine, index_step, ns);
				//				am_mod_set_tone(&mod1, SEMITONE_E, 4);
				//				am_mod_set_tone(&mod2, SEMITONE_G, 4);
				//				am_mod_set_tone(&mod3, SEMITONE_B, 4);
			}

			chord_progress++;
			if(chord_progress == 3) chord_progress = 0;

			cycle_waveshape_flag = false;
		}

		if (get_mixer_update_flag()) {
			fc_lp = mixer_get_filter_fc_low();
			//    	float new_R = filter_lowpass_compute_R(fc_lp, filters[0].C[0]);
			float new_R = 12.95;
			//    	filter_lowpass_RC_set_R(&filters[0], new_R);

			fc_hp = mixer_get_filter_fc_high();
		}
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 7;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 8;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */

	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief I2S3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2S3_Init(void)
{

	/* USER CODE BEGIN I2S3_Init 0 */

	/* USER CODE END I2S3_Init 0 */

	/* USER CODE BEGIN I2S3_Init 1 */

	/* USER CODE END I2S3_Init 1 */
	hi2s3.Instance = SPI3;
	hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
	hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
	hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
	hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_48K;
	hi2s3.Init.CPOL = I2S_CPOL_LOW;
	hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
	hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
	if (HAL_I2S_Init(&hi2s3) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2S3_Init 2 */

	/* USER CODE END I2S3_Init 2 */

}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 4294967295;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_TIMING;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 137;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 65535;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_TIMING;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 31250;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
			|Audio_RST_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : DATA_Ready_Pin */
	GPIO_InitStruct.Pin = DATA_Ready_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DATA_Ready_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : CS_I2C_SPI_Pin */
	GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : INT1_Pin INT2_Pin MEMS_INT2_Pin */
	GPIO_InitStruct.Pin = INT1_Pin|INT2_Pin|MEMS_INT2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
	GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PDM_OUT_Pin */
	GPIO_InitStruct.Pin = PDM_OUT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PA0 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : CLK_IN_Pin PB12 */
	GPIO_InitStruct.Pin = CLK_IN_Pin|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
	GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
			|Audio_RST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin : PD0 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
	GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : TEST_PIN_Pin */
	GPIO_InitStruct.Pin = TEST_PIN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TEST_PIN_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
//}
//
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
//	if (hadc == &hadc1) {
//	}
//}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef* hi2s) {
	if (hi2s == &hi2s3) {
		//		out_index_f = wavetable_outwave_update(i2s_out, I2S_OUT_N/2, out_wave_sine, out_index_f, index_step);
		i2s_tx_half = true;
	}
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef* hi2s) {
	if (hi2s == &hi2s3) {
		i2s_tx_cplt = true;
		//		out_index_f = wavetable_outwave_update(i2s_out, I2S_OUT_N, out_wave_sine, out_index_f, index_step);
		//		i2s_out[0] = i2s_out_next[0];
		//		i2s_out[1] = i2s_out_next[1];
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_0 && debounce_flag == false) {
		//		cycle_waveshape_flag = true;
		key_pressed = true;
	}
}


/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
