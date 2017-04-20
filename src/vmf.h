#pragma once

#include <opencv2/core/core.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

namespace vmf {

	void setPixel(unsigned char* p, int* pixel, int c) {
		int* pp = pixel; //pixel pointer

		for (int i = 0; i < c; i++) {
			*p = *pp;
			pp++;
			p++;
		}
	}

	void findPixel(int* window, int* pixel, int index, int c) {
		int* pp = pixel; //pixel pointer

		for (int i = 0; i < c; i++) {
			*pp = window[index*c + i];
			pp++;
		}
	}

	int findIndexOfMinValue(double* sums, int n) {
		double *p = sums;
		double min = *p;
		int index = 0;
		p++;

		for (int i = 1; i < n; i++) {
			if (min > *p) {
				min = *p;
				index = i;
			}
			p++;
		}

		return index;
	}

	void sumMatrixRows(double* matrix, double* sums, int rows, int cols) {
		double *mp = matrix;
		double *sp = sums;

		for (int i = 0; i < rows; i++) {
			double sum = 0;
			for (int j = 0; j < cols; j++) {
				sum += *mp;
				mp++;
			}
			*sp = sum;
			sp++;
		}
	}

	void calcDistanceMatrixFromWindow(int* window, double* distance_matrix, int n, int c) {

		double *rp, *cp;
		rp = distance_matrix; //distance matrix rows pointer
		cp = distance_matrix; //distance matrix colums pointer
		int *wp; //window pointer
		wp = window;
		int start[3] = { 0 };

		for (int i = 0; i < n; i++) {
			rp = rp + i + 1;
			cp = cp + n * (i + 1);

			for (int k = 0; k < c; k++) {
				start[k] = *wp;
				*wp++;
			}

			for (int j = 0; j < n - i - 1; j++) {
				double sum = 0;
				for (int l = 0; l < c; l++) {
					sum += pow(start[l] - (*wp), 2);
					wp++;
				}
				double dist = sqrt(sum);
				*rp = dist;
				*cp = dist;

				rp++;
				cp += n;
			}
			wp = &window[(i + 1)*c];
			cp = &distance_matrix[i + 1];
		}

	}

	int calcPixelsWindowAndFilterPixels(unsigned char *p, int *window, int rows, int cols, int c, int n) {
		if (rows < n || cols < n || c > 3 || n < 1) {
			return -1;
		}
		int *wp; //window pointer
		wp = window;
		unsigned char *midp; //middle point of window
		midp = p;
		int window_size = n*n;

		double *distance_matrix = new double[window_size*window_size]();
		double *sums = new double[window_size];
		int pixel[3] = { 0 };

		for (int i = 0; i < rows - (n - 1); i++) {
			for (int j = 0; j < cols - (n - 1); j++) {
				for (int k = 0; k < n; k++) {
					for (int l = 0; l < n; l++) {
						if (k == n / 2 && k == l) {
							midp = p;
						}
						for (int m = 0; m < c; m++) {
							*wp = (int)*p;
							wp++;
							p++; //grayscale or rgb values
						}

						p = p - c; //return after above changes
						p = p + cols * c;  // rows of window
					}
					p = p - n * cols * c + c; // columns of window
				}
				calcDistanceMatrixFromWindow(window, distance_matrix, window_size, c);
				sumMatrixRows(distance_matrix, sums, window_size, window_size);
				int index = findIndexOfMinValue(sums, window_size);
				findPixel(window, pixel, index, c);
				setPixel(midp, pixel, c);

				wp = &window[0];
				p = p - (n - 1) * c; // columns of matrix
			}
			p = p + (n - 1) * c; // rows of matrix
		}

		delete[] sums;
		delete[] distance_matrix;
		return 1;
	}

	void vmfFilter(Mat& src, Mat& dest, int window_side) {

		int pixel_border = window_side / 2;

		copyMakeBorder(src, dest, pixel_border, pixel_border, 
			pixel_border, pixel_border, BORDER_REPLICATE);

		unsigned char *p = (unsigned char*)(dest.data);
		int c = dest.channels();
		int cols = dest.cols;
		int rows = dest.rows;

		int* window = new int[window_side*window_side*c];
		calcPixelsWindowAndFilterPixels(p, window, rows, cols, c, window_side);
		delete[] window;

		dest = dest(Rect(pixel_border, pixel_border,
			cols - 2 * pixel_border, rows - 2 * pixel_border));

	}

}