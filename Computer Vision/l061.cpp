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
vector<vector<double>> angles;

vector<vector<vector<double>>> imageCC;
vector<vector<vector<double>>> imageV;

vector<vector<vector<double>>> finalImage;
int sizeX; int sizeY;
int TC = 200;
string strImageV = "imagev.ppm";
string strImageCC = "imageCC.ppm";

int threshold1 = 1225;
int threshold2 = 122500;


string input = "image.ppm"; 
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
    /*
        void toS()
        {
            cout<<setprecision(17);
            cout<<"(" << getX() << "," << getY() << ")";
        }   
        
    */
};

class Line{
    private: 
        point p1, p2;
        double dy, dx, change;
    public:
        Line(){
            
        }
        Line(point x, point y)
        {
            p1 = x;
            p2 = y;
            dy = (p2.getY() - p1.getY()) * -1;
            dx = p2.getX() - p1.getX();
            change = dy * p1.getX() + dx * p1.getY();
        }
        point get1()
        {
            return p1;
        }
        point get2()
        {
            return p2;
        }
        double getdy()
        {
            return dy;
        }
        double getdx()
        {
            return dx;
        }
        double getchange()
        {
            return change;
        }
       
     
        
};



point intersect(Line f,Line w)
{
    double fs = 0;
    double ws = 0;
        
    if (!((abs(f.get2().getX()-f.get1().getX()))<0.001))
    {
         fs = (f.get2().getY()-f.get1().getY())/(f.get2().getX()-f.get1().getX());
    }
    
    if (!((abs(w.get2().getX()-w.get1().getX()))<0.001))
    {
     ws = (w.get2().getY()-w.get1().getY())/(w.get2().getX()-w.get1().getX());
    }
    
    
    
    if (abs(w.get1().getX() - w.get2().getX()<0.0001))
    {
        return point(w.get1().getX(),fs*(w.get1().getX()-f.get1().getX())+f.get1().getY());
    }
    if (fs ==0) {

        return point(f.get1().getX(), ws * (f.get1().getX() - w.get1().getX()) + w.get1().getY());
    }
    
        
        double xx = (fs * f.get1().getX() - ws * w.get1().getX() + w.get1().getY() - f.get1().getY()) / (fs-ws);
        double yy = fs * (xx - f.get1().getX()) + f.get1().getY();
        fs = ws;
        ws = fs;
        return point(xx, yy);
    
}






void whichline(int x1, int y1, int x2, int y2, int r,int g,int b)
{
   bool larger = abs(y2-y1) > abs(x2-x1);
    if(larger) {
        swap(x1, y1);
        swap(x2, y2);
    }
    if(x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }
    int j = y1; int xdiff = abs(x2-x1); int ydiff = abs(y2-y1); int step = 1;
    if(y1 < y2) step = 1;
    else step = -1;
    int error = xdiff - ydiff;
    for(int i = x1; i <= x2; i++) {
        if(larger) {
            if(i < sizeY && i > -1 && j < sizeX && j > -1)
            {
                imageV[j][i][0] = /*imageV[j][i][0]*/ + r;
                imageV[j][i][1] = /*imageV[j][i][1]*/ +b;
                imageV[j][i][2] = /*imageV[j][i][2]*/ +g;
            }
        }
        else {
            if(j < sizeX && j > -1 && i < sizeY && i > -1)
            {
                imageV[j][i][0] = /*imageV[j][i][0]*/ +r;
                imageV[j][i][1] = /*imageV[j][i][1]*/ +b;
                imageV[j][i][2] = /*imageV[j][i][2]*/ +g;
            }
        }
        if(error >= 0) {
            j += step;
            error -= xdiff;
        }
        error += ydiff;
    }
}
vector<int> extendLine(point a, point b)
{
    vector<int> she;
    
    double slope;
    if (abs(b.getX()- a.getX()) <0.0001)
    {
        slope = 0.0;
    }
    else
    {
        slope = (b.getY()-a.getY())/(b.getX()-a.getX());
    }
    
    
    double yintercept = a.getY() *sizeX - slope*a.getX()*sizeX;
    double end = slope * sizeX + yintercept;
    int fx1 = 0;
    int fy1 = int((yintercept));
    int fx2 = sizeY;
    int fy2 = (int)(end);
    
    she.push_back(fx1);
    she.push_back(fy1);
    she.push_back(fx2);
    she.push_back(fy2);
    
    return she;
}


