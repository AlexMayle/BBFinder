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

#include "Coordinate.hpp"
#include "BoundingBox.hpp"
#include "Partition.hpp"
using namespace std;

int checkCount = 0;
vector< vector<bool> > bitmap;
static unsigned int numOfThreads;
static unsigned int BOX_VOLUME_THRESHOLD = 100;
const char * OUTPUT_FILE_SUFFIX = ".new";

void printHello() {
    cout << "IMPORTANT: There are binary pbm files (start with P4) and \n"
         << "ASCII pbm files (start with P1). Reading in binary proved difficult and I must study for finals\n"
         << "so I went with the ASCII. To facilitate grading I've included\n"
         << "several ASCII pbm files from small ones to large complicated images.\n"
         << "Some of the included images are small, so you may want to turn down the\n"
         << "box volume threshold by providing a third argument. It defaults to 100\n"
         << "Output file can be found in the same folder as the input\n "
         << "Other than that, everything, including the pthreads is working well.\n";
}

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
        size_t yPartitions = 2;
        int counter = threadExponant - 1;
        while (counter != 0) {
            yPartitions = yPartitions * 2;
            --counter;
        }
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
        cout << "This is not a PBMA file. It is prbably the binary version.\n"
             << "Use a provided ASCII pbm files instead. Aborting... \n";
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
}

void printUsage(ostream& out) {
    out << "Usage: filename and number of threads required in that order.\n";
}

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
int belongsToBox(const vector<BoundingBox>& boxes, int x, int y) {
    for (BoundingBox box : boxes) {
        if (box.contains(x, y)) {
            return box.max().x();
        }
    }
    return -1;
}

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
bool findBlackPixel(const vector< vector<bool> >& bitmap,
                    const Coordinate& startingPoint,
                    Coordinate * result,
                    const vector<BoundingBox>& boxes,
                    const Partition * const partition) {
    
    int x = startingPoint.x();
    for (int y = startingPoint.y(); y < partition->max().y(); ++y) {
        for (x; x < partition->max().x(); ++x) {
            ++checkCount;
            if (bitmap[y][x]) {
                int nextXCoordinate = belongsToBox(boxes, x, y);
                
                if (nextXCoordinate == -1) {
                    *result = Coordinate(x, y);
                    return true;
                } else {
                    x = nextXCoordinate + 3;
                }
            }
        }
        x = partition->min().x();
    }
    return false;
}

//  Grows a BoundingBox object starting from blackPixelLocation to its
//  maximum size within partition on the 2D array bitmap. The resulting
//  BoundingBox is assigned to the result argument
//
//  bitmap:                 2D array representing the image
//  blackPixelLocation:     start growing the box from this pixel
//  result:                 The resulting BoundingBox object
//  partition:              Partition object representing a section of the image
//
void makeBox(const vector< vector<bool> >& bitmap,
             const Partition * const partition,
             const Coordinate& startingPoint,
             BoundingBox * result) {
    BoundingBox boundingBox = BoundingBox(startingPoint, startingPoint);
    Coordinate pixel = boundingBox.min();
    
    do {
        while (!boundingBox.quickExpand(bitmap, partition, pixel));
    } while (!boundingBox.checkPerimeter(bitmap, pixel, partition));
    
    *result = boundingBox;
}

//  Finds all BoundingBox objects within a partition and returns a pointer
//  to a vector containing such boxes
//
//  partitionPtr:   A pointer to the partition to search within
//
//  returns:        A pointer to a vector of BoundingBox objects
//
void* findBoxesInPartition(void * partitionPtr) {
    const Partition * const partition = static_cast<Partition*>(partitionPtr);
    vector<BoundingBox> * boundingBoxes = new vector<BoundingBox>;
    Coordinate startLookingHere = partition->min();
    Coordinate blackPixelLocation;
    BoundingBox currentBox;
    
    while (findBlackPixel(bitmap, startLookingHere, &blackPixelLocation,
                          *boundingBoxes, partition)) {
        makeBox(bitmap, partition, blackPixelLocation, &currentBox);
        boundingBoxes->push_back(currentBox);
        
        if (currentBox.max().x() + 1 < partition->max().x()) {
            startLookingHere.setX(currentBox.max().x() + 1);
            startLookingHere.setY(currentBox.min().y());
        } else {
            startLookingHere.setX(partition->min().x());
            startLookingHere.setY(blackPixelLocation.y());
        }
    }
    return (void*) boundingBoxes;
}

int main(int argc, const char * argv[]) {
    printHello();
    
    if (argc > 1) readInput(argv[1], bitmap);
    else {
        printUsage(cout);
        exit(-1);
    }
    if (argc > 2) {
        const char * tmp;
        tmp = argv[2];
        stringstream ss(tmp);
        ss >> numOfThreads;
    } else {
        printUsage(cout);
        exit(-1);
    }
    if (argc > 3) {
        {
            const char * tmp;
            tmp = argv[3];
            stringstream ss(tmp);
            ss >> BOX_VOLUME_THRESHOLD;
        }
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
    
    if (numOfThreads != 0) {
//        Coordinate tmp(0,0);
//        Coordinate tmp2(0,0);
//        Coordinate tmp3(0,0);
//        for (auto partition : boundingBoxes) {
//            for (auto it = partition->begin(); it != partition->end(); ++it) {
//                while (1) {
//                    tmp3 = it->checkPerimeter(bitmap, tmp2, tmp, Coordinate(bitmap[0].size(), bitmap.size()));
//                    if (tmp3 == tmp2) break;
//                    else tmp2 = tmp3;
//                }
//            }
//        }
//        
//        bool done;
//        while (1) {
//            done = true;
//            for (auto partition : boundingBoxes) {
//                for (auto box : *partition) {
//                    for (auto partition2 : boundingBoxes) {
//                        for (auto it = partition2->begin(); it != partition2->end(); ++it){
//                            if (!(box == *it)) {
//                                if (box.contains(*it)) {
//                                    box.merge(*it);
//                                    partition2->erase(it);
//                                    done = false;
//                                    break;
//                                }
//                            }
//                        }
//                        if (!done) break;
//                    }
//                    if (!done) break;
//                }
//                if (!done) break;
//            }
//            if (done) break;
//        }
    }
    
    cout << endl << "Box coordinates: ";
    for (auto partition : boundingBoxes) {
        for (auto box : *partition) {
            Coordinate difference = box.min() - box.max();
            if (difference.x() * difference.y() >= BOX_VOLUME_THRESHOLD) {
                cout << '(';
                box.min().print();
                cout << ", ";
                box.max().print();
                cout << "), ";
                box.printToBitmap(bitmap);
            }
        }
        delete partition;
    }
    
    //cout << endl << "Comparisons: " << checkCount + 2 << " Pixels: " << bitmap[0].size() * bitmap.size() << " Speedup: " << bitmap[0].size() * bitmap.size() / (float)checkCount << endl;
    
    char * outputFileName = (char *) argv[1];
    strcat(outputFileName, OUTPUT_FILE_SUFFIX);
    ofstream out(outputFileName);
    printPbm(out, bitmap);
    
}
