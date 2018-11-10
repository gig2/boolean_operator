#pragma once

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/Attributes.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <string>

struct MeshTraits : public OpenMesh::DefaultTraits
{
    // use floating point
    typedef OpenMesh::Vec3f Point;
    typedef OpenMesh::Vec3f Color;

    // use vertex normals and vertex colors
    VertexAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color );


    // use face normals
    FaceAttributes( OpenMesh::Attributes::Normal );
};


class Mesh
{
public:
    using MeshT = OpenMesh::TriMesh_ArrayKernelT<MeshTraits>;

    using PointT = MeshTraits::Point;

    using PointScalarT = PointT::value_type;

    explicit Mesh( std::string filename );

    Mesh() = default;

    MeshT mesh;


    void refreshBuffer();


    // Interface for OpenGLMeshRender
    int getNumIndex() const { return indexes_.size(); }
    int getNumVertices() const { return mesh.n_vertices(); }
    int getNumColors() const { return colors_.size(); }

    unsigned int const* getIndexesPointer() const { return indexes_.data(); }
    PointT const* getVerticesPointer() const { return mesh.points(); }
    float const* getColorsPointer() const { return colors_.data(); }

    // End interface for OpenGLMeshRender

private:
    std::vector<unsigned int> indexes_;
    std::vector<float> colors_;
};
