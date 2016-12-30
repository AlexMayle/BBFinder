//
//  Coordinate.cpp
//  ocra
//
//  Created by Alex Mayle on 12/2/16.
//  Copyright © 2016 Alex Mayle. All rights reserved.
//

#include "Coordinate.hpp"
#include "BoundingBox.hpp"
#include "Partition.hpp"

Coordinate Coordinate::getCardinalNeighbor(const vector< vector<bool> >& bitmap,
                                           const Partition * const partition,
                                           const BoundingBox& boundingBox) const {
    int sBound, wBound, eBound;
    
    //  South Bound
    sBound = partition->max().y() - 1 - mY;
    if (sBound > 3) sBound = 3;
    
    //  West Bound
    wBound = mX - partition->min().x();
    if (wBound > 3) wBound = 3;
    
    //  East Bound
    eBound = partition->max().x() - 1 - mX;
    if (eBound > 3) eBound = 3;
    
    //   X direction
    if (mX == boundingBox.max().x()) {
        for (int j = mX + eBound; j != mX; --j) {
            ++checkCount;
            if (bitmap[mY][j]) return Coordinate(j, mY);
        }
    }
    
    // Y direction
    if (mY == boundingBox.max().y()) {
        for (int i = mY + sBound; i != mY; --i) {
            ++checkCount;
            if (bitmap[i][mX]) return Coordinate(mX, i);
        }
    }
    
    if (mX == boundingBox.min().x()) {
        for (int j = mX - wBound; j != mX; ++j) {
            ++checkCount;
            if (bitmap[mY][j]) return Coordinate(j, mY);
        }
    }
    
    //        for (int i = this->y() + sBound; i >= this->y(); --i) {
    //            for (int j = this->x() + eBound; j >= this->x(); --j) {
    //                if (bitmap[i][j]) return Coordinate(j, i);
    //            }
    //        }
    //
    //        int x = this->x() + eBound;
    //        int y = this->y() + sBound;
    ////        bool flag = false;
    ////        while (Coordinate(x,y) != *this) {
    ////            while (x <= this->x() + wBound && y <= this->y() + sBound
    ////                   && x >= this->y() && y >= this->y()) {
    ////                if (bitmap[y][x]) return Coordinate(x, y);
    ////                else {
    ////                    if (flag) {
    ////                        --x;
    ////                        ++y;
    ////                    } else {
    ////                        ++x;
    ////                        --y;
    ////                    }
    ////                }
    ////            }
    ////            if (flag) --y;
    ////            else --x;
    ////            flag = !flag;
    ////        }
    //
    //        //  X direction
    //        if (this->y() == max.y()) {
    //            while (Coordinate(x,y) != *this) {
    //                x = this->x() + wBound--;
    //                y = this->y() + sBound--;
    //                while (x <= this->x()){
    //                    if (bitmap[y][x]) return Coordinate(x, y);
    //                    else ++x;
    //                }
    //            }
    //        }
    //
    //        //  Y direction
    //        if (this->x() == max.x()) {
    //            while (Coordinate(x,y) != *this) {
    //                x = this->x() + eBound;
    //                y = this->y() + nBound;
    //                while (x >= this->x()){
    //                    if (bitmap[y][x]) return Coordinate(x, y);
    //                    else --x;
    //                }
    //            }
    //        }
    
    return *this;
    
    
    
    //        //  Both x and y progress
    //        if (bitmap[y + 3][x + 3]) return  Coordinate(x + 3, y + 3);
    //        if (bitmap[y + 3][x + 2]) return  Coordinate(x + 2, y + 3);
    //        if (bitmap[y + 2][x + 3]) return  Coordinate(x + 3, y + 2);
    //        if (bitmap[y + 1][x + 3]) return  Coordinate(x + 3, y + 1);
    //        if (bitmap[y + 2][x + 2]) return  Coordinate(x + 2, y + 2);
    //        if (bitmap[y + 3][x + 1]) return  Coordinate(x + 1, y + 3);
    //        if (bitmap[y + 3][x + 0]) return  Coordinate(x + 0, y + 3);
    //        if (bitmap[y + 2][x + 1]) return  Coordinate(x + 1, y + 2);
    //        if (bitmap[y + 1][x + 2]) return  Coordinate(x + 2, y + 1);
    //        if (bitmap[y + 0][x + 3]) return  Coordinate(x + 3, y + 0);
    //        if (bitmap[y + 0][x + 2]) return  Coordinate(x + 2, y + 0);
    //        if (bitmap[y + 1][x + 1]) return  Coordinate(x + 1, y + 1);
    //        if (bitmap[y + 2][x + 0]) return  Coordinate(x + 0, y + 2);
    //        if (bitmap[y + 1][x + 0]) return  Coordinate(x + 0, y + 1);
    //        if (bitmap[y + 0][x + 0]) return  Coordinate(x + 0, y + 0);
    //
    //        //  Progress in x direction only
    //        if (bitmap[y - 3][x + 3]) return  Coordinate(x + 3, y + 3);
    //        if (bitmap[y - 2][x + 3]) return  Coordinate(x + 2, y + 3);
    //        if (bitmap[y - 2][x + 2]) return  Coordinate(x + 3, y + 2);
    //        if (bitmap[y - 1][x + 3]) return  Coordinate(x + 3, y + 1);
    //        if (bitmap[y - 1][x + 2]) return  Coordinate(x + 2, y + 2);
    //        if (bitmap[y - 1][x + 1]) return  Coordinate(x + 1, y + 3);
    //
    //        //  Progress in y direction only
    //        int tmp = x;
    //        x = y;
    //        y = tmp;
    //        if (bitmap[y - 3][x + 3]) return  Coordinate(x + 3, y + 3);
    //        if (bitmap[y - 2][x + 3]) return  Coordinate(x + 2, y + 3);
    //        if (bitmap[y - 2][x + 2]) return  Coordinate(x + 3, y + 2);
    //        if (bitmap[y - 1][x + 3]) return  Coordinate(x + 3, y + 1);
    //        if (bitmap[y - 1][x + 2]) return  Coordinate(x + 2, y + 2);
    //        if (bitmap[y - 1][x + 1]) return  Coordinate(x + 1, y + 3);
    //        
    //        return *this;
    
}

