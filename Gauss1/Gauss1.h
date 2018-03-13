#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <chrono>
#include <omp.h>


using namespace std;
using namespace cv;

#ifndef GAUSS1_H
#define GAUSS1_H

class Gauss1 {
public:
	string imageinputPath;
	string imageOutputPath;
	int threadCount;
	int imageRows=0;
	int imageCols=0;
	double **matrix;

	int gaussFilterSize;

	void compGaussianMatrix(int size, double sigma);
	int loadParams(int argc, char** argv);
	Mat * performGauss(Mat &inputImage);
	Mat * loadImage();
};
#endif