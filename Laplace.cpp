#include <iostream>
#include <math.h>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
using namespace cv;
using namespace std;

int rows, cols;
int type[4][3][3] = { { {0,1,0}, {1,-4,1} ,{0,1,0} } ,{ {0,-1,0},{-1,4,-1},{0,-1,0} },{ {1,1,1} ,{1,-8,1}, {1,1,1} },{ {-1,-1,-1}, {-1,8,-1} ,{-1,-1,-1} } };


int main() {
	
	for (int i = 0; i < 4; i++)
	{
		cout << "Type " << i  << endl;
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				cout << type[i][j][k] << " ";
			}
			cout << endl;
		}
		cout << endl << endl;
	}

	int tp;
	cout << "Enter a type: ";
	cin>>tp;
	Mat input = imread("m.png", 0);
	imshow("Original Image", input);

	rows = input.rows;
	cols = input.cols;



	Mat image;
	copyMakeBorder(input, image , 1, 1, 1, 1, BORDER_REPLICATE);

	Mat lap_filter=input.clone();
	Mat main_output = input.clone();
	Mat scl_filter = input.clone();
	Mat output_image=input.clone();
	int mini=1000, maxy=-1000;
	int miny = 1000, maxyi = -1000;
	for (int x = 0; x < rows; x++)
	{
		uchar *ptr_in = image.ptr<uchar>(x);
		uchar *ptr_lap = lap_filter.ptr<uchar>(x);
		uchar *ptr_out = output_image.ptr<uchar>(x);
		uchar *ptr_scl = scl_filter.ptr<uchar>(x);

		for (int y = 0; y < cols; y++)
		{
			
			int val = 0;
			for (int a = 0; a < 3; a++)
			{
				for (int b = 0; b < 3; b++)
				{
					val += type[tp][a][b] * (ptr_in+a)[y+b];
				
				}
			
			}
			if (val < mini)
			{
				mini = val;
			}
			
			int new_val;
			if (val < 0)new_val = 0;
			else if (val > 255)new_val = 255;
			else new_val = val;
			ptr_lap[y] = new_val;
			ptr_scl[y] = val;
			int now;
			if (tp == 1 || tp == 3)
			{
				now = ptr_in[y] + val;
			}
			else  now = ptr_in[y] - val;

			if (now < miny)
			{
				miny = now;
			}
			
		}
	}
	
	for (int x = 0; x < rows; x++)
	{
		uchar *ptr_scl = scl_filter.ptr<uchar>(x);

		for (int y = 0; y < cols; y++)
		{

			ptr_scl[y] = ptr_scl[y] - mini;
			if (maxy < ptr_scl[y])
			{
				maxy = ptr_scl[y];
			}
		}
	}
	//int miny = 1000, maxyi = -1000;
	for (int x = 0; x < rows; x++)
	{
		uchar *ptr_scl = scl_filter.ptr<uchar>(x);

		for (int y = 0; y < cols; y++)
		{

			ptr_scl[y] = ptr_scl[y] * (255.0/maxy);

		}
	}
	/*for (int x = 0; x < rows; x++)
	{
		uchar *ptr_in = image.ptr<uchar>(x);
		//uchar *ptr_lap = lap_filter.ptr<uchar>(x);
		uchar *ptr_out = output_image.ptr<uchar>(x);
		uchar *ptr_scl = scl_filter.ptr<uchar>(x);

		for (int y = 0; y < cols; y++)
		{

			int now;
			if (tp == 1 || tp == 3)
			{
				now = ptr_in[y] + ptr_scl[y];
			}
			else  now = ptr_in[y] - ptr_scl[y];
			// now = ptr_in[y] + ptr_scl[y];


			if (now < miny)
			{
				miny = now;
			}
			if (now > maxyi)
			{
				maxyi = now;
			}
		
			ptr_out[y] = now;

		}
	}*/
	for (int x = 0; x < rows; x++)
	{
		uchar *ptr_out = output_image.ptr<uchar>(x);

		for (int y = 0; y < cols; y++)
		{

			ptr_out[y] = ptr_out[y] - miny;
			if (ptr_out[y] > maxyi)
			{
				maxyi = ptr_out[y];
			}
		}
	}
	//int miny = 1000, maxyi = -1000;
	for (int x = 0; x < rows; x++)
	{
		uchar *ptr_out = output_image.ptr<uchar>(x);


		for (int y = 0; y < cols; y++)
		{

			ptr_out[y] = ptr_out[y] * (255.0 / maxyi);

		}
	}
	
	output_image.convertTo(main_output, -1, 1, 50);

	imshow("Laplacian Output", lap_filter);
	imshow("scale", scl_filter);
	imshow("Sharpened Image", output_image);
	//imshow("output Image", main_output);

	waitKey(0);
	return 0;
}
