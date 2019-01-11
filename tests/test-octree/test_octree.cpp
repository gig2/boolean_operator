#include "bbox.h"
#include "mesh_face_bbox.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <OpenMesh/Core/Geometry/Vector11T.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>


#include <algorithm>

using testing::Eq;
using testing::FloatEq;

using MeshT = OpenMesh::TriMesh_ArrayKernelT<OpenMesh::DefaultTraits>;


#include <array>


template <typename PointType, typename FaceContainerType, typename FaceAndBBoxType>
class Octree
{
public:
    Octree( BBox<PointType> const& boundingBox, FaceContainerType const& mesh1in,
            FaceContainerType const& mesh2in, FaceAndBBoxType const& faceAndBBox1,
            FaceAndBBoxType const& faceAndBBox2, Octree* parent = nullptr, int level = 0 )
        : boundingBox_{boundingBox}
        , mesh1in_{mesh1in}
        , mesh2in_{mesh2in}
        , faceAndBBox1_{faceAndBBox1}
        , faceAndBBox2_{faceAndBBox2}
        , level_{level}
        , parent_{parent}
    {
        //
        auto canGoFuther = [this]() { return level_ < maxDepth_; };

        if ( canGoFuther() )
        {
            PointType lower = boundingBox_.lower();
            PointType upper = boundingBox_.upper();

            PointType halfPoint = 0.5 * ( lower + upper );

            int count = 0;

            int nextLevel = level_ + 1;

            for ( int ix = 0; ix < 2; ++ix )
            {
                for ( int iy = 0; iy < 2; ++iy )
                {
                    for ( int iz = 0; iz < 2; ++iz )
                    {
                        PointType lowDest;
                        PointType upDest;

                        lowDest[ 0 ] = ix % 2 ? lower[ 0 ] : halfPoint[ 0 ];
                        upDest[ 0 ]  = ix % 2 ? halfPoint[ 0 ] : upper[ 0 ];

                        lowDest[ 1 ] = iy % 2 ? lower[ 1 ] : halfPoint[ 1 ];
                        upDest[ 1 ]  = iy % 2 ? halfPoint[ 1 ] : upper[ 1 ];

                        lowDest[ 2 ] = iz % 2 ? lower[ 2 ] : halfPoint[ 2 ];
                        upDest[ 2 ]  = iz % 2 ? halfPoint[ 2 ] : upper[ 2 ];


                        children_[ count ] = std::make_unique<Octree>(
                            BBox<PointType>{lowDest, upDest}, mesh1in_, mesh2in, faceAndBBox1_,
                            faceAndBBox2_, this, nextLevel );
                        ++count;
                    }
                }
            }
        }
    }


private:
    BBox<PointType> boundingBox_;

    FaceContainerType const& mesh1in_;
    FaceContainerType const& mesh2in_;

    FaceAndBBoxType const& faceAndBBox1_;
    FaceAndBBoxType const& faceAndBBox2_;

    int const level_{0};

    int const maxDepth_{5};

    Octree* parent_{nullptr};
    std::array<std::unique_ptr<Octree>, 8> children_;
};



