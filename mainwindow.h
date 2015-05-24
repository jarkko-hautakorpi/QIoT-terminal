#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    struct SerialSettings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    } currentSettings;

public slots:
    void receive();
    void receiveToHEX();
    void receiveToDEC();
    void receiveToBIN();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QTimer portPoller;
    void openSerialPort();
    void closeSerialPort();
    void enumeratePorts();
    void generateSettingsParameters();
    void connectUiActions();
    void updateSettings();
    void send(const QByteArray &data);

private slots:
    void togglePortConnection();
    void about();
    void sendString();
    void pollPorts();
};

#endif // MAINWINDOW_H
