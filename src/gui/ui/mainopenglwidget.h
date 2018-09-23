#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>

class MainOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    explicit MainOpenGLWidget( QWidget *parent )
        : QOpenGLWidget( parent )

    {
        QSurfaceFormat format;
        format.setDepthBufferSize( 24 );
        format.setStencilBufferSize( 8 );
        format.setVersion( 3, 2 );
        format.setProfile( QSurfaceFormat::CoreProfile );
        setFormat( format );

        create();

        makeCurrent();

        initializeOpenGLFunctions();

        initializeGL();
        paintGL();

        doneCurrent();
    }

protected:
    void initializeGL() override
    {
        //
        glClearColor( 1.0f, 0.0f, 1.0f, 1.0f );
    }

    void resizeGL( int width, int height ) override
    {
        //
    }

    void paintGL() override
    {
        //
        glClear( GL_COLOR_BUFFER_BIT );
    }

private:
    std::unique_ptr<QOpenGLContext> context;
};
