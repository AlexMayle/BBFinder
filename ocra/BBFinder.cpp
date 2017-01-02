//
//  bounding_box_finder.cpp
//  Created by Alex Mayle on 1/1/17.
//

#include "BBFinder.hpp"

int BBFinder::belongsToBox(const std::vector<BoundingBox>& boxes, int x, int y) {
    for (BoundingBox box : boxes) {
        if (box.contains(x, y)) {
            return box.max().x();
        }
    }
    return -1;
}

bool BBFinder::getBoxSeed(const std::vector<std::vector<bool> >& bitmap,
                          const Coordinate& startingPoint,
                          Coordinate * result,
                          const std::vector<BoundingBox>& boxes,
                          Partition * partition)
{
    //  If no partition is provided, make it the same size as bitmap
    if (!partition) partition = new Partition(Coordinate(0,0),
                                              Coordinate(bitmap[0].size(), bitmap.size()));
    
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

BoundingBox * BBFinder::growBox(const std::vector<std::vector<bool> >& bitmap,
                                const Coordinate& startingPoint,
                                Partition * partition)
{
    //  If no partition is provided, make it the same size as bitmap
    if (!partition) partition = new Partition(Coordinate(0,0),
                                              Coordinate(bitmap[0].size(), bitmap.size()));
    BoundingBox * boundingBox = new BoundingBox(startingPoint, startingPoint);
    Coordinate pixel = boundingBox->min();
    
    do {
        while (!boundingBox->quickExpand(bitmap, partition, pixel));
    } while (!boundingBox->checkPerimeter(bitmap, pixel, partition));
    
    return boundingBox;
}

std::vector<BoundingBox>* BBFinder::findBoxesInPartition(const std::vector<std::vector<bool> >& bitmap,
                                                         Partition * partition)
{
    //  If no partition is provided, make it the same size as bitmap
    if (!partition) partition = new Partition(Coordinate(0,0),
                                              Coordinate(bitmap[0].size(), bitmap.size()));
    std::vector<BoundingBox> * boundingBoxes = new std::vector<BoundingBox>;
    Coordinate startLookingHere = partition->min();
    Coordinate blackPixelLocation;
    
    while (getBoxSeed(bitmap, startLookingHere, &blackPixelLocation,
                      *boundingBoxes, partition)) {
        BoundingBox * box = growBox(bitmap, blackPixelLocation, partition);
        boundingBoxes->push_back(*box);
        
        if (box->max().x() + 1 < partition->max().x()) {
            startLookingHere.setX(box->max().x() + 1);
            startLookingHere.setY(box->min().y());
        } else {
            startLookingHere.setX(partition->min().x());
            startLookingHere.setY(blackPixelLocation.y());
        }
        delete box;
    }
    return boundingBoxes;
}
