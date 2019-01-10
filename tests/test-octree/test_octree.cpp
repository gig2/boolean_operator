
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <OpenMesh/Core/Geometry/Vector11T.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>



using testing::Eq;
using testing::FloatEq;

using MeshT = OpenMesh::TriMesh_ArrayKernelT<OpenMesh::DefaultTraits>;


#include <algorithm>
#include <array>
#include <map>
#include <vector>

#include "bbox.h"


template <typename MeshType>
class MeshFaceAndBBox
{
public:
    using FaceHandle = typename MeshType::FaceHandle;
    using Point      = typename MeshType::Point;

    MeshFaceAndBBox( MeshType& meshView )
        : mesh{meshView}
        , faceToBBox{computeFaceToBBox_()}
        , meshBBox{computeMeshBBox_()}
    {
    }

private:
    std::map<FaceHandle, BBox<Point>> computeFaceToBBox_() const
    {
        std::map<FaceHandle, BBox<Point>> faceToBBox;

        for ( auto fhIt = mesh.faces_begin(); fhIt != mesh.faces_end(); ++fhIt )
        {
            faceToBBox.emplace( *fhIt, bboxFromFace( mesh, *fhIt ) );
        }
        return faceToBBox;
    }

    BBox<Point> computeMeshBBox_() const
    {
        auto minX
            = std::min_element( std::begin( faceToBBox ), std::end( faceToBBox ),
                                []( auto const& lhs, auto const& rhs ) //
                                { return lhs.second.lower()[ 0 ] < rhs.second.lower()[ 0 ]; } );

        auto minY
            = std::min_element( std::begin( faceToBBox ), std::end( faceToBBox ),
                                []( auto const& lhs, auto const& rhs ) //
                                { return lhs.second.lower()[ 1 ] < rhs.second.lower()[ 1 ]; } );

        auto minZ
            = std::min_element( std::begin( faceToBBox ), std::end( faceToBBox ),
                                []( auto const& lhs, auto const& rhs ) //
                                { return lhs.second.lower()[ 2 ] < rhs.second.lower()[ 2 ]; } );

        auto maxX
            = std::max_element( std::begin( faceToBBox ), std::end( faceToBBox ),
                                []( auto const& lhs, auto const& rhs ) //
                                { return lhs.second.upper()[ 0 ] < rhs.second.upper()[ 0 ]; } );

        auto maxY
            = std::max_element( std::begin( faceToBBox ), std::end( faceToBBox ),
                                []( auto const& lhs, auto const& rhs ) //
                                { return lhs.second.upper()[ 1 ] < rhs.second.upper()[ 1 ]; } );

        auto maxZ
            = std::max_element( std::begin( faceToBBox ), std::end( faceToBBox ),
                                []( auto const& lhs, auto const& rhs ) //
                                { return lhs.second.upper()[ 2 ] < rhs.second.upper()[ 2 ]; } );


        Point lower;

        lower[ 0 ] = minX->second.lower()[ 0 ];
        lower[ 1 ] = minY->second.lower()[ 1 ];
        lower[ 2 ] = minZ->second.lower()[ 2 ];

        Point upper;

        upper[ 0 ] = maxX->second.upper()[ 0 ];
        upper[ 1 ] = maxY->second.upper()[ 1 ];
        upper[ 2 ] = maxZ->second.upper()[ 2 ];

        return BBox<Point>{lower, upper};
    }

public:
    MeshType const& mesh;
    std::map<FaceHandle, BBox<Point>> const faceToBBox;
    BBox<Point> const meshBBox;
};




TEST( bar, doesFoo )
{
    //

    MeshT mesh1;
    MeshT mesh2;

    std::array<MeshT::VertexHandle, 4> vhandleMesh1;
    std::array<MeshT::VertexHandle, 4> vhandleMesh2;

    vhandleMesh1[ 0 ] = mesh1.add_vertex( MeshT::Point{0., 0., 0.} );
    vhandleMesh1[ 1 ] = mesh1.add_vertex( MeshT::Point{4., 2., 0.} );
    vhandleMesh1[ 2 ] = mesh1.add_vertex( MeshT::Point{3., 6., 0.} );
    vhandleMesh1[ 3 ] = mesh1.add_vertex( MeshT::Point{0., 4., 0.} );


    vhandleMesh2[ 0 ] = mesh2.add_vertex( MeshT::Point{3., 1., 0.} );
    vhandleMesh2[ 1 ] = mesh2.add_vertex( MeshT::Point{5., 3., 0.} );
    vhandleMesh2[ 2 ] = mesh2.add_vertex( MeshT::Point{8., 4., 0.} );
    vhandleMesh2[ 3 ] = mesh2.add_vertex( MeshT::Point{0., 6., 0.} );

    std::vector<MeshT::VertexHandle> faceVhandles;
    faceVhandles.resize( 4 );
    std::copy( std::begin( vhandleMesh1 ), std::end( vhandleMesh1 ), std::begin( faceVhandles ) );

    mesh1.add_face( faceVhandles );

    std::copy( std::begin( vhandleMesh2 ), std::end( vhandleMesh2 ), std::begin( faceVhandles ) );

    mesh2.add_face( faceVhandles );

    using FaceHandle = typename MeshT::FaceHandle;

    using Point = typename MeshT::Point;

    auto faceAndBBox1 = MeshFaceAndBBox{mesh1};
    auto faceAndBBox2 = MeshFaceAndBBox{mesh2};

    auto const& mesh1box = faceAndBBox1.meshBBox;

    auto const& mesh2box = faceAndBBox2.meshBBox;
}
