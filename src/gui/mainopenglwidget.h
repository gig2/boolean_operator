#pragma once

#if 0
#include "engine/Engine-main.h"
#include "engine/Mesh.h"
#include "engine/MeshManager.h"
#include "tools/Loader.h"
#endif

#include <GL/glew.h>

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


// Mesh file

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/Attributes.hh>
#include <OpenMesh/Core/Mesh/Handles.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include <algorithm>
#include <functional>
#include <string>
#include <vector>


struct MeshTraits : public OpenMesh::DefaultTraits
{
    // use floating point
    typedef OpenMesh::Vec3f Point;

    // use vertex normals and vertex colors
    VertexAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color );


    // use face normals
    FaceAttributes( OpenMesh::Attributes::Normal );
};

class MeshNode
{
public:
    MeshNode( std::string const &filename )
    {
        //
        if ( !OpenMesh::IO::read_mesh( mesh_, filename ) )
        {
            throw std::runtime_error( std::string( "Read error for file: " ) + filename );
        }
    }

    void updateVertexBuffer( int const positionLocation, int const colorLocation )
    {
        //
        numIndexes_ = mesh_.n_faces() * verticesNumberPerFace_;

        auto *vertexPointBuffer = mesh_.points();

        std::vector<unsigned int> indexes;
        indexes.reserve( numIndexes_ );



        std::for_each(
            mesh_.faces_begin(), mesh_.faces_end(), [this, &indexes]( auto const &faces ) {
                std::transform( mesh_.cfv_begin( faces ), mesh_.cfv_end( faces ),
                                std::back_inserter( indexes ),
                                []( auto const &faceVertex ) { return faceVertex.idx(); } );
            } );


        // let's put a color
        std::vector<GLfloat> colors;
        colors.resize( mesh_.n_vertices() * numberColors_ );
        for ( auto &cols : colors )
        {
            cols = 0.5;
        }


        regenerateBuffers_();

        glBindVertexArray( vao_ );

        glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer_ );
        glBufferData( GL_ARRAY_BUFFER,
                      sizeof( decltype( *vertexPointBuffer ) ) * mesh_.n_vertices(),
                      vertexPointBuffer, GL_STATIC_DRAW );

        glVertexAttribPointer( positionLocation, 3, GL_FLOAT, GL_FALSE,
                               sizeof( decltype( *vertexPointBuffer ) ), 0 );
        glEnableVertexAttribArray( positionLocation );

        glBindBuffer( GL_ARRAY_BUFFER, colorBuffer_ );
        glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * colors.size(), colors.data(),
                      GL_STATIC_DRAW );

        glVertexAttribPointer( colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 3, 0 );
        glEnableVertexAttribArray( colorLocation );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer_ );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * numIndexes_, indexes.data(),
                      GL_STATIC_DRAW );

        glBindVertexArray( 0 );
    }

    void draw()
    {
        //
    }

    ~MeshNode() { cleanBuffers_(); }

private:
    OpenMesh::TriMesh_ArrayKernelT<MeshTraits> mesh_;

    GLuint vao_{0};
    GLuint indexBuffer_{0};
    GLuint vertexBuffer_{0};
    GLuint colorBuffer_{0};

    std::size_t numIndexes_{0};

    const unsigned int verticesNumberPerFace_{3};
    const unsigned int numberColors_{3};

    void cleanBuffers_()
    {
        if ( glIsBuffer( vao_ ) )
        {
            glDeleteBuffers( 1, &vao_ );
        }
        if ( glIsBuffer( indexBuffer_ ) )
        {
            glDeleteBuffers( 1, &indexBuffer_ );
        }
        if ( glIsBuffer( vertexBuffer_ ) )
        {
            glDeleteBuffers( 1, &vertexBuffer_ );
        }
        if ( glIsBuffer( colorBuffer_ ) )
        {
            glDeleteBuffers( 1, &colorBuffer_ );
        }
    }

    void regenerateBuffers_()
    {
        cleanBuffers_();

        glGenBuffers( 1, &vertexBuffer_ );
        glGenBuffers( 1, &indexBuffer_ );
        glGenBuffers( 1, &colorBuffer_ );

        glGenVertexArrays( 1, &vao_ );
    }
};


