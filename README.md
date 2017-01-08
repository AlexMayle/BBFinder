# BBFinder - Bounding Box Finder

BBFinder is a library used to create bounding boxes around the objects in an ASCII .pbm file. (Not to be confused with normal .pbm files.) These functions are contained within `BBFinder.h`, the most general-use one being `findBoxesInPartition()`. `main.cpp` provides an example program that uses the library to take in a .pbm file and then render a new .pbm file with the bounding boxes drawn in. Example usage of thise program:
```sh
$ make
$ ./bounding_box_finder test_images/washington.pbm
```
The resulting file will be test_images/washington.pbm.new

### Object Definition
An object is defined as a collection of neighboring black pixels. (.pbm files are black & white) Neighboring pixels are defined, by default, as any two black pixels whose distance between their respective x and y coordinates are both less than 3 pixels. (i.e. |x1 - x2| <= 3 and |y1 - y2| <= 3) This number can be changed by a `#define` directive in `BoundingBox.h`.

### Features
  - ASCII .pbm file I/O through `pbm_io.h`.
  - Functions accept `Partition` objects to constrain the functions to operate only relevant portions of the input image.
  - Ability to change the distance that defines "neighboring" pixels from a `#define` directive in `BoundingBox.h`.
  - Functions available for controlling the order of boxes generated and the starting pixel from which they are generated (i.e. `getBoxSeed()`).

### Complexity
The complexity of this algorithm, in terms of pixels visited, is typically a third of the pixels in the image. However, with certain images, such as a black image containing a grid of white lines, the complexity will be no more than twice the number of pixels within the image. For more information regarding the underlying algorith see my [blog post](alexmayle.github.io/algorithms/2017/01/06/Bounding-Boxs.html).

### Documentation & Support
Extensive API documentation may be found within `BBFinder.h` and more information about bounding boxes, specifically the BoundingBox object, can be found in `BoundingBox.h`. 

Feel free to open an issue, or contact me at alexmayle@gmail.com

### Parallel branch
The parallel branch technically works when compiled with the `-pthread` gcc flag, although it really only exists because it was part of an assignment. In general, it is not a very good parallel algorithm. Specifically, the code is a wreck and it has never ran faster than the sequential version. 
