#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <list>
#include <algorithm>
#include <cfloat>
#include <set>
#include <stack>
#include <math.h>

using namespace std;

vector<vector<vector<double>>> inp;
vector<vector<vector<double>>> gray;
vector<vector<vector<double>>> canEdge;
vector<vector<vector<double>>> sobel2;
vector<vector<vector<double>>> maxSuppression;

vector<vector<vector<double>>> finalImage;
int sizeX; 
int sizeY;


string input = "image.ppm"; string output = "image1.ppm";
string typeP;
int bright;

class point
{
    private:
        double X, Y;
    public:
        point () {}
        point(double a, double b)
        {
            X = a;
            Y = b;
        }
        double getX(void)
        {
            return X;
        }
        double getY(void)
        {
        return Y;
        }
    
        void setX(double x)
        {
            X=x;
        }
        void setY(double x)
        {
            Y=x;
        }
        void toS()
        {
            cout<<setprecision(17);
            cout<<"(" << getX() << "," << getY() << ")";
        }   
};


void ppmFinishUp(string file, vector<vector<vector<double>>> finalone, int num)
{
    ofstream wfile;wfile.open(file);
    string header = "P3 "+ to_string(sizeY) + " " + to_string(sizeX) + " " + to_string(num);
    wfile <<header<<endl;
    
    for(int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {   
                wfile<<finalone[i][j][0]; wfile<<" ";
                wfile<<finalone[i][j][1]; wfile<<" ";
                wfile<<finalone[i][j][2]; wfile<<" ";
        }
      wfile <<"\n";
    }
    wfile.close();                           
}


vector<vector<vector<double>>> ppmSetup()
{
    vector<vector<vector<double>>> finalone; finalone.resize(sizeX);
    for (int i = 0; i<sizeX;i++)
    {   finalone[i].resize(sizeY);  }
    
    for(int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
            for (int k = 0; k <3; k++)
            {    finalone[i][j].push_back(0);  }    
        }
    }
    return finalone;
}

vector<vector<vector<double>>> cppmSetup()
{
    vector<vector<vector<double>>> finalone;
    finalone.resize(sizeX);
    
    for (int i = 0; i<sizeX;i++)
    {   finalone[i].resize(sizeY);  }
    cout<<1<<endl;
    for(int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
            for (int k = 0; k <3; k++)
            { finalone[i][j].push_back(0);  } 
        }
    }
    //cout<<"here: "<<finalone.size()<<" "<<finalone[0].size()<<endl;
    return finalone;
}


void ppmSetupFile(string file, vector<vector<vector<double>>>& finalone)
{
    ifstream inp(file);
    string gg; int ff; 
    inp>> gg>> sizeY>>sizeX>>ff;
    typeP = gg; bright = ff;
    finalone.resize(sizeX);
  
    for (int i = 0; i<sizeX;i++)
    {   finalone[i].resize(sizeY);  }
    
    for(int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
            for (int k = 0; k <3; k++)
            {
               int temp; inp >> temp;  finalone[i][j].push_back(temp); 
            }
        }
    }   
}



void grayScale(string fileName){
    
    for(int i = 0; i<sizeX; i++)
    {
        for(int j = 0; j<sizeY; j++)
        {
            int grey = (inp[i][j][0] + inp[i][j][1] + inp[i][j][2])/3;
            gray[i][j][0] = grey; gray[i][j][1] = grey; gray[i][j][2] = grey;
        }
    }
}

void cannyEdge(string ppm)
{
    vector<vector<double>> xDir = {{1,0,-1},{2,0,-2},{1,0,-1}};
    vector<vector<double>> yDir = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    for(int i = 1; i< sizeX-1;i++)
    {
        for(int j = 1; j<sizeY-1; j++)  
        {
            int gx = 0; int gy = 0;
            for(int x = -1; x <2; x++)
            {
                for(int y = -1; y<2; y++)
                {
                    int xchange = xDir[x+1][y+1]; int ychange = yDir[x+1][y+1];
                    gx = gx + gray[i+x][j+y][0] * xchange; gy = gy + gray[i+x][j+y][0] * ychange;
                }
            }
            if ((gx*gx+gy*gy)>9000)
            { canEdge[i][j][0] = bright; canEdge[i][j][1] = bright; canEdge[i][j][2] = bright; }
        }
    }
}

void fillaround(int row, int col, vector<int> &ppm)
{
     if (row < 0 || row >= sizeX || col < 0 || col >= sizeY || ppm[row * sizeY + col] == 1 || ppm[row * sizeY + col] == 0){
        return;
    }
    ppm[col*sizeY + row];
    for (int i = -1; i<2; i++)
    {
        for(int j = -1; j<-1;j++)
        {
            if (!(i== 0 && j == 0)){
                fillaround(row + i, col + j, ppm);
            }
        }
    }
    
    
}


