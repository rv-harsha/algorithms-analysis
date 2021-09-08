#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <numeric>
#include <chrono>
#include <iostream>

using namespace cv;
using namespace std;


void edgeDetection(Mat & image, vector<int> & xKernel, vector<int> & yKernel)
{
  using chrono::duration_cast;
  using chrono::nanoseconds;
  typedef chrono::high_resolution_clock clock;
  Mat tempImage;

  if (image.channels() != 1)
  {
    cvtColor(image, tempImage, COLOR_BGR2GRAY);

    tempImage.copyTo(image);
  }
  else
  {
    image.copyTo(tempImage);
  }

  int kernelSize = sqrt (xKernel.size());

  vector<int> values;

  int halfSize{ kernelSize / 2 };

  auto start = clock::now();

  for (int i{ halfSize }; i < tempImage.rows - halfSize; i++)
  {
    auto startin = clock::now();
    for (int j{ halfSize }; j < tempImage.cols - halfSize; j++)
    {
      values.clear();
      for (int x = { -halfSize }; x <= halfSize; x++)
      {
        for (int y = { -halfSize }; y <= halfSize; y++)
        {
          unsigned char * pixelValuePtr = tempImage.ptr(i + x) + (j + y);

          values.push_back(*pixelValuePtr);
        }
      }

      long gX = inner_product(begin(values), end(values), begin(xKernel), 0);

      long gY = inner_product(begin(values), end(values), begin(yKernel), 0);

      long newPixelValue = abs(gX) + abs(gY);

      unsigned char * pixelValuePtr = image.ptr(i) + j;

      *pixelValuePtr = newPixelValue > 255 ? 255 : newPixelValue < 0 ? 0 : newPixelValue;
    }
    auto endin = clock::now();
//    std::cout << "Outer loop: " << duration_cast<nanoseconds>(endin-startin).count() << "ns\n";
  }
  auto end = clock::now();
  std::cout << "Outer loop: " << duration_cast<nanoseconds>(end-start).count() << "ns\n";
}

void prewittEdgeDetection(Mat & image, const unsigned char threshold)
{
  vector<int> xKernel = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

  vector<int> yKernel = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };

  edgeDetection(image, xKernel, yKernel);

  image = image > threshold;
}

void sobelEdgeDetection(Mat & image, const unsigned char threshold)
{
  vector<int> xKernel = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };

  vector<int> yKernel = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

  edgeDetection(image, xKernel, yKernel);

  image = image > threshold;
}

int main(int argc, char** argv )
{
  using chrono::duration_cast;
  using chrono::nanoseconds;
  typedef chrono::high_resolution_clock clock;

  if ( argc != 3 )
  {
    printf("usage: DisplayImage.out <Image_Path> <threshold>\n");
    return -1;
  }
  Mat image;
  auto start = clock::now();
  image = imread( argv[1], 1 );
  auto end = clock::now();
  std::cout << duration_cast<nanoseconds>(end-start).count() << "ns\n";
  if ( !image.data )
  {
    printf("No image data \n");
    return -1;
  }
  namedWindow("Display Image", WINDOW_AUTOSIZE );
//  imshow("Display Image", image);
  start = clock::now();
  sobelEdgeDetection(image, atoi(argv[2]));
  end = clock::now();
  std::cout << duration_cast<nanoseconds>(end-start).count() << "ns\n";
  imshow("Display Image", image);
  waitKey(0);
  return 0;
}