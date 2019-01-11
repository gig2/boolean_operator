#include "mainopenglwidget.h"

Mesh::MeshT constructCube()
{
    Mesh::MeshT mesh;

    // let's generate a cube
    using Point        = typename Mesh::MeshT::Point;
    using VertexHandle = typename Mesh::MeshT::VertexHandle;

    std::array<VertexHandle, 8> vhandle;

    vhandle[ 0 ] = mesh.add_vertex( 0.15 * Point{-1, -1, 1} );
    vhandle[ 1 ] = mesh.add_vertex( 0.15 * Point{1, -1, 1} );
    vhandle[ 2 ] = mesh.add_vertex( 0.15 * Point{1, 1, 1} );
    vhandle[ 3 ] = mesh.add_vertex( 0.15 * Point{-1, 1, 1} );
    vhandle[ 4 ] = mesh.add_vertex( 0.15 * Point{-1, -1, -1} );
    vhandle[ 5 ] = mesh.add_vertex( 0.15 * Point{1, -1, -1} );
    vhandle[ 6 ] = mesh.add_vertex( 0.15 * Point{1, 1, -1} );
    vhandle[ 7 ] = mesh.add_vertex( 0.15 * Point{-1, 1, -1} );

    std::vector<VertexHandle> faceVhandles;

    faceVhandles.clear();
    faceVhandles.push_back( vhandle[ 0 ] );
    faceVhandles.push_back( vhandle[ 1 ] );
    faceVhandles.push_back( vhandle[ 2 ] );
    faceVhandles.push_back( vhandle[ 3 ] );
    mesh.add_face( faceVhandles );


    faceVhandles.clear();
    faceVhandles.push_back( vhandle[ 7 ] );
    faceVhandles.push_back( vhandle[ 6 ] );
    faceVhandles.push_back( vhandle[ 5 ] );
    faceVhandles.push_back( vhandle[ 4 ] );
    mesh.add_face( faceVhandles );


    faceVhandles.clear();
    faceVhandles.push_back( vhandle[ 1 ] );
    faceVhandles.push_back( vhandle[ 0 ] );
    faceVhandles.push_back( vhandle[ 4 ] );
    faceVhandles.push_back( vhandle[ 5 ] );
    mesh.add_face( faceVhandles );

    faceVhandles.clear();
    faceVhandles.push_back( vhandle[ 2 ] );
    faceVhandles.push_back( vhandle[ 1 ] );
    faceVhandles.push_back( vhandle[ 5 ] );
    faceVhandles.push_back( vhandle[ 6 ] );
    mesh.add_face( faceVhandles );


    faceVhandles.clear();
    faceVhandles.push_back( vhandle[ 3 ] );
    faceVhandles.push_back( vhandle[ 2 ] );
    faceVhandles.push_back( vhandle[ 6 ] );
    faceVhandles.push_back( vhandle[ 7 ] );
    mesh.add_face( faceVhandles );


    faceVhandles.clear();
    faceVhandles.push_back( vhandle[ 0 ] );
    faceVhandles.push_back( vhandle[ 3 ] );
    faceVhandles.push_back( vhandle[ 7 ] );
    faceVhandles.push_back( vhandle[ 4 ] );
    mesh.add_face( faceVhandles );

    return mesh;
}



template <typename MeshType>
void applyTransform( MeshType& openmesh, glm::mat4 otherMeshTransform )
{
    std::for_each( openmesh.vertices_begin(), openmesh.vertices_end(),
                   [&openmesh, otherMeshTransform]( auto const& vh ) //
                   {
                       auto point = openmesh.point( vh );
                       glm::vec4 vpoint{point[ 0 ], point[ 1 ], point[ 2 ], 1.};

                       vpoint = otherMeshTransform * vpoint;

                       for ( int iDir = 0; iDir < 3; ++iDir )
                       {
                           point[ iDir ] = vpoint[ iDir ];
                       }

                       openmesh.set_point( vh, point );
                   } );
}

MainOpenGLWidget::MainOpenGLWidget( QWidget* parent )
    : QOpenGLWidget( parent )
    , referenceMesh_{"bunnyLowPoly.obj"}
    , referenceMeshNode_{referenceMesh_}
    , otherMesh_{}
    , otherMeshNode_{otherMesh_}

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

    otherMeshTransform_ = glm::translate( glm::mat4{1.f}, glm::vec3{.4, 0., 0.} );
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

    referenceMeshNode_.updateVertexBuffer( positionLocation, colorLocation );

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
        = glm::lookAt( glm::vec3( 1., 1., 1. ), glm::vec3( 0., 0., 0. ), glm::vec3( 0, 1, 0 ) );

    //
    glEnable( GL_DEPTH_TEST );
    glClearColor( .0f, 0.f, 0.f, .0f );

    previousTime_ = std::chrono::high_resolution_clock::now();

    resetColors( referenceMesh_.mesh );

    referenceMesh_.refreshBuffer();
    referenceMeshNode_.updateVertexBuffer();

    // HERE we create the mesh
    auto& openmesh = otherMesh_.mesh;

    openmesh = constructCube();
    // createMyMesh(openmesh);
    // create all the needed element
    // when finish
    // update otherMesh_ vertex tables, color tables


    // set a colors
    resetColors( openmesh );

    otherMeshOrig_ = openmesh;

    applyTransform( openmesh, otherMeshTransform_ );

    otherMesh_.refreshBuffer();
    otherMeshNode_.updateVertexBuffer();

    computeOctree_
        = std::make_unique<ComputeOctree<Mesh::MeshT>>( referenceMesh_.mesh, otherMesh_.mesh );
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

    modelview_ = glm::lookAt( camPos_, camTar_, glm::vec3( 0, 1, 0 ) );

    glm::mat4 mvp = projection_ * modelview_;

    auto mvpLoc = simpleShader_.GetUniformLocation( "MVP" );


    glUniformMatrix4fv( mvpLoc, 1, GL_FALSE, glm::value_ptr( mvp ) );

    referenceMeshNode_.draw();

    otherMeshNode_.draw();

    simpleShader_.Disable();

    previousTime_ = currentTime_;
}

