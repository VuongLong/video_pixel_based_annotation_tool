#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#define KEY_UP 0
#define KEY_DOWN 1
#define KEY_LEFT 2
#define KEY_RIGHT 3
#define KEY_TAB 9
#define KEY_SPACE 32


int COLOR[] = {0, 255, 0};
const int maxn = 100;
int x[maxn], y[maxn], count_point;
Mat currentFrame, ROI, RES, overlayerFrame;
int frameNumber = 1; /* The current frame number */
int numberOfFrame = 1000;
int isOriginal = 1;
double circle_zie = 4;

void processVideo(char* input, char* imageROI, char* output);
void overlayImage(Mat* src, Mat* overlay, Mat* pRes);


void draw_currentFrame(int height, int width, int x[], int y[], int size, int color[]){
	//currentFrame = Mat::zeros( height, width, CV_8UC1 );
	Point rook_points[1][size];
	for (int i=0;i<size;i++)
		rook_points[0][i] = Point(x[i], y[i]);
	const Point* ppt[1] = { rook_points[0] };
	int npt[] = { size };
 	fillPoly( RES, ppt, npt, 1, Scalar(color[0], color[1], color[2]), 8 );
}

void CallBackFunc(int event, int x_m, int y_m, int flags, void* userdata)
{
	if  ( event == EVENT_LBUTTONDOWN )
	{
	  	x[count_point]=x_m;
	  	y[count_point]=y_m;
	  	count_point++;
	  	if (count_point>1){
		  	line(overlayerFrame, Point( x[count_point-2], y[count_point-2] ), Point(x[count_point-1], y[count_point-1]), Scalar(255,0,0),  1, 8 );
	  		imshow("My Window", overlayerFrame);

	  	}
	  	cout << count_point << endl;
	}
	else if  ( event == EVENT_RBUTTONDOWN )
	{
	  	
	}
	else if  ( event == EVENT_MBUTTONDOWN )
	{
	  cout << "Middle button of the mouse is clicked - position (" << x_m << ", " << y_m << ")" << endl;
	}
	else if ( event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_CTRLKEY)
    {
    	circle( RES, Point( x_m, y_m ), circle_zie, Scalar(0,0,255), -1, 4 );
		overlayImage( &currentFrame, &RES, &overlayerFrame);    	
  		imshow("My Window", overlayerFrame);    
    }
    else if ( event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_SHIFTKEY)
    {
    	circle( RES, Point( x_m, y_m ), circle_zie, Scalar(0,0,0), -1, 4 );
		overlayImage( &currentFrame, &RES, &overlayerFrame);
  		imshow("My Window", overlayerFrame);    
    }
}

int main(int argc, char** argv)
{
	//Create a window
	namedWindow("My Window");
	//set the callback function for any mouse event
	setMouseCallback("My Window", CallBackFunc, NULL);
	
	numberOfFrame = atoi(argv[4]);
	cout << numberOfFrame;
    createTrackbar("Frame:", "My Window", &frameNumber, numberOfFrame);

  	processVideo(argv[1], argv[2], argv[3]);
}

void overlayImage(Mat* src, Mat* overlay, Mat* pRes)
{
    for (int y = 0; y < src->rows; ++y)
    {
        for (int x = 0; x < src->cols; ++x)
        {
            for (int c = 0; c < src->channels(); ++c)
            {
                unsigned char overlayPx = overlay->data[y * overlay->step + x * overlay->channels() + c];
                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
                pRes->data[y * src->step + src->channels() * x + c] = min(srcPx + overlayPx,255);
            }
        }
    }
}


void processVideo(char* input, char* imageROI, char* output)
{
 	//ROI = imread( imageROI, 1 );
	
	/* Variables. */
	int height, width;
	int pos = 0;

	while (true) {
		cout << frameNumber << '\n';
		currentFrame.release();
		RES.release();
		// Read original image 
		currentFrame = imread(input+to_string(frameNumber) +".png");

		//if fail to read the image
		if (!currentFrame.data) 
		{ 
			cout << "Error loading the image" << endl;
			return; 
		}
		else {
			RES = imread(output+to_string(frameNumber) +".png");
			//if not exist, create new one
			int currF = frameNumber;
			while (!RES.data) 
			{ 
				currF--;
				if (currF==0)
				{RES = Mat(currentFrame.rows, currentFrame.cols, CV_8UC3);
      			rectangle( RES, Point(0,0), Point(currentFrame.cols-1,currentFrame.rows-1), Scalar(0,0,0), -1, 8 );
					break;}
      			RES = imread(output+to_string(currF) +".png");

			}
		}

		pos = frameNumber;

      	overlayerFrame = Mat(currentFrame.rows, currentFrame.cols, CV_8UC3);
		
		overlayImage( &currentFrame, &RES, &overlayerFrame);
    	imshow("My Window", overlayerFrame);
    	isOriginal = 0;
		//show the image
		while (true) {

			if (pos!=frameNumber) break;

			int key = (waitKey(0) & 0xFF);
			/*
			if (key=='c'){ 
			    resize(currentFrame, currentFrame, cv::Size(), 2, 2);
				imshow("My Window", currentFrame);
			}
			if (key=='a'){ 
		 	    resize(currentFrame, currentFrame, cv::Size(), 0.5, 0.5);
				imshow("My Window", currentFrame);
			}
			*/
			//cout << key <<'\n'; 

			if (key==KEY_RIGHT && frameNumber < numberOfFrame) {
				setTrackbarPos("Frame:", "My Window",++frameNumber);
				break;
			}
			if (key==KEY_LEFT && frameNumber > 1) {
				setTrackbarPos("Frame:", "My Window",--frameNumber);
				break;
			}
			if (key=='s') {
				imwrite(output+to_string(frameNumber) +".png", RES);
			}
			if (key==KEY_TAB) {
				if (isOriginal==1){
					overlayImage( &currentFrame, &RES, &overlayerFrame);
					imshow("My Window", overlayerFrame);
					isOriginal = 0;
				}
				else {
					imshow("My Window", currentFrame);
					isOriginal = 1;						
				}
			}
			if (key=='f') {
				circle_zie++;
			}
			if (key=='g') {
				circle_zie--;
			}
			if (key=='w') {
				COLOR[1] = 255;
				COLOR[2] = 0;			
			}
			if (key=='e') {
				COLOR[1] = 0;
				COLOR[2] = 255;			
			}
			if (key==KEY_SPACE) {
				cout << "update" << endl;
				if (count_point >2){
					draw_currentFrame(currentFrame.rows, currentFrame.cols, x, y, count_point, COLOR);
					overlayImage( &currentFrame, &RES, &overlayerFrame);    	
			 		count_point = 0;
			  		imshow("My Window", overlayerFrame);
			  	}			
			}
			if (key=='q'){
	  			return;
			}
		}
	    
	}
}


/*
tab: show result or not
control left mouse: draw point
Shift left mouse: erase point
W: object color
E: shadow color
click mouse to select polygon them using SPACE to draw
S: Save groundtruth image
Left or right arrow: to select frame, or using trackbar
Q: to quit programe
f: increase circle size
g: decrease circle size
commandline: ./anotation_tool input/ input/ROI.png output/
*/


