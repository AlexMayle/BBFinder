//
//  Coordinate.hpp
//  ocra
//
//  Created by Alex Mayle on 12/2/16.
//  Copyright © 2016 Alex Mayle. All rights reserved.
//

#ifndef Coordinate_hpp
#define Coordinate_hpp

#include <iostream>
#include <vector>

using namespace std;

extern int checkCount;

class BoundingBox;

enum Direction {
    north = 1,
    east = 2,
    west = 3,
    south = 4
};

class Coordinate {
private:
    int mX;
    int mY;
    
public:
    inline Coordinate(int x = 0, int y = 0) {
        mX = x;
        mY = y;
    }
    
    inline const int x() const {
        return mX;
    }
    
    inline const int y() const {
        return mY;
    }
    
    inline void setX(int x) {
        mX = x;
    }
    
    inline void setY(int y) {
        mY = y;
    }
    
    inline void print() const {
        cout << '(' << mX << ',' << mY << ')';
    }
    
    inline bool operator==(const Coordinate& other) const {
        return ((mX == other.mX) && (mY == other.mY));
    }
    
    inline bool operator!=(const Coordinate& other) const {
        return !(*this == other);
    }
    
    inline Coordinate operator-(const Coordinate& other) const {
        return Coordinate(mX - other.mX, mY - other.mY);
    }
    
    inline bool isNeighborWith(const Coordinate& candidate) const {
        Coordinate difference = *this - candidate;
        return ( (difference.x() <= 3) && (difference.y() <= 3) );
    }
    
    Coordinate getCardinalNeighbor(const vector< vector<bool> >& bitmap,
                                   const Coordinate& partitionMin,
                                   const Coordinate& partitionMax,
                                   const BoundingBox& boundingBox) const;
    
    Coordinate getAnyNeighbor(const vector< vector<bool> >& bitmap,
                              const Coordinate& partitionMin,
                              const Coordinate& partitionMax,
                              const BoundingBox& boundingBox,
                              const Direction direction,
                               int distance) const;
};

#endif /* Coordinate_hpp */
