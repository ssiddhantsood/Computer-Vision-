#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>

using namespace std;
using namespace cv;

//Create Default names:
string fileInput = "./coins.jpg";
string outGrey = "./imageg.jpg";
string outF = "./imagef.jpg";
string outCircles = "./imageCircles.jpg";
string outCoins = "./imageCoins.jpg";

int threshold1 = 30;
int threshold2 = 36;
int rad = 3;
int canMultiplier = 3;
int middleS = 90;
int bigR = 115;
int smallR = 68;
int totalCheck = 55;
int minimumK = 105;


vector<Vec3f> circles;  
Mat image;



//Global variables
vector<Vec3f> coinsValues;

void readFiles(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {       
        if(string(argv[i]) == "-f")
        { fileInput = argv[i + 1]; }
        if(string(argv[i]) == "-lt")
        { threshold1 = stoi(argv[i + 1]); }
        if(string(argv[i]) == "-ht")
        { threshold2 = stoi(argv[i + 1]); }
        if(string(argv[i]) == "-rad")
        { rad = stoi(argv[i + 1]);  }
        if(string(argv[i]) == "-cm")
        { canMultiplier = stoi(argv[i + 1]); }
        if(string(argv[i]) == "-ms")
        { middleS = stoi(argv[i + 1]); }
        if(string(argv[i]) == "-br")
        { bigR = stoi(argv[i + 1]); }
        if(string(argv[i]) == "-sr")
        { smallR = stoi(argv[i + 1]); }
        if(string(argv[i]) == "-c")
        { totalCheck = stoi(argv[i + 1]); }
        if(string(argv[i]) == "-m")
        { minimumK = stoi(argv[i + 1]); }
    }
}

void getAllImages()
{
    image = imread(fileInput);
    Mat gray = imread(fileInput, IMREAD_GRAYSCALE);
    Mat canny;
    
    blur(gray, canny, Size(3,3));
    Canny(canny, canny, threshold1, threshold2*rad,canMultiplier);
    
    imwrite(outGrey, gray);
    imwrite(outF, canny);
 
    medianBlur(gray, gray, 5);
     
    
    if (120<bigR)
    {
        HoughCircles(gray, circles, HOUGH_GRADIENT,1, middleS, threshold2, totalCheck, smallR, 120);
        if (159<bigR)
        {
            vector<Vec3f> bigCircles;
            HoughCircles(gray, bigCircles, HOUGH_GRADIENT, 1, middleS, threshold2, max(totalCheck-30, 0), 160, bigR);
            circles.insert(circles.end(), bigCircles.begin(), bigCircles.end());
        }
    }
    else
    {
        HoughCircles(gray, circles, HOUGH_GRADIENT, 1, middleS, threshold2, totalCheck, smallR, bigR);
    }
    
}


vector<double> aRGB(Mat src, double radius, Point center)
{
    vector<double> rgb(3); 
    Rect roi(center.x - radius, center.y - radius, radius * 2, radius * 2);
    
    Mat roiMat = src(roi);
    Scalar meanScalar = mean(roiMat);
    
    for(int i = 0; i <3; i++){
        rgb[i] = meanScalar[2 - i];
    }
    
    vector<double> ret(2); 
    ret[0] = rgb[0];
    ret[1] = rgb[1];
    return ret;
}

void sortCoins()
{
    Mat copyImage = imread(fileInput);
    int p = 0; int n = 0; int d = 0; int q = 0; int sd = 0;
    Scalar colors[5] = { Scalar(0,0,255)  ,
                         Scalar(128,0,128), 
                         Scalar(255,0,0)  , 
                         Scalar(0,255,0)  , 
                         Scalar(0,255,255)};

    for (int i=0; i<circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int diameter = cvRound(circles[i][2]);
        
        vector<double> avg = aRGB(image,diameter/4,center);
        double averageRed = avg[0];
        double averageBlue = avg[1];
        int tVal;
        if (diameter<89)
        {
            if (averageRed>1.1*averageBlue)
            {
                p = p +1; tVal = 0;
            } 
            else
            {
                d = d +1; tVal = 2;
            }
        } 
        else if ( diameter < minimumK )
        {
            if ( averageRed < 1.3*averageBlue )
            { 
                n = n + 1; tVal = 1;
            }
            else
            {
                p = p + 1; tVal = 0;
            }
        } 
        else if ( diameter < 121 )
        {
            q = q +1;
            tVal = 3;
        } 
        else if (diameter > 160) 
        {
            sd = sd + 1;
            tVal = 4;
        }
        circle(image,center,3,colors[tVal],-1,8,0 );
        circle(copyImage,center,3,Scalar(0,0,255),-1,8,0);
        
        for (int j = 0; j < 4; j++ )
        {
            circle( copyImage , center , diameter+j , Scalar(0,0,255) , 3 , 8 , 0 );
            circle( image , center , diameter+j , colors[tVal] , 3 , 8 , 0 );
        }
    }
    
    double sumOfCoins = p*0.01 + d * .1 + n*.05 + q*0.25 + sd * .5;
    imwrite( "./imageCoins.jpg", image );
    imwrite( "./imageCircles.jpg", copyImage );
    ofstream myfile;
    myfile.open("./results.txt", ios::out);
    myfile << p << " Pennies" << endl;
    myfile << n << " Nickels" << endl;
    myfile << d << " Dimes" << endl;
    myfile << q << " Quarters" << endl;
    myfile << sd << " Silver Dollars" << endl;
    myfile << "Total: $" << sumOfCoins << endl;
    
    cout << p << " Pennies" << endl;
    cout << n << " Nickels" << endl;
    cout << d << " Dimes" << endl;
    cout << q << " Quarters" << endl;
    cout << sd << " Silver Dollars" << endl;
    cout << "Total: $" << sumOfCoins << endl;
    
    
}

void part1(int argc, char** argv)
{
    readFiles(argc, argv);
    getAllImages();
    sortCoins();
}


int main(int argc, char** argv)
{
    part1(argc, argv);
}
