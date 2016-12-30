//
//  Partition.cpp
//  ocra
//
//  Created by Alex Mayle on 12/29/16.
//  Copyright © 2016 Alex Mayle. All rights reserved.
//

#include "Partition.hpp"

Partition::Partition(Coordinate& min, Coordinate& max) {
    mPartition.setMin(min);
    mPartition.setMax(max);
}
