//
//  Partition.hpp
//  ocra
//
//  Created by Alex Mayle on 12/29/16.
//  Copyright © 2016 Alex Mayle. All rights reserved.
//

#ifndef Partition_hpp
#define Partition_hpp

#include "Coordinate.hpp"
#include "BoundingBox.hpp"

//  A semantic wrapper for the BoundingBox class when it is used in the context of
//  denoting a partition, rather than denoting the bounding box of some object
//  within a parition.
//
//  Unlike a BoundingBox object, however, Parition objects cannot be changed after
//  initialization.
//
class Partition {
public:
    //  Takes a min and max coordinate to designate the top-left and bottom-right
    //  corners of the parition
    Partition(Coordinate& min, Coordinate& max);
    
    Partition(Coordinate&& min, Coordinate&& max);
    
    //  Get the top left coordinate of the parition
    inline const Coordinate min() const;
    
    //  Get the bottom right coordinate of the parition
    inline const Coordinate max() const;
    
private:
    BoundingBox mPartition;
};

#endif /* Partition_hpp */
