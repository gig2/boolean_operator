#pragma once

#include "bbox.h"

#include <map>

template <typename MeshType>
class MeshFaceAndBBox
{
public:
    using FaceHandle = typename MeshType::FaceHandle;
    using Point      = typename MeshType::Point;

    explicit MeshFaceAndBBox( MeshType const& meshView )
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
        Point lower;
        Point upper;

        for ( int iDir = 0; iDir < 3; ++iDir )
        {
            auto min = std::min_element(
                std::begin( faceToBBox ), std::end( faceToBBox ),
                [&iDir]( auto const& lhs, auto const& rhs ) //
                { return lhs.second.lower()[ iDir ] < rhs.second.lower()[ iDir ]; } );

            auto max = std::max_element(
                std::begin( faceToBBox ), std::end( faceToBBox ),
                [&iDir]( auto const& lhs, auto const& rhs ) //
                { return lhs.second.upper()[ iDir ] < rhs.second.upper()[ iDir ]; } );

            lower[ iDir ] = min->second.lower()[ iDir ];
            upper[ iDir ] = max->second.upper()[ iDir ];
        }


        return BBox<Point>{lower, upper};
    }

public:
    MeshType const& mesh;
    std::map<FaceHandle, BBox<Point>> const faceToBBox;
    BBox<Point> const meshBBox;
};
