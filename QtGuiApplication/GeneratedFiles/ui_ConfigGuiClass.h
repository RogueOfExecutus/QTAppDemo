/********************************************************************************
** Form generated from reading UI file 'ConfigGuiClass.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGGUICLASS_H
#define UI_CONFIGGUICLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConfigGuiClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBoxLogLevel;
    QSpacerItem *verticalSpacer;
    QCheckBox *checkBoxEOFCR;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *verticalSpacer_3;
    QPushButton *saveButton;
    QSpacerItem *verticalSpacer_4;
    QPushButton *cancelButton;

    void setupUi(QDialog *ConfigGuiClass)
    {
        if (ConfigGuiClass->objectName().isEmpty())
            ConfigGuiClass->setObjectName(QString::fromUtf8("ConfigGuiClass"));
        ConfigGuiClass->resize(400, 300);
        verticalLayout = new QVBoxLayout(ConfigGuiClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(ConfigGuiClass);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        comboBoxLogLevel = new QComboBox(ConfigGuiClass);
        comboBoxLogLevel->setObjectName(QString::fromUtf8("comboBoxLogLevel"));

        horizontalLayout->addWidget(comboBoxLogLevel);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout->addItem(verticalSpacer);

        checkBoxEOFCR = new QCheckBox(ConfigGuiClass);
        checkBoxEOFCR->setObjectName(QString::fromUtf8("checkBoxEOFCR"));

        horizontalLayout->addWidget(checkBoxEOFCR);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout->addItem(verticalSpacer_2);

        horizontalLayout->setStretch(4, 1);

        verticalLayout->addLayout(horizontalLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalSpacer_3 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout_2->addItem(verticalSpacer_3);

        saveButton = new QPushButton(ConfigGuiClass);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));

        horizontalLayout_2->addWidget(saveButton);

        verticalSpacer_4 = new QSpacerItem(10, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout_2->addItem(verticalSpacer_4);

        cancelButton = new QPushButton(ConfigGuiClass);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout_2->addWidget(cancelButton);

        horizontalLayout_2->setStretch(0, 1);

        verticalLayout->addLayout(horizontalLayout_2);

        verticalLayout->setStretch(1, 1);

        retranslateUi(ConfigGuiClass);
        QObject::connect(cancelButton, SIGNAL(clicked()), ConfigGuiClass, SLOT(close()));

        QMetaObject::connectSlotsByName(ConfigGuiClass);
    } // setupUi

    void retranslateUi(QDialog *ConfigGuiClass)
    {
        ConfigGuiClass->setWindowTitle(QApplication::translate("ConfigGuiClass", "ConfigGuiClass", nullptr));
        label->setText(QApplication::translate("ConfigGuiClass", "Log Level:", nullptr));
        checkBoxEOFCR->setText(QApplication::translate("ConfigGuiClass", "EOFCR", nullptr));
        saveButton->setText(QApplication::translate("ConfigGuiClass", "\344\277\235\345\255\230", nullptr));
        cancelButton->setText(QApplication::translate("ConfigGuiClass", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigGuiClass: public Ui_ConfigGuiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGGUICLASS_H
