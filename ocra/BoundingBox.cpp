//
//  BoundingBox.cpp
//  ocra
//
//  Created by Alex Mayle on 12/3/16.
//  Copyright © 2016 Alex Mayle. All rights reserved.
//

#include "BoundingBox.hpp"
#include "Partition.hpp"
    
void BoundingBox::expandBoundaries(const Coordinate& blackPixel) {
    Coordinate difference = blackPixel - this->mMax;
    if (difference.x() > 0)
        mMax.setX(blackPixel.x());
    if (difference.y() > 0)
        mMax.setY(blackPixel.y());
    
    difference = blackPixel - this->mMin;
    if (difference.x() < 0)
        mMin.setX(blackPixel.x());
    //  if (difference.y() < 0)
    //      mMin.setX(blackPixel.y());
}

int BoundingBox::checkPerimeter(const vector< vector<bool> >& bitmap,
                                Coordinate& lastBlackPixelVisited,
                                const Partition * const partition) {
        //  +X direction
    int distanceFromLastBlackPixel = 0;
        for (int i = mMax.y() - 1; i >= mMin.y(); --i) {
            ++checkCount;
            if (bitmap[i][mMax.x()]) {
                Coordinate blackPixel(mMax.x(), i);
                //if (blackPixel != lastBlackPixelVisited) {
                    Coordinate neighbor = blackPixel.getAnyNeighbor(bitmap, partition, *this, north, distanceFromLastBlackPixel);
                distanceFromLastBlackPixel = 1;
                    if (blackPixel != neighbor) {
                        this->expandBoundaries(neighbor);
                        lastBlackPixelVisited = neighbor;
                        return 0;
                    }
                //}
            } else ++distanceFromLastBlackPixel;
        }
    distanceFromLastBlackPixel = 0;
        //  +Y direction
        for (int j = mMax.x(); j >= mMin.x(); --j) {
            ++checkCount;
            if (bitmap[mMax.y()][j]) {
                Coordinate blackPixel(j, mMax.y());
                //if (blackPixel != lastBlackPixelVisited) {
                    Coordinate neighbor = blackPixel.getAnyNeighbor(bitmap, partition, *this, west, distanceFromLastBlackPixel);
                    if (blackPixel != neighbor) {
                        this->expandBoundaries(neighbor);
                        lastBlackPixelVisited = neighbor;
                        return 0;
                    }
                // }
            } else ++distanceFromLastBlackPixel;
        }
        //  -X direction
    distanceFromLastBlackPixel = 0;
        for (int i = mMax.y() - 1; i >= mMin.y(); --i) {
            ++checkCount;
            if (bitmap[i][mMin.x()]) {
                Coordinate blackPixel(mMin.x(), i);
                // if (blackPixel != lastBlackPixelVisited) {
                    Coordinate neighbor = blackPixel.getAnyNeighbor(bitmap, partition, *this, north, distanceFromLastBlackPixel);
                distanceFromLastBlackPixel = 1;
                    if (blackPixel != neighbor) {
                        this->expandBoundaries(neighbor);
                        lastBlackPixelVisited = neighbor;
                        return 0;
                    }
                // }
            } else ++distanceFromLastBlackPixel;
        }
    
//    //  +X direction
//    for (int i = mMax.y() - 1; i >= mMin.y(); --i) {
//        ++checkCount;
//        if (bitmap[i][mMax.x()]) {
//            Coordinate blackPixel(mMax.x(), i);
//            //if (blackPixel != lastBlackPixelVisited) {
//                Coordinate neighbor = blackPixel.getAnyNeighbor(bitmap, Coordinate(0,0), Coordinate(bitmap[0].size() -1, bitmap.size() -1), *this);
//                if (blackPixel != neighbor) {
//                    this->expandBoundaries(neighbor);
//                    return neighbor;
//                }
//            //}
//        }
//    }
//    //  +Y direction
//    for (int j = mMax.x(); j >= mMin.x(); --j) {
//        if (bitmap[mMax.y()][j]) {
//            Coordinate blackPixel(j, mMax.y());
//            //if (blackPixel != lastBlackPixelVisited) {
//                Coordinate neighbor = blackPixel.getAnyNeighbor(bitmap, Coordinate(0,0), Coordinate(bitmap[0].size() -1, bitmap.size() -1), *this);
//                if (blackPixel != neighbor) {
//                    this->expandBoundaries(neighbor);
//                    return neighbor;
//                }
//            // }
//        }
//    }
//    //  -X direction
//    for (int i = mMax.y() - 1; i >= mMin.y(); --i) {
//        if (bitmap[i][mMin.x()]) {
//            Coordinate blackPixel(mMin.x(), i);
//            // if (blackPixel != lastBlackPixelVisited) {
//                Coordinate neighbor = blackPixel.getAnyNeighbor(bitmap, Coordinate(0,0), Coordinate(bitmap[0].size() -1, bitmap.size()-1), *this);
//                if (blackPixel != neighbor) {
//                    this->expandBoundaries(neighbor);
//                    return neighbor;
//                }
//            // }
//        }
//    }
    
    return -1;
}

