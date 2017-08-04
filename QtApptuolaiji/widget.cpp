#include "widget.h"
#include "ui_widget.h"
#include <QDate>
#include <iostream>
#include <QTime>
#include <QThread>


WorkObject::WorkObject()
{
    Debug("构造");
    serialport = new QSerialPort();
}

WorkObject::~WorkObject()
{
    delete serialport;
    Debug("析构");
}

 void WorkObject::Log(QString text)
 {

 }

void WorkObject::StartWork(QString xmlfile)
{
    if (serialport->open(QIODevice::ReadWrite)) {
        Log("打开串口");
    } else {
        Log("打开串口失败");
        return;
    }

    QThread::sleep(10);

    if(serialport->isOpen())
    {
        serialport->close();
        Log("关闭串口");
    }
}

void WorkObject::setPortName(QString name)
{
    serialport->setPortName(name);
    serialport->setParity(QSerialPort::NoParity);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setStopBits(QSerialPort::OneStop);
    serialport->setFlowControl(QSerialPort::NoFlowControl);
}

void WorkObject::setBaudRate(qint32 baudRate)
{
    serialport->setBaudRate(baudRate);
}

CSimpleThread::CSimpleThread()
{
    serialport = new QSerialPort();
}

CSimpleThread::~CSimpleThread()
{
    delete serialport;
}

void CSimpleThread::setSerialPort(QString port,qint32 baudRate)
{
    serialport->setPortName(port);
    serialport->setBaudRate(baudRate);
    serialport->setParity(QSerialPort::NoParity);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setStopBits(QSerialPort::OneStop);
    serialport->setFlowControl(QSerialPort::NoFlowControl);
}

void CSimpleThread::run()
{
    if (serialport->open(QIODevice::ReadWrite)) {
        Log("打开串口");
    } else {
        Log("打开串口失败");
        return;
    }

    ReadXml();

    if(serialport->isOpen())
    {
        serialport->close();
        Log("关闭串口");
    }

}

void CSimpleThread::setXmlName(QString name)
{
    this->XmlName = name;
}

void CSimpleThread::Log(QString text)
{
    emit SendLog(text);

}


void CSimpleThread::ReadXml()
{
    QDomDocument doc;
    QFile file(XmlName);
    doc.setContent(&file);

    QDomElement docElem = doc.documentElement();
    QDomNode n;

    int count = 0;
    for(n = docElem.firstChild(); !n.isNull(); n = n.nextSibling())
        count++;

    int i = 0;
    for(n = docElem.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        i++;
        SendImageFile(n.toElement().attribute("Address"), n.toElement().attribute("FileName"));
        emit SendTotalProgress((i*100)/count);
    }
}


qint64 CSimpleThread::writeCom(const char *data, qint64 len)
{
    serialport->write(data, len);
    serialport->flush();
   // Log(QByteArray(data, len).toHex());
    return 0;
}

qint64 CSimpleThread::writeCom(const QByteArray &data)
{
    serialport->write(data);
    serialport->flush();
  //  Log(data.toHex());
    return 0;
}

void CSimpleThread::SendImageFile(QString address, QString filepath)
{
 //   qDebug()<<address <<filepath;
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot open file for reading: ";
    }else{
        QDataStream in(&file);
        uint len;
        char buff[1024];
        qint64 hasReadSize = 0;
        qint64 fileSize = file.size();

        //Log(QByteArray(data, len).toHex());
        //发送地址
        bool ok;
        int addressInt =  address.toInt(&ok, 16);
        //qDebug()<< QString::number(addressInt,16).toUpper();

        char addressbuff[5] = {0x01, 0x02, 0x03, 0x04, 0x05};

        writeCom(QByteArray(addressbuff, sizeof(addressbuff)));
        //等待回复
        if(serialport->waitForReadyRead(1000*2) )
        {
            QByteArray readdata = serialport->read(1);
        }else{
            Log("Error:" + address + " 发送文件 " + filepath +" 地址未准备好");
            return;
        }

        while( (len = in.readRawData(buff, sizeof(buff)) ) > 0)
        {
            //发送内容

            //等待回复
            hasReadSize += len;
            emit SendSingleProgress((hasReadSize * 100) / fileSize );
        }
        file.close();
    }
}


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    SimpleThread = NULL;

    WorkThread = new QThread;
    workject = new WorkObject;
    workject->moveToThread(WorkThread);
    WorkThread->start();

    if(!QSerialPortInfo::availablePorts().isEmpty())
    {
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            ui->comboBoxCom->addItem(info.portName());
        }
    }else{
        ui->comboBoxCom->addItem(tr("No COM"));
    }

    connect( ui->pushButtonConnect, SIGNAL(clicked()) , this, SLOT(ComCheckConnect()) );

    ui->lineEdit->setReadOnly(true);
    ui->pushButtonStart->setDisabled(true);
    ui->pushButtonOpenFile->setDisabled(true);

}