void edgeCheck2(string ppm, int thresh1, int thresh2)
{
    if (thresh1 > thresh2) { int temp = thresh2; thresh2 = thresh1; thresh1 = temp;}
    
    vector<point> pps;
    vector<int> ddgs;
    
    vector<vector<double>> xDir = {{1,0,-1},{2,0,-2},{1,0,-1}};
    vector<vector<double>> yDir = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    for(int i = 0; i< sizeX;i++)
    {
        for(int j = 0; j<sizeY; j++)  
        {
            
            if(i != 0 && j != 0 && i != sizeX - 1 && j != sizeY - 1){
            int gx = 0; int gy = 0;
            
            
            for(int x = -1; x <2; x++)
            {
                for(int y = -1; y<2; y++)
                {
                    int xchange = xDir[x+1][y+1]; int ychange = yDir[x+1][y+1];
                    gx = gx + gray[i+x][j+y][0] * xchange; gy = gy + gray[i+x][j+y][0] * ychange;
                }
            }
           // cout<<i<<endl;
            if ((gx*gx+gy*gy)>thresh2)
            {
            //  cout<<"not first"<<endl;
              sobel2[i][j][0] = 3; 
              sobel2[i][j][1] = 3; 
              sobel2[i][j][2] = 3; 
             // cout<<"not second"<<endl;
              pps.push_back(point(i,j));
            }
            else if ((gx*gx+gy*gy)>thresh1)
            {
             // cout<<"eenot first"<<endl;
              sobel2[i][j][0] = 2; 
              sobel2[i][j][1] = 2; 
              sobel2[i][j][2] = 2;       
              //cout<<"eenot firs44444st"<<endl;
             }   
            }
            //cout<<"issue w ddgs";
            ddgs.push_back(sobel2[i][j][0]);
        }
    }
    cout<<"escaped the sim";
    
    for (point p : pps)
    {
        p.toS();
        fillaround(p.getX(), p.getY(), ddgs);
    }
    
    for (int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
                
            if (sobel2[i][j][0] != 1)
            {
                sobel2[i][j][0] == 0;
                sobel2[i][j][1] == 0;
                sobel2[i][j][2] == 0;
            }
        }
    } 
}


void suppressionAlgo (string fileName, int thresh1, int thresh2)
{
    if (thresh1 > thresh2) { int temp = thresh2; thresh2 = thresh1; thresh1 = temp;}
    
    vector<point> pps;
    vector<int> ddgs;
    vector<vector<double>> angles;
    vector<vector<double>> magnitude;
   
    
    angles.resize(sizeX);
    
    for (int i = 0; i<sizeX;i++)
    {   angles[i].resize(sizeY);  }
    
    magnitude.resize(sizeX);
    for (int i = 0; i<sizeX;i++)
    {   magnitude[i].resize(sizeY);  }
    
    vector<vector<double>> xDir = {{1,0,-1},{2,0,-2},{1,0,-1}};
    vector<vector<double>> yDir = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    for(int i = 0; i< sizeX;i++)
    {
        for(int j = 0; j<sizeY; j++)  
        {
            
            if(i != 0 && j != 0 && i != sizeX - 1 && j != sizeY - 1){
            int gx = 0; int gy = 0;
            //cout<<j<<endl;
            for(int x = -1; x <2; x++)
            {
                for(int y = -1; y<2; y++)
                {
                    
                    int xchange = xDir[x+1][y+1]; int ychange = yDir[x+1][y+1];
                    gx = gx + gray[i+x][j+y][0] * xchange; gy = gy + gray[i+x][j+y][0] * ychange;
                }
            }
            if ((gx*gx+gy*gy)>thresh2)
            {
                
                
             // cout<<"not first"<<endl;
              maxSuppression[i][j][0] = 3; 
              maxSuppression[i][j][1] = 3; 
              maxSuppression[i][j][2] = 3; 
              //cout<<"not second"<<endl;
              pps.push_back(point(i,j));
            }
            else if ((gx*gx+gy*gy)>thresh1)
            {
              maxSuppression[i][j][0] = 2; maxSuppression[i][j][1] = 2; maxSuppression[i][j][2] = 2;       
            }   
                magnitude[i][j] = gx*gx + gy*gy;
                angles[i][j] = abs((atan2(gx,gy)*180/3.141529)*45);
        }
            ddgs.push_back(sobel2[i][j][0]);
        }
    }
    cout<<"escaped the sim";
    
    for (point p : pps)
    {
        fillaround(p.getX(), p.getY(), ddgs);
    }
    
    for (int i = 0; i < sizeX; i++)
    {
            for (int j = 0; j < sizeY; j++)
            {
                 if(i != 0 && i != sizeX - 1 && j != 0 && j != sizeY - 1)
                 {
                     if (ddgs[i*sizeY +j] == 2)
                     {
                         ddgs[i*sizeY +j] = 0;
                     }
                     double m1 = 0;
                     double m2 = 0;
                     
                     double tty = angles[i][j]-180;
                     if (tty == 0)
                     {
                         m1 = magnitude[i][j+1];
                         m2 = magnitude[i][j-1];
                     }
                     else if(angles[i][j] == 45)
                     {
                        m1 = magnitude[i - 1][j + 1];
                        m2 = magnitude[i + 1][j - 1];
                     } 
                     else if(angles[i][j] == 90)
                     {
                         m1 = magnitude[i + 1][j];
                         m2 = magnitude[i - 1][j];
                     } 
                     else 
                     {
                         m1 = magnitude[i + 1][j + 1];
                         m2 = magnitude[i - 1][j - 1];
                     }
                     
                     if(magnitude[i][j] >= m1 && magnitude[i][j] >= m2)
                     {
                         maxSuppression[i][j][0] = 1;
                         maxSuppression[i][j][1] = 1;
                         maxSuppression[i][j][2] = 1;
                     } 
                     else
                     {
                         maxSuppression[i][j][0] = 0;
                         maxSuppression[i][j][1] = 0;
                         maxSuppression[i][j][2] = 0;
                     }
                     
                 }
            }
      }
}


