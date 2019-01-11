#pragma once

#include <array>
#include <memory>

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
