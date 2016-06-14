#ifndef _2DALGO_HPP_
#define _2DALGO_HPP_
#include<fstream>
#include<cmath>
using namespace cv;
using namespace std;
// <i><x><col> are equivalent and similarly <j><y><row>
// convention : (col,row), (x,y) order is followed
//(c,r) i.e., the top left corner represents the address of subwindow
/*********************** Auxiliary Functions ***********************/
double avg(const int x2,const int y2, Mat image,const int x1,const int y1)//average of sub matrix
{
    double sum=0.0;
    for (int i=x1; i<x2; i++) //window starting at (x,y)
    {
        for (int j=y1; j<y2; j++) // <i><x><col> equivalent and similar
        {
            double a = (int)image.at<uchar> (i,j);
            sum+=a;
        }
    }
    sum/=((x2-x1)*(y2-y1));
    return sum;
}
double sd(const int x2,const int y2, Mat image,const int x1,const int y1)//standard deviation of sub matrix
{
    double var= 0.0;//variance
    double aver = avg(x2,y2,image,x1,y1);
    //myfile<<aver<<endl;
    for (int i=x1; i<x2; i++) //window starting at (x,y)
    {
        for (int j=y1; j<y2; j++)
        {
            double a = (int)image.at<uchar> (i,j);
            var+=((a-aver)*(a-aver));
        }
    }
    var=sqrt(var)/256;
    return var;
}
double cor_coeff(Mat image1,Mat image2,double avg1,double avg2,const int subcol,const int subrow,const int x2,const int y2,const int c,const int r) // return correlation coefficient
{
    //(c,r)-address of subwindow in image1
    //(x2,y2)-address of subwindow in image2
    double val=0.0;//correlation value
    for (int i=0; i<subcol; i++) //window of image1 starting at (0,0)
    {
        for (int j=0; j<subrow; j++)
        {
            double a = (int)image1.at<uchar> (c+i,r+j);
            double b = (int)image2.at<uchar> (x2+i,y2+j);
            val+=((a-avg1)*(b-avg2)); // main equation
        }
    }
    return val;

}
double max_coef(vector< vector<double> > t,const int wincol,const int winrow,int& max_x,int& max_y )
{
    int a=-1;// all intensities are greater than this
    int m=max_x,n=max_y;
    for(int i=m; i<wincol+m; i++)
    {
        for(int j=n; j<winrow+n; j++)
        {
            if(t[i][j]>a)
            {
                a=t[i][j];
                max_x=i;
                max_y=j;
            }
        }
    }
    return a;
}

