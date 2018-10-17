#pragma once
// Mesh file
#include <GL/glew.h>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/Attributes.hh>
#include <OpenMesh/Core/Mesh/Handles.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include <string>


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
    explicit MeshNode( std::string const &filename );

    void updateVertexBuffer( int const positionLocation, int const colorLocation );

    void draw();

    ~MeshNode();

private:
    OpenMesh::TriMesh_ArrayKernelT<MeshTraits> mesh_;

    GLuint vao_{0};
    GLuint indexBuffer_{0};
    GLuint vertexBuffer_{0};
    GLuint colorBuffer_{0};

    std::size_t numIndexes_{0};

    const unsigned int verticesNumberPerFace_{3};
    const unsigned int numberColors_{3};

    void cleanBuffers_();

    void regenerateBuffers_();
};


// end mesh file
