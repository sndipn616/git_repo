#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stack>

using namespace cv;
using namespace std;
const float width = 512;
const float height = 512;
int k = 0;
int l = 0;
int flag=0;
stack<char> path;
char shapetype[6][6];
char arena[6][6];
float xcoordinate[6][6];
float ycoordinate[6][6];
Point2f botsqr[4];
Point2f contvector[1];
Mat src;
Mat src_hsv;
Mat src_gray;
Mat threshold1;
char state[6][6];
int visited[6][6];
char ans[100];
int act[100];
int direction[2], a, b;
int count2;
int c = 0;
char dir;
char temp2;
//int thresh = 70;
int max_thresh = 255;
RNG rng(12345);
int xs, ys, xe, ye, xt, yt, xb, yb, xa, ya;
char next_sh;
FILE *file;
/// Function header
struct dir_list
{
	int dirn[2];
	char direc;
};

struct dir_list lst[4];

void wait(int seconds)
{
	clock_t endwait;
	endwait = clock() + seconds * CLOCKS_PER_SEC;
	while (clock() < endwait) {}
}


int retkey(int *tempdir)
{
	if (tempdir[0] == 0 && tempdir[1] == -1)
		return 1;

	if (tempdir[0] == 0 && tempdir[1] == 1)
		return 0;

	if (tempdir[0] == 1 && tempdir[1] == 0)
		return 3;

	return 2;
}

int retx(char d)
{
	if (d=='n')
		return -1;

	else if (d=='s')
		return 1;

	return 0;
}

int rety(char d)
{
	if (d=='e')
		return 1;

	else if (d=='w')
		return -1;

	return 0;
}

int DFS(int i, int j)
{
	//wait(2);
	//printf("AT %d %d %c\n",i,j,dir);
	int temp, x, y, k, perdir[2], key;
	char t;
	path.push(dir);
	state[i][j] = dir;
	x = i;
	y = j;

	i = i + direction[1];
	j = j + direction[0];

	perdir[0] = direction[0];
	perdir[1] = direction[1];


	key = retkey(perdir);

	if (key == 0)
		t = 's';
	else if (key == 1)
		t = 'n';
	else if (key == 2)
		t = 'w';
	else
		t = 'e';

	if (visited[i][j] == 0)
		visited[i][j] = 1;

	else if (visited[i][j] == 1 && state[i][j] == t)
	{
		path.pop();
		return 0;
	}



	if (i == xe && j == ye)
	{
		//printf("Destination : returning 1  %c\n",dir);
		//path.push(dir);
		return 1;
	}

	if (i >= 0 && i<6 && j >= 0 && j<6)
	{
		if (arena[i][j] == 'r')
		{
			//printf("returning 0 at %d %d\n",i,j);	
			path.pop();
			return 0;
		}

		count2 = 1;

		if (arena[i][j] == 'w')
		{
			direction[0] = 0;
			direction[1] = -1;
			dir = 'n';
		}

		else if (arena[i][j] == 's')
		{
			direction[0] = 0;
			direction[1] = 1;
			dir = 's';
		}

		else if (arena[i][j] == 'a')
		{
			direction[0] = -1;
			direction[1] = 0;
			dir = 'w';
		}

		else if (arena[i][j] == 'd')
		{
			direction[0] = 1;
			direction[1] = 0;
			dir = 'e';
		}



		else if (arena[i][j] == '+')
		{



			for (k = 0; k<3; k++)
			{

				if (k != key)
				{
					direction[0] = lst[k].dirn[0];
					direction[1] = lst[k].dirn[1];
					dir = lst[k].direc;
				}

				else
				{
					direction[0] = lst[3].dirn[0];
					direction[1] = lst[3].dirn[1];
					dir = lst[3].direc;
				}

				temp = DFS(i, j);
				if (temp != 0)
				{
					//printf("pushing : returning 1 at %d %d %c\n",i,j,state[x][y]);
					//action.push(temp);
					act[c] = temp;
					c++;
					//path.push(state[x][y]);
					return 1;
				}

				//printf("k= %d\n",k);
			}
			//printf("returning 0 at %d %d\n",i,j);
			path.pop();
			return 0;
		}


	}

	else
	{
		//printf("returning 0 at %d %d\n",i,j);	
		path.pop();
		return 0;
	}

	if (count2 == 1)
	{
		temp = DFS(i, j);
		if (temp != 0)
		{
			//printf("pushing :  at %d %d %c returning ",i,j,state[x][y]);
			//action.push(temp);
			act[c] = temp;
			c++;

			if (arena[i][j] == 'h')
			{
				//printf("5\n");
				return 5;
			}


			if (arena[i][j] == 'l')
			{
				//printf("2\n");
				return 2;
			}


			else
			{
				//printf("1\n");
				return 1;
			}

			//path.push(state[x][y]);
		}
		else
		{
			path.pop();
			//printf("returning %d at %d %d\n",temp,i,j);
			return 0;
		}



	}
}

