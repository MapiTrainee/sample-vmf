#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../src/vmf.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestSampleVMF
{
	TEST_CLASS(UnitTestSampleVMF)
	{
	public:

		TEST_METHOD(TestFilterVMF)
		{
			Assert::IsTrue(1 == 1);
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
			double* actual_matrix = new double[n*n];

			vmf::calcDistanceMatrix(window, actual_matrix, n, channels);
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
			double* actual_matrix = new double[n*n];

			vmf::calcDistanceMatrix(window, actual_matrix, n, channels);
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
			double* actual_matrix = new double[n*n];

			vmf::calcDistanceMatrix(window, actual_matrix, n, channels);
			for (int i = 0; i < n*n; i++) {
				Assert::AreEqual(expected_matrix[i], actual_matrix[i]);
			}

			delete[] actual_matrix;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window2by2InMatrix5by5_ExpectedWindow)
		{
			Mat example_image_5by5 = (Mat_<unsigned char>(5, 5) <<
				11, 12, 13, 14, 15,
				21, 22, 23, 24, 25,
				31, 32, 33, 34, 35,
				41, 42, 43, 44, 45,
				51, 52, 53, 54, 55);

			int expected_window[4] = { 44, 54, 45, 55 };
			const int window_size = 2;

			unsigned char *p = (unsigned char*)(example_image_5by5.data);
			const int c = example_image_5by5.channels();

			int* window = new int[window_size*window_size*c];

			vmf::calcPixelsWindow(p, window, example_image_5by5.rows, example_image_5by5.cols, c, window_size);
			for (int i = 0; i < window_size*window_size*c; i++) {
				Assert::AreEqual(expected_window[i], window[i]);
			}

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window2by2InMatrix2by5_ExpectedWindow)
		{
			Mat example_image_2by5 = (Mat_<unsigned char>(2, 5) <<
				11, 12, 13, 14, 15,
				21, 22, 23, 24, 25);

			int expected_window[4] = { 14, 24, 15 ,25 };
			const int window_size = 2;

			unsigned char *p = (unsigned char*)(example_image_2by5.data);
			const int c = example_image_2by5.channels();

			int* window = new int[window_size*window_size*c];

			vmf::calcPixelsWindow(p, window, example_image_2by5.rows, example_image_2by5.cols, c, window_size);
			for (int i = 0; i < window_size*window_size*c; i++) {
				Assert::AreEqual(expected_window[i], window[i]);
			}

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window2by2InMatrix5by2_ExpectedWindow)
		{
			Mat example_image_5by2 = (Mat_<unsigned char>(5, 2) <<
				11, 12,
				21, 22,
				31, 32,
				41, 42,
				51, 52);

			int expected_window[4] = { 41, 51, 42, 52 };
			const int window_size = 2;

			unsigned char *p = (unsigned char*)(example_image_5by2.data);
			const int c = example_image_5by2.channels();

			int* window = new int[window_size*window_size*c];

			vmf::calcPixelsWindow(p, window, example_image_5by2.rows, example_image_5by2.cols, c, window_size);
			for (int i = 0; i < window_size*window_size*c; i++) {
				Assert::AreEqual(expected_window[i], window[i]);
			}

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window4by4InMatrix3by3_ReturnMinus1)
		{
			Mat example_image_3by3 = (Mat_<unsigned char>(3, 3) <<
				11, 12, 13,
				21, 22, 23,
				31, 32, 33);

			const int window_size = 4;
			unsigned char *p = (unsigned char*)(example_image_3by3.data);
			const int c = example_image_3by3.channels();

			int* window = new int[window_size*window_size*c];

			int result = vmf::calcPixelsWindow(p, window, example_image_3by3.rows, example_image_3by3.cols, c, window_size);
			Assert::AreEqual(-1, result);

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_WindowSize0InMatrix3by3_ReturnMinus1)
		{
			Mat example_image_3by3 = (Mat_<unsigned char>(3, 3) <<
				11, 12, 13,
				21, 22, 23,
				31, 32, 33);

			const int window_size = 0;
			unsigned char *p = (unsigned char*)(example_image_3by3.data);
			const int c = example_image_3by3.channels();

			int* window = new int[window_size*window_size*c];

			int result = vmf::calcPixelsWindow(p, window, example_image_3by3.rows, example_image_3by3.cols, c, window_size);
			Assert::AreEqual(-1, result);

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window2by2InMatrix2by2_ExpectedWindow)
		{
			Mat example_image_2by2 = (Mat_<unsigned char>(2, 2) <<
				11, 12,
				21, 22);

			int expected_window[4] = { 11, 21, 12, 22 };
			const int window_size = 2;

			unsigned char *p = (unsigned char*)(example_image_2by2.data);
			const int c = example_image_2by2.channels();

			int* window = new int[window_size*window_size*c];

			vmf::calcPixelsWindow(p, window, example_image_2by2.rows, example_image_2by2.cols, c, window_size);
			for (int i = 0; i < window_size*window_size*c; i++) {
				Assert::AreEqual(expected_window[i], window[i]);
			}

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window1by1InMatrix2by2_ExpectedWindow)
		{
			Mat example_image_2by2 = (Mat_<unsigned char>(2, 2) <<
				11, 12,
				21, 22);

			int expected_window[1] = { 22 };
			const int window_size = 1;

			unsigned char *p = (unsigned char*)(example_image_2by2.data);
			const int c = example_image_2by2.channels();

			int* window = new int[window_size*window_size*c];

			vmf::calcPixelsWindow(p, window, example_image_2by2.rows, example_image_2by2.cols, c, window_size);
			for (int i = 0; i < window_size*window_size*c; i++) {
				Assert::AreEqual(expected_window[i], window[i]);
			}

			delete[] window;
		}

		TEST_METHOD(TestCalcPixelsWindow_Window2by2InRgbMatrix2by2_ExpectedWindow)
		{
			Mat example_rgb_image_2by2(2, 2, CV_8UC3, Scalar(1, 100, 255));

			int expected_window[4 * 3] = { 1, 100, 255, 1, 100, 255, 1, 100, 255, 1, 100, 255 };
			const int window_size = 2;

			unsigned char *p = (unsigned char*)(example_rgb_image_2by2.data);
			const int c = example_rgb_image_2by2.channels();

			int* window = new int[window_size*window_size*c];

			vmf::calcPixelsWindow(p, window, example_rgb_image_2by2.rows, example_rgb_image_2by2.cols, c, window_size);
			for (int i = 0; i < window_size*window_size*c; i++) {
				Assert::AreEqual(expected_window[i], window[i]);
			}

			delete[] window;
		}

	};
}