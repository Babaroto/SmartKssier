//Función que inicializa el Timer1 con un intervalo de tiempo definido.
void setupTimer1()
{
  Timer1.initialize(uSecTime);
  Timer1.attachInterrupt(Timer1Handler);
}
