void envioQT(){
  String mensajeQT = "";

  mensajeQT = "{\"A_MENOS\":\"" + A_MENOS + "\",";
  mensajeQT += "\"A_MAS\":\"" + A_MAS + "\",";
  mensajeQT += "\"B_MENOS\":\"" + B_MENOS + "\",";
  mensajeQT += "\"B_MAS\":\"" + B_MAS + "\",";
  mensajeQT += "\"C_MENOS\":\"" + C_MENOS + "\",";
  mensajeQT += "\"C_MAS\"_\"" + C_MAS + "\"}";
  Serial.print(mensajeQT);
  delay(1000);
  SerialNIP()
}