int xcell(float a)
{
	int x = 0;
	if (a >= 0 && a <= width / 6)
		x = 0;
	else if (a >= width / 6 && a <= 2 * width / 6)
		x = 1;
	else if (a >= 2 * width / 6 && a <= 3 * width / 6)
		x = 2;
	else if (a >= 3 * width / 6 && a <= 4 * width / 6)
		x = 3;
	else if (a >= 4 * width / 6 && a <= 5 * width / 6)
		x = 4;
	else if (a >= 5 * width / 6 && a <= 6 * width / 6)
		x = 5;
	return x;
}


int ycell(float a)
{
	int y = 0;
	if (a >= 0 && a <= height / 6)
		y = 0;
	else if (a > height / 6 && a <= 2 * height / 6)
		y = 1;
	else if (a > 2 * height / 6 && a <= 3 * height / 6)
		y = 2;
	else if (a > 3 * height / 6 && a <= 4 * height / 6)
		y = 3;
	else if (a > 4 * height / 6 && a <= 5 * height / 6)
		y = 4;
	else if (a > 5 * height / 6 && a <= 6 * height / 6)
		y = 5;
	return y;
}
int triangletype(int a1, int a2, int a3, int b1, int b2, int b3)
{
	if ((-10 < a1 - a2 && a1 - a2 < 10) && a3 < a1)
		return 1;
	else if ((-10 < a2 - a3 && a2 - a3 < 10) && a1 < a3)
		return 1;
	else if ((-10 < a3 - a1 && a3 - a1 < 10) && a2 < a3)
		return 1;
	else if ((-10 < a1 - a2 && a1 - a2 < 10) && a3 > a1)
		return 2;
	else if ((-10 < a2 - a3 && a2 - a3 < 10) && a1 > a3)
		return 2;
	else if ((-10 < a3 - a1 && a3 - a1 < 10) && a2 > a3)
		return 2;
	else if ((-10 < b1 - b2 && b1 - b2 < 10) && b3 < b1)
		return 3;
	else if ((-10 < b2 - b3 && b2 - b3 < 10) && b1 < b3)
		return 3;
	else if ((-10 < b3 - b1 && b3 - b1 < 10) && b2 < b3)
		return 3;
	else if ((-10 < b1 - b2 && b1 - a2 < 10) && b3 > b1)
		return 4;
	else if ((-10 < b2 - b3 && b2 - b3 < 10) && b1 > b3)
		return 4;
	else if ((-10 < b3 - b1 && b3 - b1 < 10) && b2 > b3)
		return 4;

}
void unit_vector(float j, float k, float l, float m)
{
	float a = j - l;
	float b = k - m;
	float c = (j - l)*(j - l) + (k - m)*(k - m);
	float d = sqrt(c);
	botsqr[3].x = a / d;
	botsqr[3].y = b / d;

}
void unit_contvector(float j, float k, float l, float m)
{
	float a = j - l;
	float b = k - m;
	float c = (j - l)*(j - l) + (k - m)*(k - m);
	float d = sqrt(c);
	contvector[0].x = a / d;
	contvector[0].y = b / d;

}
int direction_check(int c, int d)
{
	int w, x;
	float  y, z, p, q, j, k;
	w = xcell(botsqr[2].x);
	x = ycell(botsqr[2].y);
	y = xcoordinate[x][w];
	z = ycoordinate[x][w];
	p = xcoordinate[c][d];
	q = ycoordinate[c][d];
	unit_contvector(y, z, p, q);

	if (contvector[0].x - botsqr[0].x >= -0.5 && contvector[0].x - botsqr[0].x <= 0.5)
		return 0;
	else if (botsqr[0].x - contvector[0].x >= 0.5)
		return 1;
	else if (botsqr[0].x - contvector[0].x <= -0.5)
		return -1;

}
int detect_bot(int g, int h)
{
	namedWindow("Control1", CV_WINDOW_AUTOSIZE);
	int BotLowH = 0;
	int BotHighH = 179;

	int BotLowS = 0;
	int BotHighS = 255;

	int BotLowV = 0;
	int BotHighV = 255;
	cvCreateTrackbar("LowH", "Control1", &BotLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control1", &BotHighH, 179);

	cvCreateTrackbar("LowS", "Control1", &BotLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control1", &BotHighS, 255);

	cvCreateTrackbar("LowV", "Control1", &BotLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control1", &BotHighV, 255);
	src = imread("arena3.jpg", 1);
	resize(src, src, Size(512, 512), 0, 0, CV_INTER_AREA);
	cvtColor(src, src_hsv, CV_BGR2HSV);
	while (1){
		inRange(src_hsv, Scalar(BotLowH, BotLowS, BotLowV), Scalar(BotHighH, BotHighS, BotHighV), threshold1);
		namedWindow("source1", CV_WINDOW_AUTOSIZE);
		imshow("source1", threshold1);
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	threshold(threshold1, src_gray, 10, 255, 0);//10 or any random value

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	vector<Point> approx;
	//morphological opening (remove small objects from the foreground)
	erode(src_gray, src_gray, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	dilate(src_gray, src_gray, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

	//morphological closing (fill small holes in the foreground)
	dilate(src_gray, src_gray, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	erode(src_gray, src_gray, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	findContours(src_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}
	Mat drawing = Mat::zeros(src_gray.size(), CV_8UC3);
	int a, b;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contourArea(contours[i]) > 300 && contourArea(contours[i]) < 20000)
		{
			if (arcLength(contours[i], true) > 50)
			{
				a = xcell(mc[i].x);
				b = ycell(mc[i].y);
				//printf("");
				xcoordinate[b][a] = mc[i].x;
				ycoordinate[b][a] = mc[i].y;
				Vec3b intensity = src_hsv.at<Vec3b>(mc[i].y, mc[i].x);
				unsigned char hue = intensity.val[0];
				unsigned char sat = intensity.val[1];
				unsigned char value = intensity.val[2];
				printf(" Area OpenCV: %.2f - Length: %.2f x-coordinate: %.2f y-coordinate: %.2f\n", contourArea(contours[i]), arcLength(contours[i], true), mc[i].x, mc[i].y);

				approxPolyDP(Mat(contours[i]), approx,
					arcLength(Mat(contours[i]), true)*0.03, true);
				//printf("%d\n", approx.size());

				if ((hue >= 115 && hue <= 125) && (sat >= 150 && sat <= 200) && (value >= 175 && value <= 225) && approx.size() == 4)
				{
					printf("%c \n", 'u');
					botsqr[k].x = mc[i].x;
					botsqr[k].y = mc[i].y;
					k++;

				}
				else if ((hue >= 115 && hue <= 125) && (sat >= 150 && sat <= 200) && (value >= 175 && value <= 225))
				{
					printf("%c \n", 'u');
					botsqr[k].x = mc[i].x;
					botsqr[k].y = mc[i].y;
					k++;
				}
			}
		}
	}
	botsqr[2].x = (botsqr[0].x + botsqr[1].x) / 2;
	botsqr[2].y = (botsqr[0].y + botsqr[1].y) / 2;
	unit_vector(botsqr[0].x, botsqr[0].y, botsqr[1].x, botsqr[1].y);
	printf("%f %f \n", botsqr[3].x, botsqr[3].y);
	/*if (l < 1)
	{
		ys = xcell(botsqr[2].x);
		xs = ycell(botsqr[2].y);
		l++;
		if (botsqr[3].y > 0.9)
		{
			dir = 'n';
			direction[0] = 0;
			direction[1] = -1;
		}
		else if (botsqr[3].y < -0.9)
		{
			dir = 's';
			direction[0] = 0;
			direction[1] = 1;
		}
		else if (botsqr[3].x > 0.9)
		{
			dir = 'e';
			direction[0] = 1;
			direction[1] = 0;
		}
		else if (botsqr[3].y < -0.9)
		{
			dir = 'w';
			direction[0] = -1;
			direction[1] = 0;
		}
	}
	printf("%d %d \n", xs, ys);
	printf("%c \n", dir);*/
	return direction_check(g, h);

}
//orient function
void orient(int o, int p)
{
	int db;
	db=detect_bot(o,p);
	while(db!=0)
	{
		if(db==-1)
		{
			fprintf(file,"l");
			fflush(file);
			wait(0.2);
			fprintf(file,"s");
			fflush(file);
		}
		else
		{
			fprintf(file,"r");
			fflush(file);
			wait(0.2);
			fprintf(file,"s");
			fflush(file);
		}
		db=detect_bot(o,p);
	}

}
/** @function main */
int main()
{
	
	int t,z;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
			shapetype[i][j] = '-';
	}
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 42;
	int iHighH = 101;

	int iLowS = 24;
	int iHighS = 255;

	int iLowV = 54;
	int iHighV = 255;

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);

	/// Load source image and convert it to gray
	src = imread("arena3.jpg", 1);
	resize(src, src, Size(512, 512), 0, 0, CV_INTER_AREA);
	cvtColor(src, src_hsv, CV_BGR2HSV);
	while (1){
		inRange(src_hsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), threshold1);
		namedWindow("source1", CV_WINDOW_AUTOSIZE);
		imshow("source1", threshold1);
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	threshold(threshold1, src_gray, 10, 255, 0);//10 or any random value

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	vector<Point> approx;
	float xcoordinate[6][6];
	float ycoordinate[6][6];

	//morphological opening (remove small objects from the foreground)
	erode(src_gray, src_gray, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	dilate(src_gray, src_gray, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

	//morphological closing (fill small holes in the foreground)
	dilate(src_gray, src_gray, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	erode(src_gray, src_gray, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	namedWindow("canny_window", CV_WINDOW_AUTOSIZE);
	imshow("canny_window", src_gray);

	/// Find contours
	findContours(src_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


	/// Get the moments
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	///  Get the mass centers:
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}
	/// Draw contours
	Mat drawing = Mat::zeros(src_gray.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		if (contourArea(contours[i]) > 300)
		{
			if (arcLength(contours[i], true) > 50 && arcLength(contours[i], true) < 500)
			{

				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
				circle(drawing, mc[i], 4, color, -1, 8, 0);
			}
		}
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	int a, b;
	/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
	printf("\t Info: Area and Contour Length \n");
	for (int i = 0; i < contours.size(); i++)
	{
		if (contourArea(contours[i]) > 300 && contourArea(contours[i])<20000)
		{
			if (arcLength(contours[i], true) > 50)
			{
				a = xcell(mc[i].x);
				b = ycell(mc[i].y);
				//printf("");
				xcoordinate[b][a] = mc[i].x;
				ycoordinate[b][a] = mc[i].y;
				Vec3b intensity = src_hsv.at<Vec3b>(mc[i].y, mc[i].x);
				unsigned char hue = intensity.val[0];
				unsigned char sat = intensity.val[1];
				unsigned char value = intensity.val[2];
				printf(" Area OpenCV: %.2f - Length: %.2f x-coordinate: %.2f y-coordinate: %.2f\n", contourArea(contours[i]), arcLength(contours[i], true), mc[i].x, mc[i].y);

				approxPolyDP(Mat(contours[i]), approx,
					arcLength(Mat(contours[i]), true)*0.03, true);
				//printf("%d\n", approx.size());
				if (approx.size() == 3)
				{
					if (triangletype(approx[0].x, approx[1].x, approx[2].x, approx[0].y, approx[1].y, approx[2].y) == 1)
					{
						shapetype[b][a] = 'a';
						printf("%c \n", 'a');
					}
					else if (triangletype(approx[0].x, approx[1].x, approx[2].x, approx[0].y, approx[1].y, approx[2].y) == 2)
					{
						shapetype[b][a] = 'd';
						printf("%c \n", 'd');
					}
					else if (triangletype(approx[0].x, approx[1].x, approx[2].x, approx[0].y, approx[1].y, approx[2].y) == 3)
					{
						shapetype[b][a] = 'w';
						printf("%c \n", 'w');
					}
					else
					{
						shapetype[b][a] = 's';
						printf("%c \n", 's');
					}


				}
				else if (approx.size() == 12)
				{
					printf("%c \n", '+');
					shapetype[b][a] = '+';
				}
				else if (approx.size() == 4)
				{
					if ((hue >= 170 && hue <= 180) && (sat >= 225 && sat <= 255) && (value >= 100 && value <= 200))//red
					{
						printf("%c \n", 'r');
						shapetype[b][a] = 'r';
					}
					else if ((hue >= 100 && hue <= 140) && (sat >= 0 && sat <= 50) && (value >= 150 && value <= 255))//white
					{
						printf("%c \n", 'h');
						shapetype[b][a] = 'h';
					}
					else if ((hue >= 120 && hue <= 160) && (sat >= 20 && sat <= 120) && (value >= 5 && value <= 30))//black
					{
						printf("%c \n", 'b');
						shapetype[b][a] = 'b';
					}
					else if ((hue >= 90 && hue <= 120) && (sat >= 170 && sat <= 210) && (value >= 75 && value <= 125))//blue
					{
						printf("%c \n", 'l');
						shapetype[b][a] = 'l';

					}
					else if ((hue >= 20 && hue <= 40) && (sat >= 170 && sat <= 230) && (value >= 120 && value <= 200))//yellow
					{
						printf("%c \n", 'y');
						shapetype[b][a] = 'y';
						ye = xcell(mc[i].x);
						xe = ycell(mc[i].y);
						printf("%d %d \n", xe, ye);
					}
					else if ((hue >= 115 && hue <= 125) && (sat >= 150 && sat <= 200) && (value >= 175 && value <= 225))
					{
						shapetype[b][a] = 'u';
						botsqr[k].x = mc[i].x;
						botsqr[k].y = mc[i].y;
						k++;
					}
				}
				else if ((hue >= 115 && hue <= 125) && (sat >= 150 && sat <= 200) && (value >= 175 && value <= 225))
				{
					shapetype[b][a] = 'u';
					botsqr[k].x = mc[i].x;
					botsqr[k].y = mc[i].y;
					k++;
				}
			}
		}


	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (shapetype[i][j] == '-')
			{
				xcoordinate[i][j] = -1.00;
				ycoordinate[i][j] = -1.00;
			}
		}
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
			printf("%c ", shapetype[i][j]);
		printf("\n");
	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			printf("%.2f  ", xcoordinate[i][j]);
		}
		printf("\n");
	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			printf("%.2f  ", ycoordinate[i][j]);
		}
		printf("\n");
	}

	//waitKey(0);
    botsqr[2].x = (botsqr[0].x + botsqr[1].x) / 2;
	botsqr[2].y = (botsqr[0].y + botsqr[1].y) / 2;
	int i, j, x, y, k, m, n;
	char ch;

	for (i = 0; i<6; i++)
	{
		for (j = 0; j<6; j++)
		{
			arena[i][j] = shapetype[i][j];
			visited[i][j] = 0;
			state[i][j]='z';
		}

	}
unit_vector(botsqr[0].x,botsqr[0].y,botsqr[1].x,botsqr[1].y);
	if (l < 1)
	{
		ys = xcell(botsqr[2].x);
		xs = ycell(botsqr[2].y);
		l++;
		if (botsqr[3].y > 0.9)
		{
			dir = 'n';
			direction[0] = 0;
			direction[1] = -1;
		}
		else if (botsqr[3].y < -0.9)
		{
			dir = 's';
			direction[0] = 0;
			direction[1] = 1;
		}
		else if (botsqr[3].x > 0.9)
		{
			dir = 'e';
			direction[0] = 1;
			direction[1] = 0;
		}
		else if (botsqr[3].y < -0.9)
		{
			dir = 'w';
			direction[0] = -1;
			direction[1] = 0;
		}
	}
	printf("%d %d \n", xs, ys);
	printf("%c \n", dir);
	
	// xs=x-cord of start, ys=y-cord of start, xe=x-cord of start,ye=y-cord of start
	//dir=direction
	lst[0].dirn[0] = 0;
	lst[0].dirn[1] = -1;
	lst[0].direc = 'n';
	lst[1].dirn[0] = 0;
	lst[1].dirn[1] = 1;
	lst[1].direc = 's';
	lst[2].dirn[0] = 1;
	lst[2].dirn[1] = 0;
	lst[2].direc = 'e';
	lst[3].dirn[0] = -1;
	lst[3].dirn[1] = 0;
	lst[3].direc = 'w';

	/*if(arena[x][y]=='U')
	{
	direction[0]=0;
	direction[1]=-1;
	dir='n';

	}

	else if(arena[x][y]=='A')
	{
	direction[0]=0;
	direction[1]=1;
	dir='s';

	}

	else if(arena[x][y]=='(')
	{
	direction[0]=1;
	direction[1]=0;
	dir='e';

	}

	else
	{
	direction[0]=-1;
	direction[1]=0;
	dir='w';

	}*/
	//detect_bot(0,1);

	temp2 = dir;


	printf(" xs ys xe ye %d %d %d %d\n",xs,ys,xe,ye);
	t=DFS(xs, ys);
	act[c]=t;


	for (i = 0; i<6; i++)
	{
		for (j = 0; j<6; j++)
		{
			printf("%c ", arena[i][j]);
		}
		printf("\n");


	}

	
	i = path.size() - 1;
	//printf("i=%d\n",i);

	j = i;
	while (!path.empty())
	{
		//printf("%c\n",path.top());
		ans[i] = path.top();
		path.pop();
		i--;
	}

	//c--;
	//printf("671\n");

	
	//keep two temporary variables for x and y coordinate of centroid next shape.
	//initialize it as xt=xs+direction[1], yt=ys+direction[0] (untill next mark, a loop is reqd)
/*	for(i=0;i<6;i++)
	{
		float tot1,tot2;
		tot1=0;
		tot2=0;
		for(j=0;j<6;j++)
		{
			if(xcoordinate[j][i]>=0)
			{
				tot1=tot1+xcoordinate[j][i];
				tot2++;
			}

		}

		for(j=0;j<6;j++)
		{
			if(xcoordinate[j][i]>=-1.5 && xcoordinate[j][i]<=-0.5)
			{
				xcoordinate[j][i]=tot1/tot2;
			}

		}
	}
	
	for(i=0;i<6;i++)
	{
		float tot1,tot2;
		tot1=0;
		tot2=0;
		for(j=0;j<6;j++)
		{
			if(ycoordinate[i][j]>=0)
			{
				tot1=tot1+ycoordinate[i][j];
				tot2++;
			}

		}

		for(j=0;j<6;j++)
		{
			if(ycoordinate[i][j]>=-1.5 && ycoordinate[i][j]<=-0.5)
			{
				ycoordinate[i][j]=tot1/tot2;
			}

		}
	}*/
	
	xt=xs+retx(ans[0]);
	yt=ys+rety(ans[0]);
	while(arena[xt][yt]=='-')
	{
		//printf("while");
		xt=xt+retx(ans[0]);
		yt=yt+rety(ans[0]);

	}
	//initialize it as xt=xs+direction[1], yt=ys+direction[0] (untill next mark, a loop is reqd)
	//keep a temporary variable, next_sh for keeping track of next shape
	next_sh=arena[xt][yt];
	flag=0;
	xa=xs;
	ya=ys;


	//printf("%d %d %c\n",xt,yt,ans[0]);
	
	file = popen("minicom -b 9600 -o -D /dev/rfcomm0", "w");
		printf("Path\n");
		//printf("ans[0]=%c\n",ans[0]);
	for(z=0;z<=j;z++)
	{
		//if(i<j)
			//printf("z= %d\n",z);
			printf("%c-%d-",ans[z],act[c]);
		//else 
			//printf("%c-1-",ans[i]);
			
		//printf("temp2 %c",temp2);	

		//if next shape is h,l perform accordingly.
			xa=xa+retx(ans[z]);
			ya=ya+rety(ans[z]);

			//orient
			orient(xt,yt);
			fprintf(file,"f");
			fflush(file);
			wait(0.5);
			fprintf(file,"s");
			fflush(file);
			if(next_sh=='h' && flag==0)
			{
				wait(5);
				flag=1;

			}
			if(next_sh=='h' && flag==0)
			{
				fprintf(file,"g");
				fflush(file);
				flag=1;

			}

			fprintf(file,"f");
			fflush(file);
			wait(0.5);

	
			if(ans[z]=='n')
			{
				if(temp2=='n')
					{
						printf("forward\n");
						//orient
						

						

						//while current shape is not crossed
						//while centroid of next mark is not reached
						//take an image and orient towards centroid of next mark
						//fprintf(file,"f");
						//fflush(file);
						//wait(0.1);
					}
				else if(temp2=='e')
					{
						printf("left\n");
						//fprintf(file,"l");
						//fflush(file);
						//wait(1);
						//while centroid of next mark is not reached
						//fprintf(file,"f");
						//fflush(file);
						//wait(1);

					}
				else if(temp2=='w')
					{
						printf("right\n");
						//fprintf(file,"r");
						//fflush(file);
						//wait(1);
						//while centroid of next mark is not reached
						//fprintf(file,"f");
						//fflush(file);
						//wait(1);
					}
				else
				{
					printf("back turn\n");
					//fprintf(file,"r");
					//fflush(file);
					//wait(2);
					//while centroid of next mark is not reached
					//fprintf(file,"f");
					//fflush(file);
					//wait(1);

				}
			}
			
			else if(ans[z]=='s')
			{
				if(temp2=='s')
					{
						printf("forward\n");
						//while centroid of next mark is not reached
						//fprintf(file,"f");
						//fflush(file);
						//wait(1);
					}
				else if(temp2=='w')
					{
						printf("left\n");
						//fprintf(file,"l");
						//fflush(file);
						//wait(1);
						//while centroid of next mark is not reached
						//fprintf(file,"f");
						//fflush(file);
						//wait(1);
					}
				else if(temp2=='e')	
					{
						printf("right\n");
						//fprintf(file,"r");
						//fflush(file);
						//wait(1);
						//while centroid of next mark is not reached
						//fprintf(file,"f");
						//fflush(file);
						//wait(1);
					}

				else
				{
					printf("back turn\n");
					//fprintf(file,"r");
					//fflush(file);
					//wait(2);
					//while centroid of next mark is not reached
					//fprintf(file,"f");
					//fflush(file);
					//wait(1);

				}
			}
			
			 else if(ans[z]=='e')
			{
				if(temp2=='e')
					{
						printf("forward\n");
						//while centroid of next mark is not reached
						//fprintf(file,"f");
						//fflush(file);
						//wait(1);
					}
				else if(temp2=='s')
					{
						printf("left\n");
						//fprintf(file,"l");
						//fflush(file);
						//wait(1);
						//while centroid of next mark is not reached
						//fprintf(file,"f");
						//fflush(file);
						//wait(1);
					}
				else if(temp2=='n')	
					{
						printf("right\n");
						//fprintf(file,"r");
						//fflush(file);
						//wait(1);
						//while centroid of next mark is not reached
						//fprintf(file,"f");
						//fflush(file);
						//wait(1);
					}
				else
					{
						printf("back turn\n");
						//fprintf(file,"r");
						//fflush(file);
						//wait(2);
						//while centroid of next mark is not reached
						//fprintf(file,"f");
						//fflush(file);
						//wait(1);

					}
			}
			
			else if(ans[z]=='w')
			{
				if(temp2=='w')
					{
						printf("forward\n");
						//while centroid of next mark is not reached
						//fprintf(file,"f");
						//fflush(file);
						//wait(1);
					}
				else if(temp2=='n')
					{
						printf("left\n");
						//fprintf(file,"l");
						//fflush(file);
						//wait(1);
						//while centroid of next mark is not reached
						//fprintf(file,"f");
						//fflush(file);
						//wait(1);
					}
				else if(temp2=='s')	
					{
						printf("right\n");
						//fprintf(file,"r");
						//fflush(file);
						//wait(1);
						//while centroid of next mark is not reached
						//fprintf(file,"f");
						//fflush(file);
						//wait(1);
					}
				else
				{
					printf("back turn\n");
					//fprintf(file,"r");
					//fflush(file);
					//wait(2);
					//while centroid of next mark is not reached
					//fprintf(file,"f");
					//fflush(file);
					//wait(1);

				}
				
			}
		
			
		
		
		temp2=ans[z];
		c--;
		//update xt, yt and next_sh
		
	
	if(xa==xt && ya==yt)
		{
			xt=xt+retx(ans[0]);
			yt=yt+rety(ans[0]);
			while(arena[xt][yt]=='-')
			{
				//printf("while");
				xt=xt+retx(ans[z]);
				yt=yt+rety(ans[z]);

			}
			next_sh=arena[xt][yt];
			flag=0;

		}
		//update xt, yt and next_sh
		
	}
	
	//printf("%c-1\n",ans[i]);	
			
	
	fclose(file);
	return(0);
}