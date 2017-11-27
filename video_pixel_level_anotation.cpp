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

const int maxn = 100;
int x[maxn], y[maxn], count_point;
Mat currentFrame, ROI, RES, overlayerFrame;

void processVideo(char* videoFilename, char* imageROI, char* output);
void overlayImage(Mat* src, Mat* overlay, Mat* pRes);


void draw_currentFrame(int height, int width, int x[], int y[], int size){
	//currentFrame = Mat::zeros( height, width, CV_8UC1 );
	Point rook_points[1][size];
	for (int i=0;i<size;i++)
		rook_points[0][i] = Point(x[i], y[i]);
	const Point* ppt[1] = { rook_points[0] };
	int npt[] = { size };
 	fillPoly( RES, ppt, npt, 1, Scalar(0, 0, 255), 8 );
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
	  	cout << "update" << endl;
		if (count_point >2){
			draw_currentFrame(currentFrame.rows, currentFrame.cols, x, y, count_point);
			overlayImage( &currentFrame, &RES, &overlayerFrame);    	
	 		count_point = 0;
	  		imshow("My Window", overlayerFrame);
	  	}
	}
	else if  ( event == EVENT_MBUTTONDOWN )
	{
	  cout << "Middle button of the mouse is clicked - position (" << x_m << ", " << y_m << ")" << endl;
	}
	else if ( event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_CTRLKEY)
    {
    	circle( RES, Point( x_m, y_m ), 4.0, Scalar(0,0,255), -1, 4 );
		overlayImage( &currentFrame, &RES, &overlayerFrame);    	
  		imshow("My Window", overlayerFrame);    
    }
    else if ( event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_SHIFTKEY)
    {
    	circle( RES, Point( x_m, y_m ), 4.0, Scalar(0,0,0), -1, 4 );
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
	
	int framePos = 1;
	int numberOfFrame = 1000;
    createTrackbar("Frame:", "My Window", &framePos, numberOfFrame);

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


void processVideo(char* videoFilename, char* imageROI, char* output)
{

 	ROI = imread( imageROI, 1 );

	/* Create the capture object. */
	VideoCapture capture(videoFilename);

	if (!capture.isOpened()) {
		/* Error in opening the video input. */
		cerr << "Unable to open video file: " << videoFilename << endl;
		exit(EXIT_FAILURE);
	}

	/* Variables. */
	static int frameNumber = 1; /* The current frame number */
	int keyboard = 0;           /* Input from keyboard. Used to stop the program. Enter 'q' to quit. */


	int height, width;
	/* Read input data. ESC or 'q' for quitting. */
	while (true) {
	    /* Read the current frame. */
	    if (!capture.read(currentFrame)) {
	      cerr << "Unable to read next frame." << endl;
	      cerr << "Exiting..." << endl;
	  	  capture.release();

	      break;
	      exit(EXIT_FAILURE);
	    }

      	if (frameNumber==1) RES = Mat(currentFrame.rows, currentFrame.cols, CV_8UC3);
      	overlayerFrame = Mat(currentFrame.rows, currentFrame.cols, CV_8UC3);
      	rectangle( RES, Point(0,0), Point(currentFrame.rows-1, currentFrame.cols-1), Scalar(0,0,0), -1, 8 );
		
		overlayImage( &currentFrame, &RES, &overlayerFrame);
    	imshow("My Window", overlayerFrame);

		//show the image
		while (cvWaitKey(1) != '\33') {
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

			if (key==KEY_RIGHT) {
				imwrite(output+to_string(frameNumber) +".png", RES);
				break;
			}
			if (key==KEY_LEFT) {
				imwrite(output+to_string(frameNumber) +".png", RES);
				break;
			}
			if (key=='t') {
				overlayImage( &currentFrame, &RES, &overlayerFrame);
				imshow("My Window", overlayerFrame);
			}
			if (key=='r') {
				imshow("My Window", currentFrame);
			}
			if (key=='e') {
				imshow("My Window", RES);
			}
			if (key=='q'){
				cout << "asdasd: "<<(char)26;
	  			capture.release();
	  			return;
			}
		}
	    ++frameNumber;
	}
}
