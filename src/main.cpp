#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	string image_name = "img\\test.png";
	cout << image_name << endl;

	Mat image;
	image = imread(image_name, IMREAD_COLOR);

	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	namedWindow(image_name, CV_WINDOW_AUTOSIZE);
	imshow(image_name, image);

	system("pause");
	return 0;
}
