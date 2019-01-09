
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


template <typename PointType>
class BBox
{
public:
    BBox( PointType lower, PointType upper )
        : lower_{lower}
        , upper_{upper}
    {
        //
    }

    PointType const& lower() const { return lower_; }

    PointType const& upper() const { return upper_; }

private:
    PointType lower_;
    PointType upper_;
};


template <typename MeshType>
BBox<typename MeshType::Point> bboxFromFace( MeshType const& mesh,
                                             typename MeshType::FaceHandle fh )
{
    using Point = typename MeshType::Point;
    Point lower;
    Point upper;

    return BBox{lower, upper};
}

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

    std::map<FaceHandle, BBox<Point>> mesh1faceToBB;
    std::map<FaceHandle, BBox<Point>> mesh2faceToBB;


    for ( auto fhIt = mesh1.faces_begin(); fhIt != mesh1.faces_end(); ++fhIt )
    {
        mesh1faceToBB.emplace( *fhIt, bboxFromFace( mesh1, *fhIt ) );
    }

    for ( auto fhIt = mesh2.faces_begin(); fhIt != mesh2.faces_end(); ++fhIt )
    {
        mesh2faceToBB.emplace( *fhIt, bboxFromFace( mesh2, *fhIt ) );
    }


    auto bboxFromBBoxCollections = []( auto const& meshfaceToBB ) {
        auto minX
            = std::min_element( std::begin( meshfaceToBB ), std::end( meshfaceToBB ),
                                []( auto const& lhs, auto const& rhs ) //
                                { return lhs.second.lower()[ 0 ] < rhs.second.lower()[ 0 ]; } );

        auto minY
            = std::min_element( std::begin( meshfaceToBB ), std::end( meshfaceToBB ),
                                []( auto const& lhs, auto const& rhs ) //
                                { return lhs.second.lower()[ 1 ] < rhs.second.lower()[ 1 ]; } );

        auto minZ
            = std::min_element( std::begin( meshfaceToBB ), std::end( meshfaceToBB ),
                                []( auto const& lhs, auto const& rhs ) //
                                { return lhs.second.lower()[ 2 ] < rhs.second.lower()[ 2 ]; } );

        auto maxX
            = std::max_element( std::begin( meshfaceToBB ), std::end( meshfaceToBB ),
                                []( auto const& lhs, auto const& rhs ) //
                                { return lhs.second.upper()[ 0 ] < rhs.second.upper()[ 0 ]; } );

        auto maxY
            = std::max_element( std::begin( meshfaceToBB ), std::end( meshfaceToBB ),
                                []( auto const& lhs, auto const& rhs ) //
                                { return lhs.second.upper()[ 1 ] < rhs.second.upper()[ 1 ]; } );

        auto maxZ
            = std::max_element( std::begin( meshfaceToBB ), std::end( meshfaceToBB ),
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

        return BBox{lower, upper};
    };


    auto mesh1box = bboxFromBBoxCollections( mesh1faceToBB );

    auto mesh2box = bboxFromBBoxCollections( mesh2faceToBB );
}
