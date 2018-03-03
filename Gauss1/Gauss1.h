#pragma once

#include <tchar.h>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>


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

	void compGaussianMatrix(int size, double sigma);
	void loadParams(int argc, char** argv);
	Mat * performGauss(Mat &inputImage);
	Mat * loadImage();
	//int main(int argc, char** argv);
};
#endif