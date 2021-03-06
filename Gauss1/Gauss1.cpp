#pragma once

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "Gauss1.h"


int main(int argc, char** argv)
{
	Gauss1 gauss;
	if(gauss.loadParams(argc, argv)<0) return 0;
	Mat * inputImage = gauss.loadImage();
	if (inputImage == NULL) return 0;
	Mat * outputImage;
	gauss.imageCols = inputImage->cols;
	//cout << gauss.imageCols << " image width \n";
	gauss.imageRows = inputImage->rows;
	//cout << gauss.imageRows << " image height \n";

	gauss.compGaussianMatrix(5, 1.5);

	chrono::steady_clock::time_point start = chrono::high_resolution_clock::now();
	outputImage = gauss.performGauss(*inputImage);
	chrono::steady_clock::time_point stop = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = stop - start;
	cout << "Czas: " << elapsed.count();
	//ZMIENIC NA TO: 
	try {
		imwrite(gauss.imageOutputPath, *outputImage);
	}
	catch(exception e){
		cout << "Cant save image!";
		return 0;
	}
	
	//imwrite("new.jpeg", *outputImage);
	return 0;
};

//tworzy macierz rozmycia
void Gauss1::compGaussianMatrix(int size, double sigma) {
	gaussFilterSize = size;
	matrix = new double*[size];
	for (int i = 0; i < size; ++i) {
		matrix[i] = new double[size];
	}

	double result = 0.;
	int i, j;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			matrix[i][j] = exp(-(i*i + j * j) / (2 * sigma*sigma)) / (2 * M_PI *sigma*sigma);
			result += matrix[i][j];
		}
	}

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			matrix[i][j] = matrix[i][j] / result;
			//cout << matrix[i][j] << " ";
		}
		//cout << "\n";
	}
};

//zwraca macierz po obrobce
Mat* Gauss1::performGauss(Mat &inputImage) {
	static Mat newImageMat = Mat(this->imageRows - gaussFilterSize + 1, this->imageCols - gaussFilterSize + 1, inputImage.type());
	int newImageHeight = imageRows - gaussFilterSize;
	int newImageWidth = imageCols - gaussFilterSize;
	int i, j, h, w;

	omp_set_dynamic(0); //disable dinamic thread creation 
	omp_set_num_threads(threadCount); //set number of threads
#pragma omp parallel for private(i,j)
	for (i = 0; i < newImageWidth; i++) {
		for (j = 0; j < newImageHeight; j++) {
			for (h = i; h < i + gaussFilterSize; h++) {
				for (w = j; w < j + gaussFilterSize; w++) {
					Vec3b *color1 = &newImageMat.at<Vec3b>(Point(i, j));
					Vec3b *color2 = &inputImage.at<Vec3b>(Point(h, w));
					double matrixVal = matrix[h - i][w - j];
					color1->val[0] += matrixVal * color2->val[0];
					color1->val[1] += matrixVal * color2->val[1];
					color1->val[2] += matrixVal * color2->val[2];
					//cout << i << " i " << j << " j "<< h <<" h "<< w <<" w\n" ;
				}
			}
		}
	}
	return &newImageMat;
}

//wczytuje parametry z argv
int Gauss1::loadParams(int argc, char** argv) {
	threadCount;
	imageinputPath = "2.jpg";
	imageOutputPath = "out.jpg";

	if (argc == 1) {
		cout << "Nie podano ilosci watkow ani sciezek do pliku wejsciowego i wyjsciowego!";
		return -1;
	}
	
	string tc;
	tc = argv[1];
	try {
		threadCount = stoi(tc);
	}
	catch (exception &e) {
		cout << "Niepoprawna wartosc argumentu okreslajacego ilosc watkow!";
		return -1;
	}

	if (argc == 2) {
		cout << "Nie podano sciezek do pliku wejsciowego i wyjsciowego!";
		return -1;
	}
	else {
		imageinputPath = argv[2];
	}

	if (argc == 3) {
		cout << "Nie podano sciezki wyjsciowej do pliku!";
		return -1;
	}
	else {
		imageOutputPath = argv[3];
	}
	
	
	
};

//zwraca macierz obrazu
Mat * Gauss1::loadImage() {
	static Mat inputImage;
	if (imageinputPath.length() != 0) {
		inputImage = imread(imageinputPath, IMREAD_COLOR);
		if (inputImage.empty()) {
			cout << "Cant load file! \n";
			return NULL;
		}
	}
	else {
		cout << "error! Invalid image path!\n";
		return NULL;
	}
	return &inputImage;
}

