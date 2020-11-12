void envioPHP(){
   HTTPClient http; //Creamos la instancia
  String datos_a_enviar = "NIP=" + (String)NIP; //Carga de mensaje a enviar

  http.begin("http://192.168.100.3/filtrodato.php"); //Dirección de la peticion POST
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Tipo de dato a enviar

  int codigo_respuesta = http.POST(datos_a_enviar); //Envio de POST

  if (codigo_respuesta > 0) { //Si se recibe un digito es que hay conexión y se procede
    Serial.println("Codigo HTTP: " + String(codigo_respuesta)); //Nos despliega el codigo

    if (codigo_respuesta == 200) { //Sí el HTTP nos envía un 200
      String cuerpo_respuesta = http.getString(); //Leemos el string que arroja PHP
      JsonObject& root = jsonBuffer.parseObject(cuerpo_respuesta); //Decodificamos el JSON

      nip = root["NIP"];
      NOMBRE = root["NOMBRE"];
      A_MENOS = root["AMENOS"];
      A_MAS = root["AMAS"];
      B_MENOS = root["BMENOS"];
      B_MAS = root["BMAS"];
      C_MENOS = root["CMENOS"];
      C_MAS = root["CMAS"];

      jsonBuffer = StaticJsonBuffer<400>();

      if (!root.success()) {
        Serial.println("parseObject() failed");
      }

      else {
        Serial.println(cuerpo_respuesta);
        Serial.println(NOMBRE);
      }

    }
  }

  else { //ELSE DE CODIGO DE RESPUESTA, Si no recibe 200 se imprime lo que viene dentro

    Serial.print("Error enviando POST, codigo: ");
    Serial.println(codigo_respuesta);
  }

  http.end(); //Se cierra la conexión HTTP
  delay(2000);
  envioQT();
}
