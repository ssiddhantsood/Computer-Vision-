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
#include <unordered_map>
#include <set>
#include <stack>

using namespace std;

vector<vector<vector<double>>> inp;
vector<vector<vector<double>>> gray;
vector<vector<vector<double>>> canEdge;
int sizeX; 
int sizeY;

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



vector<vector<vector<double>>> ppmSetup()
{
    
    vector<vector<vector<double>>> finalone;
    
    finalone.resize(sizeX);
    
    for (int i = 0; i<sizeX;i++)
    {   finalone[i].resize(sizeY);  }
    
    for(int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
            for (int k = 0; k <3; k++)
            {
               finalone[i][j].push_back(0); 
            }
            
        }
    }
    return finalone;
}

vector<vector<vector<double>>> cppmSetup()
{
    
    vector<vector<vector<double>>> finalone;
    cout<<1<<endl;
    finalone.resize(sizeX);
    cout<<1<<endl;
    for (int i = 0; i<sizeX;i++)
    {   finalone[i].resize(sizeY);  }
    cout<<1<<endl;
    for(int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
            for (int k = 0; k <3; k++)
            {
               finalone[i][j].push_back(0); 
            }
            
        }
    }
    cout<<1<<endl;
    return finalone;
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
    cout<<sizeX<<endl;
    cout<<sizeY<<endl;
    vector<vector<double>> xDir = {{1,0,-1},{2,0,-2},{1,0,-1}};
    vector<vector<double>> yDir = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    
    
    
    for(int i = 1; i< sizeX-1;i++)
    {
        for(int j = 1; j<sizeY-1; j++)
            
        {
            int gx = 0;
            int gy = 0;
            
            for(int x = -1; x <2; x++)
            {
                for(int y = -1; y<2; y++)
                {
                    int xchange = xDir[x+1][y+1];
                    int ychange = yDir[x+1][y+1];
                    gx = gx + gray[i+x][j+y][0] * xchange;
                    gy = gy + gray[i+x][j+y][0] * ychange;
                    
                    
                }
            }
            if ((gx*gx+gy*gy)>10000)
            {
                canEdge[i][j][0] = bright;
                canEdge[i][j][1] = bright;
                canEdge[i][j][2] = bright;
               
            }
        }
    }
    
    
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
               int temp; inp >> temp;
               finalone[i][j].push_back(temp); 
            }
        }
    }   
}

void ppmFinishUp(string file, vector<vector<vector<double>>> finalone)
{
    ofstream wfile;wfile.open(file);
    string header = "P3 "+ to_string(sizeY) + " " + to_string(sizeX) + " 255 ";
    wfile <<header<<endl;
    
    for(int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
            
                wfile<<finalone[i][j][0];
                wfile<<" ";
                wfile<<finalone[i][j][1];
                wfile<<" ";
                wfile<<finalone[i][j][2];
                wfile<<" ";
        }
      wfile <<"\n";
    }
    wfile.close();                           
}

/*
Deleted WhichLine, drawCircle, cirBres due to the fact that it wasn't used for this lab
If needed from this point onwards, updates will be needed and make them so they are rgb based
*/

                            
void part1()
{
    
    cout<<"beginning"<<endl;
    ppmSetupFile("image.ppm", inp);
    
    gray = ppmSetup();
    cout<<"ppm file reading good"<<endl;
    canEdge = cppmSetup();
    
    cout<<"basic method setup good "<<endl;
    grayScale("gray");
    cout<<"grayscale good"<<endl;
    
  
    
    
    
    cannyEdge("canEdge");
   
    cout<<"cannyEdge good"<<endl;
 
    ppmFinishUp("imageg.ppm", gray);
    ppmFinishUp("imagem.ppm", canEdge);
    
    cout<<"finished"<<endl;
    
}

int main()
{
    part1();  
}