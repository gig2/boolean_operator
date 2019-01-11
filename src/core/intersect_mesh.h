#pragma once

#include "mesh.h"

class IntersectMesh
{
public:
    using MeshT = Mesh::MeshT;

private:
    OpenMesh::FPropHandleT<int> is_checked; // Status faces (Propriété)

    std::vector<MeshT::FaceHandle> intersection1; // Triangles aux intersections
    std::vector<MeshT::FaceHandle> intersection2;

    std::vector<std::vector<MeshT::FaceHandle>> boucles1; // Boucles aux intersections
    std::vector<std::vector<MeshT::FaceHandle>> boucles2; // Boucles aux intersections


public:
    MeshT &mesh1;
    MeshT &mesh2;

    IntersectMesh() = default;

    IntersectMesh( MeshT &, MeshT & );

    void operator()( std::vector<MeshT::FaceHandle> const &,
                     std::vector<MeshT::FaceHandle> const & );

    MeshT get_mesh1();
    MeshT get_mesh2();

    void genere_mesh1();
    void genere_mesh2();

    // Intersection entre deux triangles
    bool intersect( MeshT::FaceHandle, MeshT::FaceHandle );

    // Intersection entre deux ensembles de triangles
    void genere_intersection( std::vector<MeshT::FaceHandle> const &,
                              std::vector<MeshT::FaceHandle> const & );

    // Genere les boucles
    void genere_boucles( MeshT::FaceHandle, int );

    void genere_boucles();

    void genere_boucle2( MeshT::FaceHandle fh1, int indice,
                         std::vector<MeshT::FaceHandle> &boucle );

    void genere_boucle1( MeshT::FaceHandle fh1, int indice,
                         std::vector<MeshT::FaceHandle> &boucle );
};
