#pragma once

#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

namespace vmf {

	int calcPixelsWindow(unsigned char *p, unsigned int *window, int rows, int cols, int c, int n) {
		if (rows < n || cols < n || n < 1) {
			return -1;
		}
		unsigned int *wp; //window pointer
		wp = window;

		for (int i = 0; i < rows - (n - 1); i++) {
			for (int j = 0; j < cols - (n - 1); j++) {
				for (int k = 0; k < n; k++) {
					for (int l = 0; l < n; l++) {
						for (int m = 0; m < c; m++) {
							//cout << (int)*p << " ";
							*wp = (int)*p;
							wp++;
							p++; //grayscale or rgb values
						}
						p = p - c; //return after above changes
						p = p + cols * c;  // rows of window
					}
					p = p - n * cols * c + c; // columns of window
				}
				wp = &window[0];
				p = p - (n - 1) * c; // columns of matrix
			}
			p = p + (n - 1) * c; // rows of matrix
		}
		return 1;
	}

	void createPixelsWindow(Mat image, unsigned int size) {

		unsigned char *p = (unsigned char*)(image.data);
		const int c = image.channels();
		const int n = size;
		unsigned int* window = new unsigned int[n*n*c];
		calcPixelsWindow(p, window, image.rows, image.cols, c, n);
		for (int a = 0; a < n*n*c; a++) {
			cout << window[a] << " ";
		}
		delete[] window;

	}

	int vmfFilter(Mat image) {
		return -1;
	}

}