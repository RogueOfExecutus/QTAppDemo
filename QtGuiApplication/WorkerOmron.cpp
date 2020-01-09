#include "WorkerOmron.h"
#include "log4cplus/log4cplus.h"
#include <QSettings>

#define GET_LOGGER(x) log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(x))

WorkerOmron::WorkerOmron(QObject* parent)
	: QObject(parent)
{
	timeoutTimes = 0;
	RunningFlag = false;
	plcCRFlag = true;
	sendFlag = false;
}

WorkerOmron::~WorkerOmron()
{
	if (plcPort->isOpen())
	{
		plcPort->clear();
		plcPort->close();
	}
	delete plcPort;
	timer->stop();
	delete timer;
}

void WorkerOmron::initAll()
{
	QSettings configIni("config/config.ini", QSettings::IniFormat);
	connect(this, &WorkerOmron::plcPortSender, this, &WorkerOmron::plcPortSend);
	plcPort = new QSerialPort();
	plcPort->setBaudRate(QSerialPort::Baud115200);
	plcPort->setDataBits(QSerialPort::Data7);
	plcPort->setFlowControl(QSerialPort::NoFlowControl);
	plcPort->setParity(QSerialPort::EvenParity);
	plcPort->setStopBits(QSerialPort::TwoStop);
	plcPort->setPortName(configIni.value("serial/plc", "COM3").toString());
	connect(plcPort, &QSerialPort::readyRead, this, &WorkerOmron::plcPortReceive);
	if (!plcPort->open(QIODevice::ReadWrite))
	{
		emit workMsgShow(u8"打开串口失败");
		return;
	}
	timer = new QTimer();
	timer->setInterval(1000);
	connect(timer, &QTimer::timeout, this, &WorkerOmron::plcRecTimeOut);
	emit plcPortSender();
	RunningFlag = true;
	LOG4CPLUS_INFO(GET_LOGGER("work"), u8"plc工作线程开始");
}

void WorkerOmron::plcPortSend()
{
	std::string msg = checkTrigger;
	sendMsgMtx.lock();
	if (sendFlag)
	{
		if (!msgs.empty())
		{
			msg = msgs[0];
			msgs.erase(msgs.begin());
			if (msgs.empty()) sendFlag = false;
		}
	}
	sendMsgMtx.unlock();
	//if (msg == "") msg = checkTrigger;
	plcPort->write(msg.c_str());
	plcPort->flush();
	plcPort->waitForBytesWritten();
	timer->start();
}

void WorkerOmron::plcPortReceive()
{
	if (plcCRFlag) plcMsg.clear();
	QByteArray info = plcPort->readAll();
	plcMsg += info.toStdString();
	if (plcCRFlag = plcMsg[plcMsg.length() - 1] == '\r')
	{
		/**
		解析plc数据，获取到触发要把复位指令放入msgs
		*/
		timeoutTimes = 0;
		runTypeMtx.lock();
		if(RunningFlag) emit plcPortSender();
		runTypeMtx.unlock();
	}
}

void WorkerOmron::plcRecTimeOut()
{
	if (++timeoutTimes >= 3) emit workMsgShow(u8"PLC连续" + QString::number(timeoutTimes) + u8"次以上回复超时，请检查通信！！");
	emit plcPortSender();
}

void WorkerOmron::add2SendMsg(std::string msg)
{
	sendMsgMtx.lock();
	msgs.push_back(msg);
	sendFlag = true;
	sendMsgMtx.unlock();
}

void WorkerOmron::stopRunning()
{
	runTypeMtx.lock();
	RunningFlag = false;
	runTypeMtx.unlock();
}