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
vector<vector<vector<double>>> sobel2;
int sizeX; 
int sizeY;

string input = "image.ppm";
string output = "imagem.ppm";
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


void ppmFinishUp(string file, vector<vector<vector<double>>> finalone)
{
    ofstream wfile;wfile.open(file);
    string header = "P3 "+ to_string(sizeY) + " " + to_string(sizeX) + " 1 ";
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
    cout<<1<<endl;
    cout<<"here: "<<finalone.size()<<" "<<finalone[0].size()<<endl;
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



void part1()
{

    ppmSetupFile("image.ppm", inp);  gray = ppmSetup(); canEdge = cppmSetup();
    grayScale("gray"); cannyEdge("canEdge");
    ppmFinishUp("imageg.ppm", gray); ppmFinishUp("imagem.ppm", canEdge);
    
}

void part2(int argc, char** argv)
{
    int thresh1 = 9500; int thresh2 = 10000;
    
    for (int i = 1; i < argc; i++)
    {
        cout<< "val: "<<argv[i]<<endl;
        bool m = argv[i] == "f";
        cout<<m<<endl;
       
        if(string(argv[i]) == "-f")
        { input = argv[i + 1]; cout<<"ran"<<endl; }
        else if(string(argv[i]) == "-lt")
        { thresh1 = stoi(argv[i + 1]); }
        else if(string(argv[i]) == "-ht")
        { thresh2 = stoi(argv[i + 1]); }
        if(string(argv[i]) == "-of")
        { output = argv[i + 1];  }
    }

    ppmSetupFile(input, inp); 
     //cout<<"here 3"<<endl;
    gray = ppmSetup();
    
    cout<<"here 3"<<endl;
    sobel2 = cppmSetup();
    
    cout<<"setup works"<<endl;
    grayScale("gray");
    edgeCheck2("soble2",thresh1,thresh2);
    
   
    //cout<<"sobel 2 good"<<endl;
    ppmFinishUp(output, sobel2);
    cout<<"done"<<endl;
    
}


int main(int argc, char** argv)
{
    //part1();
    part2(argc, argv); 
}