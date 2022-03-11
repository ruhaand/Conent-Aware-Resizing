// resize.cpp
// Project UID af1f95f547e44c8ea88730dfb185559d
//new commit
#include <iostream>
#include <fstream>
#include "processing.h"
using namespace std;

//look at exxample to see how this is formatted 
int main(int argc, char* argv[]) {

//Checking correct argument number
if (argc != 4 && argc != 5) {
    cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
     << "WIDTH and HEIGHT must be less than or equal to original" << endl;
     return 1;
}

int newWidth = atoi(argv[3]);
int newHeight = 0;


ifstream inFile;
inFile.open(argv[1]);

if (!inFile.is_open()) {
    cout << "Error opening file: " << argv[1] << endl;
    return 1;
}

Image *Img = new Image;
Image_init(Img, inFile);

if (argc == 5) {
    newHeight = atoi(argv[4]);
}
else {
    newHeight = Image_height(Img);
}

//checking proper width and height;
if (newWidth <= 0 || newWidth > Image_width(Img)) {
    cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
     << "WIDTH and HEIGHT must be less than or equal to original" << endl;
     return 1;
}

if (newHeight <= 0 || newHeight > Image_height(Img)) {
    cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
     << "WIDTH and HEIGHT must be less than or equal to original" << endl;
     return 1;
}



seam_carve_width(Img, newWidth);
seam_carve_height(Img, newHeight);


ofstream outFile;

outFile.open(argv[2]);
if (!outFile.is_open()) {
    cout << "Error opening file: " << argv[2] << endl;
    return 1;
}

Image_print(Img, outFile);

delete Img;


return 0;
}