#pragma once

#include <algorithm>
#include <array>

template <typename PointType>
class BBox
{
public:
    BBox( PointType lower, PointType upper )
        : lower_{lower}
        , upper_{upper}
        , isValid_{true}
    {
        //
    }

    BBox() = default;

    PointType const& lower() const { return lower_; }

    PointType const& upper() const { return upper_; }

    bool isValid() const { return isValid_; }

private:
    PointType lower_;
    PointType upper_;
    bool isValid_{false};
};

template <typename MeshType>
BBox<typename MeshType::Point> bboxFromFace( MeshType const& mesh,
                                             typename MeshType::FaceHandle fh )
{
    using Point = typename MeshType::Point;
    Point lower;
    Point upper;

    auto fvIt = mesh.cfv_iter( fh );

    auto firstPoint  = mesh.point( *fvIt );
    auto secondPoint = mesh.point( *std::next( fvIt ) );
    auto thirdPoint  = mesh.point( *std::next( fvIt, 2 ) );

    for ( int iDir = 0; iDir < 3; ++iDir )
    {
        std::array<double, 3> position{firstPoint[ iDir ], secondPoint[ iDir ], thirdPoint[ iDir ]};

        auto minMax   = std::minmax_element( std::begin( position ), std::end( position ) );
        lower[ iDir ] = *minMax.first;
        upper[ iDir ] = *minMax.second;
    }



    return BBox<Point>{lower, upper};
}

template <typename PointType>
bool intersect( BBox<PointType> const& lhs, BBox<PointType> const& rhs )
{
    for ( int iDir = 0; iDir < 3; ++iDir )
    {
        bool lhsIntersectLower = lhs.lower()[ iDir ] <= rhs.lower()[ iDir ]
                                 && lhs.upper()[ iDir ] >= rhs.lower()[ iDir ];

        bool rhsIntersectLower = rhs.lower()[ iDir ] <= lhs.lower()[ iDir ]
                                 && rhs.upper()[ iDir ] >= lhs.lower()[ iDir ];

        if ( !lhsIntersectLower && !rhsIntersectLower )
        {
            return false;
        }
    }

    return true;
}

template <typename PointType>
BBox<PointType> bboxIntersect( BBox<PointType> const& lhs, BBox<PointType> const& rhs )
{
    if ( !intersect( lhs, rhs ) )
    {
        return BBox<PointType>{};
    }

    PointType lower;
    PointType upper;

    for ( int iDir = 0; iDir < 3; ++iDir )
    {
        lower[ iDir ] = std::max( lhs.lower()[ iDir ], rhs.lower()[ iDir ] );
        upper[ iDir ] = std::min( lhs.upper()[ iDir ], rhs.upper()[ iDir ] );
    }

    return BBox<PointType>{lower, upper};
}

template <typename PointType>
BBox<PointType> bboxExtendUnion( BBox<PointType> const& lhs, BBox<PointType> const& rhs )
{
    PointType lower;
    PointType upper;

    for ( int iDir = 0; iDir < 3; ++iDir )
    {
        lower[ iDir ] = std::min( lhs.lower()[ iDir ], rhs.lower()[ iDir ] );
        upper[ iDir ] = std::max( lhs.upper()[ iDir ], rhs.upper()[ iDir ] );
    }

    return BBox<PointType>{lower, upper};
}
