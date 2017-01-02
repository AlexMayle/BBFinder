//
//  BoundingBox.hpp
//  Created by Alex Mayle on 12/3/16.
//
//  class:      BoundingBox
//
//  Represents the box that bounds some object / shape within
//  an ASCII PBM image. The BoundingBox is described by the coordinates
//  of its top-left and bottom-right corner. An object / shape is defined
//  as a collection of neighboring black pixels. Two black pixels are neighbors
//  if the distance between their x and y coordinates are both less than
//  or equal to the static member PXL_DIS_THRESH.
//
//  Most public member functions take a bitmap argument, which should
//  represent the ASCII PBM file as a 2D array of booleans such that
//  black pixels are True and white pixels are False. They also take
//  a partition argument, which is itself a BoundingBox that describes
//  a section of the bitmap to be concerned with.
//

#ifndef BoundingBox_hpp
#define BoundingBox_hpp

#include <fstream>
#include <vector>
#include "Coordinate.hpp"
#include "Partition.hpp"

extern int checkCount;

class BoundingBox {
public:
    //  The BoundingBox contains a region of "neighboring"
    //  black pixels. Two black pixels are "neighbors" if the
    //  distance between their x coordinates and their y coordinates
    //  are both less than PXL_DIS_THRESH.
    static const unsigned int PXL_DIS_THRESH = 3;
    
    /*    Constructors    -----------------------------------    */
    BoundingBox();

    BoundingBox(const BoundingBox& other);
    
    BoundingBox(const Coordinate& min, const Coordinate& max);
    
    BoundingBox(const Coordinate&& min, const Coordinate&& max);
    
    /*    Setters / Getters    ------------------------------    */
    
    //  Returns the coordinate representing the top-left corner of the box
    inline const Coordinate min() const {
        return mMin;
    }
    
    //  Sets the top-left corner of the box
    inline void setMin(const Coordinate& min) {
        mMin = min;
    }
    
    //  Returns the coordinate representing the bottom-right corner of the box
    inline const Coordinate max() const {
        return mMax;
    }
    
    //  Sets the bottom-right corner of the box
    inline void setMax(const Coordinate& max) {
        mMax = max;
    }
    
    /*    Operators    ---------------------------------------    */

    //  Returns true if this BoundingBox is in the exact same position
    //  as other
    bool operator==(const BoundingBox& other) const;
    
    /*    Public Member Functions    -------------------------    */
    
    //  Checks if the pixel lies within the BoundingBox
    //
    //  pixel:      A pixel represented by a Coordinate object
    //
    //  return:     True if pixel is contained within the BoundingBox
    //              false, otherwise
    //
    bool contains(const Coordinate& pixel) const;
    
    //  Checks if the pixel described by (x, y) lies within the BoundingBox
    //
    //  x:          Pixel's location on the x axis
    //  y:          Pixel's location on the y axis
    //
    //  return:     True if the pixel represented by (x, y) is contained
    //              within the BoundingBox. False, otherwise.
    //
    bool contains(const int x, const int y) const;
    
    //  Checks if the other BoundingBox and this BoundingBox overlap
    //  each other.
    //
    //  other:      A BoundingBox
    //
    //  return:     True if this BoundingBox object and other overlap
    //              False, otherwise.
    //
    bool contains(const BoundingBox& other) const;
    
    //  Checks the entire perimeter of the BoundingBox for black pixels,
    //  attempts to find a neighbor of the black pixel, and then
    //  expands the BoundingBox based on that neighbor. The neighbor
    //  is then made available through pixelOnPerimeter. If no black
    //  pixel with a neighbor that can expand the box is found within the
    //  area of bitmap described by partition, the box is unchanged,
    //  as well as pixelOnPerimeter.
    //
    //  bitmap:                 2D array representing an ASCII PBM image
    //  pixelOnPerimeter:       The neighbor used to expand the BoundingBox
    //                          is made available here
    //  partition:              A partition of bitmap
    //
    //  return:                 0 if the box is expanded, -1 otherwise
    //
    int checkPerimeter(const std::vector<std::vector<bool> >& bitmap,
                        Coordinate& pixelOnPerimeter,
                        const Partition * const partition);
    
    //  Tries to find a neighbor of pixelOnPerimeter within the area
    //  of bitmap described by partition that can be used to expand the
    //  BoundingBox. The neighbor used to expand the box is then made
    //  available through pixelOnPerimeter.
    //
    //  bitmap:                 2D array representing an ASCII PBM image
    //  pixelOnPerimeter:       A pixel to look for neighbors from.
    //                          If a neighbor is found, it is then placed here
    //  partition:              A partition of bitmap
    //
    //  return:                 0 if the box is expanded, -1 otherwise
    //
    int quickExpand(const std::vector<std::vector<bool> >& bitmap,
                    const Partition * const partition,
                    Coordinate& pixelOnPerimeter);

    //  Prints the BoundingBox to bitmap
    //
    //  bitmap:                 2D array representing an ASCII PBM image
    //
    void printToBitmap(std::vector<std::vector<bool> >& bitmap) const;
    
    //  Erases the BoundingBox to bitmap
    //
    //  bitmap:                 2D array representing an ASCII PBM image
    //
    void eraseFromBitmap(std::vector<std::vector<bool> >& bitmap) const;
    
    //  Prints the BoundingBox's top left corner and bottom right corner
    //  to out. (e.g. ((0,0), (5,5))).
    //
    //  out:    Output stream to send data to
    //
    void print(std::ostream& out) const;
    
private:
    Coordinate mMin;        // Top-left corner
    Coordinate mMax;        // Bottom-Right corner
    
    int fullExpandRight(const std::vector<std::vector<bool> >& bitmap,
                        const Partition * const partition,
                        Coordinate& pixelOnPerimeter,
                        int distanceFromLastBlackPixel);
    
    int fullExpandBottom(const std::vector<std::vector<bool> >& bitmap,
                         const Partition * const partition,
                         Coordinate& pixelOnPerimeter,
                         int distanceFromLastBlackPixel);
    
    int fullExpandLeft(const std::vector<std::vector<bool> >& bitmap,
                       const Partition * const partition,
                       Coordinate& pixelOnPerimeter,
                       int distanceFromLastBlackPixel);
    
    void expandBoundaries(const Coordinate& blackPixel);
};

#endif /* BoundingBox_hpp */
