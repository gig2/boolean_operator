#pragma once

#include <GL/glew.h>

#include "OpenGLMeshRender/meshnode.h"
#include "OpenGLShader/shader.h"
#include "mesh.h"

#include <QOpenGLWidget>
#include <QWidget>

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <algorithm>
#include <chrono>
#include <string>

template <typename MeshType>
void resetColors( MeshType &mesh )
{
    using Color = typename MeshType::Color;

    int nVertices = std::distance( mesh.vertices_begin(), mesh.vertices_end() );

    int modulo = std::min( 20, nVertices / 2 + 1 );

    int increment = 0;

    // set color for mesh
    std::for_each( mesh.vertices_begin(), mesh.vertices_end(),
                   [&mesh, &modulo, &increment]( auto &vertex ) {
                       Color color1{0.25, 0.5, 0.5};
                       Color color2{0.5, 1., 1.};

                       increment %= modulo;

                       float t = increment / static_cast<float>( modulo );

                       Color color = ( 1.f - t ) * color1 + t * color2;

                       mesh.set_color( vertex, color );

                       ++increment;
                   } );
}

class MainOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit MainOpenGLWidget( QWidget *parent );

signals:

public slots:

protected:
    virtual void initializeGL() override;

    virtual void resizeGL( int width, int height ) override;

    virtual void paintGL() override;



private:
    std::chrono::high_resolution_clock::time_point previousTime_;
    std::chrono::high_resolution_clock::time_point currentTime_;

    Mesh bunny_;
    MeshNode<Mesh> bunnyNode_;
    glm::mat4 bunnyTransform_{1.f};

    Mesh otherMesh_;
    MeshNode<Mesh> otherMeshNode_;
    glm::mat4 otherMeshTransform_{1.f};


    S3DE::Shader simpleShader_;

    glm::mat4 projection_;
    glm::mat4 modelview_;
};
