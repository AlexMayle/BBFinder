//
//  main.cpp
//  ocr
//
//  Created by Alex Mayle on 12/2/16.
//  Copyright © 2016 Alex Mayle. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>

#include "bounding_box_finder.hpp"

using namespace std;

int checkCount = 0;
static string OUTPUT_FILE_SUFFIX = ".new";

void printUsage() {
    cout << "Please provide a filepath to an ASCII PBM file.\n";
}

int main(int argc, const char * argv[]) {
    vector< vector<bool> > bitmap;

    //  Get input
    if (argc > 1) readPBMFile(argv[1], bitmap);
    else {
        printUsage();
        exit(-1);
    }
    
    //  Find boxes
    Partition partition(Coordinate(0,0), Coordinate(bitmap[0].size(), bitmap.size()));
    vector<BoundingBox> * boxes = findBoxesInPartition(bitmap, &partition);
    
    //  Output boxes
    for (auto box : *boxes) {
        box.printToBitmap(bitmap);
        box.print(cout);
        cout << ',';
    }
    delete boxes;
    
    //  Print output ASCII PBM file
    string outputFileName(argv[1]);
    outputFileName.append(OUTPUT_FILE_SUFFIX);
    writePBMFile(outputFileName, bitmap);
    
    //  Print performance statistics
    cout << endl << "Comparisons: " << checkCount + 2 << " Pixels: "
         << bitmap[0].size() * bitmap.size() << " Speedup: "
         << bitmap[0].size() * bitmap.size() / (double) checkCount << endl;
}
