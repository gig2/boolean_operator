#pragma once

#include "engine/Engine-main.h"
/* #include <QOpenGLFunctions> */
#include <QOpenGLWidget>
#include <QWidget>

class MainOpenGLWidget : public QOpenGLWidget //, protected QOpenGLFunctions
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
    }

protected:
    void initializeGL() override
    {
        glewExperimental = GL_TRUE;
        GLenum initGlew{glewInit()};

        if ( initGlew != GLEW_OK )
        {
            throw std::runtime_error(
                reinterpret_cast<const char *>( glewGetErrorString( initGlew ) ) );
        }
        /* initializeOpenGLFunctions(); */
        //
        glClearColor( .3f, 0.3f, 0.3f, 1.0f );
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
    S3DE::CEngine<MainOpenGLWidget> engine_;
    /* std::unique_ptr<QOpenGLContext> context; */
};
