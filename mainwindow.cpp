#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plainTextEdit_port_info_window->hide();
    ui->plainTextEdit_BIN->hide();
    ui->plainTextEdit_DEC->hide();
    ui->plainTextEdit_HEX->hide();
    SerialSettings currentSettings;
    serial = new QSerialPort(this);
    this->generateSettingsParameters();
    this->enumeratePorts();
    connectUiActions();
    QTimer *portPoller = new QTimer(this);
    portPoller->setInterval(1000);
    portPoller->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enumeratePorts()
{
    ui->comboBox_ports->clear();
    ui->plainTextEdit_port_info_window->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        list << info.portName()
             << (!description.isEmpty() ? description : "")
             << (!manufacturer.isEmpty() ? manufacturer : "")
             << (!serialNumber.isEmpty() ? serialNumber : "")
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : "")
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : "");

        ui->comboBox_ports->addItem(list.first(), list);
        ui->plainTextEdit_port_info_window->appendPlainText("------------");
        for (int i = 0; i < list.size(); ++i)
             ui->plainTextEdit_port_info_window->appendPlainText( list.at(i).toLocal8Bit().constData());
    }
    ui->comboBox_ports->addItem(tr("Custom"));
}

void MainWindow::generateSettingsParameters()
{
    ui->comboBox_baudrates->addItem(QStringLiteral("1200"), QSerialPort::Baud1200);
    ui->comboBox_baudrates->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
    ui->comboBox_baudrates->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    ui->comboBox_baudrates->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->comboBox_baudrates->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->comboBox_baudrates->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->comboBox_baudrates->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    ui->comboBox_baudrates->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->comboBox_baudrates->addItem(tr("Custom"));

    ui->comboBox_databits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->comboBox_databits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->comboBox_databits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->comboBox_databits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->comboBox_databits->setCurrentIndex(3);

    ui->comboBox_parity->addItem(tr("None"), QSerialPort::NoParity);
    ui->comboBox_parity->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->comboBox_parity->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->comboBox_parity->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->comboBox_parity->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->comboBox_stopbits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->comboBox_stopbits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->comboBox_flowcontrol->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->comboBox_flowcontrol->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->comboBox_flowcontrol->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void MainWindow::connectUiActions()
{
    connect(ui->pushButton_connectToggle, SIGNAL(clicked()),
            this, SLOT(togglePortConnection()) );
    connect(ui->pushButton_about, SIGNAL(clicked()),
            this, SLOT(about()) );
    connect(serial, SIGNAL(readyRead()),
            this, SLOT(receive()));
    connect(ui->pushButton_send, SIGNAL(clicked()),
            this, SLOT(sendString()));
    // Port polling timer
    connect(&portPoller, SIGNAL(timeout()),
            this, SLOT(pollPorts()) );
}

/**
 * SLOTS
 */

void MainWindow::togglePortConnection()
{
    if(!serial->isOpen()) {
        this->openSerialPort();
    } else {
        this->closeSerialPort();
    }
}

// ui->pushButton_connectToggle->setText(tr("Disconnect"));
void MainWindow::openSerialPort()
{
    updateSettings();
    serial->setPortName(this->currentSettings.name);
    serial->setBaudRate(this->currentSettings.baudRate);
    serial->setDataBits(this->currentSettings.dataBits);
    serial->setParity(this->currentSettings.parity);
    serial->setStopBits(this->currentSettings.stopBits);
    serial->setFlowControl(this->currentSettings.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        ui->pushButton_connectToggle->setText("Disconnect");
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(this->currentSettings.name).arg(this->currentSettings.stringBaudRate).arg(this->currentSettings.stringDataBits)
                                       .arg(this->currentSettings.stringParity).arg(this->currentSettings.stringStopBits).arg(this->currentSettings.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
        ui->statusBar->showMessage(tr("Open error"));
    }
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    ui->statusBar->showMessage(tr("Disconnected"));
    ui->pushButton_connectToggle->setText(tr("Connect"));
}

void MainWindow::updateSettings()
{
    this->currentSettings.name = ui->comboBox_ports->currentText();
    if (ui->comboBox_baudrates->currentIndex() == 4) {
        this->currentSettings.baudRate = ui->comboBox_baudrates->currentText().toInt();
    } else {
        this->currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->comboBox_baudrates->itemData(ui->comboBox_baudrates->currentIndex()).toInt());
    }
    this->currentSettings.stringBaudRate = QString::number(this->currentSettings.baudRate);

    this->currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->comboBox_databits->itemData(ui->comboBox_databits->currentIndex()).toInt());
    this->currentSettings.stringDataBits = ui->comboBox_databits->currentText();

    this->currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->comboBox_parity->itemData(ui->comboBox_parity->currentIndex()).toInt());
    this->currentSettings.stringParity = ui->comboBox_parity->currentText();

    this->currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->comboBox_stopbits->itemData(ui->comboBox_stopbits->currentIndex()).toInt());
    this->currentSettings.stringStopBits = ui->comboBox_stopbits->currentText();

    this->currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->comboBox_flowcontrol->itemData(ui->comboBox_flowcontrol->currentIndex()).toInt());
    this->currentSettings.stringFlowControl = ui->comboBox_flowcontrol->currentText();
}

void MainWindow::receive()
{
    bool ok = false;
    QByteArray data = serial->readAll();
    ui->plainTextEdit_console->insertPlainText(QString(data));

    // iterate through chars
    for (int i = 0; i < data.size(); ++i) {
        QChar character = data.at(i);
        if(ui->checkBox_Hex->isChecked()) {
            //ui->plainTextEdit_HEX->insertPlainText();
        }
        if(ui->checkBox_Dec->isChecked()) {
            //ui->plainTextEdit_DEC->insertPlainText(data.toUInt());
        }
        if(ui->checkBox_Bin->isChecked()) {
            //QString bin = QString::number(data.toLongLong(&ok, 16), 2);
            //ui->plainTextEdit_BIN->insertPlainText(data.toLongLong(&ok, 16));
        }

    }
    //QScrollBar *elevator = ui->plainTextEdit_console->verticalScrollBar();
    //elevator->setValue(elevator->maximum());
    ui->plainTextEdit_console->ensureCursorVisible();
    //ui->plainTextEdit_console->verticalScrollBar()->setValue(ui->plainTextEdit_console->verticalScrollBar()->maximum());
}

void MainWindow::receiveToHEX()
{
    QByteArray data = serial->readAll();
    //ui->plainTextEdit_console->insertPlainText(QString(data));
}

void MainWindow::receiveToDEC()
{
    QByteArray data = serial->readAll();
    //ui->plainTextEdit_console->insertPlainText(QString(data));
}

void MainWindow::receiveToBIN()
{
    QByteArray data = serial->readAll();
    ui->plainTextEdit_console->insertPlainText(QString(data));
}

void MainWindow::sendString()
{
    QByteArray data;

}

void MainWindow::send(const QByteArray &data)
{
    this->serial->write(data);
}

void MainWindow::pollPorts()
{
    ui->statusBar->showMessage(tr("Enumerating"), 500);
    this->enumeratePorts();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About QIoT-terminal"),
                       tr("Serial console tool for programmers. "
                          "For use with Arduino, ESP8266 etc."
                          "See https://github.com/jarkko-hautakorpi/QIoT-terminal.git"));
}
