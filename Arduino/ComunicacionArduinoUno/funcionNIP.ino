void funcionNIP()
{
  tecla = customKeypad.getKey();

  switch (IngresoNIP)
  {
    case REPOSO:
      /*Transicion a asignacion de valor de nip*/
      if ((tecla >= 48) && (tecla <= 57)) //SI SE DETECTA UN '0' HASTA UN '9'
      {
        IngresoNIP = ASIGNACION; //ASIGNAMOS LA TECLA A EL ARREGLO
        NIP[NIP_Counter] = tecla; //VAMOS INCREMENTANDO EL INDICE
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("*"); //DESPLEGAMOS LA VARIABLE
      }
      else if ((65 == tecla) && (NIP_Counter > 1)) //SI DETECTAMOS UNA 'A' Y EL INDICE CORRESPONDE A ALGUN NUMERO DEL NIP COMO LIMTE '0'
      {
        IngresoNIP = RETROCESO; //RETROCEDEMOS A UNA LOCALIDAD ANTERIOR
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Retrocediendo");
        NIP_Counter --; //RETROCEDEMOS EL INDICE
      }
      break;

    case ASIGNACION:
      if ((4 == NIP_Counter)) //SI EL NIP YA SE LLENÓ
      {
        IngresoNIP = ENVIO;
        envioNIP(); //PROCEDEMOS A ENVIAR EL NIP
      }
      else if (NO_KEY == tecla) //SI NO SE DETECTA UNA TECLA
      {
        IngresoNIP = REPOSO; //REGRESAMOS AL ESTADO DE REPOSO
        NIP_Counter ++; //INCREMENTAMOS EL INDICE
      }
      break;

    case RETROCESO:
      if (NO_KEY == tecla) //S NO HAY UNA TECLA PRESIONADA
      {
        IngresoNIP = REPOSO; //NOS QUEDAMOS EN ESTE ESTADO
      }
      break;

    case ENVIO:
      IngresoNIP = REPOSO; //CUANDO SE ENVIEN LOS DATOS, REGRESAMOS A EL ESTADO DE REPOSO
      break;
  }
}
