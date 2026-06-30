/********************************************************************************
** Form generated from reading UI file 'hospitalsoftware.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOSPITALSOFTWARE_H
#define UI_HOSPITALSOFTWARE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HospitalSoftware
{
public:
    QAction *actionLese_Aufenthalte;
    QAction *actionLese_ICD_Codes;
    QAction *actionAufenthaltsueberblick;
    QAction *actionPatient_verlegen;
    QAction *actionPatient_entlassen;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuDatei;
    QMenu *menuAufenthalte;
    QMenu *menuOperationen_und_Prozeduren;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *HospitalSoftware)
    {
        if (HospitalSoftware->objectName().isEmpty())
            HospitalSoftware->setObjectName("HospitalSoftware");
        HospitalSoftware->resize(1200, 750);
        actionLese_Aufenthalte = new QAction(HospitalSoftware);
        actionLese_Aufenthalte->setObjectName("actionLese_Aufenthalte");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/stays.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionLese_Aufenthalte->setIcon(icon);
        actionLese_ICD_Codes = new QAction(HospitalSoftware);
        actionLese_ICD_Codes->setObjectName("actionLese_ICD_Codes");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icd.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionLese_ICD_Codes->setIcon(icon1);
        actionAufenthaltsueberblick = new QAction(HospitalSoftware);
        actionAufenthaltsueberblick->setObjectName("actionAufenthaltsueberblick");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/overview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionAufenthaltsueberblick->setIcon(icon2);
        actionPatient_verlegen = new QAction(HospitalSoftware);
        actionPatient_verlegen->setObjectName("actionPatient_verlegen");
        actionPatient_entlassen = new QAction(HospitalSoftware);
        actionPatient_entlassen->setObjectName("actionPatient_entlassen");
        centralwidget = new QWidget(HospitalSoftware);
        centralwidget->setObjectName("centralwidget");
        HospitalSoftware->setCentralWidget(centralwidget);
        menubar = new QMenuBar(HospitalSoftware);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 30));
        menuDatei = new QMenu(menubar);
        menuDatei->setObjectName("menuDatei");
        menuAufenthalte = new QMenu(menubar);
        menuAufenthalte->setObjectName("menuAufenthalte");
        menuOperationen_und_Prozeduren = new QMenu(menubar);
        menuOperationen_und_Prozeduren->setObjectName("menuOperationen_und_Prozeduren");
        HospitalSoftware->setMenuBar(menubar);
        statusbar = new QStatusBar(HospitalSoftware);
        statusbar->setObjectName("statusbar");
        HospitalSoftware->setStatusBar(statusbar);
        toolBar = new QToolBar(HospitalSoftware);
        toolBar->setObjectName("toolBar");
        HospitalSoftware->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        menubar->addAction(menuDatei->menuAction());
        menubar->addAction(menuAufenthalte->menuAction());
        menubar->addAction(menuOperationen_und_Prozeduren->menuAction());
        menuDatei->addAction(actionLese_Aufenthalte);
        menuDatei->addAction(actionLese_ICD_Codes);
        menuAufenthalte->addAction(actionAufenthaltsueberblick);
        menuAufenthalte->addAction(actionPatient_verlegen);
        menuAufenthalte->addAction(actionPatient_entlassen);
        toolBar->addAction(actionLese_Aufenthalte);
        toolBar->addAction(actionLese_ICD_Codes);
        toolBar->addAction(actionAufenthaltsueberblick);

        retranslateUi(HospitalSoftware);

        QMetaObject::connectSlotsByName(HospitalSoftware);
    } // setupUi

    void retranslateUi(QMainWindow *HospitalSoftware)
    {
        HospitalSoftware->setWindowTitle(QCoreApplication::translate("HospitalSoftware", "HospitalSoftware", nullptr));
        actionLese_Aufenthalte->setText(QCoreApplication::translate("HospitalSoftware", "Lese Aufenthalte", nullptr));
        actionLese_ICD_Codes->setText(QCoreApplication::translate("HospitalSoftware", "Lese ICD Codes", nullptr));
        actionAufenthaltsueberblick->setText(QCoreApplication::translate("HospitalSoftware", "Aufenthalts\303\274berblick", nullptr));
        actionPatient_verlegen->setText(QCoreApplication::translate("HospitalSoftware", "Patient verlegen", nullptr));
#if QT_CONFIG(tooltip)
        actionPatient_verlegen->setToolTip(QCoreApplication::translate("HospitalSoftware", "Patient in andere Abteilung verlegen", nullptr));
#endif // QT_CONFIG(tooltip)
        actionPatient_entlassen->setText(QCoreApplication::translate("HospitalSoftware", "Patient entlassen", nullptr));
        menuDatei->setTitle(QCoreApplication::translate("HospitalSoftware", "Datei", nullptr));
        menuAufenthalte->setTitle(QCoreApplication::translate("HospitalSoftware", "Aufenthalte", nullptr));
        menuOperationen_und_Prozeduren->setTitle(QCoreApplication::translate("HospitalSoftware", "Operationen und Prozeduren", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("HospitalSoftware", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HospitalSoftware: public Ui_HospitalSoftware {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOSPITALSOFTWARE_H
