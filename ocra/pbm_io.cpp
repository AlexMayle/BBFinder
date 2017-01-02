//
//  pbm_io.cpp
//  Created by Alex Mayle on 1/1/17.
//

#include "pbm_io.hpp"

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
