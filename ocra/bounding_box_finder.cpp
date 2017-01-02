//
//  bounding_box_finder.cpp
//  ocra
//
//  Created by Alex Mayle on 1/1/17.
//  Copyright © 2017 Alex Mayle. All rights reserved.
//

#include "bounding_box_finder.hpp"

int belongsToBox(const vector<BoundingBox>& boxes, int x, int y) {
    for (BoundingBox box : boxes) {
        if (box.contains(x, y)) {
            return box.max().x();
        }
    }
    return -1;
}

bool findBlackPixel(const vector< vector<bool> >& bitmap,
                    const Coordinate& startingPoint,
                    Coordinate * result,
                    const vector<BoundingBox>& boxes,
                    const Partition * const partition) {
    
    int x = startingPoint.x();
    for (int y = startingPoint.y(); y < partition->max().y(); ++y) {
        for (x; x < partition->max().x(); ++x) {
            ++checkCount;
            if (bitmap[y][x]) {
                int nextXCoordinate = belongsToBox(boxes, x, y);
                
                if (nextXCoordinate == -1) {
                    *result = Coordinate(x, y);
                    return true;
                } else {
                    x = nextXCoordinate + 3;
                }
            }
        }
        x = partition->min().x();
    }
    return false;
}

void makeBox(const vector< vector<bool> >& bitmap,
             const Partition * const partition,
             const Coordinate& startingPoint,
             BoundingBox * result) {
    BoundingBox boundingBox = BoundingBox(startingPoint, startingPoint);
    Coordinate pixel = boundingBox.min();
    
    do {
        while (!boundingBox.quickExpand(bitmap, partition, pixel));
    } while (!boundingBox.checkPerimeter(bitmap, pixel, partition));
    
    *result = boundingBox;
}

vector<BoundingBox>* findBoxesInPartition(vector<vector<bool> >& bitmap,
                                          Partition * partition) {
    vector<BoundingBox> * boundingBoxes = new vector<BoundingBox>;
    Coordinate startLookingHere = partition->min();
    Coordinate blackPixelLocation;
    BoundingBox currentBox;
    
    while (findBlackPixel(bitmap, startLookingHere, &blackPixelLocation,
                          *boundingBoxes, partition)) {
        makeBox(bitmap, partition, blackPixelLocation, &currentBox);
        boundingBoxes->push_back(currentBox);
        
        if (currentBox.max().x() + 1 < partition->max().x()) {
            startLookingHere.setX(currentBox.max().x() + 1);
            startLookingHere.setY(currentBox.min().y());
        } else {
            startLookingHere.setX(partition->min().x());
            startLookingHere.setY(blackPixelLocation.y());
        }
    }
    return boundingBoxes;
}
