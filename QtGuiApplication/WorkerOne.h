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
	WorkerOne(QObject *parent = Q_NULLPTR);
	~WorkerOne();

signals:
	void answer2plc(std::string msg);
	void workMsgShow(QString msg);
	void sendError();

public slots:
	void initAll(QString portName);
	void recTrigger(std::string value);
	//void scanPortReceive();
	void stopRunning();

private:
	//QSerialPort* scanPort;
	QTimer* timer;
	//bool scanCRFlag;
	//std::string scanMsg;
	bool RunningFlag;
	QMutex runTypeMtx;
	const std::string sendOKOne = "";
	const std::string sendNGOne = "";
};
