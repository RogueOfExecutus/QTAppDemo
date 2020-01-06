#include "QtGuiApplication.h"
#include "ConfigGuiClass.h"
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

void QtGuiApplication::closeEvent(QCloseEvent* e)
{
	if (runningFlag)
	{
		showMsg(QString(u8"程序运行中，待停止后再退出。。。"));
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
	//Qt中使用QSettings类读写ini文件  
   //QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省  
	QSettings configIni("config/config.ini", QSettings::IniFormat);
	//向ini文件中写入内容,setValue函数的两个参数是键值对
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
	//QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省  
	QSettings configIni("config/config.ini", QSettings::IniFormat);
	configIni.setValue("log/LogLevel", logLevel);
	configIni.setValue("serial/plc", ui.comboBoxPLC->currentText());
	configIni.setValue("serial/scanner", ui.comboBoxScanner->currentText());
	configIni.setValue("config/EOFCR", isCR);
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
		ui.runButton->setText(u8"运行");
	}
	else
	{
		if (ui.comboBoxPLC->currentIndex() == -1 || ui.comboBoxScanner->currentIndex() == -1)
		{
			QMessageBox::about(NULL, "tips", u8"请选择串口号");
			return;
		}
		writeConfig();
		plcWorker = new WorkerOmron();
		plcWorker->moveToThread(&plcWorkThread);
		connect(this, &QtGuiApplication::startThread, plcWorker, &WorkerOmron::initAll);
		connect(this, &QtGuiApplication::stopThread, plcWorker, &WorkerOmron::stopRunning, Qt::DirectConnection);
		connect(&plcWorkThread, &QThread::finished, plcWorker, &QObject::deleteLater);
		connect(plcWorker, &WorkerOmron::workMsgShow, this, &QtGuiApplication::showMsg);
		runningFlag = true;
		plcWorkThread.start();
		emit startThread(ui.comboBoxPLC->currentText());
		ui.runButton->setText(u8"暂停"); 
	}
}

void QtGuiApplication::configWindow()
{
	ConfigGuiClass config;
	config.exec();
	initConfig();
}
