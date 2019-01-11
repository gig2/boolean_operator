#pragma once


#include <QMainWindow>

#include <memory>

namespace Ui
{
class SimpleWindowWithGL;
}

class SimpleWindowWithGL : public QMainWindow
{
    Q_OBJECT


public:
    explicit SimpleWindowWithGL( QWidget *parent = nullptr );

    virtual ~SimpleWindowWithGL();

signals:

    void referenceModelToLoad( QString );
    void otherMeshToLoad( QString );

public slots:

    void requestLoadingReferenceModel();
    void requestLoadingOtherMeshModel();


private:
    std::unique_ptr<Ui::SimpleWindowWithGL> ui;
};
