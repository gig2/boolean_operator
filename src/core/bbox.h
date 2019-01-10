#pragma once


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
