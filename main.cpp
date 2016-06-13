#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "2dalgo.hpp"

//mhkhaniitb@gmail.com PhD, advisor
//gowthamkuntumalla@gmail.com,krishnasandeepbhogaraju@gmail.com

/*********************** Main Function ***********************/

int main() //calculation of correlation coefficient
{

    /**************Declarations**************/

    Mat image1,image2;// new blank image, "2D matrix class"
    Mat image3=image1;//image storing vectors of displacament
    vector< vector <pair<int,int> > > max_coef_point;
    int totrow1= image1.rows,totcol1=image1.cols; //opencv functions to get the rows and columns of image.
    int totrow2= image2.rows,totcol2=image2.cols;
    int wincol=64,winrow=64;

    /**************browse option may be included here**************/
    /**************Read Images**************/
    image1 = cv::imread("image1.tif", 0);// read the file into gray scale(0)
    image2 = cv::imread("image2.tif", 0);//size: 800x600 pixels here
    //after successfully reading images

    piv_2d(image1,image2,max_coef_point);//the vertices of maximum correlation are obtained from this function

    /**************Drawing Vectors in Image**************/

    for(int c=0; c<totcol1-wincol; c+=32)
    {
        for(int r=0; r<totrow1-winrow; r+=32)
        {
            Point pt1 = Point(c,r);
            Point pt2 = Point(max_coef_point[c][r].first,max_coef_point[c][r].second);
            //syntax =  void arrowedLine(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int line_type=8, int shift=0, double tipLength=0.1)
            arrowedLine(image3, pt1, pt2, Scalar( 255, 0, 0 ), 1, 8, 0, .1);// Blue in BGR system
        }
    }
    cv::namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// or NORMAL create a window for display.
    cv::imshow( "Display window",image3);
    waitKey(0);// wait for a keystroke in the window
    return 0;
}

