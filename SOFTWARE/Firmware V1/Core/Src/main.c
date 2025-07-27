/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>

#include "MyCode/config.h"
#include "Libraries/Display/ssd1306.h"
#include "MyCode/display.h"
#include "Libraries/temperature.h"
#include "MyCode/materials.h"
#include "MyCode/buttons.h"
#include "MyCode/buzzer.h"
#include "MyCode/pid.h"
#include "MyCode/settings.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* TIMERS */
uint32_t timerLedAlive = 0;
uint32_t timerDisplayUpdate = 0;
uint32_t timerTempRead = 0;
uint32_t timerTimer = 10000; // not zero to make the first popup at the startup of the device

/* MENUS */
volatile uint8_t currentMenu = MENU_HOME; // active menu (starts with the home menu)

/* MATERIALS */
volatile uint8_t currentMaterialID;

/* TIMER */
volatile uint16_t minutesLeft = 0;	// 2h10m

/* TEMPERATURE CONTROL */
volatile float kp = 20.0f;
volatile float ki = 1.0f;
volatile float kd = 0.2f;
uint8_t runPid = 0;	// 1 if pid must be run
volatile uint8_t duty = 0;
volatile uint8_t heaterOn = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

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
	MX_ADC1_Init();
	MX_I2C1_Init();
	MX_USART2_UART_Init();
	MX_TIM16_Init();
	MX_TIM3_Init();
	MX_TIM17_Init();
	/* USER CODE BEGIN 2 */
	/* -------------------------------- INITIALIZATIONS -------------------------------- */
	/* DISPLAY INIT*/
	ssd1306_Init();
	/* ADC INIT */
	HAL_Delay(50);
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_Delay(50);
	HAL_ADC_Init(&hadc1);
	/* BUTTONS INIT */
	initButtons();
	/* UART INIT */
	HAL_UART_Init(&huart2);
	/* MATERIALS */
	MATERIAL_initMaterials();
	currentMaterialID = MATERIAL_ID_PLA;
	/* BUZZER INIT */
	HAL_TIM_Base_Init(&htim16);
	HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
	TIM16->CCR1 = 0;
	/* HEATER INIT */
	HAL_TIM_Base_Init(&htim17);
	HAL_TIM_Base_Start_IT(&htim17);
	HAL_TIM_PWM_Start_IT(&htim17, TIM_CHANNEL_1);
	setDuty(0);
	/* INIT 500ms PID TIMER */
	HAL_TIM_Base_Init(&htim3);
	HAL_TIM_Base_Start_IT(&htim3);
	/* LOAD SETTINGS FROM EEPROM */
	HAL_Delay(2000);	// otherwise the serial debug will not work
	if (read_byte_eeprom(EEPROM_ISFIRSTBOOT_ADDRESS) != FIRST_TIME_EEPROM_VALUE) {// if this is the first time eeprom is written or is empty, write the default settings
		printString("First boot, or eeprom was empty\n");
		storeSettingsToEeprom(kp, ki, kd);	// store settings

		loadSettingsFromEeprom(&kp, &ki, &kd);	// than read settings for a check
		uint8_t message[100];
		sprintf(message, "Stored default kp, ki, kd value (%f, %f, %f)\n", kp, ki, kd);
		printString(message);
		write_byte_eeprom(EEPROM_ISFIRSTBOOT_ADDRESS, FIRST_TIME_EEPROM_VALUE);
		printString("Now the eeprom was initialized the first time\n");
	} else {
		loadSettingsFromEeprom(&kp, &ki, &kd);
		uint8_t message[100];
		sprintf(message, "Loaded kp, ki, kd value from eeprom (%f, %f, %f)\n", kp, ki, kd);
		printString(message);
	}

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* BUTTON CODE */
		uint8_t relase = BUTTONS_tick();

		/* TEMPERATURE CODE */
		float realTemp = getTemperature();	// caluclate the real temp

		/* TIMER CHECK */
		if (minutesLeft <= 0) {
			minutesLeft = 0;
			heaterOn = 0;
			// make some noise, repeat for some time than do nothing !!! NOISE MADE ON THE PERIOD ROUTIN !!!
		} else {
			heaterOn = 1;
		}

		if (heaterOn) {
			HAL_GPIO_WritePin(LED_LIGHT_GPIO_Port, LED_LIGHT_Pin, 1);	// turn on the light
			if (runPid) {	// run the pid control every 500ms
				duty = pid_control(kp, ki, kd, MATERIAL_getMaterialTargetTemp(currentMaterialID), realTemp, UINT8_MAX);
				setDuty(duty);
				runPid = 0;
			}
		} else {
			HAL_GPIO_WritePin(LED_LIGHT_GPIO_Port, LED_LIGHT_Pin, 0); // turn off the light
			duty = 0;
			setDuty(duty);
		}

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		/* DISPLAY UPDATE */
		if (HAL_GetTick() - timerTimer >= MINUTES_STEP * 60 * 1000) {
			//if (HAL_GetTick() - timerTimer >= 10000) {// decrement every 10seconds for testing
			if (minutesLeft - MINUTES_STEP == 0 || minutesLeft <= 0) {
				minutesLeft = 0;
				// display the popup
				ssd1306_Fill(Black);
				DISPLAY_PrintStrLarge("TIME OVER", 14, 0);
				DISPLAY_PrintStrMedium("Set new time!", 18, 22);
				ssd1306_UpdateScreen();
				// make some sound
				BUZZER_shortPressBeep();
				BUZZER_shortPressBeep();
				HAL_Delay(200);
				BUZZER_shortPressBeep();
				BUZZER_shortPressBeep();
				HAL_Delay(200);
				BUZZER_shortPressBeep();
				BUZZER_shortPressBeep();
				HAL_Delay(200);
				BUZZER_shortPressBeep();
				BUZZER_shortPressBeep();
				HAL_Delay(1000);
				BUZZER_shortPressBeep();
				BUZZER_shortPressBeep();
				HAL_Delay(200);
				BUZZER_shortPressBeep();
				BUZZER_shortPressBeep();
				HAL_Delay(200);
				BUZZER_shortPressBeep();
				BUZZER_shortPressBeep();
				HAL_Delay(200);
				BUZZER_shortPressBeep();
				BUZZER_shortPressBeep();
				HAL_Delay(1000);
			} else {
				minutesLeft -= MINUTES_STEP;
				BUZZER_longPressBeep();
			}
			timerTimer = HAL_GetTick();
		}
		if (HAL_GetTick() - timerDisplayUpdate >= 1000 / DISPLAY_FPS) {
			DISPLAY_drawMenu(currentMenu, currentMaterialID, realTemp, minutesLeft, duty);

			if (relase) {
				DISPLAY_PrintStrSmall("RELASE", 50, 24);
			}

			ssd1306_UpdateScreen();
			timerDisplayUpdate = HAL_GetTick();
		}
		/* LED ALIVE BLINK */
		if (HAL_GetTick() - timerLedAlive >= LED_ALIVE_FLASHING_PERIOD / 2) {
			HAL_GPIO_TogglePin(LED_ALIVE_GPIO_Port, LED_ALIVE_Pin);
			timerLedAlive = HAL_GetTick();
		}
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
	RCC_OscInitStruct.PLL.PLLN = 8;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim3) {
		runPid = 1;
	}
	if (htim == &htim17) {
		if (duty != 0 && currentMenu != MENU_AUTOCAL_1) {
			HAL_GPIO_WritePin(LED_HEATING_GPIO_Port, LED_HEATING_Pin, 1);
		}
	}
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim17) {
		if (currentMenu != MENU_AUTOCAL_1) {
			HAL_GPIO_WritePin(LED_HEATING_GPIO_Port, LED_HEATING_Pin, 0);
		}

	}
}
void printString(char *str) {
	HAL_UART_Transmit(&huart2, str, strlen(str), HAL_MAX_DELAY);
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
