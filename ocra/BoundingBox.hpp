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
    
    inline bool contains(const Coordinate& pixel) {
        bool betweenX = pixel.x() >= mMin.x() && pixel.x() <= mMax.x();
        bool betweenY = pixel.y() >= mMin.y() && pixel.y() <= mMax.y();
        return betweenX && betweenY;
    }
    
    void expandBoundaries(Coordinate& blackPixel);
    
    const Coordinate checkPerimeter(vector< vector<bool> >& bitmap,
                                    Coordinate& lastBlackPixelVisited,
                                    const Coordinate& partitionMin,
                                    const Coordinate& partitionMax);
    
    void printToBitmap(vector< vector<bool> >& bitmap) const;
    
    
};

#endif /* BoundingBox_hpp */
