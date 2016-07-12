#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lookuptables.h"
#include <QMessageBox>
#include <QContiguousCache>

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
    //QTimer *portPoller = new QTimer(this);
    //portPoller->start(1000);
    //connect(portPoller, SIGNAL(timeout()), this, SLOT(callEnumeratePorts()));
    QTimer *textRefresh = new QTimer(this);
    connect(textRefresh, SIGNAL(timeout()), this, SLOT(textAreaUpdate()));
    textRefresh->start(200);
    consoleTextBufferUpdated = false;
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
    connect(ui->pushButton_send, SIGNAL(clicked()),
            this, SLOT(send()) );
    connect(serial, SIGNAL(readyRead()),
            this, SLOT(receive()));
    connect(ui->pushButton_send, SIGNAL(clicked()),
            this, SLOT(sendString()));
    // Port polling timer
    connect(&portPoller, SIGNAL(timeout()),
            this, SLOT(pollPorts()) );
    connect(ui->pushButton_CLEAR, SIGNAL(clicked()),
            this, SLOT(textAreasClear()) );
    connect(ui->radioButton_ASCII, SIGNAL(clicked()),
            this, SLOT(setConsoleTextUpdate()) );
    connect(ui->radioButton_HEX, SIGNAL(clicked()),
            this, SLOT(setConsoleTextUpdate()) );
    connect(ui->checkBox_Hex, SIGNAL(clicked()),
            this, SLOT(setConsoleTextUpdate()) );
    connect(ui->checkBox_Dec, SIGNAL(clicked()),
            this, SLOT(setConsoleTextUpdate()) );
    connect(ui->checkBox_Bin, SIGNAL(clicked()),
            this, SLOT(setConsoleTextUpdate()) );
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
    MainWindow::consoleTextBufferUpdated = true;
    MainWindow::consoleTextBuffer += serial->readAll();
    // write to plaintext area in update slot textAreaUpdate
}

void MainWindow::send()
{
    QByteArray data;
    data.append(ui->lineEdit_send_input->text());
    serial->write( data );
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

void MainWindow::textAreaUpdate()
{
    if(!MainWindow::consoleTextBufferUpdated)
    {
        return;
    }
    QTextCursor c =  ui->plainTextEdit_console->textCursor();
    QString hex_console, hex_data, bin_data, dec_data;
    if (ui->radioButton_ASCII->isChecked())
    {
        ui->plainTextEdit_console->setPlainText(QTextCodec::codecForMib(4)->toUnicode(MainWindow::consoleTextBuffer));
    } else {
        for (int i = 0; i < MainWindow::consoleTextBuffer.size(); ++i) {
            hex_console.append( hex_strings[(quint8)MainWindow::consoleTextBuffer.at(i)]);
            hex_console.append(" ");
            if((i+1) % 16 == 0) {
                hex_console.append("\n");
            }
        }
        ui->plainTextEdit_console->setPlainText(QString(hex_console));
        //ui->plainTextEdit_console->setPlainText(QString(MainWindow::consoleTextBuffer.toHex().toUpper()));
    }

    c.movePosition(QTextCursor::End);
    ui->plainTextEdit_console->setTextCursor(c);
    ui->plainTextEdit_console->ensureCursorVisible();

    for (int i = 0; i < MainWindow::consoleTextBuffer.size(); ++i) {
        if(ui->checkBox_Hex->isChecked()) {
            hex_data.append( hex_strings[(quint8)MainWindow::consoleTextBuffer.at(i)] );
            hex_data.append("\n");
        }

        if(ui->checkBox_Dec->isChecked()) {
            dec_data.append( QString::number((quint8)MainWindow::consoleTextBuffer.at(i)) );
            dec_data.append("\n");
        }

        if(ui->checkBox_Bin->isChecked()) {
            bin_data.append( QString(bin_strings[(quint8)MainWindow::consoleTextBuffer.at(i)]) );
            bin_data.append("\n");
        }
    }

    if(ui->checkBox_Hex->isChecked()) {
        ui->plainTextEdit_HEX->setPlainText(hex_data);
        QTextCursor c =  ui->plainTextEdit_HEX->textCursor();
        c.movePosition(QTextCursor::End);
        ui->plainTextEdit_HEX->setTextCursor(c);
        ui->plainTextEdit_HEX->ensureCursorVisible();
    }
    if(ui->checkBox_Dec->isChecked()) {
        ui->plainTextEdit_DEC->setPlainText(dec_data);
        QTextCursor c =  ui->plainTextEdit_DEC->textCursor();
        c.movePosition(QTextCursor::End);
        ui->plainTextEdit_DEC->setTextCursor(c);
        ui->plainTextEdit_DEC->ensureCursorVisible();
    }
    if(ui->checkBox_Bin->isChecked()) {
        ui->plainTextEdit_BIN->setPlainText(bin_data);
        QTextCursor c =  ui->plainTextEdit_BIN->textCursor();
        c.movePosition(QTextCursor::End);
        ui->plainTextEdit_BIN->setTextCursor(c);
        ui->plainTextEdit_BIN->ensureCursorVisible();
    }
    MainWindow::consoleTextBufferUpdated = false;
}

void MainWindow::textAreasClear()
{
    MainWindow::consoleTextBuffer.clear();
    ui->plainTextEdit_console->clear();
    ui->plainTextEdit_BIN->clear();
    ui->plainTextEdit_DEC->clear();
    ui->plainTextEdit_HEX->clear();
}

void MainWindow::setConsoleTextUpdate()
{
    MainWindow::consoleTextBufferUpdated = true;
}

void MainWindow::callEnumeratePorts()
{
    this->enumeratePorts();
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
                          "For use with Arduino, HC-11, ESP8266 etc."
                          "See https://github.com/jarkko-hautakorpi/QIoT-terminal.git"));
}
