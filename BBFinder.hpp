//
//  BBFinder.hpp
//  Created by Alex Mayle on 1/1/17.
//
//  namespace:  BBFinder    (BoundingBoxFinder)
//
//  BBFinder encapsulates a number of functions used to
//  find Bounding Box's of object's within an ASCII PBM file. Bounding Box's
//  are rectangles that fully contain a shape or object within an image.
//  The BoundingBox object starts as a single black pixel and "grows" until
//  all the black pixels encompassing the shape are sourrounded by the box.
//  For more specific information about the concept of Bounding Box's refer
//  to BoundingBox.hpp.
//
//  Use the top-level function findBoxesInPartition() to find all Bounding
//  Box's within an image. Other functions allow the creation of a BoundingBox
//  from a single "seed" black pixel. An optional partition argument may be
//  provided to most functions to find all the Bounding Box's within
//  only a section of the image. For more info on Partitions refer to Partition.hpp
//
//  Most public member functions take a bitmap argument, which should
//  represent the ASCII PBM file as a 2D array of booleans such that
//  black pixels are True and white pixels are False. For help with
//  generating such a bitmap, see pbm_io.hpp.
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
    //  bitmap:         2D array representing the image
    //  partition:      A pointer to the partition to search within
    //
    //  returns:        A pointer to a vector of BoundingBox objects
    //
    std::vector<BoundingBox>* findBoxesInPartition(const std::vector<std::vector<bool> >& bitmap,
                                              Partition * partition = nullptr);
    
    //  Grows a single BoundingBox object starting from startingPoint to its
    //  maximum size within partition on the 2D array bitmap. If one is concerned
    //  with the whole bitmap, do not supply a partition argument. A pointer to the
    //  resulting BoundingBox object is returned.
    //
    //  bitmap:                 2D array representing the image
    //  startingPoint:          start growing the box from this pixel
    //  partition:              Partition object representing a section of the image
    //                          in which the BoundingBox is allowed to grow
    //
    BoundingBox * growBox(const std::vector<std::vector<bool> >& bitmap,
                          const Coordinate& startingPoint,
                          Partition * partition = nullptr);
    
    //  Used to find a black pixel, from which a BoundingBox may grow from
    //
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
    bool getBoxSeed(const std::vector<std::vector<bool> >& bitmap,
                    const Coordinate& startingPoint,
                    Coordinate * result,
                    const std::vector<BoundingBox>& boxes,
                    Partition * partition = nullptr);

    //  Used to determine if a black pixel is part of a BoundingBox
    //  already, or if it can be used to grow a new BoundingBox
    //
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
    
};
#endif /* bounding_box_finder_hpp */
