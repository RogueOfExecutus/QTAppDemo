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
	//��ini�ļ���д������,setValue���������������Ǽ�ֵ��
	QStringList levelList = { "TRACE","DEBUG","INFO","WARN","ERROR","FATAL" };
	ui.comboBoxLogLevel->addItems(levelList);
	ui.comboBoxLogLevel->setCurrentIndex(configIni.value("log/LogLevel", 20000).toInt() / 10000);
	ui.checkBoxEOFCR->setChecked(configIni.value("config/EOFCR", 1).toInt());
}

void ConfigGuiClass::writeConfig()
{
	//QSettings���캯���ĵ�һ��������ini�ļ���·��,�ڶ���������ʾ���ini�ļ�,��������������ȱʡ   
	QSettings configIni("config\\config.ini", QSettings::IniFormat);
	configIni.setValue("log/LogLevel", ui.comboBoxLogLevel->currentIndex() * 10000);
	configIni.setValue("config/EOFCR", ui.checkBoxEOFCR->isChecked() ? 1 : 0);
	close();
}
