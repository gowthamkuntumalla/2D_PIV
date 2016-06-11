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
    image1 = cv::imread("image1.tif", 0);// read the file into gray scale(0)
    image2 = cv::imread("image2.tif", 0);//size: 800x600 pixels
    //after successfully reading images

    /******* Declarations *******/

    int initial_value = 0;
    int avg1 = 0, avg2=0;
    int sd1=0, sd2=0;
    int rows = 64,cols = 64;// interrogation window size
    int subrow=16,subcol=16;// sub matrix
    int x=0,y=0;//displacements
    vector< vector<double> > cortable; // 2D array of correlation at various (x.y)
    cortable.resize(rows,vector<double>(cols,initial_value));//initializing the vector

    /******* Computing Averages START *******/

    avg1=avg(subrow,subcol,image1,0,0);//computing average of sub window1
    sd1=sd(subrow,subcol,image1,0,0);//computing standard deviation of sub window

    for(x=0; x<cols*.5; x++) // for accuracy, max displacement is N/4
    {
        for(y=0; y<rows*.5; y++)
        {
            avg2=avg(subrow+y,subcol+x,image2,x,y);// only the second image subwindows are in iteration.
            sd2=sd(subrow+y,subcol+x,image2,x,y);
            //cout<<avg1<<"  "<<sd1<<"  "<<avg2<<"  "<<sd2<<endl;
            if(sd1!=0&&sd2!=0)
            {
                cortable[x][y]= cor_coeff(image1,image2,avg1,avg2,subrow,subcol,x,y)/(sd1*sd2)/(subrow*subcol);//normalized correlation coefficient
            }
            else
            {
                cortable[x][y]= 0;
                cerr<<"zero SD"<<endl;
            }
            cout<<cortable[x][y]<<endl;
        }
    }
    cout<<"max value = "<<max_coef(cortable);
    waitKey(0);// wait for a keystroke in the window
    return 0;
}

