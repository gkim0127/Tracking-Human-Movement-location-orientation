/**
  ******************************************************************************
  * @file    main.c

	#CS704

	README:
	1. BLE and USB-CDC Serial Debug is already done for you.
		1.1 BLE - When connected to the STM BLE Sensor App -
			data in 9 global variables :
			ACC_Value.x, ACC_Value.y, ACC_Value.z &
			MAG_Value.x, MAG_Value.y, MAG_Value.z &
			COMP_Value.Steps , COMP_Value.Heading, COMP_Value.Distance
			gets sent to the app and plotted in the graphs Accelerometer, Magnetometer and Gyroscope respectively.
		1.2 Serial Debug - Use XPRINTF("",param1,..) to print debug statements to USB-UART
	2. Intialise all sensors, variables, etc in the Block ***Intialise Here***
	3. Read sensors and process the sensor data in the block ***READ SENSORS & PROCESS**
		3.1 This block gets executed every 100ms, when ReadSensor gets set by TIM4 - you can change TIM4 clock input to change this, in the InitTimers function
	4. The device name seen by the ST BLE Sensor app is set by the NodeName[1] - NodeName[7] variables, change them in ***Intialise here**** block
	5. The SPI Read and Write functions have been written for you. Example usage for these are in the function InitLSM(). Noted there are separate functions
		for magnetometer and accelerometer.
	6. startAcc and startMag are empty functions to initialise acc and mag sensors - use hints in these functions
	7. readAcc and readMag are empty functions to read acc and mag sensors - use hints in these functions
	8. SensorTile has multiple SPI devices on the same bus as LSM303AGR(acc and mag sensor). It is critical to keep the I2C interface disabled for all sensors.
		9.1 For this - Bit 0 in Register 0x23 in accelerometer should always be SET(1)
		9.2 For this - Bit 5 in Register 0x62 in magnetometer should always be SET(1)
		9.3 Examples for these are present in InitLSM function.
		9.4 Failure to do (8), this will prevent acc and mag sensors from responding over SPI
	9. Important parts of the code are marked with #CS704


*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

#include <limits.h>
#include "main.h"

#include "ble_interface.h"
#include "bluenrg_utils.h"
#include "SensorTile.h"

#include <stdbool.h>


#ifdef ALLMEMS1_ENABLE_PRINTF
  #include "usbd_core.h"
  #include "usbd_cdc.h"
  #include "usbd_cdc_interface.h"
#endif /* ALLMEMS1_ENABLE_PRINTF */

#include "SensorTile_bus.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

/* for ACC */
#define STATUS_REG_A 0x27

#define CTRL_REG1_A 0x20
#define CTRL_REG2_A 0x21
#define CTRL_REG3_A 0x22
#define CTRL_REG4_A 0x23

#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29

#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B

#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D


/* for MAG */
#define ZYXDA_BIT 0x08
#define STATUS_REG_M  0x67

#define CFG_REG_A_M   0x60
#define CFG_REG_B_M   0x61
#define CFG_REG_C_M   0x62


#define OUTX_L_REG_M  0x68
#define OUTX_H_REG_M  0x69

#define OUTY_L_REG_M  0x6A
#define OUTY_H_REG_M  0x6B

#define OUTZ_L_REG_M  0x6C
#define OUTZ_H_REG_M  0x6D

/* Defined values ------------------------------------------------------------- */
#define PI 3.141592
#define NUM_SAMPLES 5
#define STEP_START_THRESHOLD 1100
#define STEP_FINISH_THRESHOLD 900
#define MAGNITUDE_THRESHOLD 0
#define STRIDE_LENGTH 57


/* Shutdown mode enabled as default for SensorTile */
#define ENABLE_SHUT_DOWN_MODE 0


#define BSP_LSM6DSM_INT2_GPIO_PORT           GPIOA
#define BSP_LSM6DSM_INT2_GPIO_CLK_ENABLE()   __GPIOA_CLK_ENABLE()
#define BSP_LSM6DSM_INT2_GPIO_CLK_DISABLE()  __GPIOA_CLK_DISABLE()
#define BSP_LSM6DSM_INT2                 GPIO_PIN_2
#define BSP_LSM6DSM_INT2_EXTI_IRQn           EXTI2_IRQn

#define BSP_LSM6DSM_CS_PORT GPIOB
#define BSP_LSM6DSM_CS_PIN GPIO_PIN_12
#define BSP_LSM6DSM_CS_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()

#define BSP_LSM303AGR_M_CS_PORT GPIOB
#define BSP_LSM303AGR_M_CS_PIN GPIO_PIN_1
#define BSP_LSM303AGR_M_CS_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()

#define BSP_LSM303AGR_X_CS_PORT GPIOC
#define BSP_LSM303AGR_X_CS_PIN GPIO_PIN_4
#define BSP_LSM303AGR_X_CS_GPIO_CLK_ENABLE()  __GPIOC_CLK_ENABLE()

#define BSP_LPS22HB_CS_PORT GPIOA
#define BSP_LPS22HB_CS_PIN GPIO_PIN_3
#define BSP_LPS22HB_CS_GPIO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()


#define LSM_MAG_CS_LOW() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
#define LSM_MAG_CS_HIGH() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);

#define LSM_ACC_CS_LOW() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,GPIO_PIN_RESET);
#define LSM_ACC_CS_HIGH() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,GPIO_PIN_SET);

/* Imported Variables -------------------------------------------------------------*/
extern uint8_t set_connectable;
extern int connected;

#ifdef ALLMEMS1_ENABLE_PRINTF
   extern USBD_DescriptorsTypeDef VCP_Desc;
#endif /* ALLMEMS1_ENABLE_PRINTF */
#ifdef ALLMEMS1_ENABLE_PRINTF
  extern TIM_HandleTypeDef  TimHandle;
  extern void CDC_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#endif /* ALLMEMS1_ENABLE_PRINTF */

