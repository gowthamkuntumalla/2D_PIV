#ifndef _2DALGO_HPP_
#define _2DALGO_HPP_
using namespace cv;
using namespace std;
/*********************** Auxiliary Functions ***********************/
int avg(const int row,const int col, Mat image,const int x,const int y)//average of sub matrix
{
    int sum=0;
    for (int i=x; i<col; i++) //window starting at (x,y)
    {
        for (int j=y; j<row; j++) // <i><x><col> equivalent and similar
        {
            int a = (int)image.at<uchar> (i,j);
            sum+=a;
        }
    }
    sum/=(row*col);
   return sum;
}
int sd(const int row,const int col, Mat image,const int x,const int y)//standard deviation of sub matrix
{
    int var= 0;//variance
    int aver = avg(row,col,image,x,y);
    for (int i=x; i<col; i++) //window starting at (x,y)
    {
        for (int j=y; j<row; j++)
        {
            int a = (int)image.at<uchar> (i,j);
            var+=(a-aver)^2;
        }
    }
    return sqrt(var);
}
double cor_coeff(Mat image1,Mat image2,int avg1,int avg2,const int row,const int col,const int x,const int y) // return correlation coefficient
{
    double c=0;
    for (int i=0; i<col; i++) //window of image1 starting at (0,0)
    {
        for (int j=0; j<row; j++)
        {
            int a = (int)image1.at<uchar> (i,j);
            int b = (int)image2.at<uchar> (i+x,j+y);
            c+=(a-avg1)*(b-avg2); // main equation
        }
    }
    return c;

}
double max_coef(vector< vector<double> > t)
{
    int a=-1;// all intensities are greater than this
    for(int i=0; i<64; i++)
    {
        for(int j=0; j<64; j++)
        {
            if(t[i][j]>a)
            {
                a=t[i][j];
            }
        }
    }
    return a;
}

/********* function *********/
void piv_2d(cv::Mat image1, cv::Mat image2)
{

}
//core function of 2D_PIV
#endif // _2DALGO_HPP_
