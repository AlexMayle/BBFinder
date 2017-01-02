//
//  BoundingBox.cpp
//  ocra
//
//  Created by Alex Mayle on 12/3/16.
//  Copyright © 2016 Alex Mayle. All rights reserved.
//

#include "BoundingBox.hpp"
#include "Partition.hpp"

BoundingBox::BoundingBox() {
    mMin.setX(0);
    mMin.setY(0);
    mMax.setX(0);
    mMax.setY(0);
}

BoundingBox::BoundingBox(const BoundingBox& other) {
    mMin = other.mMin;
    mMax = other.mMax;
}

BoundingBox::BoundingBox(const Coordinate& min, const Coordinate& max) {
    mMin = min;
    mMax = max;
}

BoundingBox::BoundingBox(const Coordinate&& min, const Coordinate&& max) {
    mMin = min;
    mMax = max;
}

inline const Coordinate BoundingBox::min() const {
    return mMin;
}

inline void BoundingBox::setMin(const Coordinate& min) {
    mMin = min;
}

inline const Coordinate BoundingBox::max() const {
    return mMax;
}

inline void BoundingBox::setMax(const Coordinate& max) {
    mMax = max;
}

inline bool BoundingBox::operator==(const BoundingBox& other) const {
    return (mMax == other.mMax) && (mMin == other.mMin);
}

bool BoundingBox::contains(const Coordinate& pixel) const {
    bool betweenX = pixel.x() >= mMin.x() && pixel.x() <= mMax.x();
    bool betweenY = pixel.y() >= mMin.y() && pixel.y() <= mMax.y();
    return betweenX && betweenY;
}

bool BoundingBox::contains(const int x, const int y) const {
    bool betweenX = x >= mMin.x() && x <= mMax.x();
    bool betweenY = y >= mMin.y() && y <= mMax.y();
    return betweenX && betweenY;
}

bool BoundingBox::contains(const BoundingBox& other) const {
    Coordinate topRight(other.max().x(), other.min().y());
    Coordinate bottomLeft(other.min().x(), other.max().y());
    bool case1 = this->contains(other.max());
    bool case2 = this->contains(bottomLeft);
    bool case3 = this->contains(other.min());
    bool case4 = this->contains(topRight);
    return case1 || case2 || case3 || case4;
}

void BoundingBox::expandBoundaries(const Coordinate& blackPixel) {
    Coordinate difference = blackPixel - mMax;
    if (difference.x() > 0)
        mMax.setX(blackPixel.x());
    if (difference.y() > 0)
        mMax.setY(blackPixel.y());
    
    difference = blackPixel - mMin;
    if (difference.x() < 0)
        mMin.setX(blackPixel.x());
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
            if (this->fullExpandRight(bitmap, partition, blackPixel,
                                      distanceFromLastBlackPixel) != -1) {
                distanceFromLastBlackPixel = 1;
                this->expandBoundaries(blackPixel);
                lastBlackPixelVisited = blackPixel;
                return 0;
            }
        } else ++distanceFromLastBlackPixel;
    }
    
    distanceFromLastBlackPixel = 0;
    //  +Y direction
    for (int j = mMax.x(); j >= mMin.x(); --j) {
        ++checkCount;
        if (bitmap[mMax.y()][j]) {
            Coordinate blackPixel(j, mMax.y());
            if (this->fullExpandBottom(bitmap, partition, blackPixel,
                                      distanceFromLastBlackPixel) != -1) {
                distanceFromLastBlackPixel = 1;
                this->expandBoundaries(blackPixel);
                lastBlackPixelVisited = blackPixel;
                return 0;
            }
        } else ++distanceFromLastBlackPixel;
    }
    
    //  -X direction
    distanceFromLastBlackPixel = 0;
    for (int i = mMax.y() - 1; i >= mMin.y(); --i) {
        ++checkCount;
        if (bitmap[i][mMin.x()]) {
            Coordinate blackPixel(mMin.x(), i);
            if (this->fullExpandLeft(bitmap, partition, blackPixel,
                                      distanceFromLastBlackPixel) != -1) {
                distanceFromLastBlackPixel = 1;
                this->expandBoundaries(blackPixel);
                lastBlackPixelVisited = blackPixel;
                return 0;
            }
        } else ++distanceFromLastBlackPixel;
    }
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

