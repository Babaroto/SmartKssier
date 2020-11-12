#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void cronometro();
    void msleep(int msec);
    void recepcionSerialAsincrona();
    void conectarArduino();

private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    bool arduino_esta_conectado = false;
};

#endif // MAINWINDOW_H
