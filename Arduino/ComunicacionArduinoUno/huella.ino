uint8_t Huella() {
  uint8_t imagenHuella = Dedo.getImage();

  switch (imagenHuella) {
    case FINGERPRINT_OK: //EN CASO DE DETECTAR HUELLA
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Huella recibida");
      break;

    case FINGERPRINT_NOFINGER: //SI NO SE DETECTA HUELLA
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Huella no");
      lcd.setCursor(0, 1);
      lcd.print("detectada");
      return imagenHuella; //REGRESA EL ESTADO DE LA HUELLA PARA SEGUIR EN LOS CASES

    default: //SI NO SE SABE QUE OCURRIÓ
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ERROR");
      return imagenHuella;
  }

  imagenHuella = Dedo.image2Tz();
  switch (imagenHuella) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Imagen tomada");
      break;

    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Imagen borrosa");
      return imagenHuella;

    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No reconocida");
      return imagenHuella;

    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ERROR");
  }

  imagenHuella = Dedo.fingerFastSearch();
  if (FINGERPRINT_OK == imagenHuella) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Concordancia");
  }

  else if (FINGERPRINT_PACKETRECIEVEERR == imagenHuella) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("NO COMUNICACION");
    return imagenHuella;
  }

  else if (FINGERPRINT_NOTFOUND == imagenHuella) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sin concordancia");
    return imagenHuella;
  }

  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ERROR");
  }

  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("Huella");
  lcd.setCursor(0, 0);
  lcd.print("Detectada");
  return Dedo.fingerID;
}

int obtenerHuellaID() { //FUNCION DE OBTENCION DE HUELLA
  uint8_t imagenHuella = Dedo.getImage(); //OBTENEMOS LA IMAGEN DE LA HUELLA
  if (FINGERPRINT_OK != imagenHuella) {
    return -1;
  }

  imagenHuella = Dedo.image2Tz();
  if (FINGERPRINT_OK != imagenHuella) {
    return -1;
  }

  imagenHuella = Dedo.fingerFastSearch();
  if (FINGERPRINT_OK != imagenHuella) {
    return -1;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Huella");
  lcd.setCursor(0, 1);
  lcd.print("Detectada");
  return Dedo.fingerID;
}
