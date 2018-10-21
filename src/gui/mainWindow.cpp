#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <QMainWindow>

SimpleWindowWithGL::SimpleWindowWithGL( QMainWindow *parent )
    : QMainWindow( parent )
    , ui{std::make_unique<Ui::SimpleWindowWithGL>()}
{
    ui->setupUi( this );
}

SimpleWindowWithGL::~SimpleWindowWithGL()
{
    //
}
