#pragma once

#include "bbox.h"
#include "mesh_face_bbox.h"
#include "octree.h"

#include <vector>

template <typename MeshType>
class ComputeOctree
{
public:
    using FaceHandle = typename MeshType::FaceHandle;
    using PointType  = typename MeshType::Point;



    ComputeOctree( MeshType const& mesh1, MeshType const& mesh2 )
        : mesh1{mesh1}
        , mesh2{mesh2}
        , faceAndBBox1{mesh1}
        , faceAndBBox2{mesh2}
        , octree_{initOctree_()}

    {
        //
    }


    MeshType const& mesh1;
    MeshType const& mesh2;

    MeshFaceAndBBox<MeshType> const faceAndBBox1;
    MeshFaceAndBBox<MeshType> const faceAndBBox2;

    std::vector<FaceHandle> mesh1in;
    std::vector<FaceHandle> mesh1out;

    std::vector<FaceHandle> mesh2in;
    std::vector<FaceHandle> mesh2out;

    auto const& octree() const { return octree_; }

private:
    Octree<PointType, decltype( mesh1in ), decltype( faceAndBBox1 )> octree_;


    decltype( octree_ ) initOctree_()
    {
        auto const& mesh1box = faceAndBBox1.meshBBox;
        auto const& mesh2box = faceAndBBox2.meshBBox;

        if ( !intersect( mesh1box, mesh2box ) )
        {
            throw std::runtime_error( "Imbricated meshes not supported" );
        }

        auto intersectionBox = bboxIntersect( mesh1box, mesh2box );


        auto computeInAndOut = [&intersectionBox]( auto meshinfirst, auto meshoutfirst,
                                                   auto const& faceAndBBox ) {
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


        computeInAndOut( std::back_inserter( mesh1in ), std::back_inserter( mesh1out ),
                         faceAndBBox1 );

        computeInAndOut( std::back_inserter( mesh2in ), std::back_inserter( mesh2out ),
                         faceAndBBox2 );



        auto computeUnionIn = []( auto const& faceAndBBox1, auto const& faceAndBBox2,
                                  auto mesh1inFirst, auto mesh1inLast, auto mesh2inFirst,
                                  auto mesh2inLast ) {
            auto computeBoxes = []( auto meshinFirst, auto meshinLast, auto const& faceToBBox ) {
                PointType lower;
                PointType upper;
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

                    if ( lowerIt == meshinLast || upperIt == meshinLast )
                    {
                        return BBox<PointType>{};
                    }
                    auto lowerFaceToBBoxIt = faceToBBox.find( *lowerIt );
                    auto upperFaceToBBoxIt = faceToBBox.find( *upperIt );

                    lower[ iDir ] = lowerFaceToBBoxIt->second.lower()[ iDir ];
                    upper[ iDir ] = upperFaceToBBoxIt->second.upper()[ iDir ];
                }

                return BBox<PointType>{lower, upper};
            };

            auto bboxmesh1in = computeBoxes( mesh1inFirst, mesh1inLast, faceAndBBox1.faceToBBox );

            auto bboxmesh2in = computeBoxes( mesh2inFirst, mesh2inLast, faceAndBBox2.faceToBBox );

            return bboxExtendUnion( bboxmesh1in, bboxmesh2in );
        };


        auto boundingBox
            = computeUnionIn( faceAndBBox1, faceAndBBox2, std::begin( mesh1in ),
                              std::end( mesh1in ), std::begin( mesh2in ), std::end( mesh2in ) );

        return {boundingBox, mesh1in, mesh2in, faceAndBBox1, faceAndBBox2};
    }
};
