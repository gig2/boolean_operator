#pragma once

#include "ui_mainWindow.h"

#include <QMainWindow>

#include <memory>

class SimpleWindowWithGL : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimpleWindowWithGL( QMainWindow *parent = 0 );

private:
    std::unique_ptr<Ui::SimpleWindowWithGL> ui;
};
