/********************************************************************************
** Form generated from reading UI file 'mainWindowtQLMTC.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOWTQLMTC_H
#define MAINWINDOWTQLMTC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "mainopenglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_SimpleWindowWithGL
{
public:
    QWidget *centralwidget;
    MainOpenGLWidget *openGLWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SimpleWindowWithGL)
    {
        if (SimpleWindowWithGL->objectName().isEmpty())
            SimpleWindowWithGL->setObjectName(QStringLiteral("SimpleWindowWithGL"));
        SimpleWindowWithGL->resize(800, 600);
        centralwidget = new QWidget(SimpleWindowWithGL);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        openGLWidget = new MainOpenGLWidget(centralwidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(50, 110, 300, 200));
        SimpleWindowWithGL->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SimpleWindowWithGL);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 20));
        SimpleWindowWithGL->setMenuBar(menubar);
        statusbar = new QStatusBar(SimpleWindowWithGL);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        SimpleWindowWithGL->setStatusBar(statusbar);

        retranslateUi(SimpleWindowWithGL);

        QMetaObject::connectSlotsByName(SimpleWindowWithGL);
    } // setupUi

    void retranslateUi(QMainWindow *SimpleWindowWithGL)
    {
        SimpleWindowWithGL->setWindowTitle(QApplication::translate("SimpleWindowWithGL", "MainWindow GL Render", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SimpleWindowWithGL: public Ui_SimpleWindowWithGL {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOWTQLMTC_H
