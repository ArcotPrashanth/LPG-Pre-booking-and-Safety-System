// config.h - shared settings for the LPG-Pre-booking-and-Safety-System

#ifndef CONFIG_H
#define CONFIG_H

//  Firebase
#define API_KEY 				 "API_KEY"
#define DATABASE_URL     "https://example-database.firebaseio.com"

//  WiFi 
#define WIFI_SSID        "YOUR_WIFI_SSID"
#define WIFI_PASSWORD    "YOUR_WIFI_PASSWORD"

//  Registered user 
#define USER_PHONE       "+91XXXXXXXXXX"
#define USER_EMAIL       "user@example.com"

//  Pins (NodeMCU - ESP8266)
// #define HX711_DT_PIN     D2
// #define HX711_SCK_PIN    D1
// #define MQ2_PIN          A0     // smoke / LPG analog input
// #define FLAME_PIN        D0     // flame sensor digital output (LOW = flame)
// #define BUZZER_PIN       D7
// #define ALERT_LED_PIN    D8
// #define GSM_RX_PIN       D5     // ESP RX  <- SIM800L TX
// #define GSM_TX_PIN       D6     // ESP TX  -> SIM800L RX

//  Thresholds 
// #define GAS_LEAK_THRESHOLD   400    // raw ADC value from MQ-2 (calibrate in clean air)
// #define EMPTY_CYLINDER_KG    15.0   // tare weight of empty cylinder (kg)
// #define FULL_CYLINDER_KG     29.5   // full 14.2 kg cylinder (kg)
// #define LOW_LEVEL_PERCENT    20     // notify + auto-book below this %
// #define ALERT_COOLDOWN_MS    300000UL  // 5 min between repeated notifications

//  Load cell 
// Value obtained from the Load Calibration sketch
// #define LOADCELL_CALIBRATION_FACTOR  -7050.0

#endif
