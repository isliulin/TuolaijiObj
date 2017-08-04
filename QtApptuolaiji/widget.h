#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QComboBox>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QTextEdit>
#include <QFileDialog>
#include <QtXml/QtXml>
#include <QDataStream>
#include <QFile>
#include <QThread>

#define Debug(x) do{\
        qDebug()<<__LINE__<<":"<<QString(x); \
    }while(0)

namespace Ui {
class Widget;
}



class WorkObject:public QObject
{
    Q_OBJECT
public:
    WorkObject();
    ~WorkObject();
    void Log(QString text);
public slots:
    void StartWork(QString xmlfile);
    void setPortName(QString name);
    void setBaudRate(qint32 baudRate);
signals:
    void SendLog(QString);
private:
    QSerialPort* serialport;
};

class CSimpleThread : public QThread
{
    Q_OBJECT
public:
    CSimpleThread();
    ~CSimpleThread();
    void setXmlName(QString name);
    void ReadXml();
    void SendImageFile(QString address, QString filepath);
    void Log(QString text);
    qint64 writeCom(const char *data, qint64 len);
    qint64 writeCom(const QByteArray &data);

    void setSerialPort(QString port,qint32 baudRate);

    void run();

    QSerialPort* serialport;

signals:
      void resultReady(const QString &result);
      void SendTotalProgress(int value);
      void SendSingleProgress(int value);
      void SendLog(QString);


public slots:

private:
      QString XmlName;
      QSerialPortInfo SerialPortInfo;
};




class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void autoScrollTextEdit(QString text);
    void Log(QString text);

public slots:
    void writeLog(QString value);
    void ComCheckConnect();
    void RecviceData();

private slots:

    void SlotTotalProgress(int value);
    void SlotSingleProgress(int value);
    void on_pushButtonOpenFile_clicked();
    void on_pushButtonStart_clicked();

private:
    Ui::Widget *ui;
    QSerialPort* serialport;
    QFileDialog* filedialog;
    CSimpleThread *SimpleThread;
    QThread *WorkThread;
    WorkObject *workject;

};

#endif // WIDGET_H