void drawCircle(int xc, int yc, int x, int y, int cc)
{
    if (xc+x<sizeY && yc+y<sizeX && xc+x>=0 && yc+y>=0)  
    {
        imageCC[xc+x][yc+y][0] = cc;
        imageCC[xc+x][yc+y][1] = cc;
        imageCC[xc+x][yc+y][2] = cc;
    }
    if (xc-x<sizeY && yc+y<sizeX&& xc-x>=0 && yc+y>=0)
    {
        imageCC[xc-x][yc+y][0] = cc;
        imageCC[xc-x][yc+y][1] = cc;
        imageCC[xc-x][yc+y][2] = cc;
    }
    
    if (xc+x<sizeY && yc-y<sizeX && xc+x>=0 && yc-y>=0)
    {
        imageCC[xc+x][yc-y][0] = cc;
        imageCC[xc+x][yc-y][1] = cc;
        imageCC[xc+x][yc-y][2] = cc;
    }
    //------------------
    if (xc-x<sizeY && yc-y<sizeX && xc-x>=0 && yc-y>=0)
    {
        imageCC[xc-x][yc-y][0] = cc;
        imageCC[xc-x][yc-y][1] = cc;
        imageCC[xc-x][yc-y][2] = cc;
    }
    if (xc+y<sizeY && yc+x<sizeX && xc+y>=0 && yc+x>=0)
    {
        imageCC[xc+y][yc+x][0] = cc;
        imageCC[xc+y][yc+x][1] = cc;
        imageCC[xc+y][yc+x][2] = cc;
    }
    //------------------
    if (xc-y<sizeY && yc+x<sizeX && xc-y>=0 && yc+x>=0)
    {
        imageCC[xc-y][yc+x][0] = cc;
        imageCC[xc-y][yc+x][1] = cc;
        imageCC[xc-y][yc+x][2] = cc;
    }
    if (xc+y<sizeY && yc-x<sizeX && xc+y>=0 && yc-x>=0)
    {
        imageCC[xc+y][yc-x][0] = cc;
        imageCC[xc+y][yc-x][1] = cc;
        imageCC[xc+y][yc-x][2] = cc;
    }
    if (xc-y<sizeY && yc-x<sizeX && xc-y>=0 && yc-x>=0)
    {
        imageCC[xc-y][yc-x][0] = cc;
        imageCC[xc-y][yc-x][1] = cc;
        imageCC[xc-y][yc-x][2] = cc;
    }
}

