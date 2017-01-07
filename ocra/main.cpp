//
//  main.cpp
//  Created by Alex Mayle on 12/2/16.
//
//  Driver program for BBFinder
//
//  This program takes any ASCII PBM file, finds the Bounding
//  Box's for all of the shapes in the image and then outputs a
//  new PBM file "<input file>.new" with the BoundingBox's drawn
//  to the image.
//
//  For interface specifics, refer to BBFinder.hpp. For more information
//  about the concept of Bounding Box's, refer to BoundingBox.hpp
//
//  The Bounding Box's are found using the findBoxesInPartition() function.
//  The PBM file i/o is handled using the functions in pbm_io.h.
//

#include <iostream>
#include <string>
#include <vector>

#include "pbm_io.hpp"           //  ASCII PBM file i/o
#include "BBFinder.hpp"         //  Bounding Box Finder

//  Used to check the number of pixels in bitmap checked
int checkCount = 0;
//  Suffix to distinguish the PBM file that is output
static std::string OUTPUT_FILE_SUFFIX = ".new";
//  Type declaration for bitmap
typedef std::vector<std::vector<bool> > Bitmap;


int main(int argc, const char * argv[]) {
    Bitmap bitmap;

    //  Get input
    if (argc > 1) pbmIO::readPBMFile(argv[1], bitmap);
    else {
        std::cout << "Please provide a filepath to an ASCII PBM file.\n";
        exit(-1);
    }
    
    //  Find boxes
    std::vector<BoundingBox> * boxes = BBFinder::findBoxesInPartition(bitmap);
    
    //  Output boxes
    for (auto box : *boxes) {
        box.printToBitmap(bitmap);          //  Print the box to the bitmap
        box.print(std::cout);               //  Print the box's co-ord's to stdout
        std::cout << ',';
    }
    delete boxes;
    
    //  Print output ASCII PBM file
    std::string outputFileName(argv[1]);
    outputFileName.append(OUTPUT_FILE_SUFFIX);
    pbmIO::writePBMFile(outputFileName, bitmap);
    
    std::cout << std::endl << "Comparisons: " << checkCount + 2 << " Pixels: "
              << bitmap[0].size() * bitmap.size() << " Speedup: "
              << bitmap[0].size() * bitmap.size() / (double) checkCount
              << "\nOutput file can be found at " << outputFileName << std::endl;
}
