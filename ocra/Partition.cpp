//
//  Partition.cpp
//  Created by Alex Mayle on 12/29/16.
//

#include "Partition.hpp"
#include "BoundingBox.hpp"

Partition::Partition(const Coordinate& topLeft, const Coordinate& bottomRight)
    : mPartition(new BoundingBox(topLeft, bottomRight)) {
}
Partition::Partition(const Coordinate&& topLeft, const Coordinate&& bottomRight)
    : mPartition(new BoundingBox(topLeft, bottomRight)) {
}

Partition::Partition(const BoundingBox& other)
    : mPartition(new BoundingBox(other)) {
}

Partition::Partition(const BoundingBox&& other)
    : mPartition(new BoundingBox(other)) {
}

Partition::Partition(const Partition& other)
    : mPartition(other.mPartition) {
}

const Coordinate Partition::min() const {
    return mPartition->min();
};

const Coordinate Partition::max() const {
    return mPartition->max();
};