/* BlueNRG SPI */
extern SPI_HandleTypeDef SPI_SD_Handle;

extern volatile float RMS_Ch[];
extern float DBNOISE_Value_Old_Ch[];
extern uint16_t PCM_Buffer[];
extern uint32_t NumSample;

/* Exported Variables -------------------------------------------------------------*/
extern SPI_HandleTypeDef hbusspi2;

extern uint8_t NodeName[8];

//float sensitivity;

/* Acc sensitivity multiply by FROM_MG_TO_G constant */
//float sensitivity_Mul;

#ifdef ALLMEMS1_ENABLE_PRINTF
  USBD_HandleTypeDef  USBD_Device;
#endif /* ALLMEMS1_ENABLE_PRINTF */

uint32_t ConnectionBleStatus  =0;

uint32_t FirstConnectionConfig =0;
uint8_t BufferToWrite[256];
int32_t BytesToWrite;
TIM_HandleTypeDef    TimCCHandle;
TIM_HandleTypeDef    TimEnvHandle;

/* Defined Variables ---------------------------------------------------------*/

static int stepCount =0;			// check for static and just int
bool stepDetected = false;
bool stepCounted = false;
float oldAccZ =900;
//static float currentHeading =0;
static float initialHeading = -1;
static float totalDistance =0.0;
static float currentPositionX = 0.0;
static float currentPositionY = 0.0;

uint8_t bdaddr[6];
uint32_t uhCCR4_Val = DEFAULT_uhCCR4_Val;
uint32_t MagCalibrationData[5];
uint32_t AccCalibrationData[7];
uint8_t  NodeName[8];
/* Private variables ---------------------------------------------------------*/
static volatile int MEMSInterrupt        =0;
static volatile uint32_t ReadSensor        =0;
static volatile uint32_t SendAccGyroMag  =0;
static volatile uint32_t TimeStamp = 0;
volatile uint32_t HCI_ProcessEvent=0;
typedef struct  {
	 uint32_t x;
	 uint32_t y;
	 uint32_t Heading;
}COMP_Data;
BSP_MOTION_SENSOR_Axes_t ACC_Value;
COMP_Data COMP_Value;
BSP_MOTION_SENSOR_Axes_t MAG_Value;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

static void Init_BlueNRG_Custom_Services(void);
static void Init_BlueNRG_Stack(void);

static void DeinitTimers(void);
static void InitTimers(void);
static void SendMotionData(void);
static void InitTargetPlatform(void);
static void InitLSM();
uint8_t Sensor_IO_SPI_CS_Init_All(void);
static int32_t BSP_LSM303AGR_WriteReg_Mag(uint16_t Reg, uint8_t *pdata, uint16_t len);
static int32_t BSP_LSM303AGR_ReadReg_Mag(uint16_t Reg, uint8_t *pdata, uint16_t len);
static int32_t BSP_LSM303AGR_WriteReg_Acc(uint16_t Reg, uint8_t *pdata, uint16_t len);
static int32_t BSP_LSM303AGR_ReadReg_Acc(uint16_t Reg, uint8_t *pdata, uint16_t len);
void LSM303AGR_SPI_Read_nBytes(SPI_HandleTypeDef* xSpiHandle, uint8_t *val, uint16_t nBytesToRead);
void LSM303AGR_SPI_Read(SPI_HandleTypeDef* xSpiHandle, uint8_t *val);
void LSM303AGR_SPI_Write(SPI_HandleTypeDef* xSpiHandle, uint8_t val);


static void InitLSM() {
	uint8_t inData[10];
	//setup CS pins on all SPI devices
	Sensor_IO_SPI_CS_Init_All();
	//#CS704 - sample usage of SPI READ and WRITE functions
	//Disable I2C on Acc and Mag - WRITE
	inData[0] = 0x01;
	BSP_LSM303AGR_WriteReg_Acc(0x23,inData,1);
	inData[0] = 0x20;
	BSP_LSM303AGR_WriteReg_Mag(0x62U,inData,1);

	//Read IAM registers for Acc and Mag to verify connection - READ
	BSP_LSM303AGR_ReadReg_Mag(0x4F,inData,1);
	XPRINTF("IAM Mag= %d,%d",inData[0],inData[1]);
	BSP_LSM303AGR_ReadReg_Acc(0x0F,inData,1);
	XPRINTF("IAM Acc= %d,%d",inData[0],inData[1]);
}


static void startMag() {
	//#CS704 - Write SPI commands to initiliase Magnetometer
	uint8_t inData[10]; // Buffer for data to be written to the registers
	// Set specific operational mode and enable temperature compensation
	inData[0] = 0x8C;
	BSP_LSM303AGR_WriteReg_Mag(CFG_REG_A_M,inData,1);
	// Set the data rate for the Magnetometer
	inData[0] = 0x03;
	BSP_LSM303AGR_WriteReg_Mag(CFG_REG_B_M,inData,1);
	// Set the device to single conversion mode and enable BDU
	inData[0] = 0x10;
	BSP_LSM303AGR_WriteReg_Mag(CFG_REG_C_M,inData,1);
}

static void startAcc() {
	//#CS704 - Write SPI commands to initiliase Accelerometer
	uint8_t inData[10];
	// Reset all the settings in control register 2
	inData[0] = 0x00;
	BSP_LSM303AGR_WriteReg_Acc(CTRL_REG2_A,inData,1);
	// Reset all the settings in control register 3
	inData[0] = 0x00;
	BSP_LSM303AGR_WriteReg_Acc(CTRL_REG3_A,inData,1);
	// Set full-scale selection and enable high-resolution output mode
	inData[0] = 0x81;
	BSP_LSM303AGR_WriteReg_Acc(CTRL_REG4_A,inData,1);
	// Set data rate to 12.5 Hz and enable all axes
	inData[0] = 0x57;
	BSP_LSM303AGR_WriteReg_Acc(CTRL_REG1_A,inData,1);
}