TEST( meshFaceAndBBox, computeCorrectBoxes )
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


    auto faceAndBBox1 = MeshFaceAndBBox{mesh1};
    auto faceAndBBox2 = MeshFaceAndBBox{mesh2};

    auto const& mesh1box = faceAndBBox1.meshBBox;

    auto const& mesh2box = faceAndBBox2.meshBBox;

    EXPECT_DOUBLE_EQ( 0., mesh1box.lower()[ 0 ] );
    EXPECT_DOUBLE_EQ( 0., mesh1box.lower()[ 1 ] );
    EXPECT_DOUBLE_EQ( 0., mesh1box.lower()[ 2 ] );

    EXPECT_DOUBLE_EQ( 4., mesh1box.upper()[ 0 ] );
    EXPECT_DOUBLE_EQ( 6., mesh1box.upper()[ 1 ] );
    EXPECT_DOUBLE_EQ( 0., mesh1box.upper()[ 2 ] );


    ASSERT_TRUE( intersect( mesh1box, mesh2box ) );

    auto intersectionBox = bboxIntersect( mesh1box, mesh2box );


    auto computeInAndOut
        = [&intersectionBox]( auto meshinfirst, auto meshoutfirst, auto const& faceAndBBox ) {
              auto const& mesh = faceAndBBox.mesh;
              std::partition_copy( mesh.faces_begin(), mesh.faces_end(), meshinfirst, meshoutfirst,
                                   [&faceAndBBox, &intersectionBox]( auto const& fh ) //
                                   {
                                       auto const& faceToBBox = faceAndBBox.faceToBBox;

                                       auto fIt = faceToBBox.find( fh );
                                       if ( fIt != std::end( faceToBBox ) )
                                       {
                                           return intersect( fIt->second, intersectionBox );
                                       }
                                       else
                                       {
                                           auto bbox = bboxFromFace( faceAndBBox.mesh, fh );
                                           return intersect( bbox, intersectionBox );
                                       }
                                   } );
          };

    using FaceHandle = MeshT::FaceHandle;

    std::vector<FaceHandle> mesh1in;
    std::vector<FaceHandle> mesh1out;

    computeInAndOut( std::back_inserter( mesh1in ), std::back_inserter( mesh1out ), faceAndBBox1 );

    std::vector<FaceHandle> mesh2in;
    std::vector<FaceHandle> mesh2out;

    computeInAndOut( std::back_inserter( mesh2in ), std::back_inserter( mesh2out ), faceAndBBox2 );


    using Point = MeshT::Point;

    auto computeUnionIn = []( auto const& faceAndBBox1, auto const& faceAndBBox2, auto mesh1inFirst,
                              auto mesh1inLast, auto mesh2inFirst, auto mesh2inLast ) {
        auto computeBoxes = []( auto meshinFirst, auto meshinLast, auto const& faceToBBox ) {
            Point lower;
            Point upper;
            for ( int iDir = 0; iDir < 3; ++iDir )
            {
                auto lowerIt = std::min_element(
                    meshinFirst, meshinLast,
                    [&faceToBBox, &iDir]( auto const& lhs, auto const& rhs ) {
                        auto lhsIt = faceToBBox.find( lhs );
                        auto rhsIt = faceToBBox.find( rhs );

                        assert( lhsIt != std::end( faceToBBox ) );
                        assert( rhsIt != std::end( faceToBBox ) );

                        return lhsIt->second.lower()[ iDir ] < rhsIt->second.lower()[ iDir ];
                    } );

                auto upperIt = std::max_element(
                    meshinFirst, meshinLast,
                    [&faceToBBox, &iDir]( auto const& lhs, auto const& rhs ) {
                        auto lhsIt = faceToBBox.find( lhs );
                        auto rhsIt = faceToBBox.find( rhs );

                        assert( lhsIt != std::end( faceToBBox ) );
                        assert( rhsIt != std::end( faceToBBox ) );

                        return lhsIt->second.upper()[ iDir ] < rhsIt->second.upper()[ iDir ];
                    } );

                auto lowerFaceToBBoxIt = faceToBBox.find( *lowerIt );
                auto upperFaceToBBoxIt = faceToBBox.find( *upperIt );

                lower[ iDir ] = lowerFaceToBBoxIt->second.lower()[ iDir ];
                upper[ iDir ] = upperFaceToBBoxIt->second.upper()[ iDir ];
            }

            return BBox<Point>{lower, upper};
        };

        auto bboxmesh1in = computeBoxes( mesh1inFirst, mesh1inLast, faceAndBBox1.faceToBBox );

        auto bboxmesh2in = computeBoxes( mesh2inFirst, mesh2inLast, faceAndBBox2.faceToBBox );

        return bboxExtendUnion( bboxmesh1in, bboxmesh2in );
    };


    auto boundingBox
        = computeUnionIn( faceAndBBox1, faceAndBBox2, std::begin( mesh1in ), std::end( mesh1in ),
                          std::begin( mesh2in ), std::end( mesh2in ) );

    Octree octree{boundingBox, mesh1in, mesh2in, faceAndBBox1, faceAndBBox2};
}
