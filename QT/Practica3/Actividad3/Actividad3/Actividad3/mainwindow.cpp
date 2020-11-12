//Librerias de OpenCV y QT

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"mat2qimage.h"
#include<opencv2/core/core.hpp>
#include<opencv2/ml/ml.hpp>
#include<opencv/cv.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/video/background_segm.hpp>
#include<opencv2/videoio.hpp>
#include<opencv2/imgcodecs.hpp>

#include<QTimer>                                    //Funciones de cronometro
#include<QDebug>                                    //Imprimir mensajes en terminal
#include<stdlib.h>
#include<opencv2/objdetect.hpp>                     //Libreria para la detección de objetos
#include<QThread>
#include<QtMath>
#include<QSerialPort>
#include<QSerialPortInfo>
#include<QtNetwork>
#include<QFile>
#include<QFileInfo>
#include<QDate>
#include<QDateTime>
#include<QTextStream>

using namespace cv;                                 //Nombre de espacio de opencv
using namespace std;                                //Nombre de espacio de C++

VideoCapture camara(0);                             //Declaracíon para el uso de la camara de la PC

//Variables para los clasificadores
CascadeClassifier detectorCara;
CascadeClassifier detectorSonrisas;
CascadeClassifier detectorOjoIz;
CascadeClassifier detectorOjoDer;

//Variable para almacenar el centro del rostro
Point centroCara;
//Variables para almacenar el angulo del triangulo
double anguloA, anguloB, anguloC;
int variableAmenos, variableAmas, variableBmenos, variableBmas, variableCmenos, variableCmas;


void MainWindow::recepcionSerialAsincrona(){

    if(arduino_esta_conectado && arduino->isReadable()){            //Si arduino está conextado
       QByteArray datosLeidos = arduino->readLine();                //Lemos los datos
       int indice0 = datosLeidos.indexOf("}");                      //Vemos los corchetes del mensaje JSON
       int indice1 = datosLeidos.indexOf("{");

       QString infoExtraida = datosLeidos.mid(indice1,(indice0-indice1+1)); //Guardamos la info obtenida

       if(indice0>=0 && indice1>=0) {
           qDebug() << "Datos extraidos: " << infoExtraida.toUtf8().constData();

           //Convertir el mensaje JSON en una cadena de bytes
           QByteArray cadenaJSON = infoExtraida.toUtf8().constData();

           //Se convierte la variable anterior a un documento JSON
           QJsonDocument jsonDocumento = QJsonDocument::fromJson(cadenaJSON);

           //Se convierte el documento Json a Qstring para despliegue en interfaz.
           QString jsonString(jsonDocumento.toJson(QJsonDocument::Compact));
           ui->label_10->clear();
           ui->label_10->setText(jsonString);

           //Convertir el documento Json a un objeto JSON
           QJsonObject jsonObjeto = jsonDocumento.object();
           //Decodificar la variable "Value"
           QJsonValue anguloAmenos = jsonObjeto.value(QString("A_MENOS"));      //Recibimos los datos de A_Menos que llegan mediante JSON
           QJsonValue anguloAmas = jsonObjeto.value(QString("A_MAS"));          //Recibimos los datos ed A_Mas que llegan mediante JSON
           QJsonValue anguloBmenos = jsonObjeto.value(QString("B_MENOS"));      //Recibimos los datos de B_Menos que llegan mediante JSON
           QJsonValue anguloBmas = jsonObjeto.value(QString("B_MAS"));          //Recibimos los datos de B_Mas que llegan mediante JSON
           QJsonValue anguloCmenos = jsonObjeto.value(QString("C_MENOS"));      //Recibimos los datos de C_Menos que llegan mediante JSON
           QJsonValue anguloCmas = jsonObjeto.value(QString("C_MAS"));          //Recibimos los datos de C_Mas que llegan mediante JSON

           //GUARDAMOS LAS VARIABLES A STRING
           QString variable1 = anguloAmas.toString();
           QString variable2 = anguloAmenos.toString();
           QString variable3 = anguloBmenos.toString();
           QString variable4 = anguloBmas.toString();
           QString variable5 = anguloCmenos.toString();
           QString variable6 = anguloCmas.toString();

           //CAMBIAMOS LOS DATOS A ENTEROS PARA USARLOS DE COMPARACIÓN
           variableAmenos = variable1.toInt();
           variableAmas = variable2.toInt();
           variableBmenos = variable3.toInt();
           variableBmas = variable4.toInt();
           variableCmenos = variable5.toInt();
           variableCmas = variable6.toInt();

           }

           else{
               //Acciones ante sensor en reposo
               //De momento, ninguna se debe de ejecutar.
           }
       }
   }


