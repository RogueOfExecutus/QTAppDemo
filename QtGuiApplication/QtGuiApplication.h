#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include "ui_QtGuiApplication.h"
#include <QSerialPortInfo>
#include <QSettings>
#include <QDateTime>
#include <QCloseEvent>
#include <QThread>
#include <QDir>
#include "WorkerOmron.h"
#include "WorkerOne.h"
#include "log4cplus/log4cplus.h"

class QtGuiApplication : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication(QWidget *parent = Q_NULLPTR);
	void closeEvent(QCloseEvent* e);

signals:
	void startThread(QString portName);
	void stopThread();

private:
	Ui::QtGuiApplicationClass ui;
	log4cplus::Initializer initializer;
	QThread plcWorkThread;
	WorkerOmron *plcWorker;
	bool runningFlag;
	void initConfig();
	void writeConfig();
	int isCR;
	int logLevel;
	void initLog4cplus(std::string name, std::string path);
	void checkFile(QString path);

public slots:
	void startWork();
	void configWindow();
	void showMsg(QString str);
};
