#include "simpleWindow.h"


SimpleWindow::SimpleWindow()
    : quitButton_{std::make_unique<QPushButton>( "Quit", this )}
    , aboutQtButton_{std::make_unique<QPushButton>( "About Qt", this )}
    , widthSlider_{std::make_unique<QSlider>( Qt::Horizontal, this )}
    , heightSlider_{std::make_unique<QSlider>( Qt::Vertical, this )}
{
    setFixedSize( baseWidth_, baseHeight_ );

    aboutQtButton_->move( 100, 0 );

    widthSlider_->setGeometry( 10, 60, 150, 200 );
    heightSlider_->setGeometry( 0, 60, 10, 100 );


    widthSlider_->setRange( 0, 100 );
    heightSlider_->setRange( 0, 100 );


    QObject::connect( quitButton_.get(), SIGNAL( clicked() ), qApp, SLOT( quit() ) );

    QObject::connect( aboutQtButton_.get(), SIGNAL( clicked() ), qApp, SLOT( aboutQt() ) );

    QObject::connect( widthSlider_.get(), SIGNAL( valueChanged( int ) ), this,
                      SLOT( increaseBaseWidth( int ) ) );


    QObject::connect( heightSlider_.get(), SIGNAL( valueChanged( int ) ), this,
                      SLOT( increaseBaseHeight( int ) ) );
}

void SimpleWindow::increaseBaseWidth( int pixel )
{
    setFixedSize( baseWidth_ + pixel, height() );
}
void SimpleWindow::increaseBaseHeight( int pixel )
{
    setFixedSize( width(), baseHeight_ + pixel );
}
