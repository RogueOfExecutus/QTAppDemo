#pragma once

#include <QObject>
#include <QSerialPort>
#include <QMutex>
#include <QThread>
#include <QTimer>

class WorkerOne : public QObject
{
	Q_OBJECT

public:
	WorkerOne(QObject *parent);
	~WorkerOne();

signals:
	void answer2plc(std::string msg);

public slots:
	void initAll(QString portName);
	void recTrigger(QString value);
	//void scanPortReceive();

private:
	//QSerialPort* scanPort;
	QTimer* timer;
	//bool scanCRFlag;
	//std::string scanMsg;
};
