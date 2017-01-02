//
//  Coordinate.hpp
//  Created by Alex Mayle on 12/2/16.
//
//  Class:  Coordinate
//
//  An abstraction for a mutable coordinate pair (e.g. (x, y)).
//

#ifndef Coordinate_hpp
#define Coordinate_hpp

#include <iostream>
using namespace std;


class Coordinate {
private:
    int mX;     //  X coordinate
    int mY;     //  Y coordinate
    
public:
    /*    Constructors    -------------------------------    */
    
    inline Coordinate(int x = 0, int y = 0) {
        mX = x;
        mY = y;
    }
    
    inline Coordinate(const Coordinate& other) {
        mX = other.mX;
        mY = other.mY;
    }
    
    /*    Getters / Setters    --------------------------    */
    
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
    
    /*    Operators    ---------------------------------    */
    
    //  Returns true if both x and y coordinates are the same
    inline bool operator==(const Coordinate& other) const {
        return (mX == other.mX) && (mY == other.mY);
    }
    
    //  Returns true if either the x or y coordinates differ
    inline bool operator!=(const Coordinate& other) const {
        return !(mX == other.mX) && (mY == other.mY);
    }
    
    inline Coordinate operator-(const Coordinate& other) const {
        return Coordinate(mX - other.mX, mY - other.mY);
    }
    
    /*    Output    -----------------------------------    */
    
    inline void print(ostream& out) const {
        out << '(' << mX << ',' << mY << ')';
    }
};

#endif /* Coordinate_hpp */