void Widget::RecviceData()
{
    QByteArray data;
    data = serialport->readAll();
    QString s;
    s.prepend(data);
    Log(s);
    //Log(str(data));
}

void Widget::ComCheckConnect()
{
    if(ui->pushButtonConnect->text() == tr("连接"))
    {
        serialport = new QSerialPort();
        serialport->setPortName(ui->comboBoxCom->currentText() );

        if(ui->comboBoxRate->currentText() == "115200")
            serialport->setBaudRate(QSerialPort::Baud115200);
        else if(ui->comboBoxRate->currentText() == "38400")
            serialport->setBaudRate(QSerialPort::Baud38400);
        else if(ui->comboBoxRate->currentText() == "19200")
            serialport->setBaudRate(QSerialPort::Baud19200);
        else if(ui->comboBoxRate->currentText() == "9600")
            serialport->setBaudRate(QSerialPort::Baud9600);

        serialport->setParity(QSerialPort::NoParity);
        serialport->setDataBits(QSerialPort::Data8);
        serialport->setStopBits(QSerialPort::OneStop);
        serialport->setFlowControl(QSerialPort::NoFlowControl);

        if (serialport->open(QIODevice::ReadWrite)) {
            Log("锁定串口");
        } else {
            Log("锁定串口失败");
            return;
        }
        ui->comboBoxCom->setDisabled(true);
        ui->comboBoxRate->setDisabled(true);
        ui->pushButtonConnect->setText(tr("断开"));
        ui->pushButtonStart->setDisabled(false);
        ui->pushButtonOpenFile->setDisabled(false);
        serialport->close();
    } else {
        ui->pushButtonConnect->setText(tr("连接"));
        ui->comboBoxCom->setDisabled(false);
        ui->comboBoxRate->setDisabled(false);
        ui->pushButtonStart->setDisabled(true);
        ui->pushButtonOpenFile->setDisabled(true);
        Log("解锁串口");
    }

}


//编辑框自动滚屏，每次新增一行或多行字符调用一次
void Widget::autoScrollTextEdit(QString text)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    //添加行字符并刷新编辑框
    ui->textBrowser->append("--> " + str + ":" + text);
}

void Widget::Log(QString text)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    //添加行字符并刷新编辑框
    ui->textBrowser->append("--> " + str + ":" + text);
}



Widget::~Widget()
{    
    WorkThread->exit();
    while(!WorkThread->isFinished());
    delete WorkThread;
    delete workject;

    delete ui;
}

void Widget::SlotTotalProgress(int value)
{
    ui->progressBar_2->setValue(value);
}

void Widget::SlotSingleProgress(int value)
{
    ui->progressBar->setValue(value);
}


void Widget::on_pushButtonOpenFile_clicked()
{
    QString fileName, file_full, file_name, file_path, file_suffix;
    file_full = QFileDialog::getOpenFileName(NULL,"选择xml文件",".","*.xml;*.*");
    QFileInfo fileinfo = QFileInfo(file_full);
    ui->lineEdit->setText( fileinfo.fileName());
    Log( "打开文件:" + fileinfo.absoluteFilePath());
}

void Widget::writeLog(QString value)
{
    qDebug()<<value;
    Log(value);
}



void Widget::on_pushButtonStart_clicked()
{


    return;
    if(SimpleThread == NULL)
    {
        SimpleThread = new CSimpleThread();
        connect( SimpleThread, SIGNAL(SendTotalProgress (int)) , this, SLOT(SlotTotalProgress(int)));
        connect( SimpleThread, SIGNAL(SendSingleProgress(int)) , this, SLOT(SlotSingleProgress(int)));
        connect( SimpleThread, SIGNAL(SendLog(QString)) ,        this, SLOT( writeLog(QString)));

    }
    if(!SimpleThread->isRunning())
    {
        qDebug()<<"启动线程!";
        SimpleThread->setSerialPort(serialport->portName(), serialport->baudRate());
        SimpleThread->setXmlName("image.xml");
        SimpleThread->start();

    }
}
