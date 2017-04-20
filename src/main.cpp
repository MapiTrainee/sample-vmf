#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include "vmf.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	string image_name = "img\\sample.png";
	cout << image_name << endl;

	Mat image, gray_image;
	image = imread(image_name, IMREAD_COLOR);
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	cvtColor(image, gray_image, CV_BGR2GRAY);

	vector<string> image_titles = { "Plain image", "Gray image", "Filtered color image", "Filtered gray image" };
	for (int i = 0; i < image_titles.size(); i++) {
		namedWindow(image_titles[i], CV_WINDOW_AUTOSIZE);
	}

	imshow(image_titles[0], image);
	imshow(image_titles[1], gray_image);

	Mat filtered_image,
		filtered_gray_image;

	int window_side = 3;

	vmf::vmfFilter(image, filtered_image, window_side);
	vmf::vmfFilter(gray_image, filtered_gray_image, window_side);

	imshow(image_titles[2], filtered_image);
	imshow(image_titles[3], filtered_gray_image);

	waitKey(0);
	return 0;
}