void binary()
{
    for(int i = 0; i <sizeX;i++)
    {
        for (int j = 0; j <sizeY; j++)
        {
            if (maxSuppression[i][j][0] !=0 && sobel2[i][j][0] != 0)
            {
                finalImage[i][j][0] = 1;
                finalImage[i][j][1] = 1;
                finalImage[i][j][2] = 1;
            }
            
        }
    }
    
}








void part1()
{

    ppmSetupFile("flower.ppm", inp);  gray = ppmSetup(); canEdge = cppmSetup();
    grayScale("gray"); cannyEdge("canEdge");
    ppmFinishUp("imageg.ppm", gray,255); ppmFinishUp("imagem.ppm", canEdge,1);
    
}

void part2(int argc, char** argv)
{
    int thresh1 = 5000; int thresh2 = 9000;
    
    for (int i = 1; i < argc; i++)
    {
        //cout<< "val: "<<argv[i]<<endl;
        bool m = argv[i] == "f";
        //cout<<m<<endl;
       
        if(string(argv[i]) == "-f")
        { input = argv[i + 1]; }
        else if(string(argv[i]) == "-lt")
        { thresh1 = stoi(argv[i + 1]); }
        else if(string(argv[i]) == "-ht")
        { thresh2 = stoi(argv[i + 1]); }
        if(string(argv[i]) == "-of")
        { output = argv[i + 1];  }
    }

    ppmSetupFile(input, inp); gray = ppmSetup();  sobel2 = cppmSetup();
    
    grayScale("gray");
    edgeCheck2("soble2",thresh1,thresh2);
    ppmFinishUp(output, sobel2,1);
    
}

void part3(int argc, char** argv)
{
    int thresh1 = 150; int thresh2 = 200; string grayFprint = "imageg.ppm"; string print2 = "image2.ppm"; string print3 = "imagef.ppm";
    
    for (int i = 1; i < argc; i++)
    {
        //cout<< "val: "<<argv[i]<<endl;
        bool m = argv[i] == "f";
        cout<<m<<endl;
                
                
        if(string(argv[i]) == "-f")
        { input = argv[i + 1]; }
        else if(string(argv[i]) == "-lt")
        { thresh1 = stoi(argv[i + 1]); }
        else if(string(argv[i]) == "-ht")
        { thresh2 = stoi(argv[i + 1]); }
        if(string(argv[i]) == "-of")
        { output = argv[i + 1];  }
        if(string(argv[i]) == "-fg")
        { grayFprint = argv[i + 1]; }
        if(string(argv[i]) == "-f2")
        { print2 = argv[i + 1]; }
        if(string(argv[i]) == "-ff")
        { print3 = argv[i + 1]; }
    }

    ppmSetupFile(input, inp); 
    
    gray = ppmSetup();  
    grayScale("gray");
    ppmFinishUp(grayFprint, gray,255);
    
    sobel2 = cppmSetup();
    edgeCheck2("soble2",thresh1,thresh2);
    ppmFinishUp(output, sobel2,1);
    
    maxSuppression = cppmSetup();
    finalImage = cppmSetup();
    suppressionAlgo("maxSuppression",thresh1,thresh2);
    
    binary();
    
    cout<<"setup works"<<endl;
    cout<<"sobel 2 good"<<endl;
    
    ppmFinishUp(print2, maxSuppression,1);
    ppmFinishUp(print3, finalImage,1);
    cout<<"done"<<endl;
    
}


int main(int argc, char** argv)
{
     part3(argc, argv);
}