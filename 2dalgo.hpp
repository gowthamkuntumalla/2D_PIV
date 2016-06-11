#ifndef _2DALGO_HPP_
#define _2DALGO_HPP_
#include<fstream>
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
double cor_coeff(Mat image1,Mat image2,int avg1,int avg2,const int row,const int col,const int x,const int y,int r1,int c1) // return correlation coefficient
{
    double c=0;
    for (int i=c1; i<col; i++) //window of image1 starting at (0,0)
    {
        for (int j=r1; j<row; j++)
        {
            int a = (int)image1.at<uchar> (c1,r1);
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

    /******* Declarations *******/
    ofstream myfile;
    int initial_value = 0;
    int avg1 = 0, avg2=0;
    int sd1=0, sd2=0;
    int rows = 64,cols = 64;// interrogation window size
    int subrow=16,subcol=16;// sub matrix
    int x=0,y=0;//displacements

    int row1= image1.rows,col1=image1.cols; //gets the rows and columns of image.
    int row2= image2.rows,col2=image2.cols;

    /******* Computing Averages START *******/
    for(int c=0; c<col1-64; c++)
    {
        for(int r=0; r<row1-64; r++)
        {

            vector< vector<double> > cortable; // 2D array of correlation at various (x.y)
            cortable.resize(rows,vector<double>(cols,initial_value));//initializing the vector
            avg1=avg(subrowr,subcol+c,image1,c,r);//computing average of sub window1
            sd1=sd(subrow+r,subcol+c,image1,c,r);//computing standard deviation of sub window
            for(x=0; x<cols*.75; x++) // for accuracy, max displacement is N/4; N::window size(NxN)
            {
                for(y=0; y<rows*.75; y++)
                {
                    avg2=avg(subrow+r+y,subcol+c+x,image2,x,y);// only the second image subwindows are in iteration.
                    sd2=sd(subrow+r+y,subcol+c+x,image2,x,y);
                    //cout<<avg1<<"  "<<sd1<<"  "<<avg2<<"  "<<sd2<<endl;
                    if(sd1!=0&&sd2!=0)
                    {
                        cortable[x][y]= cor_coeff(image1,image2,avg1,avg2,subrow+r1,subcol+c1,x,y,r,c)/(sd1*sd2)/(subrow*subcol);//normalized correlation coefficient
                    }
                    else
                    {
                        cortable[x][y]= 0;
                        cerr<<"zero SD"<<endl;
                    }
                    //cout<<cortable[x][y]<<endl;
                }
            }
            myfile.open ("data.txt");
            myfile << "Writing this to a file.\n";
            myfile << "max value = " <<max_coef(cortable)<<endl;
            // cout<<"max value = "<<max_coef(cortable);
        }
    }
    myfile.close();
}
//core function of 2D_PIV
#endif // _2DALGO_HPP_
