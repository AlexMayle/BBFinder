//
//  pbm_io.hpp
//  Created by Alex Mayle on 1/1/17.
//
//  namespace:  pbmIO   (PBM file I/O)
//
//  pbmIO encapsulates I/O functions for going between ASCII PBM files
//  and 2D boolean arrays.
//
//  This is not a robust package in the sense
//  that if an error occurs during file i/o, an appropriate error
//  will be sent to stdout, but the program will simply terminate.
//
//  These are specifically useful in conjuction with the functions
//  found in the BBFinder (Bounding Box Finder) namespace, as they
//  operate on PBM files represented by 2D boolean arrays.
//

#ifndef pbm_io_hpp
#define pbm_io_hpp

#include <string>
#include <vector>

namespace pbmIO {

    //  Prints an ASCII PBM file to the file denoted by filepath outputFileName
    //  The width and height are printed based on the dimensions of bitmap
    //  and the data is printed based on the contents of bitmap
    //
    //  outputFileName:     filepath denoting the output file. If it exists
    //                      its contents are overwritten, if it does not, it
    //                      is created.
    //  bitmap:             2D boolean array representing a PBM file to be
    //                      printed to file outputFileName
    //
    void writePBMFile(const std::string& outputFileName,
                      const std::vector<std::vector<bool> >& bitmap);

    //  Reads PBM file designated by filename and places its contents
    //  in the 2D boolean array bitmap. If the file cannot be opened
    //  or the file is not an ASCII PBM file, the program will
    //  provide a relevant error message and abort.
    //
    //  filename:   ASCII pbm filepath of the existing file to be opened
    //  bitmap:     A 2D boolean array to be filled with the contents
    //              of the file designated by filename. The contents
    //              of the array prior to being pass are overwritten, if any.
    //
    void readPBMFile(const std::string& filename,
                     std::vector<std::vector<bool> >& bitmap);

};
#endif /* pbm_io_hpp */