int BoundingBox::quickExpand(const vector< vector<bool> >& bitmap,
                             const Partition * const partition,
                             Coordinate& pixelOnPerimeter) {
    
    int sBound, wBound, eBound;
    
    //  South Bound
    sBound = partition->max().y() - 1 - pixelOnPerimeter.y();
    if (sBound > 3) sBound = 3;
    
    //  West Bound
    wBound = pixelOnPerimeter.x() - partition->min().x();
    if (wBound > 3) wBound = 3;
    
    //  East Bound
    eBound = partition->max().x() - 1 - pixelOnPerimeter.x();
    if (eBound > 3) eBound = 3;
    
    //   X direction
    if (pixelOnPerimeter.x() == mMax.x()) {
        for (int j = pixelOnPerimeter.x() + eBound; j != pixelOnPerimeter.x(); --j) {
            ++checkCount;
            if (bitmap[pixelOnPerimeter.y()][j]) {
                pixelOnPerimeter.setX(j);
                this->expandBoundaries(pixelOnPerimeter);
                return 0;
            }
        }
    }
    
    // Y direction
    if (pixelOnPerimeter.y() == mMax.y()) {
        for (int i = pixelOnPerimeter.y() + sBound; i != pixelOnPerimeter.y(); --i) {
            ++checkCount;
            if (bitmap[i][pixelOnPerimeter.x()]) {
                pixelOnPerimeter.setY(i);
                this->expandBoundaries(pixelOnPerimeter);
                return 0;
            }
        }
    }
    
    if (pixelOnPerimeter.x() == mMin.x()) {
        for (int j = pixelOnPerimeter.x() - wBound; j != pixelOnPerimeter.x(); ++j) {
            ++checkCount;
            if (bitmap[pixelOnPerimeter.x()][j]) {
                pixelOnPerimeter.setX(j);
                this->expandBoundaries(pixelOnPerimeter);
                return 0;
            }
        }
    }
    
    return -1;
}

void BoundingBox::printToBitmap(vector< vector<bool> >& bitmap) const {
    for (int i = mMin.y(); i <= mMax.y(); ++i) {
        bitmap[i][mMin.x()] = true;
        bitmap[i][mMax.x()] = true;
    }
    for (int j = mMin.x(); j <= mMax.x(); ++j) {
        bitmap[mMin.y()][j] = true;
        bitmap[mMax.y()][j] = true;
    }
}

void BoundingBox::eraseFromBitmap(vector< vector<bool> >& bitmap) const {
    for (int i = mMin.y(); i <= mMax.y(); ++i) {
        bitmap[i][mMin.x()] = false;
        bitmap[i][mMax.x()] = false;
    }
    for (int j = mMin.x(); j <= mMax.x(); ++j) {
        bitmap[mMin.y()][j] = false;
        bitmap[mMax.y()][j] = false;
    }
}

