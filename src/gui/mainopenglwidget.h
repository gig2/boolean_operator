#pragma once

#include <GL/glew.h>

#include "mesh/meshnode.h"
#include "tools/shader.h"

#include <QOpenGLWidget>
#include <QWidget>

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <chrono>
#include <string>


class MainOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit MainOpenGLWidget( QWidget *parent )
        : QOpenGLWidget( parent )
        , bunny_{"bunnyLowPoly.obj"}

    {
        int major = 3;
        int minor = 2;

        QSurfaceFormat format;

        format.setDepthBufferSize( 24 );
        format.setStencilBufferSize( 8 );
        format.setVersion( major, minor );
        format.setProfile( QSurfaceFormat::CoreProfile );

        setFormat( format );

        create();

        bunnyTransform_
            = glm::rotate( bunnyTransform_, glm::radians( 90.f ), glm::vec3( 1, 0, 0 ) );
    }

signals:

public slots:

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


        // get them from shader
        int const positionLocation{0};
        int const colorLocation{1};

        bunny_.updateVertexBuffer( positionLocation, colorLocation );

        simpleShader_.SetFile( "shader/color.vs", "shader/color.fs", "shader/color.gs" );


        modelview_ = glm::lookAt( glm::vec3( -1., -1., 1. ), glm::vec3( 0., 0., 0. ),
                                  glm::vec3( 0, 0, 1 ) );

        //
        glClearColor( .0f, 0.f, 0.f, .0f );

        previousTime_ = std::chrono::high_resolution_clock::now();
    }

    void resizeGL( int width, int height ) override
    {
        //
        float near = 0.01;
        float far  = 100;
        float fov  = 70.;

        projection_ = glm::perspective( fov, static_cast<float>( width ) / height, near, far );
    }

    void paintGL() override
    {
        //
        glClear( GL_COLOR_BUFFER_BIT );

        currentTime_ = std::chrono::high_resolution_clock::now();

        simpleShader_.Enable();

        glm::mat4 mvp = projection_ * modelview_;

        auto mvpLoc = simpleShader_.GetUniformLocation( "MVP" );

        // applyBunny Transforms

        glm::mat4 bunnyMvp = mvp * bunnyTransform_;

        glUniformMatrix4fv( mvpLoc, 1, GL_FALSE, glm::value_ptr( bunnyMvp ) );

        bunny_.draw();

        simpleShader_.Disable();

        previousTime_ = currentTime_;
    }


private:
    std::chrono::high_resolution_clock::time_point previousTime_;
    std::chrono::high_resolution_clock::time_point currentTime_;

    MeshNode bunny_;
    glm::mat4 bunnyTransform_;


    S3DE::Shader simpleShader_;

    glm::mat4 projection_;
    glm::mat4 modelview_;
};
