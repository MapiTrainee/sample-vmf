#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../src/vmf.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestSampleVMF
{
	TEST_CLASS(UnitTestSampleVMF)
	{
	public:

		TEST_METHOD(TestFilterVMF_TestImage_ExpectedPixels)
		{
			string image_name = "..\\..\\img\\test.png";
			Mat image, gray_image;

			image = imread(image_name, IMREAD_COLOR);
			cvtColor(image, gray_image, CV_BGR2GRAY);

			int window_side = 3;
			Mat filtered_image,
				filtered_gray_image;

			Mat expected_image = Mat::zeros(32, 32, COLOR_BGR5552RGB),
				expected_gray_image = Mat::zeros(32, 32, CV_BGR2GRAY);

			vmf::vmfFilter(image, filtered_image, window_side);
			vmf::vmfFilter(gray_image, filtered_gray_image, window_side);

			Assert::AreEqual(expected_gray_image.cols, filtered_gray_image.cols);
			Assert::AreEqual(expected_gray_image.rows, filtered_gray_image.rows);

			Assert::AreEqual(expected_image.cols, filtered_image.cols);
			Assert::AreEqual(expected_image.rows, filtered_image.rows);

			unsigned char *p1 = (unsigned char*)(filtered_gray_image.data);
			unsigned char *p2 = (unsigned char*)(expected_gray_image.data);
			unsigned char *p3 = (unsigned char*)(filtered_image.data);
			unsigned char *p4 = (unsigned char*)(expected_image.data);

			for (int i = 0; i < filtered_gray_image.rows * filtered_gray_image.cols; i++) {
				Assert::AreEqual(*p2, *p1);
				p1++; p2++;
			}

			for (int i = 0; i < filtered_image.rows * filtered_image.cols * 3; i++) {
				Assert::AreEqual(*p4, *p3);
				p3++; p4++;
			}

		}

		TEST_METHOD(TestSetPixel_GrayPixel_ExpectedArray)
		{
			const int n = 3;
			const int c = 1;
			int pixel[c] = { 0 };
			unsigned char actual_array[n*n*c] = {
				1, 2, 3, 4, 5, 6, 7, 8, 9
			};
			unsigned char *p = actual_array;
			unsigned char expected_array[n*n*c] = {
				0, 2, 0, 4, 5, 6, 7, 8, 9
			};

			vmf::setPixel(p, pixel, c);
			p += 2 * c;
			vmf::setPixel(p, pixel, c);
			for (int i = 0; i < c; i++) {
				Assert::AreEqual(expected_array[i], actual_array[i]);
			}
		}

		TEST_METHOD(TestSetPixel_RgbPixel_ExpectedArray)
		{
			const int n = 2;
			const int c = 3;
			int pixel[c] = { 1, 2, 3 };
			unsigned char actual_array[n*n*c] = { 0 };
			unsigned char *p = actual_array;
			unsigned char expected_array[n*n*c] = {
				1, 2, 3, 0, 0, 0,
				0, 0, 0, 1, 2, 3
			};

			vmf::setPixel(p, pixel, c);
			p += 2 * c;
			vmf::setPixel(p, pixel, c);
			for (int i = 0; i < c; i++) {
				Assert::AreEqual(expected_array[i], actual_array[i]);
			}
		}

		TEST_METHOD(TestFindPixel_SampleWindow_ExpectedPixel)
		{
			const int n = 3;
			const int c = 1;
			int index = 8;
			int window[n*n*c] = {
				0, 1, 2,
				3, 4, 5,
				6, 7, 8
			};

			int actual_rgb_pixel[c] = { 0 };
			int expected_rgb_pixel[c] = { 8 };

			vmf::findPixel(window, actual_rgb_pixel, index, c);
			for (int i = 0; i < c; i++) {
				Assert::AreEqual(expected_rgb_pixel[i], actual_rgb_pixel[i]);
			}
		}

		TEST_METHOD(TestFindPixel_SampleRgbWindow_ExpectedRgbPixel)
		{
			const int n = 2;
			const int c = 3;
			int index = 2;
			int window[n*n*c] = {
				1, 1, 1, 2, 2, 2,
				3, 3, 3, 4, 4, 4
			};

			int actual_rgb_pixel[c] = { 0 };
			int expected_rgb_pixel[c] = { 3,3,3 };

			vmf::findPixel(window, actual_rgb_pixel, index, c);
			for (int i = 0; i < c; i++) {
				Assert::AreEqual(expected_rgb_pixel[i], actual_rgb_pixel[i]);
			}
		}

		TEST_METHOD(TestIndexOfMin_SampleArray_MinIndexEqual3)
		{
			const int n = 6;
			double sample_array[6] = { 1, 2, 3, 0, 4, 1.5 };

			int actual_index = 3;
			int expected_index = vmf::findIndexOfMinValue(sample_array, n);

			Assert::AreEqual(expected_index, actual_index);
		}

		TEST_METHOD(TestSumMatrixRows_Matrix3by3_ExpectedSumsOfRows)
		{
			const int cols = 3, rows = 3;
			double matrix[cols*rows] = {
				0.5 ,0.5 ,0.5,
				1 ,1 ,1,
				1.5 ,1.5 ,1.5
			};
			double expected_sums[rows] = { 1.5, 3, 4.5 };
			double* actual_sums = new double[rows]();

			vmf::sumMatrixRows(matrix, actual_sums, rows, cols);
			for (int i = 0; i < rows; i++) {
				Assert::AreEqual(expected_sums[i], actual_sums[i]);
			}

			delete[] actual_sums;
		}

		TEST_METHOD(TestSumMatrixRows_Matrix2by4_ExpectedSumsOfRows)
		{
			const int cols = 4, rows = 2;
			double matrix[cols*rows] = {
				0.5 ,0.5 ,0.5, 0.5,
				0.2 ,0.2 ,0.2, 0.2
			};
			double expected_sums[rows] = { 2, 0.8 };
			double* actual_sums = new double[rows]();

			vmf::sumMatrixRows(matrix, actual_sums, rows, cols);
			for (int i = 0; i < rows; i++) {
				Assert::AreEqual(expected_sums[i], actual_sums[i]);
			}

			delete[] actual_sums;
		}

		TEST_METHOD(TestCalcDistanceMatrix_Window2by2_ExpectedMatrix4by4)
		{
			const int n = 4;
			const int channels = 1;
			int window[n*channels] = { 1, 2, 3, 4 };
			double expected_matrix[n*n] = {
				0, 1, 2, 3,
				1, 0, 1, 2,
				2, 1, 0, 1,
				3, 2, 1, 0
			};
			double* actual_matrix = new double[n*n]();

			vmf::calcDistanceMatrixFromWindow(window, actual_matrix, n, channels);
			for (int i = 0; i < n*n; i++) {
				Assert::AreEqual(expected_matrix[i], actual_matrix[i]);
			}

			delete[] actual_matrix;
		}

		TEST_METHOD(TestCalcDistanceMatrix_RgbWindow2by2_ExpectedMatrix4by4)
		{
			const int n = 4;
			const int channels = 3;
			int window[n*channels] = { 1, 2, 1, 1, 2, 2, 1, 2, 3, 1, 2, 4 };
			double expected_matrix[n*n] = {
				0, 1, 2, 3,
				1, 0, 1, 2,
				2, 1, 0, 1,
				3, 2, 1, 0
			};
			double* actual_matrix = new double[n*n]();

			vmf::calcDistanceMatrixFromWindow(window, actual_matrix, n, channels);
			for (int i = 0; i < n*n; i++) {
				Assert::AreEqual(expected_matrix[i], actual_matrix[i]);
			}

			delete[] actual_matrix;
		}

		TEST_METHOD(TestCalcDistanceMatrix_RgbWindow1by1_ExpectedMatrix1by1)
		{
			const int n = 1;
			const int channels = 3;
			int window[n*channels] = { 1, 2, 3 };
			double expected_matrix[n*n] = {
				0
			};
			double* actual_matrix = new double[n*n]();

			vmf::calcDistanceMatrixFromWindow(window, actual_matrix, n, channels);
			for (int i = 0; i < n*n; i++) {
				Assert::AreEqual(expected_matrix[i], actual_matrix[i]);
			}

			delete[] actual_matrix;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window2by2InMatrix5by5_ExpectedWindow)
		{
			Mat sample_image_5by5 = (Mat_<unsigned char>(5, 5) <<
				11, 12, 13, 14, 15,
				21, 22, 23, 24, 25,
				31, 32, 33, 34, 35,
				41, 42, 43, 44, 45,
				51, 52, 53, 54, 55);

			int expected_window[4] = { 21, 21, 21, 55 };
			const int window_size = 2;

			unsigned char *p = (unsigned char*)(sample_image_5by5.data);
			const int c = sample_image_5by5.channels();

			int* window = new int[window_size*window_size*c];

			vmf::calcPixelsWindowAndFilterPixels(p, window, sample_image_5by5.rows, sample_image_5by5.cols, c, window_size);
			for (int i = 0; i < window_size*window_size*c; i++) {
				Assert::AreEqual(expected_window[i], window[i]);
			}

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window2by2InMatrix2by5_ExpectedWindow)
		{
			Mat sample_image_2by5 = (Mat_<unsigned char>(2, 5) <<
				11, 12, 13, 14, 15,
				21, 22, 23, 24, 25);

			int expected_window[4] = { 14, 21, 15 ,25 };
			const int window_size = 2;

			unsigned char *p = (unsigned char*)(sample_image_2by5.data);
			const int c = sample_image_2by5.channels();

			int* window = new int[window_size*window_size*c];

			vmf::calcPixelsWindowAndFilterPixels(p, window, sample_image_2by5.rows, sample_image_2by5.cols, c, window_size);
			for (int i = 0; i < window_size*window_size*c; i++) {
				Assert::AreEqual(expected_window[i], window[i]);
			}

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window2by2InMatrix5by2_ExpectedWindow)
		{
			Mat sample_image_5by2 = (Mat_<unsigned char>(5, 2) <<
				11, 12,
				21, 22,
				31, 32,
				41, 42,
				51, 52);

			int expected_window[4] = { 41, 51, 31, 52 };
			const int window_size = 2;

			unsigned char *p = (unsigned char*)(sample_image_5by2.data);
			const int c = sample_image_5by2.channels();

			int* window = new int[window_size*window_size*c];

			vmf::calcPixelsWindowAndFilterPixels(p, window, sample_image_5by2.rows, sample_image_5by2.cols, c, window_size);
			for (int i = 0; i < window_size*window_size*c; i++) {
				Assert::AreEqual(expected_window[i], window[i]);
			}

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window4by4InMatrix3by3_ReturnMinus1)
		{
			Mat sample_image_3by3 = (Mat_<unsigned char>(3, 3) <<
				11, 12, 13,
				21, 22, 23,
				31, 32, 33);

			const int window_size = 4;
			unsigned char *p = (unsigned char*)(sample_image_3by3.data);
			const int c = sample_image_3by3.channels();

			int* window = new int[window_size*window_size*c];

			int result = vmf::calcPixelsWindowAndFilterPixels(p, window, sample_image_3by3.rows, sample_image_3by3.cols, c, window_size);
			Assert::AreEqual(-1, result);

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_WindowSize0InMatrix3by3_ReturnMinus1)
		{
			Mat sample_image_3by3 = (Mat_<unsigned char>(3, 3) <<
				11, 12, 13,
				21, 22, 23,
				31, 32, 33);

			const int window_size = 0;
			unsigned char *p = (unsigned char*)(sample_image_3by3.data);
			const int c = sample_image_3by3.channels();

			int* window = new int[window_size*window_size*c];

			int result = vmf::calcPixelsWindowAndFilterPixels(p, window, sample_image_3by3.rows, sample_image_3by3.cols, c, window_size);
			Assert::AreEqual(-1, result);

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window2by2InMatrix2by2_ExpectedWindow)
		{
			Mat sample_image_2by2 = (Mat_<unsigned char>(2, 2) <<
				11, 12,
				21, 22);

			int expected_window[4] = { 11, 21, 12, 22 };
			const int window_size = 2;

			unsigned char *p = (unsigned char*)(sample_image_2by2.data);
			const int c = sample_image_2by2.channels();

			int* window = new int[window_size*window_size*c];

			vmf::calcPixelsWindowAndFilterPixels(p, window, sample_image_2by2.rows, sample_image_2by2.cols, c, window_size);
			for (int i = 0; i < window_size*window_size*c; i++) {
				Assert::AreEqual(expected_window[i], window[i]);
			}

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window1by1InMatrix2by2_ExpectedWindow)
		{
			Mat sample_image_2by2 = (Mat_<unsigned char>(2, 2) <<
				11, 12,
				21, 22);

			int expected_window[1] = { 22 };
			const int window_size = 1;

			unsigned char *p = (unsigned char*)(sample_image_2by2.data);
			const int c = sample_image_2by2.channels();

			int* window = new int[window_size*window_size*c];

			vmf::calcPixelsWindowAndFilterPixels(p, window, sample_image_2by2.rows, sample_image_2by2.cols, c, window_size);
			for (int i = 0; i < window_size*window_size*c; i++) {
				Assert::AreEqual(expected_window[i], window[i]);
			}

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window2by2InRgbMatrix2by2_ExpectedWindow)
		{
			Mat sample_rgb_image_2by2(2, 2, CV_8UC3, Scalar(1, 100, 255));

			int expected_window[4 * 3] = { 1, 100, 255, 1, 100, 255, 1, 100, 255, 1, 100, 255 };
			const int window_size = 2;

			unsigned char *p = (unsigned char*)(sample_rgb_image_2by2.data);
			const int c = sample_rgb_image_2by2.channels();

			int* window = new int[window_size*window_size*c];

			vmf::calcPixelsWindowAndFilterPixels(p, window, sample_rgb_image_2by2.rows, sample_rgb_image_2by2.cols, c, window_size);
			for (int i = 0; i < window_size*window_size*c; i++) {
				Assert::AreEqual(expected_window[i], window[i]);
			}

			delete[] window;
		}

	};
}