/********* function *********/
void piv_2d(cv::Mat image1, cv::Mat image2,vector< vector <pair<int,int> > > &max_points)
{

    /******* Declarations *******/
     ofstream myfile;
    int initial_value = 0;
    double avg1=0,avg2=0;
    double sd1=0, sd2=0;
    int winrow = 64,wincol = 64;// interrogation window size
    int subrow=16,subcol=16;// sub matrix (region of interest-roi)
    int x=0,y=0;// point coordinates

    int totrow1= image1.rows,totcol1=image1.cols; //opencv functions to get the rows and columns of image.
    int totrow2= image2.rows,totcol2=image2.cols;

    /*vector< vector <pair<int,int> > > max_coef_point;//  vector for storing of max coeff coordinates in image 2 corresponding to image 1
    max_coef_point.resize(totrow1-winrow,vector<pair<int,int> >(totcol1-wincol));//initializing the vector
*/
    /******* Start Computing *******/
     myfile.open ("data.txt");
    for(int c=0; c<totcol1-wincol; c+=32)
    {
        for(int r=0; r<totrow1-winrow; r+=32)
        {
            int m=0,n=0;//the max coefficent point
            vector< vector<double> > cortable; // 2D array of correlation at various (x.y)
            cortable.resize(totrow1,vector<double>(totcol1,initial_value));//initializing the vector
            avg1=avg(subcol+c,subrow+r,image1,c,r);//computing average of sub window1
            sd1=sd(subcol+c,subrow+r,image1,c,r);//computing standard deviation of sub window

            //assume image size is greater than 200X200 pixel
            if(((c-32)>=0)&&((c+32)<=totcol2))//inner region
            {
                if((r-32)>=0&&(r+32)<=totrow2)//inner region
                {
                    for(x=c-32,m=x; x<c+32; x++)
                    {
                        for(y=r-32,n=y; y<r+32; y++) // m,n are initialized to starting point
                        {
                            avg2=avg(subcol+x,subrow+y,image2,x,y);// only the second image subwindows are in iteration.
                            sd2=sd(subcol+x,subrow+y,image2,x,y);
                            if((sd1!=0)&&(sd2!=0))
                            {
                                //Mat image1,Mat image2,int avg1,int avg2,const int subcol,const int subrow,const int x2,const int y2,const int c,const int r
                                cortable[x][y]= cor_coeff(image1,image2,avg1,avg2,subcol,subrow,x,y,c,r)/(sd1*sd2)/(subrow*subcol);//normalized correlation coefficient
                            }
                            else
                            {
                                cortable[x][y]= 0;
                                myfile<<"zero SD"<<endl;
                            }
                            //myfile<<cortable[x][y]<<endl;
                        }
                    }

                    myfile << "max value = " <<max_coef(cortable,wincol,winrow,m,n)<<endl;//myfile << "Writing this to a file.\n";
                    /*max_coef_point[c][r].first=m;*/
                   /* max_coef_point[c][r].second=n;*/
                    myfile<<m<<" "<<n<<endl;
                }
                if((r+32)>totrow2)//bottom border
                {
                    for(x=c-32,m=x; x<c+32; x++)
                    {
                        for(y=totrow2-64,n=y; y<totrow2; y++)
                        {
                            avg2=avg(subcol+x,subrow+y,image2,x,y);// only the second image subwindows are in iteration.
                            sd2=sd(subcol+x,subrow+y,image2,x,y);
                            if((sd1!=0)&&(sd2!=0))
                            {
                                //Mat image1,Mat image2,int avg1,int avg2,const int subcol,const int subrow,const int x2,const int y2,const int c,const int r
                                cortable[x][y]= cor_coeff(image1,image2,avg1,avg2,subcol,subrow,x,y,c,r)/(sd1*sd2)/(subrow*subcol);//normalized correlation coefficient
                            }
                            else
                            {
                                cortable[x][y]= 0;
                                myfile<<"zero SD"<<endl;
                            }
                            //myfile<<cortable[x][y]<<endl;
                        }
                    }

                    myfile << "max value = " <<max_coef(cortable,wincol,winrow,m,n)<<endl;//myfile << "Writing this to a file.\n";
                    /*max_coef_point[c][r].first=m;*/
                   /* max_coef_point[c][r].second=n;*/
                    myfile<<m<<" "<<n<<endl;
                }
                if((r-32)<0)//top border
                {
                    for(x=c-32,m=x; x<c+32; x++)
                    {
                        for(y=0,n=y; y<64; y++)
                        {
                            avg2=avg(subcol+x,subrow+y,image2,x,y);// only the second image subwindows are in iteration.
                            sd2=sd(subcol+x,subrow+y,image2,x,y);
                            if((sd1!=0)&&(sd2!=0))
                            {
                                //Mat image1,Mat image2,int avg1,int avg2,const int subcol,const int subrow,const int x2,const int y2,const int c,const int r
                                cortable[x][y]= cor_coeff(image1,image2,avg1,avg2,subcol,subrow,x,y,c,r)/(sd1*sd2)/(subrow*subcol);//normalized correlation coefficient
                            }
                            else
                            {
                                cortable[x][y]= 0;
                                myfile<<"zero SD"<<endl;
                            }
                            //myfile<<cortable[x][y]<<endl;
                        }
                    }

                    myfile << "max value = " <<max_coef(cortable,wincol,winrow,m,n)<<endl;//myfile << "Writing this to a file.\n";
                    /*max_coef_point[c][r].first=m;*/
                   /* max_coef_point[c][r].second=n;*/
                    myfile<<m<<" "<<n<<endl;
                }
                if((r-32)<=0&&(r+32)>=totrow2)//bad image!!
                {
                    cerr<<"too small image";
                    break;
                }
            }
            if((c+32)>=totcol2)//right border
            {
                if((r-32)>=0&&(r+32)<=totrow2)
                {
                    for(x=totcol2-64,m=x; x<totcol2; x++)
                    {
                        for(y=r-32,n=y; y<r+32; y++)
                        {
                            avg2=avg(subcol+x,subrow+y,image2,x,y);// only the second image subwindows are in iteration.
                            sd2=sd(subcol+x,subrow+y,image2,x,y);
                            if((sd1!=0)&&(sd2!=0))
                            {
                                //Mat image1,Mat image2,int avg1,int avg2,const int subcol,const int subrow,const int x2,const int y2,const int c,const int r
                                cortable[x][y]= cor_coeff(image1,image2,avg1,avg2,subcol,subrow,x,y,c,r)/(sd1*sd2)/(subrow*subcol);//normalized correlation coefficient
                            }
                            else
                            {
                                cortable[x][y]= 0;
                                myfile<<"zero SD"<<endl;
                            }
                            //myfile<<cortable[x][y]<<endl;
                        }
                    }

                    myfile << "max value = " <<max_coef(cortable,wincol,winrow,m,n)<<endl;//myfile << "Writing this to a file.\n";
                    /*max_coef_point[c][r].first=m;*/
                   /* max_coef_point[c][r].second=n;*/
                    myfile<<m<<" "<<n<<endl;
                }
                if((r+32)>totrow2)
                {
                    for(x=totcol2-64,m=x; x<totcol2; x++)
                    {
                        for(y=totrow2-64,n=y; y<totrow2; y++)
                        {
                            avg2=avg(subcol+x,subrow+y,image2,x,y);// only the second image subwindows are in iteration.
                            sd2=sd(subcol+x,subrow+y,image2,x,y);
                            if((sd1!=0)&&(sd2!=0))
                            {
                                cortable[x][y]= cor_coeff(image1,image2,avg1,avg2,subcol,subrow,x,y,c,r)/(sd1*sd2)/(subrow*subcol);//normalized correlation coefficient
                            }
                            else
                            {
                                cortable[x][y]= 0;
                                myfile<<"zero SD"<<endl;
                            }
                            //myfile<<cortable[x][y]<<endl;
                        }
                    }

                    myfile << "max value = " <<max_coef(cortable,wincol,winrow,m,n)<<endl;//myfile << "Writing this to a file.\n";
                    /*max_coef_point[c][r].first=m;*/
                   /* max_coef_point[c][r].second=n;*/
                    myfile<<m<<" "<<n<<endl;
                }
                if((r-32)<0)
                {
                    for(x=totcol2-64,m=x; x<totcol2; x++)
                    {
                        for(y=0,n=y; y<64; y++)
                        {
                            avg2=avg(subcol+x,subrow+y,image2,x,y);// only the second image subwindows are in iteration.
                            sd2=sd(subcol+x,subrow+y,image2,x,y);
                            if((sd1!=0)&&(sd2!=0))
                            {
                                cortable[x][y]= cor_coeff(image1,image2,avg1,avg2,subcol,subrow,x,y,c,r)/(sd1*sd2)/(subrow*subcol);//normalized correlation coefficient
                            }
                            else
                            {
                                cortable[x][y]= 0;
                                myfile<<"zero SD"<<endl;
                            }
                            //myfile<<cortable[x][y]<<endl;
                        }
                    }

                    myfile << "max value = " <<max_coef(cortable,wincol,winrow,m,n)<<endl;//myfile << "Writing this to a file.\n";
                    /*max_coef_point[c][r].first=m;*/
                   /* max_coef_point[c][r].second=n;*/
                    myfile<<m<<" "<<n<<endl;
                }
                if((r-32)<=0&&(r+32)>=totrow2)
                {
                    cerr<<"too small image";
                }

            }
            if((c-32)<=0)//left border
            {
                if((r-32)>=0&&(r+32)<=totrow2)
                {
                    for(x=0,m=x; x<64; x++)
                    {
                        for(y=r-32,n=y; y<r+32; y++)
                        {
                            avg2=avg(subcol+x,subrow+y,image2,x,y);// only the second image subwindows are in iteration.
                            sd2=sd(subcol+x,subrow+y,image2,x,y);
                            if((sd1!=0)&&(sd2!=0))
                            {
                                cortable[x][y]= cor_coeff(image1,image2,avg1,avg2,subcol,subrow,x,y,c,r)/(sd1*sd2)/(subrow*subcol);//normalized correlation coefficient
                            }
                            else
                            {
                                cortable[x][y]= 0;
                                myfile<<"zero SD"<<endl;
                            }
                            //myfile<<cortable[x][y]<<endl;
                        }
                    }

                    myfile << "max value = " <<max_coef(cortable,wincol,winrow,m,n)<<endl;//myfile << "Writing this to a file.\n";
                    /*max_coef_point[c][r].first=m;*/
                   /* max_coef_point[c][r].second=n;*/
                    myfile<<m<<" "<<n<<endl;
                }
                if((r+32)>totrow2)
                {
                    for(x=0,m=x; x<64; x++)
                    {
                        for(y=totrow2-64,n=y; y<totrow2; y++)
                        {
                            avg2=avg(subcol+x,subrow+y,image2,x,y);// only the second image subwindows are in iteration.
                            sd2=sd(subcol+x,subrow+y,image2,x,y);
                            if((sd1!=0)&&(sd2!=0))
                            {
                                cortable[x][y]= cor_coeff(image1,image2,avg1,avg2,subcol,subrow,x,y,c,r)/(sd1*sd2)/(subrow*subcol);//normalized correlation coefficient
                            }
                            else
                            {
                                cortable[x][y]= 0;
                                myfile<<"zero SD"<<endl;
                            }
                            //myfile<<cortable[x][y]<<endl;
                        }
                    }

                    myfile << "max value = " <<max_coef(cortable,wincol,winrow,m,n)<<endl;//myfile << "Writing this to a file.\n";
                    /*max_coef_point[c][r].first=m;*/
                   /* max_coef_point[c][r].second=n;*/
                    myfile<<m<<" "<<n<<endl;
                }
                if((r-32)<0)
                {
                    for(x=0,m=x; x<64; x+=1)
                    {
                        for(y=0,n=y; y<64; y+=1)
                        {
                            avg2=avg(subcol+x,subrow+y,image2,x,y);// only the second image subwindows are in iteration.
                            sd2=sd(subcol+x,subrow+y,image2,x,y);
                            //myfile<<avg2<<" * "<<sd2<<endl;
                            if((sd1!=0)&&(sd2!=0))
                            {
                                cortable[x][y]= cor_coeff(image1,image2,avg1,avg2,subcol,subrow,x,y,c,r)/(sd1*sd2)/(subrow*subcol);//normalized correlation coefficient
                            }
                            else
                            {
                                cortable[x][y]= 0;
                                myfile<<"zero SD"<<endl;
                            }
                            //myfile<<cortable[x][y]<<endl;
                        }
                    }

                    myfile << "max value = " <<max_coef(cortable,wincol,winrow,m,n)<<endl;//myfile << "Writing this to a file.\n";
                   /*max_coef_point[c][r].first=m;*/
                   /*max_coef_point[c][r].second=n;*/
                    myfile<<m<<" "<<n<<endl;
                }
                if((r-32)<=0&&(r+32)>=totrow2)
                {
                    cerr<<"too small image";
                }
            }
            if((c-32)<=0&&(c+32)>=totcol2)//bad image!!
            {
                cerr<<"too small image";
            }
        }
    }
/*    max_points=max_coef_point;*/
    myfile.close();
}
//core function of 2D_PIV
#endif // _2DALGO_HPP_
