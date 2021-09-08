#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <numeric>
#include <chrono>
#include <vector>
#include <iostream>

using namespace cv;


void adaptiveThresholdingSegmentation(Mat & image, const int kernelSize)
{
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

  int totalKernelElements = kernelSize * kernelSize;

  std::vector<long> kernel(totalKernelElements, 1.0 / totalKernelElements);

  std::vector<long> values;

  int halfSize{ kernelSize / 2 };

  for (int i{ halfSize }; i < tempImage.rows - halfSize; i++)
  {
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

      long averageValue = inner_product(begin(values), end(values), begin(kernel), 0.0);

      unsigned char * pixelValuePtr = image.ptr(i) + j;

      *pixelValuePtr = *pixelValuePtr > averageValue ? 0 : 255;
    }
  }
}

int main(int argc, char** argv )
{
  using std::chrono::duration_cast;
  using std::chrono::nanoseconds;
  typedef std::chrono::high_resolution_clock clock;

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
  adaptiveThresholdingSegmentation(image, atoi(argv[2]));
  end = clock::now();
  std::cout << duration_cast<nanoseconds>(end-start).count() << "ns\n";
  imshow("Display Image", image);
  waitKey(0);
  return 0;
}