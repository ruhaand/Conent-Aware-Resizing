// Project UID af1f95f547e44c8ea88730dfb185559d

#include "Matrix.h"
#include "Matrix_test_helpers.h"
#include "unit_test_framework.h"

using namespace std;


// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----

// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test_fill_basic) {
  Matrix *mat = new Matrix; // create a Matrix in dynamic memory

  const int width = 3;
  const int height = 5;
  const int value = 42;
  Matrix_init(mat, 3, 5);
  Matrix_fill(mat, value);

  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(mat, r, c), value);
    }
  }

  delete mat; // delete the Matrix
}



// ADD YOUR TESTS HERE
// You are encouraged to use any functions from Matrix_test_helpers.h as needed.

//tests matrix_height and matrix_width
TEST(test_matrix_dimensions) {
  Matrix *mat = new Matrix;
  Matrix_init(mat, 5, 10);

  ASSERT_EQUAL(mat->width, 5);
  ASSERT_EQUAL(mat->height, 10);

  delete mat;

}

//tests fill border
//tests fill
//tests max
//tests row and column
//tests at
TEST(test_matrix_basic) {
  Matrix *mat = new Matrix;
  Matrix_init(mat, 5, 5);

  ASSERT_EQUAL(Matrix_width(mat), 5);
  ASSERT_EQUAL(Matrix_height(mat), 5);

  Matrix_fill(mat, 0);

  int *ptr = Matrix_at(mat, 2, 3);
  ASSERT_EQUAL(Matrix_row(mat, ptr), 2);
  ASSERT_EQUAL(Matrix_column(mat, ptr), 3);
  ASSERT_EQUAL(*ptr, 0);
  *ptr = 42;

  const int *cptr = Matrix_at(mat, 2, 3);
  ASSERT_EQUAL(*cptr, 42);

  Matrix_fill_border(mat, 2);
  ASSERT_EQUAL(*Matrix_at(mat, 0, 0), 2);

  ASSERT_EQUAL(Matrix_max(mat), 42);

  delete mat;
}


//tests print
TEST(test_matrix_print) {
  Matrix *mat = new Matrix;
  Matrix_init(mat, 1, 1);

  *Matrix_at(mat, 0, 0) = 42;
  ostringstream expected;
  expected << "1 1\n"
           << "42 \n";
  ostringstream actual;
  Matrix_print(mat, actual);
  ASSERT_EQUAL(expected.str(), actual.str());

  delete mat;
}


TEST(test_fill_border){
  Matrix *mat = new Matrix;
  Matrix_init(mat, 6,7);

  Matrix_fill(mat, 10);
  Matrix_fill_border(mat, -1);

  //test fill_border
  ASSERT_EQUAL(*Matrix_at(mat,0, 5), -1);
  ASSERT_EQUAL(*Matrix_at(mat,6,0), -1);

  delete mat;
}
//tests column of min value
//tests min value
TEST(test_matrix_rowValues){
  Matrix *mat = new Matrix;
  Matrix_init(mat, 6,7);

  Matrix_fill(mat, 10);
  Matrix_fill_border(mat, -1);

  //all the following tests are edge cases

  //testing row 0, min in first index
  ASSERT_EQUAL(Matrix_min_value_in_row(mat, 0, 0, 5), -1);

  //testing last row, min in final index
  ASSERT_EQUAL(Matrix_min_value_in_row(mat, 5, 1, 5), 10);

  //testing middle row, min in middle index--all same
  ASSERT_EQUAL(Matrix_min_value_in_row(mat, 3, 1, 4), 10);

  

  //testing leftmost minimum
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(mat, 0, 0, 5), 0);

  //testing minimum at last index
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(mat, 4, 1, 5),1);


  //testing min in middle of row, all same value
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(mat, 3, 2, 4), 2);

  delete mat;


}

// NOTE: The unit test framework tutorial in Lab 2 originally
// had a semicolon after TEST_MAIN(). Although including and
// excluding the semicolon are both correct according to the c++
// standard, the version of g++ we are using reports a warning
// when there is a semicolon. Therefore do NOT add a semicolon
// after TEST_MAIN()
TEST_MAIN() // Do NOT put a semicolon here
