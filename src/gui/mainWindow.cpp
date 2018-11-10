#include "mainopenglwidget.h"

#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <QMainWindow>

#include <memory>

SimpleWindowWithGL::SimpleWindowWithGL( QWidget *parent )
    : QMainWindow( parent )
    , ui{std::make_unique<Ui::SimpleWindowWithGL>()}
{
    ui->setupUi( this );
}

SimpleWindowWithGL::~SimpleWindowWithGL()
{
    //
}
