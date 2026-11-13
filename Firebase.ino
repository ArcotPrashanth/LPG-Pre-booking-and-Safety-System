void firebaseSetup()
{
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  config.user.email = USER_EMAIL;
  config.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  
  Firebase.begin(&config);
  //Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);
}

void setJsonData(int currentLPGLevel, float temp, float hum)
{
  FirebaseJson json;
  json.add("currentLPGLevel", currentLPGLevel);
  json.add("temperature", temp);
  json.add("humidity", hum);
  setFirebaseJsonData(&json, "/");
}

void setFirebaseJsonData(FirebaseJson *fJson, String path)
{
  if (Firebase.updateNode(firebaseData, path, *fJson))
  {
    Serial.println("Data sent!");
  }
  else
  {
    Serial.println("REASON: " + firebaseData.errorReason());
  }
}

void setFirebaseIntData(int data, String path)
{
  if (Firebase.setInt(firebaseData, path, data))
  {
    return;
  }
  else
  {
    Serial.println(firebaseData.errorReason());
  }
}

int getFirebaseData(String path)
{
  if (Firebase.getInt(firebaseData, path))
  {
    if (firebaseData.dataTypeEnum() == fb_esp_rtdb_data_type_integer)
    {
      return firebaseData .to<int>();
    }
    else
    {
      Serial.println(firebaseData.errorReason());
      return -1;
    }
  }
  else
  {
    Serial.println(firebaseData.errorReason());
    return -1;
  }
}

int getFirebaseDataStr(String path)
{
  if (Firebase.getString(firebaseData, path))
  {
    if (firebaseData.dataTypeEnum() == fb_esp_rtdb_data_type_string)
    {
      String data = firebaseData.to<String>(); //ex : \"25\"
      String newData = "";
      boolean start = false;
      for(int i = 0; i < data.length(); i++)
      {
        Serial.println(data.charAt(i));
        if(data.charAt(i) == '"')
        {
          start = !start;
          continue;  
        }
        if(start)
        {
          newData += data.charAt(i);
        }
      }
      Serial.println(newData.toInt());
      return newData.toInt();
    }
    else
    {
      Serial.println(firebaseData.errorReason());
      return -1;
    }
  }
  else
  {
    Serial.println(firebaseData.errorReason());
    return -1;
  }
}