static void readMag() {
	// Function to read data from the Magnetometer
	// The function reads the x, y, and z magnetic fields, averages the readings, and applies a hard iron correction.

	uint8_t magStatus;		// checking for magnetometer status
	uint8_t magLSB, magMSB;	// variables to hold LSB and MSB
	int16_t OUTX_NOST_M, OUTY_NOST_M, OUTZ_NOST_M;	// variables to hold combined LSB and MSB
	int16_t magX_mG,magY_mG,magZ_mG; //in milli-Gauss

	float magRange = 50;
	int adcMaxValue = 32767 ; //2^15-1 for 2^16
	float magConversion = magRange / adcMaxValue;

	float magX_G,magY_G,magZ_G;
	int16_t magX_sum, magY_sum, magZ_sum;

	//#CS704 - Read Magnetometer Data over SPI

	// collect 5 samples and get average
	for (int i=0; i< NUM_SAMPLES; i++){

	// check until ZYXDA bit is ready
	do {
		BSP_LSM303AGR_ReadReg_Mag(STATUS_REG_M,&magStatus,1);
		//XPRINTF("WAITING");
	} while (!(magStatus & ZYXDA_BIT));

	// read LSB and MSB for X
	BSP_LSM303AGR_ReadReg_Mag(OUTX_L_REG_M,&magLSB,1);
	BSP_LSM303AGR_ReadReg_Mag(OUTX_H_REG_M,&magMSB,1);
	// store in variable for X, combine LSB and MSB
	OUTX_NOST_M = (int16_t)(magMSB <<8 | magLSB);
	magX_mG = (int16_t)(OUTX_NOST_M * magConversion *1000);		// multiply by mag conversion unit in mg


	// read LSB and MSB for Y
	BSP_LSM303AGR_ReadReg_Mag(OUTY_L_REG_M,&magLSB,1);
	BSP_LSM303AGR_ReadReg_Mag(OUTY_H_REG_M,&magMSB,1);
	OUTY_NOST_M = (int16_t)(magMSB <<8 | magLSB);
	magY_mG = (int16_t)(OUTY_NOST_M * magConversion*1000);

	// read LSB and MSB for Z
	BSP_LSM303AGR_ReadReg_Mag(OUTZ_L_REG_M,&magLSB,1);
	BSP_LSM303AGR_ReadReg_Mag(OUTZ_H_REG_M,&magMSB,1);
	OUTZ_NOST_M = (int16_t)(magMSB <<8 | magLSB);
	magZ_mG = (int16_t)(OUTZ_NOST_M * magConversion*1000);

	// Accumulate the readings for averaging
	magX_sum +=magX_mG;
	magY_sum +=magY_mG;
	magZ_sum +=magZ_mG;

	}

	int16_t hardiron_x = 0;		// Hard iron offset for X
	int16_t hardiron_y = 0;		// Hard iron offset for Y

	//average and apply hard iron offset calcuation
	MAG_Value.x= magX_sum / NUM_SAMPLES - hardiron_x;
	MAG_Value.y= magY_sum / NUM_SAMPLES - hardiron_y;
	MAG_Value.z= magZ_sum / NUM_SAMPLES;

	//XPRINTF("ROW MAG=%d,%d,%d\r\n",OUTX_NOST_M,OUTY_NOST_M,OUTZ_NOST_M);
	// print magnetometer value in milli-Gauss
	//XPRINTF("Converted MAG=%d,%d,%d\r\n",magX_mG, magY_mG, magZ_mG);
}

static void readAcc() {
	uint8_t accStatus;
	uint8_t accLSB, accMSB;
	int16_t OUTX_NOST_A, OUTY_NOST_A, OUTZ_NOST_A;

	const float accFullScale = 2.0;
	const float accConversion = accFullScale / 32768.0; //(2^15 for 2^16)
	int16_t accX_mg, accY_mg, accZ_mg;
	int16_t accX_sum, accY_sum, accZ_sum;

	for (int i=0; i<NUM_SAMPLES; i++){
	//#CS704 - Read Accelerometer Data over SPI
	// check until ZYXDA bit is ready
	do {
		BSP_LSM303AGR_ReadReg_Acc(STATUS_REG_A,&accStatus,1);
		//XPRINTF("WAITING");
	} while (!(accStatus & ZYXDA_BIT));

	// For ACC X
	BSP_LSM303AGR_ReadReg_Acc(OUT_X_L_A,&accLSB,1);
	BSP_LSM303AGR_ReadReg_Acc(OUT_X_H_A,&accMSB,1);
	// shift MSB to the left
	OUTX_NOST_A = (int16_t)(accMSB <<8 | accLSB);
	// multiply by accConversion factor to get +-2g.
	accX_mg = (int16_t)(OUTX_NOST_A * accConversion *1000);

	// For ACC Y
	BSP_LSM303AGR_ReadReg_Acc(OUT_Y_L_A,&accLSB,1);
	BSP_LSM303AGR_ReadReg_Acc(OUT_Y_H_A,&accMSB,1);
	OUTY_NOST_A = (int16_t)(accMSB <<8 | accLSB);
	accY_mg = (int16_t)(OUTY_NOST_A * accConversion *1000);

	// For ACC Z
	BSP_LSM303AGR_ReadReg_Acc(OUT_Z_L_A,&accLSB,1);
	BSP_LSM303AGR_ReadReg_Acc(OUT_Z_H_A,&accMSB,1);
	OUTZ_NOST_A = (int16_t)(accMSB <<8 | accLSB);
	accZ_mg = (int16_t)(OUTZ_NOST_A * accConversion * 1000);

	// Accumulate the readings for averaging
	accX_sum +=accX_mg;
	accY_sum +=accY_mg;
	accZ_sum +=accZ_mg;

	}
	//#CS704 - store sensor values into the variables below
	ACC_Value.x= accX_sum / NUM_SAMPLES;
	ACC_Value.y= accY_sum / NUM_SAMPLES;
	ACC_Value.z= accZ_sum / NUM_SAMPLES;

	//XPRINTF("Row ACC=%d,%d,%d\r\n",OUTX_NOST_A,OUTY_NOST_A,OUTZ_NOST_A);
	// print accelerometer value in milli-g
	//XPRINTF("Converted ACC=%d,%d,%d mg\r\n",accX_mg,accY_mg,accZ_mg);
}



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  HAL_Init();

  // Configure the System clock
  SystemClock_Config();

  InitTargetPlatform();

