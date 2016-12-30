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
    
    inline bool contains(const int x, const int y) const {
        bool betweenX = x >= mMin.x() && x <= mMax.x();
        bool betweenY = y >= mMin.y() && x <= mMax.y();
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
    
    void expandBoundaries(const Coordinate& blackPixel);
    
    int checkPerimeter(const vector< vector<bool> >& bitmap,
                        Coordinate& lastBlackPixelVisited,
                        const Partition * const partition);
    
    int quickExpand(const vector< vector<bool> >& bitmap,
                    const Partition * const partition,
                    Coordinate& pixelOnPerimeter);
    
    int fullExpandRight(const vector< vector<bool> >& bitmap,
                        const Partition * const partition,
                        Coordinate& pixelOnPerimeter,
                        int distanceFromLastBlackPixel);
    
    int fullExpandBottom(const vector< vector<bool> >& bitmap,
                         const Partition * const partition,
                         Coordinate& pixelOnPerimeter,
                         int distanceFromLastBlackPixel);
    
    int fullExpandLeft(const vector< vector<bool> >& bitmap,
                       const Partition * const partition,
                       Coordinate& pixelOnPerimeter,
                       int distanceFromLastBlackPixel);
    
    int fullExpand(const vector< vector<bool> >& bitmap,
                   const Partition * const partition,
                   Coordinate& pixelOnPerimeter,
                   const Direction direction,
                   int distanceFromLastBlackPixel);
    
    void printToBitmap(vector< vector<bool> >& bitmap) const;
    void eraseFromBitmap(vector< vector<bool> >& bitmap) const;
};

#endif /* BoundingBox_hpp */