Coordinate Coordinate::getAnyNeighbor(const vector< vector<bool> >& bitmap,
                                      const Partition * const partition,
                                      const BoundingBox& boundingBox,
                                      const Direction direction,
                                       int distance) const {
    int nBound, sBound, wBound, eBound;
    if (distance > 3) distance = 3;
    if (distance == 0) {
        //  North Bound
        nBound = mY - partition->min().y();
        if (nBound > 3) nBound = 3;
        
        //  South Bound
        sBound = partition->max().y() - 1 - mY;
        if (sBound > 3) sBound = 3;
        
        //  West Bound
        wBound = mX - partition->min().x();
        if (wBound > 3) wBound = 3;
        
        //  East Bound
        eBound = partition->max().x() - 1 - mX;
        if (eBound > 3) eBound = 3;
    } else {
        //  North Bound
        nBound = mY - partition->min().y();
        if (nBound > 3) nBound = 3;
        
        //  South Bound
        if (direction == north) {
            sBound = 0 - (nBound - (distance - 1));
        } else {
            sBound = partition->max().y() - 1 - mY;
            if (sBound > 3) sBound = 3;
        }
        
        //  West Bound
        wBound = mX - partition->min().x();
        if (wBound > 3) wBound = 3;
        
        //  East Bound
        if (direction == west) {
            eBound =  0 - (wBound - (distance - 1));
        } else {
            eBound = partition->max().x() - 1 - mX;
            if (eBound > 3) eBound = 3;
        }
    }
    
//    //  North Bound
//    nBound = mY - partitionMin.y();
//    if (nBound > 3) nBound = 3;
//    
//    //  South Bound
//    sBound = partitionMax.y() - mY;
//    if (sBound > 3) sBound = 3;
//    
//    //  West Bound
//    wBound = mX - partitionMin.x();
//    if (wBound > 3) wBound = 3;
//    
//    //  East Bound
//    eBound = partitionMax.x() - mX;
//    if (eBound > 3) eBound = 3;
    
    //   +X direction
    if (mX == boundingBox.max().x()) {
        for (int y = mY - nBound; y <= mY + sBound; ++y) {
            for (int x = mX + eBound; x > mX; --x) {
                ++checkCount;
                if (bitmap[y][x]) return Coordinate(x, y);
            }
        }
    }
//    if (mX == boundingBox.max().x()) {
//        for (int j = mX + eBound; j != mX; --j) {
//            ++checkCount;
//            if (bitmap[mY][j]) return Coordinate(j, mY);
//        }
//    }
    
    // +Y direction
    if (mY == boundingBox.max().y()) {
        for (int y = mY + sBound; y > mY; --y) {
            for (int x = mX - wBound; x <= mX + eBound; ++x) {
                ++checkCount;
                if (bitmap[y][x]) return Coordinate(x, y);
            }
        }
    }
//    if (mY == boundingBox.max().y()) {
//        ++checkCount;
//        for (int i = mY + sBound; i != mY; --i) {
//            if (bitmap[i][mX]) return Coordinate(mX, i);
//        }
//    }
    
    //  -X direction
    if (mX == boundingBox.min().x()) {
        for (int y = mY - nBound; y <= mY + sBound; ++y) {
            for (int x = mX - wBound; x < mX; ++x) {
                ++checkCount;
                if (bitmap[y][x]) return Coordinate(x, y);
            }
        }
    }
//    if (mX == boundingBox.min().x()) {
//        ++checkCount;
//        for (int j = mX - wBound; j != mX; ++j) {
//            if (bitmap[mY][j]) return Coordinate(j, mY);
//        }
//    }
    
    return *this;
}
