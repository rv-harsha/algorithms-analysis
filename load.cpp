#include <iostream>
#include <vector>
#include <mutex>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void load_images(int start,int end){
  vector<Mat>img_lst;
  mutex mtx;

#pragma omp parallel for
  for(int i=start;i<=end;i++){
    char filename[20];
    sprintf(filename,"Test/%03d_R.png", i);
    Mat img = imread(filename);
    if (img.empty()){
      cerr << "ERROR: Failed to load " << filename << endl;
    }
    mtx.lock();
    img_lst.push_back(img);
    mtx.unlock();
  }
  mtx.lock();
  cout << "INFO: Loaded " << img_lst.size() << endl;
  mtx.unlock();
}

int
main(int argc,char*argv[])
{
  int batch_size = 16;
  int num_pictures = 16;
  int batches = num_pictures/batch_size;
  for (int i = 0; i < batches; i++)
    load_images(i*batch_size+1,(i+1)*batch_size);
}