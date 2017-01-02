//
//  Partition.hpp
//  Created by Alex Mayle on 12/29/16.
//
//  class:  Partition
//
//  This is essentially an immutable BoundingBox object used to denote
//  an area within a bitmap represented as a 2D boolean array. Like
//  the BoundingBox object it is described by it's top-left and
//  bottom-right corner coordinates.
//
//  For a more flexible and mutable version, use BoundingBox instead
//

#ifndef Partition_hpp
#define Partition_hpp

#include "Coordinate.hpp"

class BoundingBox;

//  A semantic wrapper for the BoundingBox class when it is used in the context of
//  denoting a partition, rather than denoting the bounding box of some object
//  within a parition.
//
//  Unlike a BoundingBox object, however, Parition objects cannot be changed after
//  initialization.
//
class Partition {
public:
    /*    Constructors    -----------------------------------    */

    explicit Partition(const BoundingBox& other);
    
    explicit Partition(const BoundingBox&& other);

    Partition(const Partition& other);
    
    Partition(const Coordinate& topLeft, const Coordinate& bottomRight);
    
    Partition(const Coordinate&& topLeft, const Coordinate&& bottomRight);
    
    /*    Public Member Functions    ------------------------    */

    //  Get the top left coordinate of the parition
    const Coordinate min() const;

    //  Get the bottom right coordinate of the parition
    const Coordinate max() const;
    
private:
    const BoundingBox * const mPartition;
};

#endif /* Partition_hpp */
