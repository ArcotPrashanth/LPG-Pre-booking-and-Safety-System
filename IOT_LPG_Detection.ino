//library for display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//library for DHT sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//library for wifi
#include <ESP8266WiFi.h>

//library for email
#include <ESP_Mail_Client.h>

//library for firebase
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <addons/TokenHelper.h>

//library for loadCell
#include "HX711.h" 
 
#define DHTPIN 2
#define DHTTYPE    DHT11
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1// Reset pin # (or -1 if sharing Arduino reset pin)

//Load cell pinouts
#define DOUT  16
#define CLK  13

//wifi credentials
#define WIFI_SSID "abcd"
#define WIFI_PASSWORD "ZXCV@098"

//email setup
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT esp_mail_smtp_port_465

//email credentials
#define AUTHOR_EMAIL "<Fetch from config>"
#define AUTHOR_PASSWORD "<Fetch from config>"

//fire base credentials
/* 2. Define the API Key */
#define API_KEY "<Fetch from config>"
/* 3. Define the RTDB URL */
#define DATABASE_URL "<Fetch from config>"
/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "<Fetch from config>"
#define USER_PASSWORD "<Fetch from config>"

const int flameSensorPin = 14;
const int gasSensorPin = A0;
const int fanPin = 12;     
const char rootCACert[] PROGMEM = "-----BEGIN CERTIFICATE-----\n"
                                  "-----END CERTIFICATE-----\n";
int thresholdLevel = 20;
float setWeight = 4;
unsigned int lastTimeUploaded = 0;

//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor =  -35525;
;

//This object handles display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//This object handles the DHT sensor
DHT_Unified dht(DHTPIN, DHTTYPE);
//This object handles the DHT
HX711 scale(DOUT, CLK);
/* The SMTP Session object used for Email sending */
SMTPSession smtp;
/* Declare the session config data */
ESP_Mail_Session session;

//Define Firebase Data object
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

void setup() {

  pinMode(fanPin, OUTPUT);
  pinMode(flameSensorPin, INPUT);
  
  digitalWrite(fanPin,LOW);
  
  Serial.begin(115200);

  //Init the OLED display
  Serial.println(F("INIT DISPLAY"));
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  
  // Clear the display buffer
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  displayText("connecting.....", 0, 0);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  display.clearDisplay();
  Serial.println("");
  Serial.println("WiFi connected.");
  
  setupEmail();
  firebaseSetup();
  // Initialize sensor.
  dht.begin();
  thresholdLevel = getFirebaseDataStr("/threshold");
  Serial.println("Threshold data:"+String(thresholdLevel));
  setFirebaseIntData(0,"/FlameDetected");
  setFirebaseIntData(0,"/GasLeak");

  displayText("connected!", 0, 0);
  delay(2000);
  display.clearDisplay();
}

void loop() {
  
  int currentGasPercentage = getCurrentLevel();
  float currentTemperature = getTemperature();
  float currentHumidity = getHumidity();

  if (getSmokeSensor())
  {
    //send email about LPG leak
    
    //update firebase 
    setFirebaseIntData(1,"/GasLeak");
    while (getSmokeSensor())
    {
      //activate the exaust fan
      displayText("!!ALERT!!", 20, 0);
      displayText("LPG LEAK", 20, 20);

      //turn on the buzzer
      digitalWrite(fanPin,HIGH);
      delay(2000);
      //turn off the buzzer
      digitalWrite(fanPin,LOW);
    }
  }

  if (getFlameSensor())
  {
    //send email about fire
    setFirebaseIntData(1,"/FlameDetected");
    digitalWrite(fanPin,HIGH);
    while (getFlameSensor())
    {
      displayText("!!ALERT!!", 20, 0);
      displayText("FIRE", 20, 20);

      //turn on the buzzer
      digitalWrite(fanPin,HIGH);
      delay(2000);
      //turn off the buzzer
      digitalWrite(fanPin,LOW);
    }
  }
  
  Serial.println(thresholdLevel);
  Serial.println(currentGasPercentage);
  
  if (thresholdLevel > currentGasPercentage)
  {
    //send email to the user
    Serial.println("Low fuel");
    sendEmail("LOW ON LPG!\n\nCurrent Gas Level : " + String(currentGasPercentage) + "\n\nPlace order soon!");
    thresholdLevel -= 5;
    delay(10000);
    
  }

  //update data every one minute
  if (millis() - lastTimeUploaded > 60000)
  {
    setJsonData(currentGasPercentage,currentTemperature,currentHumidity);
    lastTimeUploaded = millis();
  }

  displayText("LPG " + String(currentGasPercentage), 0, 0);
  displayText("T : " + String(currentTemperature), 0, 22);
  displayText("H : " + String(currentHumidity), 0, 38);
  delay(2000);
  display.clearDisplay();
}
