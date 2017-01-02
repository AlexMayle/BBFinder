//
//  BBFinder.hpp
//  Created by Alex Mayle on 1/1/17.
//
//  namespace:  BBFinder    (BoundingBoxFinder)
//
//  BBFinder encapsulates a number of functions used to
//  find Bounding Box's of object's within a bitmap. Bounding Box's
//  are rectangles that fully contain a shape or object within a bitmap.
//  For more specific information about the concept of Bounding Box's refer
//  to BoundingBox.h. The bitmap should be represented by a 2D
//  array of booleans.
//
//  Use the top-level function findBoxesInPartition() to generate
//  an array of BoundingBox objects from the bitmap. The other functions
//  are made public in case more control is needed. For example, if
//  each Bounding Box must be manipulated before finding another one.
//
#ifndef BBFinder_hpp
#define BBFinder_hpp

#include "Coordinate.hpp"
#include "BoundingBox.hpp"
#include "Partition.hpp"

namespace BBFinder {
    //  Finds all Bounding Box's within an area of bitmap designated
    //  by partition. To find all Bounding Box's within the entire bitmap,
    //  do not supply a second argument. A pointer to a vector of BoundingBox
    //  objects is returned.
    //
    //  partitionPtr:   A pointer to the partition to search within
    //
    //  returns:        A pointer to a vector of BoundingBox objects
    //
    std::vector<BoundingBox>* findBoxesInPartition(const std::vector<std::vector<bool> >& bitmap,
                                              Partition * partition = nullptr);

    //  Takes a vector of BoundingBox objects and a coordinate designated by
    //  x and y to determine if (x,y) is contained within any box. If so,
    //  the containing box's max x coordinate is returned. If no such box
    //  exists, -1 is returned.
    //
    //  boxes:      Vector of BoundingBox objects
    //  x, y:       Describes a coordinate (x,y) to be tested
    //              for containment within any of the elements of boxes
    //
    //  returns:    The containing box's max x coordinate, -1 otherwise
    //
    int belongsToBox(const std::vector<BoundingBox>& boxes, int x, int y);
    
    //  Places the first black pixel it finds, excluding those contained in
    //  any existing bounding boxes, given by boxes, into result. The algorithm
    //  starts looking at startingPoint and checks the region of bitmap denoted
    //  by partition. All pixels are checked on the first row before moving to the
    //  next, such that any black pixel found, if any, has the minimum y coordinate
    //  of any BoundingBox that is grown from it.
    //
    //  bitmap:         2D array representing the image
    //  startingPoint:  First spot in bitmap checked
    //  result:         Coordinate of the first black pixel found is placed here
    //                  If no such pixel, result is unmodified
    //  boxes:          Existing bounding boxes in the bitmap
    //  partition:      Area of bitmap to be searched
    //
    //  Returns:        True if black pixel is found, false otherwise
    //
    bool findBlackPixel(const std::vector<std::vector<bool> >& bitmap,
                        const Coordinate& startingPoint,
                        Coordinate * result,
                        const std::vector<BoundingBox>& boxes,
                        const Partition * const partition);
    
    //  Grows a BoundingBox object starting from blackPixelLocation to its
    //  maximum size within partition on the 2D array bitmap. The resulting
    //  BoundingBox is assigned to the result argument
    //
    //  bitmap:                 2D array representing the image
    //  blackPixelLocation:     start growing the box from this pixel
    //  result:                 The resulting BoundingBox object
    //  partition:              Partition object representing a section of the image
    //
    void makeBox(const std::vector<std::vector<bool> >& bitmap,
                 const Partition * const partition,
                 const Coordinate& startingPoint,
                 BoundingBox * result);
};
#endif /* bounding_box_finder_hpp */
