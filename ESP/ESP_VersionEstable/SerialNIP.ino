void SerialNIP() {
  static boolean estadoNip = false;

  if (Serial2.available()) {
    char nipSerial = Serial2.read();

    if ((false == estadoNip) && (38 == nipSerial)) {
      digitalWrite(pinLED, HIGH);
      NIP = "";
      estadoNip = true;
    }

    else if ((true == estadoNip) && (38 != nipSerial)) {
      NIP = NIP + nipSerial;
    }

    else if ((true == estadoNip) && (38 == nipSerial)){
      estadoNip = false;
      Serial.println(NIP);
    }
  }
  envioPHP();
}
