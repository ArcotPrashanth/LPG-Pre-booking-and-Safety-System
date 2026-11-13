float getTemperature()
{
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    return -1; // when reading temperature fails
  }
  else {
    return event.temperature; // returns proper temperature
  }
}

float getHumidity()
{
  sensors_event_t event;
  // Get humidity
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    return -1; // when reading humidity fails
  }
  else {
    return event.relative_humidity; // return proper humidity
  }
}

int getSmokeSensor()
{
  //Serial.println(analogRead(gasSensorPin));
  if (analogRead(gasSensorPin) > 800)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
//
int getFlameSensor()
{
  if (digitalRead(flameSensorPin) == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int getCurrentLevel()
{
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  float weight = scale.get_units() * -1; 
  Serial.println(weight);
  int percent = weight/setWeight * 100;
  return percent;
}
