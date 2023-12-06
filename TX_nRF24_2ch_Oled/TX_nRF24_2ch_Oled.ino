
//*********************************************************************************************************************
// Add communication nRF24L01P. Fixed RF channel, fixed address. Support for Arduino-based receivers
// and RF24 libraries from this repository https://github.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo
// Thanks to the original authors "Gabapentin" https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control       
// and "doohans" https://github.com/doohans/arduino_surface_TX_4ch for code inspiration.
//*********************************************************************************************************************


#include <RF24.h>     // https://github.com/nRF24/RF24
#include <SPI.h>      // Arduino standard library
#include <EEPROM.h>   // Arduino standard library
#include <U8g2lib.h>  // https://github.com/olikraus/u8g2
#include "Config.h"   // Load static and variable configuration settings

//*********************************************************************************************************************
// initial main settings
//*********************************************************************************************************************
void setup()
{
  Serial.begin(9600); // print value on a serial monitor
  
  radio_setup();
  
  //-----------------------------------------------------------------
  // Debouncing mechanical buttons
  // NOTE: For input pin buttons is necessary to mount on every pin
  // 0,1uF/100nF(104)ceramic capacitors from pin to GND
  //-----------------------------------------------------------------
  pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
  pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SELECT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_EXIT, INPUT_PULLUP);
  
  // LCD config with U8G2 library display init (mandatory)
  //u8g2.setBusClock(800000); //max 800000
  u8g2.begin();
  //u8g2.setFlipMode(1);   
  //u8g2.setContrast(10);
  // Set default font type used for all display sessions (mandatory)
  //u8g2.setFont(u8g2_font_6x10_tr); // height 7 pixels (X11)
  //u8g2.setFont(u8g2_font_6x13_tr); // height 9 pixels (X11)
  //u8g2.setFont(u8g2_font_7x13_tr); // height 9 pixels (X11)
  u8g2.setFont(u8g2_font_8x13_tr); // height 9 pixels (X11)
  //u8g2.setFont(u8g2_font_7x14_tr); // height 10 pixels (X11)
  //u8g2.setFont(u8g2_font_9x15_tr); // height 10 pixels (X11)
  //u8g2.setFont(u8g2_font_9x18_tr); // height 10 pixels (X11)
  
  // print boot screen
  u8g2.firstPage(); do {
    
    // Print version string
    u8g2.setCursor(0, 28);
    u8g2.print(ver_str);

  } while (u8g2.nextPage());
  
  delay(1000);
  
  
  // Default state config parameters
  // REVERSE bit mask: 0 Normal, 1 Reverse
  reverse = 0b00000000;
  
  // EPA default values
  for (int i = 0; i < 4; i++)
  {
    epa[i] = EPA_MAX;
  }
  
  // SUB TRIM default values
  for (int i = 0; i < CHANNELS; i++)
  {
    subTrim[i] = 0;
  }
  
  // Default MODEL NAME 5 byte
  for (int i = 0; i < 5; i++)
  {
    modelName[i] = 0x5f;
  }
  
  // NOTE: SHOULD BE USED FOR THE FIRST TIME AFTER CALIBRATION !!!
  resetEeprom_screen(); // print "ERASE DATA" screen
  
  // Load data from Eeprom
  modelActual = storedDataEeprom(255);
}

//*********************************************************************************************************************
// program loop
//*********************************************************************************************************************
void loop()
{
  // Start Calibration screen if button SELECT is pressed on power on
  if (calibStatus == 1 && read_button() == 2)
  {
    // Recall calibration procedure
    Calibration();
  }
  // Set condition 0 to continue loop if calibration procedure is not selected
  else
  {
    calibStatus = 0;
  }
  
  
  send_and_receive_data();
  
  TX_batt_check();         // Checking TX battery status
  
  read_button_exit();      // Macro for read button status definitions
  read_pots();             // Macro for read pots, joysticks, values, applying calibration and rules
  select();                // Select screen, calibration, step control for channels/values
}
 