//  Initialize the BlueNRG
  Init_BlueNRG_Stack();

// Initialize the BlueNRG Custom services
  Init_BlueNRG_Custom_Services();

//  initialize timers
  InitTimers();
//
  if(HAL_TIM_Base_Start_IT(&TimEnvHandle) != HAL_OK){
    // Starting Error
    Error_Handler();
  }
  connected = FALSE;


  //***************************************************
  //***************** #CS704 **************************
  //************ Initialise here **********************
  //***************************************************
  //***************************************************
  //***************************************************


  //#CS704 - use this to set BLE Device Name
  NodeName[1] = 'g';
  NodeName[2] = 'k';
  NodeName[3] = 'i';
  NodeName[4] = 'm';
  NodeName[5] = '9';
  NodeName[6] = '0';
  NodeName[7] = '2';

  startMag();
  startAcc();

  //***************************************************
  //***************************************************
  //************ Initialise ends **********************
  //***************************************************


  /* Infinite loop */
  while (1)
  {
      if(!connected) {
          if(!(HAL_GetTick()&0x3FF)) {
        	  BSP_LED_Toggle(LED1);
          }
      }
    if(set_connectable){

      /* Now update the BLE advertize data and make the Board connectable */
      setConnectable();
      set_connectable = FALSE;
    }

    /* handle BLE event */
    if(HCI_ProcessEvent) {
      HCI_ProcessEvent=0;
      hci_user_evt_proc();
    }

    //***************************************************
    //***************************************************
    //***************** #CS704 **************************
    //*********** READ SENSORS & PROCESS ****************
    //***************************************************
    //***************************************************
    //***************************************************

    //#CS704 - ReadSensor gets set every 100ms by Timer TIM4 (TimEnvHandle)
    if(ReadSensor) {
    	ReadSensor=0;

	//*********get sensor data**********
    	readMag();
    	readAcc();

	//*********process sensor data*********

    /* Calculate Heading */
    float magX_h = MAG_Value.x; // in milli Gauss
    float magY_h = MAG_Value.y;
    float magZ_h = MAG_Value.z;

    //arctang calcuation and radians to degrees
	float currentHeading = (atan2(magY_h,magX_h)*180)/PI;

	// Correct the heading to a range of 0 to 360 degrees
    if (currentHeading<0){
    	currentHeading +=360;
    }

    // Initialise the initial heading if it's not set
    if (initialHeading<0){
    	initialHeading = currentHeading;
    }

    //Calculate the relative heading with respect to the initial heading and convert to 0-360
    float relativeHeading = currentHeading - initialHeading;
    if (relativeHeading <0){
    	relativeHeading += 360;
    }

    // only for printing purpose
    int16_t currentHeadingInt = (int16_t)currentHeading;
    int16_t relativeHeadingInt = (int16_t)relativeHeading;
    XPRINTF("Heading from Mag = %d degrees, Relative Heading from Mag = %d degrees\n", currentHeadingInt, relativeHeadingInt);

    /* Step detection */

    float accX_mg = ACC_Value.x;	// in milli-g
    float accY_mg = ACC_Value.y;
    float accZ_mg = ACC_Value.z;

    int16_t accX_int = (int16_t)accX_mg;
    int16_t accY_int = (int16_t)accY_mg;
    int16_t accZ_int = (int16_t)accZ_mg;

    // Detect the start of a step
    if (!stepDetected && accZ_mg >= STEP_START_THRESHOLD){
    	XPRINTF("Step Started\n");
    	stepDetected = true;
    }
    // Detect the end of the step
    else if (stepDetected && accZ_mg <= STEP_FINISH_THRESHOLD){
    	XPRINTF("Step ended\n");
    	stepDetected = false;
    	if(!stepCounted){
    		stepCount ++;
    		stepCounted = true;		// one step only counted once
    	}
    	XPRINTF("Step count: %d\n", stepCount);
    }
    oldAccZ = accZ_mg;	// Store the old Z acceleration

    /* Position calculation */

    // Calculate the change in position after a step has been counted
    if (stepCounted){
    	float stepDistance = STRIDE_LENGTH; // one step distance
    	totalDistance += stepDistance; // total distance

    	float positionChangeX= stepDistance * sin(relativeHeading* PI/180.0);
    	float positionChangeY= stepDistance * cos(relativeHeading* PI/180.0);

    	// Update the current position
    	currentPositionX += positionChangeX;
    	currentPositionY += positionChangeY;

    	stepCounted = false; // Reset for the next step count

    	// Print the total distance and current position
    	////XPRINTF("Total Distance: %d m, Current Position: (%d, %d)\n", (int16_t)totalDistance, (int16_t)currentPositionX, (int16_t)currentPositionY);
    }

    // Store the current position and relative heading in the COMP_Value
    COMP_Value.x = (int16_t)currentPositionX;
    COMP_Value.y = (int16_t)currentPositionY;
    COMP_Value.Heading = relativeHeadingInt *10;

    }

    //***************************************************
    //***************************************************


    /* Motion Data */
    if(SendAccGyroMag) {
		SendMotionData();
    	SendAccGyroMag=0;
    }

    /* Wait for Event */
    __WFI();
  }
}

/**
  * @brief  Initialize all the Target platform's Features
  * @param  None
  * @retval None
  */
