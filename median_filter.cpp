#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <numeric>
#include <chrono>
#include <vector>
#include <iostream>

using namespace cv;


void medianFilter(Mat & image, const int kernelSize)
{
  Mat tempImage;

  image.copyTo(tempImage);

  int imageChannels = image.channels();

  std::vector<std::vector<int>> values (imageChannels);

  int halfSize{ kernelSize / 2 };

  int pos = { kernelSize * kernelSize / 2 };

  for (int i{ halfSize }; i < tempImage.rows - halfSize; i++)
  {
    for (int j{ halfSize }; j < tempImage.cols - halfSize; j++)
    {
      for (int channel = 0; channel < imageChannels; channel++)
      {
        values[channel].clear();
      }

      for (int x = { -halfSize }; x <= halfSize; x++)
      {
        for (int y = { -halfSize }; y <= halfSize; y++)
        {
          for (int channel = 0; channel < imageChannels; channel++)
          {
            unsigned char * pixelValuePtr = tempImage.ptr(i + x) + ((j + y) * imageChannels) + channel;

            values[channel].push_back(*pixelValuePtr);
          }
        }
      }

      for (int channel = 0; channel < imageChannels; channel++)
      {
        sort(begin(values[channel]), end(values[channel]));

        unsigned char * pixelValuePtr = image.ptr(i) + (j * imageChannels) + channel;

        *pixelValuePtr = values[channel][pos];
      }
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
    printf("usage: DisplayImage.out <Image_Path> <matrix_size>\n");
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
  medianFilter(image, atoi(argv[2]));
  end = clock::now();
  std::cout << duration_cast<nanoseconds>(end-start).count() << "ns\n";
  imshow("Display Image", image);
  waitKey(0);
  return 0;
}