void MainWindow::conectarArduino(){

    //Parte # 1, declaración inicial de las variables
    arduino_esta_conectado = false;     //CREAMOS UNA BANDERA DE CONEXIÓN
    arduino = new QSerialPort(this); //SI SE DETECTA UN PUERTO SERIAL DISPONIBLE LO ASIGNAMOS A UN OBJETO
    connect(arduino, &QSerialPort::readyRead, this, &MainWindow::recepcionSerialAsincrona);

    QString nombreDispositivoSerial = "";
    int nombreProductID = 0;

    //-2 buscar puertos con los identificadores de Arduino
    qDebug() << "Puertos disponibles: " << QSerialPortInfo::availablePorts().length();
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        qDebug() << "Identificador del fabricante (VENDOR ID): " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "ID Vendedor " << serialPortInfo.vendorIdentifier();
            qDebug() << "ID Producto: " << serialPortInfo.productIdentifier();

            if(serialPortInfo.productIdentifier() == 66 || serialPortInfo.productIdentifier() == 67||
               serialPortInfo.productIdentifier() == 29987||serialPortInfo.productIdentifier()==60000)
            {
                arduino_esta_conectado = true;
                nombreDispositivoSerial = serialPortInfo.portName();
                nombreProductID = serialPortInfo.productIdentifier();
           }
        }
    }

    //3-Conexion
    if(arduino_esta_conectado){
        arduino->setPortName(nombreDispositivoSerial);
        arduino->open(QIODevice::ReadWrite);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setBaudRate(QSerialPort::Baud115200);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        ui->label_8->clear();
        qDebug() << "Producto: " << nombreProductID;
        if(nombreProductID == 67) ui->label_8->setText("Arduino UNO R3 conectado");
        else if(nombreProductID == 66) ui->label_8->setText("Arduino Mega conectado");
        else if(nombreProductID == 29987) ui->label_8->setText("Arduino UNO chino conectado");
        else if (nombreProductID == 60000) ui->label_8->setText("ESP32 conectado");
        else ui->label_8->setText("Error 3");
    }

    else{
        ui->label_8->clear();
        ui->label_8->setText("No hay arduino");
    }
}

//Funcion para poder tener un tiempo de espera
   void MainWindow::msleep(int msec)
   {
       QThread::msleep(msec);
   }

