#include "mainopenglwidget.h"

#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <QFileDialog>
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

void SimpleWindowWithGL::requestLoadingReferenceModel()
{
    QString filename;
    filename = QFileDialog::getOpenFileName( this, tr( "Select an obj file  ..." ), "",
                                             tr( "OBJ Files (*.obj)" ) );


    if ( !filename.isNull() )
    {
        referenceModelToLoad( filename );
    }
}

void SimpleWindowWithGL::requestLoadingOtherMeshModel()
{
    QString filename;
    filename = QFileDialog::getOpenFileName( this, tr( "Select an obj file  ..." ), "",
                                             tr( "OBJ Files (*.obj)" ) );


    if ( !filename.isNull() )
    {
        otherMeshToLoad( filename );
    }
}