void cirBres(int xc, int yc, int r,int cc)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(xc, yc, x, y, cc);
    while (y >= x)
    {
        x=x+1;
        if (d > 0)
        {
            y--;
            d = d + 4 *(x-y) + 10;
        }
        else
            d = d + 4 * x + 6;
        drawCircle(xc, yc, x, y, cc);
    }
}


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
    for(int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
            for (int k = 0; k <3; k++)
            { finalone[i][j].push_back(0);  } 
        }
    }
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
           
            if ((gx*gx+gy*gy)>thresh2)
            {
           
              sobel2[i][j][0] = 3; 
              sobel2[i][j][1] = 3; 
              sobel2[i][j][2] = 3; 
             
              pps.push_back(point(i,j));
            }
            else if ((gx*gx+gy*gy)>thresh1)
            {
             
              sobel2[i][j][0] = 2; 
              sobel2[i][j][1] = 2; 
              sobel2[i][j][2] = 2;       
           
             }   
            }
            ddgs.push_back(sobel2[i][j][0]);
        }
    }
    cout<<"escaped the sim";
    
    for (point p : pps)
    {
        //p.toS();
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
                
                
            
              maxSuppression[i][j][0] = 3; 
              maxSuppression[i][j][1] = 3; 
              maxSuppression[i][j][2] = 3; 
            
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

void findV()
{
   
  for (int i = 1; i<sizeX-1;i++)
    {
        for (int j = 1; j<sizeY-1; j++)
        {
            
            if (gray[i][j][0]) 
            {
                
                int chX = sin(angles[i][j]);
                int chY = cos(angles[i][j]);
                if (!(chX ==0) && !(chY==0))
                {
                    whichline(i, j, i + sizeY * chX, j + sizeX * chY,1,0,0);
                }
            }
        }
    }
    int m = 0;
    cout<<"second part"<<endl;
    for (int i = 0; i < sizeX-1; i++)
    {
        for (int j = 0; j< sizeY-1;j++)
        {
            if(imageV[i][j][0]>m){
                m = imageV[i][j][0];
            }
        }
    }
    
    ppmFinishUp(strImageV, imageV,m);
    ppmFinishUp("imageCC.jpg", imageV,m);
    cout<<"found v";
    
    
}

void vote()
{

   for(int i=0;i<TC;i++)
   {
       int r = i * int(sizeX/TC);
       int finalR = min(sizeX, (i+1)*int(sizeX/TC));
       for (int j = 0; j<TC; j++)
       {
           int c = j*int(sizeY/30);
           int finalC = min(((j+1)*int(sizeY/30)),sizeY);
           double d = 0.0;
           for (int i = r; i<finalR; i++)
           {
               for (int j = c; j<finalC;j++)
               {
                   d = d + imageV[i][j][0];
               }
           }
           d = d / (finalR-r+1)*(finalC-c+1);
           if (d<(TC/2)){ d = 0.75 * TC;}
           else { d = TC;}
          
          for (int i = r; i<finalR; i++)
          {
              for(int j = c; j<finalC; j++)
              {
                  if(imageV[i][j][0]>d)
                  {
                   cirBres( i,  j, 4,255);
                   cirBres( i,  j, 3,255);
                       cirBres( i,  j, 2,255);
                       cirBres( i,  j, 1,255);
                  }
              }
          }
       }
   }
   
   
}



void part3(int argc, char** argv)
{
    threshold1 = 150; int threshold2 = 200; string grayFprint = "imageg.ppm"; string print2 = "image2.ppm"; string print3 = "imagef.ppm";
    
    string output = "image1.ppm";
    
    for (int i = 1; i < argc; i++)
    {
 
                
                
        if(string(argv[i]) == "-f")
        { input = argv[i + 1]; }
        else if(string(argv[i]) == "-lt")
        { threshold1 = stoi(argv[i + 1]); }
        else if(string(argv[i]) == "-ht")
        { threshold2 = stoi(argv[i + 1]); }
        if(string(argv[i]) == "-of")
        { output = argv[i + 1];  }
        if(string(argv[i]) == "-fg")
        { grayFprint = argv[i + 1]; }
        if(string(argv[i]) == "-f2")
        { print2 = argv[i + 1]; }
        if(string(argv[i]) == "-ff")
        { print3 = argv[i + 1]; }
        if(string(argv[i]) == "-TC")
        { TC = stoi(argv[i + 1]); }
        if(string(argv[i]) == "-fv")
        { strImageV = argv[i + 1]; }
        if(string(argv[i]) == "-fcc")
        { strImageCC = argv[i + 1]; }
    }

    ppmSetupFile(input, inp); 
    
    gray = ppmSetup();  
    grayScale("gray");
    ppmFinishUp(grayFprint, gray,255);
    
    sobel2 = cppmSetup();
    edgeCheck2("soble2",threshold1,threshold2);
    ppmFinishUp(output, sobel2,1);
    
    maxSuppression = cppmSetup();
    finalImage = cppmSetup();
    suppressionAlgo("maxSuppression",threshold1,threshold2);
    
    binary();
    
    cout<<"setup works"<<endl;
    cout<<"sobel 2 good"<<endl;
    
    ppmFinishUp(print2, maxSuppression,1);
    ppmFinishUp(print3, finalImage,1);
    cout<<"done w initial"<<endl;
    
    
}

void coinCount(int argc, char** argv)
{
    part3(argc, argv);
    
    cout<<"part3 works"<<endl;

    imageV = ppmSetup(); 
    imageCC = ppmSetup();
    
    findV();
    vote();
    
    ppmFinishUp(strImageCC, imageCC,255);
    //*/
    
}




int main(int argc, char** argv)
{
    coinCount(argc, argv);
    
}