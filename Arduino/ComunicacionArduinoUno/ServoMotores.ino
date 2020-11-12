void Dinero() {
  String saldoSolicitado; //STRING DONDE SE GUARDARÁ EL VALOR LEIDO POR EL TECLADO
  int enteroSolicitado; //VALOR EN ENTERO DEL DATO SOLICITADO (CONVERTIDO DE STRING A ENTERO)
  uint8_t relacionVueltas = 100; //RELACION BILLETES ENTREGADOS Y SALDO SOLICITADO
  int saldoTotal = 1000; //SALDO TOTAL QUE DISPONEMOS (DATO GENERAL)
  int banderaCierre = 0; //SALDO DE BANDERA
  bool finDato = false; //BANDERA DE SALIDA

  tecla = customKeypad.getKey(); //LEEMOS LOS DATOS DEL TECLADO

  if (true != finDato) {
    if (tecla != NO_KEY) { //SI LA BANDERA DE ESTADO ES FALSA
      if ((tecla >= 48) && (tecla <= 57)) { //SI SE DETECTA UN '0' HASTA UN '9'
        saldoSolicitado += tecla; //INGRESAMOS EN EL STRING LAS TECLAS
        enteroSolicitado = saldoSolicitado.toInt(); //CONVERTIMOS EL DATO INGRESADO A UN ENTERO

        if (0 == enteroSolicitado) {//SI SOLICITA 1000 DEBERÁ PRESIONAR 0
          enteroSolicitado = 10; //CREAMOS LA VARIABLE A 10
        }

        enteroSolicitado = enteroSolicitado * 100; //MULTIPLICAMOS EL NUMERO INGRESADO POR 100
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Solicitando : ");
        lcd.setCursor(3, 1);
        lcd.print(enteroSolicitado);

      }
      if ((38 == tecla)) { //SI SE DETECTA UNA PRESION DE "D" O "*"
        banderaCierre = 1;
        finDato = true; //CAMBIAMOS LA BANDERA A TRUE
      }
    }
  }

  if (((saldoTotal >= enteroSolicitado)) && (1 == banderaCierre)) { //SI SALDO TOTAL ES MAYOR AL SALDO SOLICITADO
    int vueltasServo = enteroSolicitado / relacionVueltas; //SACAMOS LA RELACION DE VUELTAS ENTRE EL DINERO SOLICITADO
    Serial.print (vueltasServo);
    saldoTotal = saldoTotal - enteroSolicitado; //VEMOS EL SALDO RESTANTE HACIENDO RESTA DEL SALDO SOLICITADO ENTRE EL SALDO TOTAL
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Espere un");
    lcd.setCursor(0, 1);
    lcd.println(" momento");
    delay(1000); //ESPERAMOS UN SEGUNDO

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Retirando");
    lcd.setCursor(0, 1);
    lcd.print(" Efectivo");

    for (int i = 0; i < vueltasServo; i++) { //SACAMOS LAS VUELTAS QUE DEBE DAR EL SERVO
      servoMotor.write(180); //MOVEMOS EL MOTOR A 180°
      delay(500); //ESPERAMOS 100 mS
      servoMotor.write(0); //REGRESAMOS EL MOTOR A 0°
      delay(500);

    }
    Serial.println (" saliendo ciclo");
    finDato = false; //CAMBIAMOS LA BANDERA A "FALSE"

  }
  if (1000 < enteroSolicitado) { //SI SE SOLICITA MÁS DINERO DEL DISPONIBLE
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Saldo no");
    lcd.setCursor(0, 1);
    lcd.print("disponible");
  }
}
