//
//  BoundingBox.cpp
//  Created by Alex Mayle on 12/3/16.
//

#include "BoundingBox.hpp"

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

bool BoundingBox::operator==(const BoundingBox& other) const {
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

int BoundingBox::checkPerimeter(const std::vector< std::vector<bool> >& bitmap,
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

int BoundingBox::quickExpand(const std::vector< std::vector<bool> >& bitmap,
                             const Partition * const partition,
                             Coordinate& pixelOnPerimeter) {
    
    int sBound, wBound, eBound;
    
    //  South Bound
    sBound = partition->max().y() - 1 - pixelOnPerimeter.y();
    if (sBound > PXL_DIS_THRESH) sBound = PXL_DIS_THRESH;
    
    //  West Bound
    wBound = pixelOnPerimeter.x() - partition->min().x();
    if (wBound > PXL_DIS_THRESH) wBound = PXL_DIS_THRESH;
    
    //  East Bound
    eBound = partition->max().x() - 1 - pixelOnPerimeter.x();
    if (eBound > PXL_DIS_THRESH) eBound = PXL_DIS_THRESH;
    
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

int BoundingBox::fullExpandRight(const std::vector< std::vector<bool> >& bitmap,
                                 const Partition * const partition,
                                 Coordinate& pixelOnPerimeter,
                                 int distanceFromLastBlackPixel) {
    int nBound, sBound, eBound;
    if (distanceFromLastBlackPixel > PXL_DIS_THRESH) distanceFromLastBlackPixel = PXL_DIS_THRESH;
    
    //  North Bound
    nBound = pixelOnPerimeter.y() - partition->min().y();
    if (nBound > PXL_DIS_THRESH) nBound = PXL_DIS_THRESH;
    //  East Bound
    eBound = partition->max().x() - 1 - pixelOnPerimeter.x();
    if (eBound > PXL_DIS_THRESH) eBound = PXL_DIS_THRESH;
    //  South Bound
    if (distanceFromLastBlackPixel == 0) {
        sBound = partition->max().y() - 1 - pixelOnPerimeter.y();
        if (sBound > PXL_DIS_THRESH) sBound = PXL_DIS_THRESH;
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

int BoundingBox::fullExpandBottom(const std::vector< std::vector<bool> >& bitmap,
                                 const Partition * const partition,
                                 Coordinate& pixelOnPerimeter,
                                 int distanceFromLastBlackPixel) {
    int wBound, sBound, eBound;
    if (distanceFromLastBlackPixel > PXL_DIS_THRESH) distanceFromLastBlackPixel = PXL_DIS_THRESH;
    
    //  South Bound
    sBound = partition->max().y() - 1 - pixelOnPerimeter.y();
    if (sBound > PXL_DIS_THRESH) sBound = PXL_DIS_THRESH;
    //  West Bound
    wBound = pixelOnPerimeter.x() - partition->min().x();
    if (wBound > PXL_DIS_THRESH) wBound = PXL_DIS_THRESH;
    //  East Bound
    if (distanceFromLastBlackPixel == 0) {
        eBound = partition->max().x() - 1 - pixelOnPerimeter.x();
        if (eBound > PXL_DIS_THRESH) eBound = PXL_DIS_THRESH;
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

int BoundingBox::fullExpandLeft(const std::vector< std::vector<bool> >& bitmap,
                                  const Partition * const partition,
                                  Coordinate& pixelOnPerimeter,
                                  int distanceFromLastBlackPixel) {
    int nBound, wBound, sBound;
    if (distanceFromLastBlackPixel > PXL_DIS_THRESH) distanceFromLastBlackPixel = PXL_DIS_THRESH;
    
    //  North Bound
    nBound = pixelOnPerimeter.y() - partition->min().y();
    if (nBound > PXL_DIS_THRESH) nBound = PXL_DIS_THRESH;
    //  West Bound
    wBound = pixelOnPerimeter.x() - partition->min().x();
    if (wBound > PXL_DIS_THRESH) wBound = PXL_DIS_THRESH;
    //  South Bound
    if (distanceFromLastBlackPixel == 0) {
        sBound = partition->max().y() - 1 - pixelOnPerimeter.y();
        if (sBound > PXL_DIS_THRESH) sBound = PXL_DIS_THRESH;
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

void BoundingBox::printToBitmap(std::vector<std::vector<bool> >& bitmap) const {
    for (int i = mMin.y(); i <= mMax.y(); ++i) {
        bitmap[i][mMin.x()] = true;
        bitmap[i][mMax.x()] = true;
    }
    for (int j = mMin.x(); j <= mMax.x(); ++j) {
        bitmap[mMin.y()][j] = true;
        bitmap[mMax.y()][j] = true;
    }
}

void BoundingBox::eraseFromBitmap(std::vector< std::vector<bool> >& bitmap) const {
    for (int i = mMin.y(); i <= mMax.y(); ++i) {
        bitmap[i][mMin.x()] = false;
        bitmap[i][mMax.x()] = false;
    }
    for (int j = mMin.x(); j <= mMax.x(); ++j) {
        bitmap[mMin.y()][j] = false;
        bitmap[mMax.y()][j] = false;
    }
}

void BoundingBox::print(std::ostream& out) const {
    out << '(';
    mMin.print(out);
    out << ',';
    mMax.print(out);
    out << ')';
}
