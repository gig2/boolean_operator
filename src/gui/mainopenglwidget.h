#pragma once

#include <GL/glew.h>

#include "OpenGLMeshRender/meshnode.h"
#include "OpenGLShader/shader.h"
#include "core/mesh.h"

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
    // set color for mesh
    std::for_each( mesh.vertices_begin(), mesh.vertices_end(), [&mesh]( auto &vertex ) {
        typename Mesh::MeshT::Color color{0.25, 0.5, 0.5};
        mesh.set_color( vertex, color );
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
