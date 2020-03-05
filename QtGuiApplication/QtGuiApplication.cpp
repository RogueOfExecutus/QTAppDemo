#include "QtGuiApplication.h"
#include "ConfigGuiClass.h"
#include <QMetaType>
//#include "opencv2/opencv.hpp"

#define GET_LOGGER(x) log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(x))

QtGuiApplication::QtGuiApplication(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.runButton, SIGNAL(clicked()), this, SLOT(startWork()));
	connect(ui.configButton, SIGNAL(clicked()), this, SLOT(configWindow()));
	checkFile("log");
	checkFile("config");
	initConfig();
	initLog4cplus("work", "./log/work.log");
	initLog4cplus("serial", "./log/serial.log");
	initLog4cplus("mes", "./log/mes.log");
	runningFlag = false;
	qRegisterMetaType<std::string>("std::string");
}

void QtGuiApplication::showMsg(QString str)
{
	QString showText = ui.textBrowser->toPlainText() + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") + str + "\r\n";
	if (showText.length() > 2000)
	{
		showText.remove(0, 1000);
		showText.remove(0, showText.indexOf("\r\n") + 2);
	}
	ui.textBrowser->setText(showText);
	ui.textBrowser->moveCursor(QTextCursor::End);
}

void QtGuiApplication::recError()
{
	runningFlag = false;
	emit stopThread();
	plcWorkThread.quit();
	plcWorkThread.wait();
	oneWorkThread.quit();
	oneWorkThread.wait();
	ui.runButton->setText(u8"����");
}

void QtGuiApplication::closeEvent(QCloseEvent* e)
{
	if (runningFlag)
	{
		showMsg(QString(u8"���������У���ֹͣ�����˳�������"));
		e->ignore();
	}
	else
	{
		QMessageBox message(QMessageBox::NoIcon, "tips", "Do you want to quit?", QMessageBox::Yes | QMessageBox::No, NULL);
		if (message.exec() == QMessageBox::Yes)
			e->accept();
		else
			e->ignore();
	}
}

void QtGuiApplication::initConfig()
{
	//Qt��ʹ��QSettings���дini�ļ�  
    //QSettings���캯���ĵ�һ��������ini�ļ���·��,�ڶ���������ʾ���ini�ļ�,��������������ȱʡ  
	QSettings configIni("config/config.ini", QSettings::IniFormat);
	//��ini�ļ���д������,setValue���������������Ǽ�ֵ��
	logLevel = configIni.value("log/LogLevel", 20000).toInt();
	isCR = configIni.value("config/EOFCR", 1).toInt();

	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		ui.comboBoxPLC->addItem(info.portName());
		ui.comboBoxScanner->addItem(info.portName());
	}
	ui.comboBoxPLC->setCurrentIndex(ui.comboBoxPLC->findText(configIni.value("serial/plc", "COM3").toString()));
	ui.comboBoxScanner->setCurrentIndex(ui.comboBoxScanner->findText(configIni.value("serial/scanner", "COM4").toString()));
}

void QtGuiApplication::writeConfig()
{
	//QSettings���캯���ĵ�һ��������ini�ļ���·��,�ڶ���������ʾ���ini�ļ�,��������������ȱʡ  
	QSettings configIni("config/config.ini", QSettings::IniFormat);
	//configIni.setValue("log/LogLevel", logLevel);
	configIni.setValue("serial/plc", ui.comboBoxPLC->currentText());
	configIni.setValue("serial/scanner", ui.comboBoxScanner->currentText());
	//configIni.setValue("config/EOFCR", isCR);
}

void QtGuiApplication::initLog4cplus(std::string name, std::string path)
{
	log4cplus::SharedAppenderPtr fileAppender(new log4cplus::RollingFileAppender(
		LOG4CPLUS_STRING_TO_TSTRING(path),
		5 * 1024 * 1024,
		5));

	fileAppender->setName(LOG4CPLUS_TEXT("test"));
	log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%y/%m/%d %H:%M:%S,%Q} [%t] %-5p %c - %m [%l]%n");
	fileAppender->setLayout(std::unique_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern)));

	log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_STRING_TO_TSTRING(name));
	logger.setLogLevel(logLevel);

	logger.addAppender(fileAppender);
}

void QtGuiApplication::checkFile(QString path)
{
	QDir dir;
	dir.mkpath(path);
}

void QtGuiApplication::startWork()
{
	if (runningFlag)
	{
		runningFlag = false;
		emit stopThread();
		plcWorkThread.quit();
		plcWorkThread.wait();
		oneWorkThread.quit();
		oneWorkThread.wait();
		ui.runButton->setText(u8"����");
		showMsg(tr(u8"��������ֹͣ������"));
	}
	else
	{
		if (ui.comboBoxPLC->currentIndex() == -1 || ui.comboBoxScanner->currentIndex() == -1)
		{
			QMessageBox::about(NULL, "tips", u8"��ѡ�񴮿ں�");
			return;
		}
		writeConfig();
		plcWorker = new WorkerOmron();
		plcWorker->moveToThread(&plcWorkThread);
		oneWorker = new WorkerOne();
		oneWorker->moveToThread(&oneWorkThread);

		connect(this, &QtGuiApplication::startThread, plcWorker, &WorkerOmron::initAll);
		connect(this, &QtGuiApplication::stopThread, plcWorker, &WorkerOmron::stopRunning, Qt::DirectConnection);
		connect(&plcWorkThread, &QThread::finished, plcWorker, &QObject::deleteLater);
		connect(plcWorker, &WorkerOmron::workMsgShow, this, &QtGuiApplication::showMsg);
		connect(plcWorker, &WorkerOmron::sendError, this, &QtGuiApplication::recError);
		connect(plcWorker, &WorkerOmron::triggerOne, oneWorker, &WorkerOne::recTrigger);

		connect(this, &QtGuiApplication::startThread, oneWorker, &WorkerOne::initAll);
		connect(this, &QtGuiApplication::stopThread, oneWorker, &WorkerOne::stopRunning, Qt::DirectConnection);
		connect(&oneWorkThread, &QThread::finished, oneWorker, &QObject::deleteLater);
		connect(oneWorker, &WorkerOne::workMsgShow, this, &QtGuiApplication::showMsg);
		connect(oneWorker, &WorkerOne::answer2plc, plcWorker, &WorkerOmron::add2SendMsg, Qt::DirectConnection);
		connect(oneWorker, &WorkerOne::sendError, this, &QtGuiApplication::recError);

		runningFlag = true;
		plcWorkThread.start();
		oneWorkThread.start();
		emit startThread();
		ui.runButton->setText(u8"��ͣ");
		showMsg(tr(u8"����ʼ���С�����"));
	}
}

void QtGuiApplication::configWindow()
{
	ConfigGuiClass config;
	config.exec();
	initConfig();
}
