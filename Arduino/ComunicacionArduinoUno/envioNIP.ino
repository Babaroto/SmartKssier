//Función de envio de datos a la ESP
void envioNIP()
{
  lcd.clear();
  for (uint8_t i = 0; i < NIP_NUMBERS ; ++i) { //EL CONTADOR INICIA EN 0 HASTA 3 (EL TOTAL DE DATOS)
    lcd.setCursor(2, 0);
    lcd.print("NIP enviado: ");
    lcd.setCursor(6, 1);
    lcd.print((char) NIP[1]);
    lcd.setCursor(7, 1);
    lcd.print((char)NIP[2]);
    lcd.setCursor(8, 1);
    lcd.print((char)NIP[3]);
    lcd.setCursor(9, 1);
    lcd.print((char)NIP[4]);

    mySerial.write(NIP[i]); //LOS ENVIAMOS 1 A 1
  }
  NIP_Counter = 1;
}
