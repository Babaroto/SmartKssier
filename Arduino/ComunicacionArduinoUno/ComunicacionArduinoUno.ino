#include <SoftwareSerial.h> //INICIALIZAMOS LIBRERÍA DEL SERIAL
#include <TimerOne.h> //LIBRERIA DE CONTADOR DE TIEMPO
#include <Keypad.h> //LIBRERIA DE CONTROL DE KEYPAD
#include <LiquidCrystal_I2C.h> //LIBRERIA LCD CON I2C
#include <Adafruit_Fingerprint.h> //LIBERIA DEL CONTROL DEL SENSOR DE HUELLA
#include <Servo.h> //LIBERIA DE CONTROL DEL SERVO MOTOR QUE SACARÁ EL DINERO

bool state =      false; //DEFINIMOS UNA BADERA DE CIERRE

/*------VARIABLES DE TRANSICIÓN DE TIEMPO ----------*/
bool Timer1Flag = false; //Bandera que define intervalos de tiempo transcurrido
uint8_t ButtonCounter = 0;
const unsigned int uSecTime   = 10000;
const unsigned int pressedBtn = 5;

/*------VARIABLES DE NIP ----------*/
const uint8_t NIP_NUMBERS = 6;
uint8_t NIP_Counter;
int NIP[NIP_NUMBERS] = {'&', '0', '0', '0', '0', '&'}; //DATO QUE GUARDA EL NIP

/*-------------- VARIABLES DE KEYPAD -------------*/
const byte ROWS = 4;
const byte COLS = 4;
byte pinesF[ROWS] = {13, 12, 11, 10};
byte pinesC[COLS] = {9, 8, 7, 6};
char tecla;
char teclas[ROWS][COLS] = {

  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'&', '0', '#', '&'}
};

Keypad customKeypad = Keypad(makeKeymap(teclas), pinesF, pinesC, ROWS, COLS);

/*------ESTADOS DE PRESIONADO DE TECLADO MATRICIAL----------*/
typedef enum estadosNIP
{
  REPOSO,
  ASIGNACION,
  RETROCESO,
  ENVIO,
} e_estadosNIP;

e_estadosNIP IngresoNIP = REPOSO;

SoftwareSerial mySerial(4, 5); //Rx, Tx //DEFINIMOS LOS PUERTOS

/*----------------INICIALIZACION MODULO I2C ----------------*/
LiquidCrystal_I2C lcd(0x27, 16, 2); //DEFINIMOS PUERTOS Y TAMAÑO DE PANTALLA (A4-SDA),(A5-SCL)


/*--------------INICIALIZACION SENSOR DE HUELLA --------------*/
SoftwareSerial serialHuella(2, 3); //Rx, Tx DEL SENSOR DE HUELLA
Adafruit_Fingerprint Dedo = Adafruit_Fingerprint(&serialHuella); //CREAMOS EL OBJETO DE HUELLA

/*------------------- SERVOMOTORES ---------------------------*/
Servo servoMotor;
int saldoTotal = 1000; //SALDO BASE DEL QUE DISPONE EL CLIENTE

/*-----------INICIO DEL SETUP ----------*/
void setup()
{
  lcd.init(); //INICIALIZAMOS LA LCD
  lcd.backlight(); //ENCENDEMOS LA LUZ DE FONDO
  lcd.clear(); //LIMPIAMOS LA LCD
  lcd.setCursor(3, 0);
  lcd.print("Bienvenido");
  lcd.setCursor(0, 1);
  lcd.print("Ingrese su NIP:");
  Serial.begin(115200); //SERIAL DEL ARDUINO
  mySerial.begin(115200); //COMENZAMOS EL SERIAL DEL SOFTWARESERIAL
  Dedo.begin(57600); //EL SERIAL DEL SENSOR DE HUELLA
  setupTimer1(); //INICIALIZACION DEL CONTADOR DE TIEMPO
  NIP_Counter = 1; //CONTADOR DEL INDICE DEL NIP
  servoMotor.attach(A1); //PIN AL QUE SE LE ASIGNARÁ EL SERVO
  servoMotor.write(0); //MANDAMOS EL SERVO A 0°
}

void loop()
{
  funcionNIP();
}
