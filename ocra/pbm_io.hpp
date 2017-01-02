//
//  pbm_io.hpp
//  ocra
//
//  Created by Alex Mayle on 1/1/17.
//  Copyright © 2017 Alex Mayle. All rights reserved.
//

#ifndef pbm_io_hpp
#define pbm_io_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

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
//  return:             VOID
//
void writePBMFile(const string& outputFileName, const vector< vector<bool> >& bitmap);

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
//  return:     VOID
//
void readPBMFile(const string& filename, vector< vector<bool> >& bitmap);

#endif /* pbm_io_hpp */