void MainOpenGLWidget::loadReferenceMesh( QString filename )
{
    referenceMesh_.load( filename.toStdString() );


    resetColors( referenceMesh_.mesh );

    referenceMesh_.refreshBuffer();


    makeCurrent();
    referenceMeshNode_.updateVertexBuffer();
    doneCurrent();

    update();
}

void MainOpenGLWidget::loadOtherMesh( QString filename )
{
    otherMesh_.load( filename.toStdString() );

    resetColors( otherMesh_.mesh );

    otherMeshOrig_ = otherMesh_.mesh;

    applyTransform( otherMesh_.mesh, otherMeshTransform_ );

    otherMesh_.refreshBuffer();

    makeCurrent();
    otherMeshNode_.updateVertexBuffer();
    doneCurrent();

    update();
}

template <typename OctreeType, typename Functor>
void applyOnOctree( OctreeType const& octree, Functor&& function )
{
    if ( octree.isLeaf() )
    {
        auto const& mesh1in = octree.getMesh1In();
        auto const& mesh2in = octree.getMesh2In();

        if ( mesh1in.size() > 0 && mesh2in.size() > 0 )
        {
            function( mesh1in, mesh2in );
        }
    }
    else
    {
        auto const& childrens = octree.childrens();
        for ( auto const& children : childrens )
        {
            applyOnOctree( *children, function );
        }
    }
}

template <typename OctreeType>
void callIntersect( OctreeType& octree )
{
    if ( octree.isLeaf() )
    {
        std::cout << "Na: " << octree.getMesh1In().size() << "\n";
        std::cout << "Nb: " << octree.getMesh2In().size() << "\n";
    }
    else
    {
        auto const& childrens = octree.childrens();
        for ( auto const& children : childrens )
        {
            callIntersect( *children );
        }
    }
}



void MainOpenGLWidget::computeOctree()
{
    try
    {
        computeOctree_
            = std::make_unique<ComputeOctree<Mesh::MeshT>>( referenceMesh_.mesh, otherMesh_.mesh );
        intersectMesh_ = std::make_unique<IntersectMesh>( referenceMesh_.mesh, otherMesh_.mesh );
    }
    catch ( std::exception const& e )
    {
        std::cerr << e.what() << "\n";
        computeOctree_ = nullptr;
    }

    if ( computeOctree_ )
    {
        auto const& octree = computeOctree_->octree();
        if ( intersectMesh_ )
        {
            applyOnOctree( octree, *intersectMesh_ );
        }
    }

    referenceMesh_.refreshBuffer();
    otherMesh_.refreshBuffer();

    makeCurrent();

    referenceMeshNode_.updateVertexBuffer();
    otherMeshNode_.updateVertexBuffer();

    doneCurrent();

    update();
}

void MainOpenGLWidget::camX( double value )
{
    camPos_[ 0 ] = value;
    update();
}
void MainOpenGLWidget::camY( double value )
{
    camPos_[ 1 ] = value;
    update();
}
void MainOpenGLWidget::camZ( double value )
{
    camPos_[ 2 ] = value;
    update();
}

void MainOpenGLWidget::tarX( double value )
{
    camTar_[ 0 ] = value;
    update();
}
void MainOpenGLWidget::tarY( double value )
{
    camTar_[ 0 ] = value;
    update();
}
void MainOpenGLWidget::tarZ( double value )
{
    camTar_[ 0 ] = value;
    update();
}

void MainOpenGLWidget::eulerX( double value )
{
    eulerAxis_[ 0 ] = value;
}

void MainOpenGLWidget::eulerY( double value )
{
    eulerAxis_[ 1 ] = value;
}
void MainOpenGLWidget::eulerZ( double value )
{
    eulerAxis_[ 2 ] = value;
}

void MainOpenGLWidget::posX( double value )
{
    translate_[ 0 ] = value;
}
void MainOpenGLWidget::posY( double value )
{
    translate_[ 1 ] = value;
}
void MainOpenGLWidget::posZ( double value )
{
    translate_[ 2 ] = value;
}

void MainOpenGLWidget::refreshTransformation()
{
    glm::mat4 eulerTransform = glm::eulerAngleXYZ( eulerAngle, eulerAxis );
    glm::mat4 translation    = glm::translate( glm::mat4{1.f}, translate_ );

    otherMeshTransform_ = translation * eulerTransform;
    otherMeshNode_.mesh = otherMeshOrig_;
    applyTransform( otherMeshNode_.mesh, otherMeshTransform_ );

    otherMesh_.refreshBuffer();
    makeCurrent();
    otherMeshNode_.updateVertexBuffer();
    doneCurrent();

    update();
}
