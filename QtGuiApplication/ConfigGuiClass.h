#pragma once

#include <QDialog>
#include "ui_ConfigGuiClass.h"

class ConfigGuiClass : public QDialog
{
	Q_OBJECT

public:
	ConfigGuiClass(QWidget *parent = Q_NULLPTR);
	~ConfigGuiClass();

public slots:
	void writeConfig();

private:
	Ui::ConfigGuiClass ui;
	void initConfig();
};