// end mesh file

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
    }

signals:
    void ambientValueChanged( double value );
    void diffuseValueChanged( double value );
    void linearValueChanged( double value );
    void expValueChanged( double value );


public slots:
    void updateAmbient( int value )
    {
#if 0
        float ambientIntensity = value / 99.;

        for ( auto &pl : pointLight_ )
        {
            pl.AmbientIntensity = ambientIntensity;
        }
        engine_.AttachLight( pointLight_ );
        ambientValueChanged( static_cast<double>( ambientIntensity ) );
        update();

#endif
    }
    void updateDiffuse( int value )
    {
#if 0
        float diffuseIntensity = value / 99.;
        for ( auto &pl : pointLight_ )
        {
            pl.DiffuseIntensity = diffuseIntensity;
        }
        engine_.AttachLight( pointLight_ );
        diffuseValueChanged( static_cast<double>( diffuseIntensity ) );
        update();
#endif
    }

    void updateLinear( int value )
    {
#if 0
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
#endif
    }

    void updateExp( int value )
    {
#if 0
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
#endif
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


        // get them from shader
        int const positionLocation{0};
        int const colorLocation{1};

        bunny_.updateVertexBuffer( positionLocation, colorLocation );

#if 0

        S3DE::EngineShader engineShader{"shader/color.vs", "shader/color.fs", "shader/color.gs", "",
                                        ""};

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

        // setup positional light

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

        // now the spotlight

        glm::vec3 direction{-5, -5, -3};
        direction = glm::normalize( direction );

        direction[ 0 ] = direction[ 0 ] * 2.;
        direction[ 1 ] = direction[ 1 ] * 2.;
        direction[ 2 ] = direction[ 2 ] * 2.;

        S3DE::SpotLight sl;
        sl.Color                = glm::vec3{1.0, 1.0, 1.0};
        sl.AmbientIntensity     = 0.2;
        sl.DiffuseIntensity     = 0.6;
        sl.Attenuation.Constant = 1.;
        sl.Attenuation.Exp      = 0.005;
        sl.Direction            = direction;
        sl.Cutoff               = M_PI / 16.0;
        sl.Position             = glm::vec3{5, 5, 3} + glm::vec3{10, 10, 0};

        spotLight_.push_back( sl );

        engine_.AttachLight( spotLight_ );


#endif

        //
        glClearColor( .0f, 0.f, 0.f, .0f );

        previousTime_ = std::chrono::high_resolution_clock::now();
    }

    void resizeGL( int width, int height ) override
    {
        //
#if 0
        float near = 0.01;
        float far  = 100;
        engine_.SetCameraSettings( 70.0, static_cast<float>( width ) / height, near, far );
#endif
    }

    void paintGL() override
    {
        //
        glClear( GL_COLOR_BUFFER_BIT );

        currentTime_ = std::chrono::high_resolution_clock::now();
#if 0
        auto elapsed = currentTime_ - previousTime_;
        engine_.Draw( elapsed );
#endif
        previousTime_ = currentTime_;
    }


private:
#if 0
    S3DE::CEngine<QtGLWindow> engine_;
    // std::vector<S3DE::MeshData> pmeshData_;
    std::vector<S3DE::PointLight> pointLight_;
    std::vector<S3DE::SpotLight> spotLight_;

    // S3DE::Camera camera_;

#endif
    std::chrono::high_resolution_clock::time_point previousTime_;
    std::chrono::high_resolution_clock::time_point currentTime_;

    MeshNode bunny_;



    // std::unique_ptr<QOpenGLContext> context;
};