void MainWindow::cronometro(){
 //Matrices para guardar la imagen capturada de la camara web
 Mat imagen, imagenChica, imagenGris;
     camara >> imagen;

     //Si la imagen posee algun elemento procedemos a ejecutar el codigo
     if(!imagen.empty()){
         //Cambiar tamaño imagen del tamaño que proporciona la camara a un tamaño menor
         cv::resize(imagen, imagenChica, Size(320,240),0,0,INTER_LINEAR);

         //Vectores donde se guardaran los datos obtenidos de los clasificadores
         vector<Rect> cara;
         vector<Rect> sonrisa;
         vector<Rect> ojoIzquierdo;
         vector<Rect> ojoDerecho;

        //Funcion para pasar de una imagen a color a imagen en escala de gris
         cvtColor(imagenChica,imagenGris,COLOR_BGR2GRAY);

         //Normalización de la imagen, ecualizador, ecualiza el histograma para una detección más sencilla
         equalizeHist(imagenGris, imagenGris);

         //deteccion de los elementos, ImagenEscaneada, MatParaGuardarDatos, Parametros
         detectorCara.detectMultiScale(imagenGris, cara,1.1,2,0|CASCADE_SCALE_IMAGE,Size(150,100));

         //Iterador constante para acceder al vector de la cara
         vector<Rect>::const_iterator r;

         //Ciclo for para la detección de la cara, el cual indica que lo hara n veces hasta que deje de detectar caras
         for (r=cara.begin();r!=cara.end();r++) {

            Mat ImagenchicaROI;         //matriz para guardar la imagen de la cara detectada
            int radio;                  //variable donde se guarda el radio del objeto
            int anchura;                //variable donde se guardará el ancho de la cara

            //Se crean puntos para la detección del rostro
            Point center,center1,center2,center3,punto1, punto2,punto3;


            //Región de detección de rostro
            //Encontrar el centro de la cara
            center.x = cvRound(r->x + r->width*0.5); //Asignamos la posición en X
            center.y = cvRound(r->y + r->height*0.5); //Asignamos la posición en Y
            radio = cvRound(r->width + r->height)*0.25; //Creamos el radio
            anchura = width();

            //Se asigna el centro de la cara o una variable global
            centroCara = center;

            //Recortar la imagen de la cara y en escala de grises hacia el tamaño indicado
            ImagenchicaROI = imagenGris(*r);

            //Detectar ojo Izquierdo y la guardamos en la matriz de la región de interes
            detectorOjoIz.detectMultiScale( ImagenchicaROI, ojoIzquierdo, 1.1, 3, 0|CASCADE_SCALE_IMAGE, Size(20,20));
                    for( vector<Rect>::const_iterator nr = ojoIzquierdo.begin(); nr != ojoIzquierdo.end(); nr++ )
                            {
                                //Encontrar centro del ojo izquierdo
                                center1.x = cvRound(r->x + nr->x + nr->width*0.5);
                                center1.y = cvRound(r->y + nr->y + nr->height*0.5);

                                //Revisar si se encuentra en la mitad izquierda de la cara
                                if(r->x + r->width*0.5 < center1.x){
                                    punto1 = center1;  //Asignar las coordenadas al punto 1
                                }
                            }

             //Detectar ojo derecho de la cara
             detectorOjoDer.detectMultiScale( ImagenchicaROI, ojoDerecho, 1.1, 3, 0|CASCADE_SCALE_IMAGE, Size(20,20));
                   for( vector<Rect>::const_iterator nr = ojoDerecho.begin(); nr != ojoDerecho.end(); nr++ )
                           {

                            //Encontrar centro del objeto
                            center2.x = cvRound(r->x + nr->x + nr->width*0.5);
                            center2.y = cvRound(r->y + nr->y + nr->height*0.5);

                            //Revisar si se encuentra en la mitad derecha
                            if(r->x + r->width*0.5 > center2.x){

                            //Asginar las coordenadas del ojo derecho al punto 2
                            punto2 = center2;
                            }
                          }

             //Detectar boca
             detectorSonrisas.detectMultiScale( ImagenchicaROI, sonrisa, 1.3, 3, 0|CASCADE_SCALE_IMAGE, Size(40, 40), Size(80,80));
                   for( vector<Rect>::const_iterator nr = sonrisa.begin(); nr != sonrisa.end(); nr++ )
                          {
                            //Encontrar el centro del objeto
                            center3.x = cvRound(r->x + nr->x + nr->width*0.5);
                            center3.y = cvRound(r->y + nr->y + nr->height*0.5);

                            //Revisar si se encuentra debajo de la mitad de la cara
                            if(r->y + r->height*0.5 < center3.y){

                           //Asignar coordenadas al punto 3
                           punto3 = center3;
                                }
                           }

               //Calculamos la distancia entre los puntos  para poder calcular después su localización
               double dis1 = norm(punto1-punto2)*anchura;
               double dis2 = norm(punto1-punto3)*anchura;
               double dis3 = norm(punto3-punto2)*anchura;

               //Se usa el teorema del coseno para poder saber los angulos
               anguloA=qAcos((qPow(dis3,2)+qPow(dis2,2)-qPow(dis1,2))/(2*dis3*dis2));

               anguloB=qAcos((qPow(dis1,2)+qPow(dis2,2)-qPow(dis3,2))/(2*dis1*dis2));

               anguloC=qAcos((qPow(dis1,2)+qPow(dis3,2)-qPow(dis2,2))/(2*dis1*dis3));

               //Conversión de grados a radianes
               anguloA=anguloA*(180/M_PI);
               anguloB=anguloB*(180/M_PI);
               anguloC=anguloC*(180/M_PI);

               //Se imprime en consola todos los angulos y se espera un tiempo para poder leerlos
               qDebug()<<"Angulo A: "<<anguloA;
               qDebug()<<"Angulo B: "<<anguloB;
               qDebug()<<"Angulo C: "<<anguloC;
               msleep(50);

               //Mostrar las angulos en los lcdNumber de la interfaz
               ui->lcdNumber->display(anguloA);
               ui->lcdNumber_2->display(anguloB);
               ui->lcdNumber_3->display(anguloC);

               //Se dibujan las lineas que unen los centros de cada objeto
               line(imagenChica,punto1,punto2,Scalar(16,255,0),1,8,0); //Punto B-C
               line(imagenChica,punto1,punto3,Scalar(16,255,0),1,8,0); //Punto C-A
               line(imagenChica,punto3,punto2,Scalar(0,0,255),1,8,0); //Punto B-A

               //Creamos las condiciones para detectar mi rostro
               if(anguloA>variableAmenos && anguloA<variableAmas && anguloB>variableBmenos && anguloB<variableBmas && anguloC>variableCmenos && anguloC<variableCmas){
                   ui->label_6->setText("Manuel Garcia");
                   circle( imagenChica, centroCara, radio, Scalar( 0, 255,0), 2, 8, 0 );
                   putText(imagenChica, "Manuel", centroCara, FONT_HERSHEY_COMPLEX_SMALL, 1, CV_RGB(0,0,0), 2);
                   //int confirmacion = 1;
                QApplication::quit();
               }


               //Condicion en caso de que no sea el rostro que se quiere
                 else {
                     ui->label_6->setText("Un Desconocido");
                     circle( imagenChica, centroCara, radio, Scalar( 0, 0, 255 ), 2, 8, 0 );
                     //putText(imagenChica, "Error", centroCara, FONT_HERSHEY_COMPLEX_SMALL, 1, CV_RGB(0,0,0), 2);
                  }
       }

         // Cambiar la imagen de opencv a una imagen de qt
         QImage qImage = Mat2QImage(imagenChica);

         // Cambiar la imagen de qt a un mapa de pixeles de qt
         QPixmap pixmap = QPixmap::fromImage(qImage);

         // Limipo el contenido de la etiqueta
         ui->label->clear();

         //Se muestra el mapa de pixeles
         ui->label->setPixmap(pixmap);

     }
      //qDebug()<<" Fin del Programa";
 }

//INICIALIZAMOS CONTADOR Y COMUNICACION SERIAL CON ARDUINO
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);
    conectarArduino();

    //QTimer es una clase
    //cronos es un objeto
    QTimer *cronos = new QTimer(this);

    //Conexion del cronometro con la funcion
    connect(cronos, SIGNAL(timeout()),this,SLOT(cronometro()));

    //Iniciar el cronometro
    cronos->start(30);

    //Se cargan los archivos xml para la detección
    if(!detectorCara.load("../frontalface.xml")){
        qDebug()<<"Error al cargar el archivo detector de rostros";
    }
    if(!detectorSonrisas.load("../smile.xml")){
        qDebug()<<"Error al cargar el archivo detector de rostros";
    }
    if(!detectorOjoIz.load("../lefteye.xml")){
        qDebug()<<"Error al cargar el archivo detector de rostros";
    }
    if(!detectorOjoDer.load("../righteye.xml")){
        qDebug()<<"Error al cargar el archivo detector de rostros";
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
