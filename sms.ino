//SMS notifications through GSM module
#include <SoftwareSerial.h>
#include "config.h"

SoftwareSerial gsm(GSM_RX_PIN, GSM_TX_PIN);

void initGSM() {
  gsm.begin(9600);
  delay(1000);
  gsm.println("AT");                 // handshake
  delay(500);
  gsm.println("AT+CMGF=1");          // text mode
  delay(500);
  Serial.println("GSM module initialised");
}

bool sendSMS(const String &number, const String &message) {
  gsm.print("AT+CMGS=\"");
  gsm.print(number);
  gsm.println("\"");
  delay(1000);
  gsm.print(message);
  delay(300);
  gsm.write(26);                    
  delay(4000);

  // Basic response check
  String resp = "";
  while (gsm.available()) resp += (char)gsm.read();
  bool ok = resp.indexOf("OK") != -1;
  Serial.println(ok ? "SMS sent" : "SMS may have failed");
  return ok;
}

void sendLowGasSMS(float level) {
  String msg = "LPG ALERT: Cylinder level is at " + String(level, 0);
  sendSMS(USER_PHONE, msg);
}

void sendLeakSMS() {
  sendSMS(USER_PHONE, "DANGER: Gas leak detected! Turn off the regulator, open windows and avoid switches.");
}

void sendFireSMS() {
  sendSMS(USER_PHONE, "FIRE ALERT: Flame detected near your LPG cylinder! Evacuate and call emergency services.");
}
