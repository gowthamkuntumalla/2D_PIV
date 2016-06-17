#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "2dalgo.hpp"

/** This is "2D Particle image velocimetry, Version 1" Programmed by Gowtham Kuntumalla **/
/** Debugged and tested by Krishnasandeep Bhogaraju,Gowtham Kuntumalla **/

/**mhkhaniitb@gmail.com {PhD}, advisor**/
/**gowthamkuntumalla@gmail.com,krishnasandeepbhogaraju@gmail.com**/
/** Output data file consists of initial point(x1,y1) final point(x2,y2) **/

/*********************** Main Function ***********************/

int main() //calculation of correlation coefficient
{
    /**************Declarations**************/
    int wincol=64,winrow=64;
    int nHeight,nWidth;
    Mat image1,image2;// new blank image, "2D matrix class"
    Mat image3;//image storing vectors of displacament
    vector< vector <pair<int,int> > > max_coef_point;

    /**************browse option may be included here**************/

    /**************Read Images**************/
    image1 = cv::imread("image1.tif", 0);// read the file into gray scale(0)
    image2 = cv::imread("image2.tif", 0);//
    //after successfully reading images
    int totrows1= image1.rows,totcols1=image1.cols; //opencv functions to get the rows and columns of image.
    int totrows2= image2.rows,totcols2=image2.cols;
    nHeight=totrows2;
    nWidth=totcols2; //setting height of the img
    Mat img(Mat(nHeight, nWidth, CV_8U));//blank image
    img=Scalar(0);//background color

    piv_2d(image1,image2,max_coef_point);//the vertices of maximum correlation are obtained from this function

    /**************Drawing Vectors in Image**************/
    image3=image1;
    cv::namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
    for(int c=0; c<totcols1-wincol/2; c+=16)
    {
        for(int r=0; r<totrows1-winrow/2; r+=16)
        {
            Point pt1 = Point(c+8,r+8);//here (x,y)=(col,row) point in cartesian coordiante system
            Point pt2 = Point(max_coef_point[r][c].second+8,max_coef_point[r][c].first+8);
            //syntax =  void arrowedLine(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int line_type=8, int shift=0, double tipLength=0.1)
            arrowedLine(img, pt1, pt2, Scalar(255), 1, 8, 0, .2);//white
        }
    }
    // or NORMAL create a window for display.
    cv::imshow( "Display window",img);
    imwrite("result_img.tif",img);
    waitKey(0);// wait for a keystroke in the window
    return 0;
}

