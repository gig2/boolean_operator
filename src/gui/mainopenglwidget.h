#pragma once

#include "engine/Engine-main.h"
#include "engine/Mesh.h"
#include "engine/MeshManager.h"
#include "tools/Loader.h"
/* #include <QOpenGLFunctions> */
#include <QOpenGLWidget>
#include <QWidget>


#include <chrono>
#include <string>

// Temporary

class QtGLWindow
{
public:
    QtGLWindow( int width, int height, std::string const &title, int major, int minor,
                bool fullscreen )
    {
    }

    void SwapWindow() {}
};


class MainOpenGLWidget : public QOpenGLWidget //, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MainOpenGLWidget( QWidget *parent )
        : QOpenGLWidget( parent )

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
    }

signals:
    void ambientValueChanged( double value );
    void diffuseValueChanged( double value );
    void linearValueChanged( double value );
    void expValueChanged( double value );


public slots:
    void updateAmbient( int value )
    {
        float ambientIntensity = value / 99.;

        for ( auto &pl : pointLight_ )
        {
            pl.AmbientIntensity = ambientIntensity;
        }
        engine_.AttachLight( pointLight_ );
        ambientValueChanged( static_cast<double>( ambientIntensity ) );
        update();
    }
    void updateDiffuse( int value )
    {
        float diffuseIntensity = value / 99.;
        for ( auto &pl : pointLight_ )
        {
            pl.DiffuseIntensity = diffuseIntensity;
        }
        engine_.AttachLight( pointLight_ );
        diffuseValueChanged( static_cast<double>( diffuseIntensity ) );
        update();
    }

    void updateLinear( int value )
    {
        float start = 0.00001;
        float end   = 1.;

        float t = value / 99.;

        float linear = start * ( 1 - t ) + t * end;

        for ( auto &pl : pointLight_ )
        {
            pl.Attenuation.Linear = linear;
        }
        engine_.AttachLight( pointLight_ );
        linearValueChanged( static_cast<double>( linear ) );
        update();
    }

    void updateExp( int value )
    {
        float start = 0.;
        float end   = 0.05;
        float t     = value / 99.;

        float exponential = start * ( 1 - t ) + end * t;
        for ( auto &pl : pointLight_ )
        {
            pl.Attenuation.Exp = exponential;
        }
        engine_.AttachLight( pointLight_ );
        expValueChanged( static_cast<double>( exponential ) );
        update();
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


        S3DE::EngineShader engineShader{"shader/lighting.vs", "shader/lighting.fs",
                                        "shader/lighting.gs", "", ""};

        S3DE::EngineWindow engineWindow;
        engineWindow.shader = engineShader;

        engine_.CreateWindow( engineWindow );


        std::vector<S3DE::MeshPair> meshPair;

        meshPair.push_back( {"bunny", "bunnyLowPoly.obj"} );

        engine_.SetCameraLocation( glm::vec3{10., -2., 6.}, glm::vec3{0., 0., 0.},
                                   glm::vec3{0., 0., 1.} );

        engine_.AddMesh( meshPair );

        auto toRad = []( auto degre ) { return ( degre / 360.0 ) * 2 * M_PI; };

        engine_.SetNodePosRot( "bunny", glm::vec3{0., 0., 0.}, glm::vec3{toRad( 90. ), 0., 0.} );

        engine_.SetNodeScale( "bunny", 5. );
        engine_.SetNodeAnimation( "bunny", "idle" );


        float ambient{0.};
        float diffuse{0.};
        float linear{0.00001};
        float constant{1.0};
        float exp{0.0};

        S3DE::PointLight pl0;
        pl0.Color                = glm::vec3{1.0, 1.0, 1.0};
        pl0.AmbientIntensity     = ambient;
        pl0.DiffuseIntensity     = diffuse;
        pl0.Attenuation.Constant = constant;
        pl0.Attenuation.Linear   = linear;
        pl0.Attenuation.Exp      = exp;
        pl0.Position             = glm::vec3{-5.0, -5.0, 3.0};

        S3DE::PointLight pl1{pl0};
        pl1.Position = glm::vec3{5., 5., 3.};

        pointLight_.push_back( pl0 );
        pointLight_.push_back( pl1 );

        ambientValueChanged( static_cast<double>( ambient ) );
        diffuseValueChanged( static_cast<double>( diffuse ) );
        linearValueChanged( static_cast<double>( linear ) );
        expValueChanged( static_cast<double>( exp ) );

        engine_.AttachLight( pointLight_ );


        // initializeOpenGLFunctions();
        //
        glClearColor( .0f, 0.f, 0.f, .0f );

        previousTime_ = std::chrono::high_resolution_clock::now();
    }

    void resizeGL( int width, int height ) override
    {
        //
        float near = 0.01;
        float far  = 100;
        engine_.SetCameraSettings( 70.0, static_cast<float>( width ) / height, near, far );
    }

    void paintGL() override
    {
        //
        // glClear( GL_COLOR_BUFFER_BIT );

        currentTime_ = std::chrono::high_resolution_clock::now();
        auto elapsed = currentTime_ - previousTime_;
        engine_.Draw( elapsed );
        previousTime_ = currentTime_;
    }


private:
    S3DE::CEngine<QtGLWindow> engine_;
    // std::vector<S3DE::MeshData> pmeshData_;
    std::vector<S3DE::PointLight> pointLight_;

    // S3DE::Camera camera_;

    std::chrono::high_resolution_clock::time_point previousTime_;
    std::chrono::high_resolution_clock::time_point currentTime_;



    // std::unique_ptr<QOpenGLContext> context;
};
