//
//  BoundingBox.hpp
//  ocra
//
//  Created by Alex Mayle on 12/3/16.
//  Copyright © 2016 Alex Mayle. All rights reserved.
//

#ifndef BoundingBox_hpp
#define BoundingBox_hpp

#include <stdio.h>
#include "Coordinate.hpp"

class BoundingBox {
private:
    Coordinate mMin;
    Coordinate mMax;
    
public:
    inline BoundingBox() {
        mMin = Coordinate(0, 0);
        mMax = Coordinate(0, 0);
    }

    
    inline BoundingBox(const Coordinate& min, const Coordinate& max) {
        mMin = min;
        mMax = max;
    }
    
    inline const Coordinate min() const {
        return mMin;
    }
    
    inline void setMin(const Coordinate& min) {
        mMin = min;
    }
    
    inline const Coordinate max() const {
        return mMax;
    }
    
    inline void setMax(const Coordinate& max) {
        mMax = max;
    }
    
    inline bool operator==(const BoundingBox& other) const {
        return ((mMax == other.mMax) && (mMin == other.mMin));
    }
    
    inline bool contains(const Coordinate& pixel) const {
        bool betweenX = pixel.x() >= mMin.x() && pixel.x() <= mMax.x();
        bool betweenY = pixel.y() >= mMin.y() && pixel.y() <= mMax.y();
        return betweenX && betweenY;
    }
    
    bool contains(const BoundingBox& box) {
        Coordinate topRight(box.max().x(), box.min().y());
        Coordinate bottomLeft(box.min().x(), box.max().y());
        bool case1 = this->contains(box.min());
        bool case2 = this->contains(box.max());
        bool case3 = this->contains(topRight);
        bool case4 = this->contains(bottomLeft);
        return case1 || case2 || case3 || case4;
    }
    
    void merge(const BoundingBox& other) {
        if (this->mMin.x() > other.mMin.x()) mMin.setX(other.mMin.x());
        if (this->mMin.y() > other.mMin.y()) mMin.setY(other.mMin.y());
        if (this->mMax.x() < other.mMax.x()) mMax.setX(other.mMax.x());
        if (this->mMax.y() < other.mMax.y()) mMax.setY(other.mMax.y());
    }
    
//    inline bool greedyContains(const Coordinate& pixel) {
//        bool betweenX = pixel.x() >= mMin.x() - 3 && pixel.x() <= mMax.x() + 3;
//        bool betweenY = pixel.y() >= mMin.y() - 3 && pixel.y() <= mMax.y() + 3;
//        return betweenX && betweenY;
//    }
    
    void expandBoundaries(Coordinate& blackPixel);
    
    const Coordinate checkPerimeter(vector< vector<bool> >& bitmap,
                                    Coordinate& lastBlackPixelVisited,
                                    const Coordinate& partitionMin,
                                    const Coordinate& partitionMax);
    
    void printToBitmap(vector< vector<bool> >& bitmap) const;
    void eraseFromBitmap(vector< vector<bool> >& bitmap) const;

//    const Coordinate checkPerimeterFinal(vector< vector<bool> >& bitmap,
//                                                 Coordinate& lastBlackPixelVisited,
//                                                 const Coordinate& partitionMin,
//                                                 const Coordinate& partitionMax) {
//        //  +X direction
//        int distanceFromLastBlackPixel = 0;
//        for (int i = mMax.y() - 1; i >= mMin.y(); --i) {
//            ++checkCount;
//            if (bitmap[i][mMax.x()]) {
//                Coordinate blackPixel(mMax.x(), i);
//                //if (blackPixel != lastBlackPixelVisited) {
//                Coordinate neighbor = blackPixel.getAnyNeighbor(bitmap, partitionMin, partitionMax, *this, north, distanceFromLastBlackPixel);
//                distanceFromLastBlackPixel = 1;
//                if (blackPixel != neighbor) {
//                    this->erase
//                    this->expandBoundaries(neighbor);
//                    
//                    return neighbor;
//                }
//                //}
//            } else ++distanceFromLastBlackPixel;
//        }
//        distanceFromLastBlackPixel = 0;
//        //  +Y direction
//        for (int j = mMax.x(); j >= mMin.x(); --j) {
//            ++checkCount;
//            if (bitmap[mMax.y()][j]) {
//                Coordinate blackPixel(j, mMax.y());
//                //if (blackPixel != lastBlackPixelVisited) {
//                Coordinate neighbor = blackPixel.getAnyNeighbor(bitmap, partitionMin, partitionMax, *this, west, distanceFromLastBlackPixel);
//                if (blackPixel != neighbor) {
//                    this->expandBoundaries(neighbor);
//                    return neighbor;
//                }
//                // }
//            } else ++distanceFromLastBlackPixel;
//        }
//        //  -X direction
//        distanceFromLastBlackPixel = 0;
//        for (int i = mMax.y() - 1; i >= mMin.y(); --i) {
//            ++checkCount;
//            if (bitmap[i][mMin.x()]) {
//                Coordinate blackPixel(mMin.x(), i);
//                // if (blackPixel != lastBlackPixelVisited) {
//                Coordinate neighbor = blackPixel.getAnyNeighbor(bitmap, partitionMin, partitionMax, *this, north, distanceFromLastBlackPixel);
//                distanceFromLastBlackPixel = 1;
//                if (blackPixel != neighbor) {
//                    this->expandBoundaries(neighbor);
//                    return neighbor;
//                }
//                // }
//            } else ++distanceFromLastBlackPixel;
//        }
//        
//        return lastBlackPixelVisited;
//    }

};

#endif /* BoundingBox_hpp */
