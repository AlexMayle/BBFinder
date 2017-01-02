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

Partition::Partition(Coordinate&& min, Coordinate&& max) {
    mPartition.setMin(min);
    mPartition.setMax(max);
}

inline const Coordinate Partition::min() const {
    return mPartition.min();
};

inline const Coordinate Partition::max() const {
    return mPartition.max();
};
