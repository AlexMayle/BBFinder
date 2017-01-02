//
//  pbm_io.cpp
//  ocra
//
//  Created by Alex Mayle on 1/1/17.
//  Copyright © 2017 Alex Mayle. All rights reserved.
//

#include "pbm_io.hpp"

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
void writePBMFile(const string& outputFileName, const vector< vector<bool> >& bitmap){
    ofstream out(outputFileName);
    
    out << "P1\n";
    out << bitmap[0].size() << " " << bitmap.size() << endl;
    for (vector<bool> row : bitmap) {
        for (bool column : row) {
            if (column)
                out << "1 ";
            else
                out << "0 ";
        }
        out << endl;
    }
    out.close();
}

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
void readPBMFile(const string& filename, vector< vector<bool> >& bitmap) {
    //  Open file
    ifstream in(filename);
    if (!in.good()) {
        cout << "Error opening file, Error code: " << strerror(errno) << endl
        << "Aborting. . .\n";
        exit(-1);
    }
    
    //  Read PBM header
    string tokenBuf;
    in >> tokenBuf;
    if (tokenBuf != "P1") {
        cout << "This is not a PBMA file. It is prbably the binary version.\n"
        << "Aborting... \n";
        exit(-1);
    }
    int width, height;
    in >> width;
    in >> height;
    
    //  Read PBM data
    bitmap.resize(height);
    for (int i = 0; i < height; ++i) {
        bitmap[i].resize(width);
        for (int j = 0; j < width; ++j) {
            char buf;
            do {
                buf = in.get();
            } while (buf == ' ' || buf == '\n');
            bitmap[i][j] = (buf == '1') ? true : false;
        }
    }
}
