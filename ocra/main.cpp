//
//  main.cpp
//  ocr
//
//  Created by Alex Mayle on 12/2/16.
//  Copyright © 2016 Alex Mayle. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <pthread.h>
#include <sys/types.h>
#include <math.h>

#include "Coordinate.hpp"
#include "BoundingBox.hpp"
using namespace std;

int checkCount = 0;
vector< vector<bool> > bitmap;
static unsigned int numOfThreads;
static const unsigned int BOX_VOLUME_THRESHOLD = 1;

vector<BoundingBox> * getPartitions(const vector< vector<bool> >& bitmap,
                                    int threadExponant) {
        vector<BoundingBox> * partitions = new vector<BoundingBox>;
    if (threadExponant == 0) {
        Coordinate min(0, 0);
        Coordinate max(bitmap[0].size(), bitmap.size());
        BoundingBox boundingBox(min, max);
        partitions->emplace_back(min, max);
        return partitions;
    } else {
        size_t xPartitions = 2;
        size_t yPartitions = pow(2, threadExponant -1);
        size_t partitionWidth = bitmap[0].size() / xPartitions;
        size_t partitionHeight = bitmap.size() / yPartitions;
        size_t croppedPartitionWidth = bitmap[0].size() % partitionWidth;
        size_t croppedPartitionHeight = bitmap.size() % partitionHeight;
        
        for (int i = 0; i < yPartitions - 1; ++i) {
            Coordinate min(0, partitionHeight * i);
            Coordinate max(partitionWidth, partitionHeight * (i + 1));
            partitions->emplace_back(min, max);
            min = Coordinate(partitionWidth, min.y());
            max = Coordinate(bitmap[0].size(), max.y());
            partitions->emplace_back(min, max);
        }
        //  Last two
        Coordinate min(0, partitionHeight * (yPartitions - 1));
        Coordinate max(partitionWidth, bitmap.size());
        partitions->emplace_back(min, max);
        min = Coordinate(partitionWidth, min.y());
        max = Coordinate(bitmap[0].size(), bitmap.size());
        partitions->emplace_back(min, max);
    }
    return partitions;
}

void printPbm(ofstream& out, vector< vector<bool> >& bitmap){
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
}

//
//  Print the usage
//
void printUsage(ostream& out);

std::string * getFileContents(const char * filename) {
    ifstream in(filename, ios::binary);
    string * fileContents = new string();
    if (in) {
        in.seekg(0, std::ios::end);
        fileContents->resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&(*fileContents)[0], fileContents->size());
        in.close();
        return fileContents;
    } else {
        cout << "Cannot open file. Aborting. . .\n";
        exit(-1);
    }
}

void readInput(const char * filename, vector< vector<bool> >& bitmap) {
    ifstream in(filename);
    if (!in.good()) {
        cout << "Error opening file: " << strerror(errno) << endl;
        exit(-1);
    }
    string tokenBuf;
    string garbageBuf;
    
    in >> tokenBuf;
    if (tokenBuf != "P1") {
        cout << "This is not a PBMA file. Aborting... \n";
        exit(-1);
    }
    int width, height;
    in >> width;
    in >> height;
    
    //bool boolBuf;
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
    cout << "Input received.\n";
    for (auto row : bitmap) {
        for (auto pixel : row) {
            cout << pixel;
        }
        cout << endl;
    }
    cout << endl;
}

void printUsage(ostream& out) {
    out << "Usage: filename and number of threads required in that order.\n";
}

//  Places the first black pixel it finds, excluding those contained in
//  any existing bounding boxes, into result. The algorithm starts looking
//  at startingPoint and checks the region within bitmap bound by partitionMin
//  and partitionMax.
//
//  bitmap:         2D array representing the image
//  startingPoint:  First spot in bitmap checked
//  result:         Coordinate of the first black pixel found
//  boxes:          Existing bounding boxes in the bitmap
//  partitionMin:   Top left corner of partition within bitmap
//  partitionMax:   Bottom right corner of partition within bitmap
//
//  Returns:        True if black pixel is found, false otherwise
//
bool findBlackPixel(vector< vector<bool> >& bitmap,
                    Coordinate& startingPoint,
                    Coordinate * result,
                    vector<BoundingBox>& boxes,
                    Coordinate& partitionMin,
                    Coordinate& partitionMax) {
    
    int x = startingPoint.x();
    for (int y = startingPoint.y(); y < partitionMax.y(); ++y) {
        while (x < partitionMax.x()) {
            ++checkCount;
            if (bitmap[y][x]) {
                bool belongsToExistingBox = false;
                for (BoundingBox box : boxes) {
                    if (box.contains(Coordinate(x, y))) {
                        belongsToExistingBox = true;
                        x = box.max().x();
                        break;
                    }
                }
                if (!belongsToExistingBox) {
                    *result = Coordinate(x, y);
                    return true;
                }
            }
            ++x;
        }
        x = partitionMin.x();
    }
    return false;
}

