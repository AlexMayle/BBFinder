bounding_box_finder : BBFinder.o Partition.o BoundingBox.o Coordinate.o pbm_io.o main.o
	g++ -std=c++11 -o bounding_box_finder BBFinder.o Partition.o BoundingBox.o Coordinate.o pbm_io.o main.o
BBFinder.o : Partition.o BoundingBox.o Coordinate.o
	g++ -c -std=c++11 -o BBFinder.o BBFinder.cpp 
Partition.o : Partition.cpp
	g++ -c -std=c++11 -o Partition.o Partition.cpp
BoundingBox.o : BoundingBox.cpp
	g++ -c -std=c++11 -o BoundingBox.o BoundingBox.cpp
Coordinate.o : Coordinate.cpp
	g++ -c -std=c++11 -o Coordinate.o Coordinate.cpp
pbm_io.o : pbm_io.cpp
	g++ -c -std=c++11 -o pbm_io.o pbm_io.cpp
main.o : main.cpp
	g++ -c -std=c++11 -o main.o main.cpp
clean :
	rm bounding_box_finder BBFinder.o Partition.o BoundingBox.o Coordinate.o pbm_io.o main.o