void InitTargetPlatform(void)
{

#ifdef ALLMEMS1_ENABLE_PRINTF
  /* enable USB power on Pwrctrl CR2 register */
  HAL_PWREx_EnableVddUSB();

  /* Configure the CDC */
  /* Init Device Library */
  USBD_Init(&USBD_Device, &VCP_Desc, 0);
  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
  /* Add Interface callbacks for AUDIO and CDC Class */
  USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
  /* Start Device Process */
  USBD_Start(&USBD_Device);
  /* 10 seconds ... for having time to open the Terminal
   * for looking the ALLMEMS1 Initialization phase */
  HAL_Delay(5000);
#endif /* ALLMEMS1_ENABLE_PRINTF */

  /* Initialize LED */
  BSP_LED_Init( LED1 );

  InitLSM(); //N4S
}



/**
  * @brief  Output Compare callback in non blocking mode
  * @param  htim : TIM OC handle
  * @retval None
  */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  uint32_t uhCapture=0;



  /* TIM1_CH4 toggling with frequency = 20 Hz */
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
  {
     uhCapture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
    /* Set the Capture Compare Register value */
    __HAL_TIM_SET_COMPARE(&TimCCHandle, TIM_CHANNEL_4, (uhCapture + uhCCR4_Val));
    SendAccGyroMag=1;
  }
}


/**
  * @brief  Period elapsed callback in non blocking mode for Environmental timer
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{


  if(htim == (&TimEnvHandle)) {
	  ReadSensor=1;

#ifdef ALLMEMS1_ENABLE_PRINTF
    } else if(htim == (&TimHandle)) {
      CDC_TIM_PeriodElapsedCallback(htim);
#endif /* ALLMEMS1_ENABLE_PRINTF */

  }
}

/**
  * @brief  Send Motion Data Acc/Mag/Gyro to BLE
  * @param  None
  * @retval None
  */
static void SendMotionData(void)
{

  AccGyroMag_Update(&ACC_Value,(BSP_MOTION_SENSOR_Axes_t*)&COMP_Value,&MAG_Value);
}



/**
* @brief  Function for initializing timers for sending the information to BLE:
 *  - 1 for sending MotionFX/AR/CP and Acc/Gyro/Mag
 *  - 1 for sending the Environmental info
 * @param  None
 * @retval None
 */
