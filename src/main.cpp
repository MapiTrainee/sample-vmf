#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	string image_name = "img\\test.png";
	cout << image_name << endl;

	Mat image, gray_image;
	image = imread(image_name, IMREAD_COLOR);
	cvtColor(image, gray_image, CV_BGR2GRAY);
	cout << image << endl << endl;
	cout << gray_image << endl;

	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	string image_titles[] = { "Plain image", "Gray image" };
	namedWindow(image_titles[0], CV_WINDOW_AUTOSIZE);
	namedWindow(image_titles[1], CV_WINDOW_AUTOSIZE);
	//for better preview test.png
	resize(image, image, Size(), 20, 20);
	resize(gray_image,gray_image, Size(), 20, 20);
	imshow(image_titles[0], image);
	imshow(image_titles[1], gray_image);

	waitKey(0);
	return 0;
}
