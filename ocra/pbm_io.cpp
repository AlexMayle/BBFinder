//
//  pbm_io.cpp
//  Created by Alex Mayle on 1/1/17.
//

#include "pbm_io.hpp"
#include <iostream>
#include <fstream>

void pbmIO::writePBMFile(const std::string& outputFileName,
                  const std::vector<std::vector<bool> >& bitmap){
    std::ofstream out(outputFileName);
    
    out << "P1\n";
    out << bitmap[0].size() << " " << bitmap.size() << std::endl;
    for (std::vector<bool> row : bitmap) {
        for (bool column : row) {
            if (column)
                out << "1 ";
            else
                out << "0 ";
        }
        out << std::endl;
    }
    out.close();
}

void pbmIO::readPBMFile(const std::string& filename,
                 std::vector<std::vector<bool> >& bitmap) {
    //  Open file
    std::ifstream in(filename);
    if (!in.good()) {
        std::cout << "Error opening file, Error code: " << strerror(errno) << std::endl
        << "Aborting. . .\n";
        exit(-1);
    }
    
    //  Read PBM header
    std::string tokenBuf;
    in >> tokenBuf;
    if (tokenBuf != "P1") {
        std::cout << "This is not a PBMA file. It is prbably the binary version.\n"
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
