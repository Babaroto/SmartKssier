# SmartKssier
It is a code used to make an automatic ATM that use multiple authentications to assure that you are the truly user of that bank account


## The parts
There are 2 fundamental parts: 
  - Hardware
  - Software
  
 ### Hardware
 In the part of hardware we use an Arduino Uno, ESP32, keyboard matrix 6x6, a fingerprint scanner from Adafruit, LCD 16x2 with I2C module, 1 servo motor and a 5v 1A power source.
 
 ### Software
 In the software side we use the Arduino IDE to make a modular script that controlls the user interactions like: NIP input, text display, the count of the motor laps and the serial comunication with our ESP32 board.
 
 On the ESP32 code we can see the serial recive of Arduino data, the download of user's data from a MYSQL database and then send it to our face recognition software on QT.
 
 By QT the only thing that do is make (with the data recived by ESP32 board) a facial recognition and allow that our Arduino board can give acces to our user to his bank acount.
 
 ## This is just a prototipe
