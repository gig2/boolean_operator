#pragma once

#include <QApplication>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

#include <memory>

class SimpleWindow : public QWidget
{
    Q_OBJECT
public:
    SimpleWindow();

public slots:
    void increaseBaseWidth( int pixels );
    void increaseBaseHeight( int pixels );

private:
    std::unique_ptr<QPushButton> quitButton_;
    std::unique_ptr<QPushButton> aboutQtButton_;
    std::unique_ptr<QSlider> widthSlider_;
    std::unique_ptr<QSlider> heightSlider_;

    int baseWidth_{300};
    int baseHeight_{200};
};
