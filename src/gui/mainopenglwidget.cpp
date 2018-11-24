#include "mainopenglwidget.h"


MainOpenGLWidget::MainOpenGLWidget( QWidget* parent )
    : QOpenGLWidget( parent )
    , bunny_{"bunnyLowPoly.obj"}
    , bunnyNode_{bunny_}

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

    bunnyTransform_ = glm::rotate( glm::mat4{1.f}, glm::radians( 90.f ), glm::vec3( 1, 0, 0 ) );
}

void MainOpenGLWidget::initializeGL()
{
    glewExperimental = GL_TRUE;
    GLenum initGlew{glewInit()};

    if ( initGlew != GLEW_OK )
    {
        throw std::runtime_error( reinterpret_cast<const char*>( glewGetErrorString( initGlew ) ) );
    }


    // get them from shader
    int const positionLocation{0};
    int const colorLocation{1};

    bunnyNode_.updateVertexBuffer( positionLocation, colorLocation );

    simpleShader_.setVertexShader( "shader/color.vert" );
    simpleShader_.setFragmentShader( "shader/color.frag" );

    try
    {
        simpleShader_.Load();
    }
    catch ( std::string const& error )
    {
        std::cerr << error << "\n";
    }
    catch ( std::exception const& error )
    {
        std::cerr << error.what() << "\n";
    }
    catch ( ... )
    {
        std::cerr << "unknown exception\n";
    }


    modelview_
        = glm::lookAt( glm::vec3( -1., -1., 1. ), glm::vec3( 0., 0., 0. ), glm::vec3( 0, 0, 1 ) );

    //
    glEnable( GL_DEPTH_TEST );
    glClearColor( .0f, 0.f, 0.f, .0f );

    previousTime_ = std::chrono::high_resolution_clock::now();

    resetColors( bunny_.mesh );

    bunny_.refreshBuffer();
    bunnyNode_.updateVertexBuffer();
}

void MainOpenGLWidget::resizeGL( int width, int height )
{
    //
    float near = 0.01;
    float far  = 100;
    float fov  = 70.;

    projection_ = glm::perspective( fov, static_cast<float>( width ) / height, near, far );
}

void MainOpenGLWidget::paintGL()
{
    //
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    currentTime_ = std::chrono::high_resolution_clock::now();

    simpleShader_.Enable();

    glm::mat4 mvp = projection_ * modelview_;

    auto mvpLoc = simpleShader_.GetUniformLocation( "MVP" );

    // applyBunny Transforms

    glm::mat4 bunnyMvp = mvp * bunnyTransform_;

    glUniformMatrix4fv( mvpLoc, 1, GL_FALSE, glm::value_ptr( bunnyMvp ) );

    bunnyNode_.draw();

    simpleShader_.Disable();

    previousTime_ = currentTime_;
}
