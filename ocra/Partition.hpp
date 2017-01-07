//
//  Partition.hpp
//  Created by Alex Mayle on 12/29/16.
//
//  class:  Partition
//
//  Represents a rectangular portion of an image. It is described
//  by its top-left and bottom-right coordinates of the rectangle.
//
//  This is essentially an immutable wrapper for the BoundingBox class
//  in BoundingBox.hpp
//

#ifndef Partition_hpp
#define Partition_hpp

#include "Coordinate.hpp"

//  Fwd Dec
class BoundingBox;

class Partition {
public:
    /*    Constructors    -----------------------------------    */

    explicit Partition(const BoundingBox& other);
    
    Partition(const Partition& other);
    
    Partition(const Coordinate& topLeft, const Coordinate& bottomRight);
    
    Partition(const Coordinate&& topLeft, const Coordinate&& bottomRight);
    
    ~Partition();
    
    /*    Operators    --------------------------------------    */
    
    Partition operator=(Partition& other);
    
    /*    Public Member Functions    ------------------------    */

    //  Get the top left coordinate of the parition
    const Coordinate min() const;

    //  Get the bottom right coordinate of the parition
    const Coordinate max() const;
    
private:
    const BoundingBox * const mPartition;
};

#endif /* Partition_hpp */