void makeBox(vector< vector<bool> >& bitmap, const Coordinate& blackPixelLocation,
             BoundingBox * result, const Coordinate& partitionMin, const Coordinate& partitionMax) {
    Coordinate newStartingPoint = blackPixelLocation;
    BoundingBox boundingBox = BoundingBox(newStartingPoint, newStartingPoint);
    Coordinate neighbor;
    Coordinate cursor;
    do {
        //boundingBox.expandBoundaries(newStartingPoint);
        cursor = newStartingPoint;
        while (1) {
            neighbor = cursor.getCardinalNeighbor(bitmap, partitionMin, partitionMax,
                                                  boundingBox);
            if (neighbor == cursor) break;
            boundingBox.expandBoundaries(neighbor);
            cursor = neighbor;
        }
        newStartingPoint = boundingBox.checkPerimeter(bitmap, cursor,
                                                      partitionMin,
                                                      partitionMax);
    } while (cursor != newStartingPoint);
    
    *result = boundingBox;
}

void* findBoxesInPartition(void * partitionPtr) {
    BoundingBox * partition = static_cast<BoundingBox*>(partitionPtr);
    Coordinate partitionMin = partition->min();
    Coordinate partitionMax = partition->max();
    vector<BoundingBox> * boundingBoxes = new vector<BoundingBox>;
    Coordinate startLookingHere, blackPixelLocation;
    BoundingBox box;
    while (findBlackPixel(bitmap, startLookingHere, &blackPixelLocation,
                          *boundingBoxes, partitionMin, partitionMax))
    {
        makeBox(bitmap, blackPixelLocation, &box, partitionMin, partitionMax);
        boundingBoxes->push_back(box);
        if (box.max().x() + 1 < bitmap[0].size()) {
            startLookingHere.setX(box.max().x() + 1);
            startLookingHere.setY(box.min().y());
        } else {
            startLookingHere.setX(0);
            startLookingHere.setY(startLookingHere.y() + 1);
        }
    }
    return (void*) boundingBoxes;
}

int main(int argc, const char * argv[]) {
    numOfThreads = 1;
    
    if (argc > 1) readInput(argv[1], bitmap);
    else {
        printUsage(cout);
        exit(-1);
    }
    
    vector<BoundingBox> * partitionBounds = getPartitions(bitmap, numOfThreads);
    vector<pthread_t> threads;
    threads.resize(partitionBounds->size());
    
    for (int i = 0; i < threads.size(); ++i) {
        pthread_create(&threads[i], NULL, findBoxesInPartition, &(*partitionBounds)[i]);
    }
    
    vector< vector<BoundingBox>* > boundingBoxes;
    for (int i = 0; i < threads.size(); ++i) {
        void ** ptr = new void *;
        int rc = pthread_join(threads[i], ptr);
        vector<BoundingBox> * boxes = static_cast<vector<BoundingBox>*>(*ptr);
        boundingBoxes.push_back(boxes);
        delete ptr;
    }
    delete partitionBounds;
    
    for (auto partition : boundingBoxes) {
        for (auto box : *partition) {
            cout << '(';
            box.min().print();
            cout << ", ";
            box.max().print();
            cout << "), ";
            box.printToBitmap(bitmap);
        }
        delete partition;
    }
    
    cout << endl << "Comparisons: " << checkCount + 2 << " Pixels: " << bitmap[0].size() * bitmap.size() << " Speedup: " << bitmap[0].size() * bitmap.size() / (float)checkCount << endl;
    
    ofstream out("output.pbm");
    printPbm(out, bitmap);
}
