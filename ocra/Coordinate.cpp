//
//  Coordinate.cpp
//  Created by Alex Mayle on 12/2/16.
//

#include "Coordinate.hpp"

Coordinate::Coordinate(int x, int y) {
    mX = x;
    mY = y;
}

Coordinate::Coordinate(const Coordinate& other) {
    mX = other.mX;
    mY = other.mY;
}

bool Coordinate::operator==(const Coordinate& other) const {
    return (mX == other.mX) && (mY == other.mY);
}

bool Coordinate::operator!=(const Coordinate& other) const {
    return !(mX == other.mX) && (mY == other.mY);
}

Coordinate Coordinate::operator-(const Coordinate& other) const {
    return Coordinate(mX - other.mX, mY - other.mY);
}

void Coordinate::print(ostream& out) const {
    out << '(' << mX << ',' << mY << ')';
}
