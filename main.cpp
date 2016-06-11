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
    /******* Read Images *******/

    Mat image1,image2;// new blank image, "2D matrix class"
    /**************browse option may be included here**************/

    image1 = cv::imread("image1.tif", 0);// read the file into gray scale(0)
    image2 = cv::imread("image2.tif", 0);//size: 800x600 pixels
    //after successfully reading images

    piv_2d(image1,image2);
    waitKey(0);// wait for a keystroke in the window
    return 0;
}

