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
    
    Coordinate(int x = 0, int y = 0);
    
    Coordinate(const Coordinate& other);
    
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
    bool operator==(const Coordinate& other) const;
    
    //  Returns true if either the x or y coordinates differ
    bool operator!=(const Coordinate& other) const;
    
    //  Member wise subtraction (e.g. (5,3) - (2,1) = (3,2))
    Coordinate operator-(const Coordinate& other) const;
    
    /*    Output    -----------------------------------    */
    
    //  Writes (x, y) to destination out
    void print(ostream& out) const;
    
};
#endif /* Coordinate_hpp */
