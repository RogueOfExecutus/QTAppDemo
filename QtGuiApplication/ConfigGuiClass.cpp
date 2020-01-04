#include "ConfigGuiClass.h"
#include <QSettings>

ConfigGuiClass::ConfigGuiClass(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.saveButton, &QPushButton::clicked, this, &ConfigGuiClass::writeConfig);
	initConfig();
}

ConfigGuiClass::~ConfigGuiClass()
{
}

void ConfigGuiClass::initConfig()
{
	QSettings configIni("config\\config.ini", QSettings::IniFormat);
	//向ini文件中写入内容,setValue函数的两个参数是键值对
	QStringList levelList = { "TRACE","DEBUG","INFO","WARN","ERROR","FATAL" };
	ui.comboBoxLogLevel->addItems(levelList);
	ui.comboBoxLogLevel->setCurrentIndex(configIni.value("log/LogLevel", 20000).toInt() / 10000);
	ui.checkBoxEOFCR->setChecked(configIni.value("config/EOFCR", 1).toInt());
}

void ConfigGuiClass::writeConfig()
{
	//QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省   
	QSettings configIni("config\\config.ini", QSettings::IniFormat);
	configIni.setValue("log/LogLevel", ui.comboBoxLogLevel->currentIndex() * 10000);
	configIni.setValue("config/EOFCR", ui.checkBoxEOFCR->isChecked() ? 1 : 0);
	close();
}