int BoundingBox::fullExpandRight(const vector< vector<bool> >& bitmap,
                                 const Partition * const partition,
                                 Coordinate& pixelOnPerimeter,
                                 int distanceFromLastBlackPixel) {
    int nBound, sBound, eBound;
    if (distanceFromLastBlackPixel > 3) distanceFromLastBlackPixel = 3;
    
    //  North Bound
    nBound = pixelOnPerimeter.y() - partition->min().y();
    if (nBound > 3) nBound = 3;
    //  East Bound
    eBound = partition->max().x() - 1 - pixelOnPerimeter.x();
    if (eBound > 3) eBound = 3;
    //  South Bound
    if (distanceFromLastBlackPixel == 0) {
        sBound = partition->max().y() - 1 - pixelOnPerimeter.y();
        if (sBound > 3) sBound = 3;
    } else {
        sBound = nBound - (distanceFromLastBlackPixel - 1);
    }
    
    //   +X direction
    for (int y = pixelOnPerimeter.y() - nBound; y <= pixelOnPerimeter.y() - sBound; ++y) {
        for (int x = pixelOnPerimeter.x() + eBound; x > pixelOnPerimeter.x(); --x) {
            ++checkCount;
            if (bitmap[y][x]){
                pixelOnPerimeter = Coordinate(x, y);
                return 0;
            }
        }
    }
    return -1;
}

int BoundingBox::fullExpandBottom(const vector< vector<bool> >& bitmap,
                                 const Partition * const partition,
                                 Coordinate& pixelOnPerimeter,
                                 int distanceFromLastBlackPixel) {
    int wBound, sBound, eBound;
    if (distanceFromLastBlackPixel > 3) distanceFromLastBlackPixel = 3;
    
    //  South Bound
    sBound = partition->max().y() - 1 - pixelOnPerimeter.y();
    if (sBound > 3) sBound = 3;
    //  West Bound
    wBound = pixelOnPerimeter.x() - partition->min().x();
    if (wBound > 3) wBound = 3;
    //  East Bound
    if (distanceFromLastBlackPixel == 0) {
        eBound = partition->max().x() - 1 - pixelOnPerimeter.x();
        if (eBound > 3) eBound = 3;
    } else {
        eBound =  wBound - (distanceFromLastBlackPixel - 1);
    }
    
    // +Y direction
    for (int y = pixelOnPerimeter.y() + sBound; y > pixelOnPerimeter.y(); --y) {
        for (int x = pixelOnPerimeter.x() - wBound; x <= pixelOnPerimeter.x() - eBound; ++x) {
            ++checkCount;
            if (bitmap[y][x]) {
                pixelOnPerimeter = Coordinate(x, y);
                return 0;
            }
        }
    }
    return -1;
}

int BoundingBox::fullExpandLeft(const vector< vector<bool> >& bitmap,
                                  const Partition * const partition,
                                  Coordinate& pixelOnPerimeter,
                                  int distanceFromLastBlackPixel) {
    int nBound, wBound, sBound;
    if (distanceFromLastBlackPixel > 3) distanceFromLastBlackPixel = 3;
    
    //  North Bound
    nBound = pixelOnPerimeter.y() - partition->min().y();
    if (nBound > 3) nBound = 3;
    //  West Bound
    wBound = pixelOnPerimeter.x() - partition->min().x();
    if (wBound > 3) wBound = 3;
    //  South Bound
    if (distanceFromLastBlackPixel == 0) {
        sBound = partition->max().y() - 1 - pixelOnPerimeter.y();
        if (sBound > 3) sBound = 3;
    } else {
        sBound = nBound - (distanceFromLastBlackPixel - 1);
    }
    
    //  -X direction
    for (int y = pixelOnPerimeter.y() - nBound; y <= pixelOnPerimeter.y() - sBound; ++y) {
        for (int x = pixelOnPerimeter.x() - wBound; x < pixelOnPerimeter.x(); ++x) {
            ++checkCount;
            if (bitmap[y][x]) {
                pixelOnPerimeter = Coordinate(x, y);
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

void BoundingBox::print(ostream& out) const {
    cout << '(';
    mMin.print(out);
    cout << ',';
    mMax.print(out);
    cout << ')';
}
