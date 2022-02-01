// Project UID af1f95f547e44c8ea88730dfb185559d

#include <cassert>
#include "processing.h"

using namespace std;

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_left is provided for you.
// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the left (counterclockwise).
void rotate_left(Image* img) {

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image *aux = new Image;
  Image_init(aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(aux, width - 1 - c, r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = *aux;
  delete aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_right is provided for you.
// REQUIRES: img points to a valid Image.
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the right (clockwise).
void rotate_right(Image* img){

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image *aux = new Image;
  Image_init(aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(aux, c, height - 1 - r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = *aux;
  delete aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// v DO NOT CHANGE v ------------------------------------------------
// The implementation of diff2 is provided for you.
static int squared_difference(Pixel p1, Pixel p2) {
  int dr = p2.r - p1.r;
  int dg = p2.g - p1.g;
  int db = p2.b - p1.b;
  // Divide by 100 is to avoid possible overflows
  // later on in the algorithm.
  return (dr*dr + dg*dg + db*db) / 100;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// ------------------------------------------------------------------
// You may change code below this line!



// REQUIRES: img points to a valid Image.
//           energy points to a Matrix.
// MODIFIES: *energy
// EFFECTS:  energy serves as an "output parameter".
//           The Matrix pointed to by energy is initialized to be the same
//           size as the given Image, and then the energy matrix for that
//           image is computed and written into it.
//           See the project spec for details on computing the energy matrix.
void compute_energy_matrix(const Image* img, Matrix* energy) {
  //initialize and fill matrix
  Matrix_init(energy, Image_width(img), Image_height(img));
  Matrix_fill(energy, 0);


  //compue energy for each non-border pixel
  //don't need to iterate through first and last rows since they are borders
  //don't need to iterate through first and last cols since they are borders
  for(int row = 1; row < (Image_height(img) - 1); row++){
    for(int col = 1; col < (Image_width(img) - 1); col++){
      //just need to access pixel above and below
      Pixel north = Image_get_pixel(img, row - 1, col);
      Pixel south = Image_get_pixel(img, row + 1, col);
      int sqDiffNS = squared_difference(north, south);

      Pixel east = Image_get_pixel(img, row, col - 1);
      Pixel west = Image_get_pixel(img, row, col + 1);
      int sqDiffEW = squared_difference(west, east);

      *Matrix_at(energy, row, col) = sqDiffNS + sqDiffEW;

    }
  }

  //find the maximum energy so fatr
  int max = Matrix_max(energy);

  //fill energy border with max Value
  Matrix_fill_border(energy, max);

}


// REQUIRES: energy points to a valid Matrix.
//           cost points to a Matrix.
//           energy and cost aren't pointing to the same Matrix
// MODIFIES: *cost
// EFFECTS:  cost serves as an "output parameter".
//           The Matrix pointed to by cost is initialized to be the same
//           size as the given energy Matrix, and then the cost matrix is
//           computed and written into it.
//           See the project spec for details on computing the cost matrix.
void compute_vertical_cost_matrix(const Matrix* energy, Matrix *cost) {
  //initialize cost matrix
  Matrix_init(cost, Matrix_height(energy), Matrix_width(energy));

  //fill first row with energy values
  for(int row = 0; row < 1 ; row++){
    for(int col = 1; col < Matrix_width(cost); col++){
      *Matrix_at(cost, row, col) = *Matrix_at(energy, row, col);
    }
  }

  //fill in all the other rows using cost equation
  for(int row = 1; row < Matrix_height(cost) ; row++){
    for(int col = 0; col < Matrix_width(cost); col++){
      //if index is first, look at the enegry above it and to the right
      if(col == 0){
          int energy = *Matrix_at(cost, row, col);
          int minEnergy = Matrix_min_value_in_row(cost, row, col, col + 1);
          *Matrix_at(cost, row, col) = energy + minEnergy;
      } else if(col == Matrix_width(cost)){
          int energy = *Matrix_at(cost, row, col);
          int minEnergy = Matrix_min_value_in_row(cost, row, col, col - 1);
          *Matrix_at(cost, row, col) = energy + minEnergy;
      } else {
          //now we know that indexs are not borders
          //need to use all three variables (N, NW, NE)
          int energy = *Matrix_at(cost, row, col);
          int minEnergy = Matrix_min_value_in_row(cost, row, col - 1, col + 1);
          *Matrix_at(cost, row, col) = energy + minEnergy;
      }
    }
  }
}


// REQUIRES: cost points to a valid Matrix
//           seam points to an array
//           the size of seam is >= Matrix_height(cost)
// MODIFIES: seam[0]...seam[Matrix_height(cost)-1]
// EFFECTS:  seam serves as an "output parameter".
//           The vertical seam with the minimal cost according to the given
//           cost matrix is found and the seam array is filled with the column
//           numbers for each pixel along the seam, with the pixel for each
//           row r placed at seam[r]. While determining the seam, if any pixels
//           tie for lowest cost, the leftmost one (i.e. with the lowest
//           column number) is used.
//           See the project spec for details on computing the minimal seam.
// NOTE:     You should compute the seam in reverse order, starting
//           with the bottom of the image and proceeding to the top,
//           as described in the project spec.
void find_minimal_vertical_seam(const Matrix* cost, int seam[]) {
  //starting at the bottom of the matrix
  int colStart = Matrix_height(cost) - 1;
  int minCol = Matrix_column_of_min_value_in_row(cost, Matrix_height(cost) - 1, colStart, 0);
  
  //start at second to last row
  //TAKE A SECOND LOOK AT THIS ONE--MAKE SURE THAT YOU DONT GET AN OUT OF BOUNDS ERROR--DRAW IT OUT
  for(int row = Matrix_height(cost) - 2; row >= 0; row++){
    //starting at the bottom of the matrix
    minCol = Matrix_column_of_min_value_in_row(cost, row, minCol - 1, minCol + 1);
    //this will access each index of seam array
    *(seam+row) = minCol;
  }
}


// REQUIRES: img points to a valid Image with width >= 2
//           seam points to an array
//           the size of seam is == Image_height(img)
//           each element x in seam satisfies 0 <= x < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Removes the given vertical seam from the Image. That is, one
//           pixel will be removed from every row in the image. The pixel
//           removed from row r will be the one with column equal to seam[r].
//           The width of the image will be one less than before.
//           See the project spec for details on removing a vertical seam.
// NOTE:     Use the new operator here to create the smaller Image,
//           and then use delete when you are done with it.
void remove_vertical_seam(Image *img, const int seam[]) {
  assert(false); // TODO Replace with your implementation!
}


// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width of the given Image to be newWidth by using
//           the seam carving algorithm. See the spec for details.
// NOTE:     Use the new operator here to create Matrix objects, and
//           then use delete when you are done with them.
void seam_carve_width(Image *img, int newWidth) {
  assert(false); // TODO Replace with your implementation!
}

// REQUIRES: img points to a valid Image
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the height of the given Image to be newHeight.
// NOTE:     This is equivalent to first rotating the Image 90 degrees left,
//           then applying seam_carve_width(img, newHeight), then rotating
//           90 degrees right.
void seam_carve_height(Image *img, int newHeight) {
  assert(false); // TODO Replace with your implementation!
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width and height of the given Image to be newWidth
//           and newHeight, respectively.
// NOTE:     This is equivalent to applying seam_carve_width(img, newWidth)
//           and then applying seam_carve_height(img, newHeight).
void seam_carve(Image *img, int newWidth, int newHeight) {
  assert(false); // TODO Replace with your implementation!
}
