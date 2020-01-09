#pragma once

#include <QObject>
#include <QSerialPort>
#include <QMutex>
#include <QThread>
#include <QTimer>

class WorkerOmron : public QObject
{
	Q_OBJECT

public:
	WorkerOmron(QObject *parent = Q_NULLPTR);
	~WorkerOmron();

signals:
	//void workDone();
	void workMsgShow(QString msg);
	void plcPortSender();
	void triggerOne(std::string value);
	void sendError();

public slots:
	void initAll();
	void plcPortSend();
	void plcPortReceive();
	void plcRecTimeOut();
	void add2SendMsg(std::string msg);
	void stopRunning();

private:
	QSerialPort* plcPort;
	QTimer* timer;
	int timeoutTimes;
	QMutex runTypeMtx;
	bool RunningFlag;
	bool plcCRFlag;
	std::string plcMsg;
	QMutex sendMsgMtx;
	std::vector<std::string> msgs;
	const std::string checkTrigger = "123456\r";
	const std::string resetTriggerOne = "";
};