static void InitTimers(void)
{

  uint32_t uwPrescalerValue;

  /* Timer Output Compare Configuration Structure declaration */
  TIM_OC_InitTypeDef sConfig;

  /* Compute the prescaler value to have TIM4 counter clock equal to 10 KHz Hz */

 // #CS704 -  change TIM4 configuration here to change frequency of execution of *** READ Sensor and Process Block **
  uwPrescalerValue = (uint32_t) ((SystemCoreClock / 10000) - 1);

  /* Set TIM4 instance ( Environmental ) */
  TimEnvHandle.Instance = TIM4;
  /* Initialize TIM4 peripheral */
  TimEnvHandle.Init.Period = 655;
  TimEnvHandle.Init.Prescaler = uwPrescalerValue;
  TimEnvHandle.Init.ClockDivision = 0;
  TimEnvHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_Base_Init(&TimEnvHandle) != HAL_OK) {
    /* Initialization Error */
  }


  /* Compute the prescaler value to have TIM1 counter clock equal to 10 KHz */
  uwPrescalerValue = (uint32_t) ((SystemCoreClock / 10000) - 1);

  /* Set TIM1 instance ( Motion ) */
  TimCCHandle.Instance = TIM1;
  TimCCHandle.Init.Period        = 65535;
  TimCCHandle.Init.Prescaler     = uwPrescalerValue;
  TimCCHandle.Init.ClockDivision = 0;
  TimCCHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
  if(HAL_TIM_OC_Init(&TimCCHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

 /* Configure the Output Compare channels */
 /* Common configuration for all channels */
  sConfig.OCMode     = TIM_OCMODE_TOGGLE;
  sConfig.OCPolarity = TIM_OCPOLARITY_LOW;

  /* Code for MotionFX integration - Start Section */
  /* Output Compare Toggle Mode configuration: Channel1 */
  sConfig.Pulse = DEFAULT_uhCCR1_Val;
  if(HAL_TIM_OC_ConfigChannel(&TimCCHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  /* Code for MotionFX integration - End Section */

  /* Code for MotionCP & MotionGR integration - Start Section */
  /* Output Compare Toggle Mode configuration: Channel2 */
  sConfig.Pulse = DEFAULT_uhCCR2_Val;
  if(HAL_TIM_OC_ConfigChannel(&TimCCHandle, &sConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  /* Code for MotionCP & MotionGR integration - End Section */

  /* Code for MotionAR & MotionID & MotionPE integration - Start Section */
  /* Output Compare Toggle Mode configuration: Channel3 */
  sConfig.Pulse = DEFAULT_uhCCR3_Val;
  if(HAL_TIM_OC_ConfigChannel(&TimCCHandle, &sConfig, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  /* Code for MotionAR & MotionID & MotionPE integration - End Section */

  /* Output Compare Toggle Mode configuration: Channel4 */
  sConfig.Pulse = DEFAULT_uhCCR4_Val;
  if(HAL_TIM_OC_ConfigChannel(&TimCCHandle, &sConfig, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
}


/**
  * @brief  Set all sensor Chip Select high. To be called before any SPI read/write
  * @param  None
  * @retval HAL_StatusTypeDef HAL Status
  */
uint8_t Sensor_IO_SPI_CS_Init_All(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  uint8_t inData[2];
  /* Set all the pins before init to avoid glitch */
  BSP_LSM6DSM_CS_GPIO_CLK_ENABLE();
  BSP_LSM303AGR_M_CS_GPIO_CLK_ENABLE();
  BSP_LSM303AGR_X_CS_GPIO_CLK_ENABLE();
  BSP_LPS22HB_CS_GPIO_CLK_ENABLE();

  HAL_GPIO_WritePin(BSP_LSM6DSM_CS_PORT, BSP_LSM6DSM_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(BSP_LSM303AGR_X_CS_PORT, BSP_LSM303AGR_X_CS_PIN,GPIO_PIN_SET);
  HAL_GPIO_WritePin(BSP_LSM303AGR_M_CS_PORT, BSP_LSM303AGR_M_CS_PIN,GPIO_PIN_SET);
  HAL_GPIO_WritePin(BSP_LPS22HB_CS_PORT, BSP_LPS22HB_CS_PIN, GPIO_PIN_SET);

  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pin = BSP_LSM6DSM_CS_PIN;
  HAL_GPIO_Init(BSP_LSM6DSM_CS_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(BSP_LSM6DSM_CS_PORT, BSP_LSM6DSM_CS_PIN, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = BSP_LSM303AGR_X_CS_PIN;
  HAL_GPIO_Init(BSP_LSM303AGR_X_CS_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(BSP_LSM303AGR_X_CS_PORT, BSP_LSM303AGR_X_CS_PIN,GPIO_PIN_SET);

  GPIO_InitStruct.Pin = BSP_LSM303AGR_M_CS_PIN;
  HAL_GPIO_Init(BSP_LSM303AGR_M_CS_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(BSP_LSM303AGR_M_CS_PORT, BSP_LSM303AGR_M_CS_PIN,GPIO_PIN_SET);

  GPIO_InitStruct.Pin = BSP_LPS22HB_CS_PIN;
  HAL_GPIO_Init(BSP_LPS22HB_CS_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(BSP_LPS22HB_CS_PORT, BSP_LPS22HB_CS_PIN, GPIO_PIN_SET);


  //setup SPI interface
  	 if(BSP_SPI2_Init() == BSP_ERROR_NONE)
  	  {
  		 //XPRINTF("NO SPI Error\r\n");
  	  }

  //disable I2C modes on all SPI devices
  //LSM303 Mag
  HAL_GPIO_WritePin(BSP_LSM303AGR_M_CS_PORT, BSP_LSM303AGR_M_CS_PIN, GPIO_PIN_RESET);
  inData[0] = (0x62U);
  BSP_SPI2_Send(inData,1);
  inData[0] = 0x20;
  BSP_SPI2_Send(inData,1);
  HAL_GPIO_WritePin(BSP_LSM303AGR_M_CS_PORT, BSP_LSM303AGR_M_CS_PIN, GPIO_PIN_SET);

  //LSM303 Acc
  HAL_GPIO_WritePin(BSP_LSM303AGR_X_CS_PORT, BSP_LSM303AGR_X_CS_PIN, GPIO_PIN_RESET);
  inData[0] = (0x23U);
  BSP_SPI2_Send(inData,1);
  inData[0] = 0x01;
  BSP_SPI2_Send(inData,1);
  HAL_GPIO_WritePin(BSP_LSM303AGR_X_CS_PORT, BSP_LSM303AGR_X_CS_PIN, GPIO_PIN_SET);

  //LPS22HB
  HAL_GPIO_WritePin(BSP_LPS22HB_CS_PORT, BSP_LPS22HB_CS_PIN, GPIO_PIN_RESET);
  inData[0] = (0x10U);
  BSP_SPI2_Send(inData,1);
  inData[0] = 0x01;
  BSP_SPI2_Send(inData,1);
  HAL_GPIO_WritePin(BSP_LPS22HB_CS_PORT, BSP_LPS22HB_CS_PIN, GPIO_PIN_SET);

  //LSM6DSM
  HAL_GPIO_WritePin(BSP_LSM6DSM_CS_PORT, BSP_LSM6DSM_CS_PIN, GPIO_PIN_RESET);
  inData[0] = (0x12U);
  BSP_SPI2_Send(inData,1);
  inData[0] = 0x0C;
  BSP_SPI2_Send(inData,1);
  HAL_GPIO_WritePin(BSP_LSM6DSM_CS_PORT, BSP_LSM6DSM_CS_PIN, GPIO_PIN_SET);


  return HAL_OK;
}

/**
 * @brief  Write register by SPI bus for LSM303AGR
 * @param  Reg the starting register address to be written
 * @param  pdata the pointer to the data to be written
 * @param  len the length of the data to be written
 * @retval BSP status
 */
static int32_t BSP_LSM303AGR_WriteReg_Mag(uint16_t Reg, uint8_t *pdata, uint16_t len)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t dataReg = (uint8_t)Reg;

  /* CS Enable */
  LSM_MAG_CS_LOW();

  if (BSP_SPI2_Send(&dataReg, 1) != 1)
  {
    ret = BSP_ERROR_UNKNOWN_FAILURE;
  }

  if (BSP_SPI2_Send(pdata, len) != len)
  {
    ret = BSP_ERROR_UNKNOWN_FAILURE;
  }

  /* CS Disable */
  LSM_MAG_CS_HIGH();

  return ret;
}

/**
* @brief  Read register by SPI bus for LSM303AGR
* @param  Reg the starting register address to be read
* @param  pdata the pointer to the data to be read
* @param  len the length of the data to be read
* @retval BSP status
*/
static int32_t BSP_LSM303AGR_ReadReg_Mag(uint16_t Reg, uint8_t *pdata, uint16_t len)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t dataReg = (uint8_t)Reg;

  /* CS Enable */
  HAL_GPIO_WritePin(BSP_LSM303AGR_M_CS_PORT, BSP_LSM303AGR_M_CS_PIN, GPIO_PIN_RESET);
//  XPRINTF("Data Read = %d,%d\r\n",(dataReg) | 0x80,pdata[0]);
  LSM303AGR_SPI_Write(&hbusspi2, (dataReg) | 0x80);
  __HAL_SPI_DISABLE(&hbusspi2);
  SPI_1LINE_RX(&hbusspi2);

  if (len > 1)
  {
    LSM303AGR_SPI_Read_nBytes(&hbusspi2, (pdata), len);
  }
  else
  {
    LSM303AGR_SPI_Read(&hbusspi2, (pdata));
  }

  /* CS Disable */
  HAL_GPIO_WritePin(BSP_LSM303AGR_M_CS_PORT, BSP_LSM303AGR_M_CS_PIN, GPIO_PIN_SET);
  SPI_1LINE_TX(&hbusspi2);
  __HAL_SPI_ENABLE(&hbusspi2);
  return ret;
}

/**
 * @brief  Write register by SPI bus for LSM303AGR
 * @param  Reg the starting register address to be written
 * @param  pdata the pointer to the data to be written
 * @param  len the length of the data to be written
 * @retval BSP status
 */
static int32_t BSP_LSM303AGR_WriteReg_Acc(uint16_t Reg, uint8_t *pdata, uint16_t len)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t dataReg = (uint8_t)Reg;

  /* CS Enable */
  LSM_ACC_CS_LOW();

  if (BSP_SPI2_Send(&dataReg, 1) != 1)
  {
    ret = BSP_ERROR_UNKNOWN_FAILURE;
  }

  if (BSP_SPI2_Send(pdata, len) != len)
  {
    ret = BSP_ERROR_UNKNOWN_FAILURE;
  }

  /* CS Disable */
  LSM_ACC_CS_HIGH();

  return ret;
}

/**
* @brief  Read register by SPI bus for LSM303AGR
* @param  Reg the starting register address to be read
* @param  pdata the pointer to the data to be read
* @param  len the length of the data to be read
* @retval BSP status
*/
static int32_t BSP_LSM303AGR_ReadReg_Acc(uint16_t Reg, uint8_t *pdata, uint16_t len)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t dataReg = (uint8_t)Reg;

  /* CS Enable */
  LSM_ACC_CS_LOW();
  if (len > 1) {
	  LSM303AGR_SPI_Write(&hbusspi2, (dataReg) | 0x80 | 0x40);
  }
  else {
	  LSM303AGR_SPI_Write(&hbusspi2, (dataReg) | 0x80);
  }
  __HAL_SPI_DISABLE(&hbusspi2);
  SPI_1LINE_RX(&hbusspi2);

  if (len > 1)
  {
    LSM303AGR_SPI_Read_nBytes(&hbusspi2, (pdata), len);
  }
  else
  {
    LSM303AGR_SPI_Read(&hbusspi2, (pdata));
  }

  /* CS Disable */
  LSM_ACC_CS_HIGH();
  SPI_1LINE_TX(&hbusspi2);
  __HAL_SPI_ENABLE(&hbusspi2);
  return ret;
}


/**
* @brief  This function reads multiple bytes on SPI 3-wire.
* @param  xSpiHandle: SPI Handler.
* @param  val: value.
* @param  nBytesToRead: number of bytes to read.
* @retval None
*/
void LSM303AGR_SPI_Read_nBytes(SPI_HandleTypeDef* xSpiHandle, uint8_t *val, uint16_t nBytesToRead)
{
  /* Interrupts should be disabled during this operation */
  __disable_irq();
  __HAL_SPI_ENABLE(xSpiHandle);

  /* Transfer loop */
  while (nBytesToRead > 1U)
  {
    /* Check the RXNE flag */
    if (xSpiHandle->Instance->SR & SPI_FLAG_RXNE)
    {
      /* read the received data */
      *val = *(__IO uint8_t *) &xSpiHandle->Instance->DR;
      val += sizeof(uint8_t);
      nBytesToRead--;
    }
  }
  /* In master RX mode the clock is automaticaly generated on the SPI enable.
  So to guarantee the clock generation for only one data, the clock must be
  disabled after the first bit and before the latest bit of the last Byte received */
  /* __DSB instruction are inserted to garantee that clock is Disabled in the right timeframe */

  __DSB();
  __DSB();
  __HAL_SPI_DISABLE(xSpiHandle);

  __enable_irq();

  while ((xSpiHandle->Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);
  /* read the received data */
  *val = *(__IO uint8_t *) &xSpiHandle->Instance->DR;
  while ((xSpiHandle->Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);
}

/**
* @brief  This function send a command through SPI bus.
* @param  command: command id.
* @param  uint8_t val: value.
* @retval None
*/
void LSM303AGR_SPI_Read(SPI_HandleTypeDef* xSpiHandle, uint8_t *val)
{
  /* In master RX mode the clock is automaticaly generated on the SPI enable.
  So to guarantee the clock generation for only one data, the clock must be
  disabled after the first bit and before the latest bit */
  /* Interrupts should be disabled during this operation */

  __disable_irq();
  __HAL_SPI_ENABLE(xSpiHandle);
  __asm("dsb\n");
  __asm("dsb\n");
  __HAL_SPI_DISABLE(xSpiHandle);
  __enable_irq();


  while ((xSpiHandle->Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);
  /* read the received data */
//  XPRINTF("Before READ, %d,%d\r\n",val[0],xSpiHandle->Instance);
  *val = *(__IO uint8_t *) &xSpiHandle->Instance->DR;
//  XPRINTF("READ, %d\r\n",val[0]);
  while ((xSpiHandle->Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);
}

/**
* @brief  This function send a command through SPI bus.
* @param  command : command id.
* @param  val : value.
* @retval None
*/
void LSM303AGR_SPI_Write(SPI_HandleTypeDef* xSpiHandle, uint8_t val)
{
  /* check TXE flag */
  while ((xSpiHandle->Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);

  /* Write the data */
  *((__IO uint8_t*) &xSpiHandle->Instance->DR) = val;

  /* Wait BSY flag */
  while ((xSpiHandle->Instance->SR & SPI_FLAG_FTLVL) != SPI_FTLVL_EMPTY);
  while ((xSpiHandle->Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);
}




/**
* @brief  Function for De-initializing timers:
 *  - 1 for sending MotionFX/AR/CP and Acc/Gyro/Mag
 *  - 1 for sending the Environmental info
 * @param  None
 * @retval None
 */
static void DeinitTimers(void)
{

  /* Set TIM4 instance (Environmental) */
  TimEnvHandle.Instance = TIM4;
  if(HAL_TIM_Base_DeInit(&TimEnvHandle) != HAL_OK) {
    /* Deinitialization Error */
    Error_Handler();
  }

  /* Set TIM1 instance (Motion)*/
  TimCCHandle.Instance = TIM1;
  if(HAL_TIM_Base_DeInit(&TimCCHandle) != HAL_OK)
  {
    /* Deinitialization Error */
	  Error_Handler();
  }
}

/** @brief Initialize the BlueNRG Stack
 * @param None
 * @retval None
 */
static void Init_BlueNRG_Stack(void)
{
  char BoardName[8];
  char customName[8] = "CSys704";
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
  int ret;
  uint8_t  data_len_out;
  uint8_t  hwVersion;
  uint16_t fwVersion;



//  for(int i=0; i<7; i++)
//    BoardName[i]= NodeName[i+1];

  for(int i=0; i<7; i++)
    BoardName[i]= customName[i];

  BoardName[7]= 0;

  /* Initialize the BlueNRG SPI driver */
  hci_init(HCI_Event_CB, NULL);

  /* get the BlueNRG HW and FW versions */
  getBlueNRGVersion(&hwVersion, &fwVersion);

  aci_hal_read_config_data(CONFIG_DATA_RANDOM_ADDRESS, 6, &data_len_out, bdaddr);

  if ((bdaddr[5] & 0xC0) != 0xC0) {
    XPRINTF("\r\nStatic Random address not well formed.\r\n");
    while(1);
  }

  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, data_len_out,
                                  bdaddr);

/* Sw reset of the device */
  hci_reset();

  ret = aci_gatt_init();
  if(ret){
     XPRINTF("\r\nGATT_Init failed\r\n");
     goto fail;
  }

  ret = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);

  if(ret != BLE_STATUS_SUCCESS){
     XPRINTF("\r\nGAP_Init failed\r\n");
     goto fail;
  }

  ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0,
                                   7/*strlen(BoardName)*/, (uint8_t *)BoardName);

  if(ret){
     XPRINTF("\r\naci_gatt_update_char_value failed\r\n");
    while(1);
  }

  ret = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
                                     OOB_AUTH_DATA_ABSENT,
                                     NULL, 7, 16,
                                     USE_FIXED_PIN_FOR_PAIRING, 123456,
                                     BONDING);
  if (ret != BLE_STATUS_SUCCESS) {
     XPRINTF("\r\nGAP setting Authentication failed\r\n");
     goto fail;
  }

  XPRINTF("SERVER: BLE Stack Initialized \r\n"
         "\tHWver= %d.%d\r\n"
         "\tFWver= %d.%d.%c\r\n"
         "\tBoardName= %s\r\n"
         "\tBoardMAC = %x:%x:%x:%x:%x:%x\r\n\n",
         ((hwVersion>>4)&0x0F),(hwVersion&0x0F),
         fwVersion>>8,
         (fwVersion>>4)&0xF,
         ('a'+(fwVersion&0xF)),
         BoardName,
         bdaddr[5],bdaddr[4],bdaddr[3],bdaddr[2],bdaddr[1],bdaddr[0]);

  /* Set output power level */
  aci_hal_set_tx_power_level(1,4);

  return;

fail:
  return;
}

/** @brief Initialize all the Custom BlueNRG services
 * @param None
 * @retval None
 */
static void Init_BlueNRG_Custom_Services(void)
{
  int ret;

  ret = Add_HW_SW_ServW2ST_Service();
  if(ret == BLE_STATUS_SUCCESS)
  {
     XPRINTF("HW & SW Service W2ST added successfully\r\n");
  }
  else
  {
     XPRINTF("\r\nError while adding HW & SW Service W2ST\r\n");
  }

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 6
  *            PLL_N                          = 40
  *            PLL_R                          = 4
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  /* Enable the LSE Oscilator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
    while(1);
  }

  /* Enable the CSS interrupt in case LSE signal is corrupted or not present */
  HAL_RCCEx_DisableLSECSS();

  /* Enable MSI Oscillator and activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM            = 6;
  RCC_OscInitStruct.PLL.PLLN            = 40;
  RCC_OscInitStruct.PLL.PLLP            = 7;
  RCC_OscInitStruct.PLL.PLLQ            = 4;
  RCC_OscInitStruct.PLL.PLLR            = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
    while(1);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    while(1);
  }

  /* Enable MSI Auto-calibration through LSE */
  HAL_RCCEx_EnableMSIPLLMode();

  /* Select MSI output as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_MSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
  clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK){
    while(1);
  }
}






/**
  * @brief This function provides accurate delay (in milliseconds) based
  *        on variable incremented.
  * @note This is a user implementation using WFI state
  * @param Delay: specifies the delay time length, in milliseconds.
  * @retval None
  */
void HAL_Delay(__IO uint32_t Delay)
{
  uint32_t tickstart = 0;
  tickstart = HAL_GetTick();
  while((HAL_GetTick() - tickstart) < Delay){
    __WFI();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1){
  }
}

/**
 * @brief  EXTI line detection callback.
 * @param  uint16_t GPIO_Pin Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin){
  case HCI_TL_SPI_EXTI_PIN:
      hci_tl_lowlevel_isr();
      HCI_ProcessEvent=1;
    break;

//  case BSP_LSM6DSM_INT2:
//    MEMSInterrupt=1;
//    break;
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: ALLMEMS1_PRINTF("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1){
  }
}
#endif




