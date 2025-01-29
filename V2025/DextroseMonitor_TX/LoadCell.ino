//Calibration Factor: 22.17

void LoadCell_Setup() {
  Serial.println("Load Cell Setup");

  float calibrationValue;           // calibration value
  calibrationValue = 0.0;           // uncomment this if you want to set this value in the sketch
  EEPROM.get(0, calibrationValue);  // uncomment this if you want to fetch this value from eeprom
  Serial.print("Fetch Calibration Factor From EEPROM:");
  Serial.println(calibrationValue);

  LoadCell.begin();
  //LoadCell.setReverseOutput();
  unsigned long stabilizingtime = 2000;  // tare preciscion can be improved by adding a few seconds of stabilizing time
  boolean _tare = true;                  //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
  } else {
    LoadCell.setCalFactor(calibrationValue);  // set calibration factor (float)
    Serial.println("Startup is complete");
  }
  while (!LoadCell.update())
    ;
  Serial.print("Calibration value: ");
  Serial.println(LoadCell.getCalFactor());
  Serial.print("HX711 measured conversion time ms: ");
  Serial.println(LoadCell.getConversionTime());
  Serial.print("HX711 measured sampling rate HZ: ");
  Serial.println(LoadCell.getSPS());
  Serial.print("HX711 measured settlingtime ms: ");
  Serial.println(LoadCell.getSettlingTime());
  Serial.println("Note that the settling time may increase significantly if you use delay() in your sketch!");
  if (LoadCell.getSPS() < 7) {
    Serial.println("!!Sampling rate is lower than specification, check MCU>HX711 wiring and pin designations");
  } else if (LoadCell.getSPS() > 100) {
    Serial.println("!!Sampling rate is higher than specification, check MCU>HX711 wiring and pin designations");
  }
  Serial.println("Done Load Cell Setup");
}

void LoadCell_Loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 500;  //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      currentWeight = i;
      // Serial.print("Load_cell output val: ");
      // Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }


  // check if last tare operation is complete:
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
}