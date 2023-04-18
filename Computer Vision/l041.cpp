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
#include <chrono>
#include <cfloat>
#include <ctime>
#include <unordered_map>

using namespace std;
int finalone[400][400];
int size = 400;

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

vector<point> hull;

void ppmSetup()
{
    for(int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            finalone[i][j] = 0;
        }
    }
}

void ppmFinishUp()
{
    ofstream wfile;
    wfile.open("points.ppm");
    wfile <<"P3 400 400 1 "<<endl;
    for(int i = 0; i < size; i++)
    {
       
        for (int j = 0; j < size; j++)
        {
            if (finalone[i][j] != 0)
            {
                wfile<<finalone[i][j];
                wfile<<" ";
                wfile<<1;
                wfile<<" ";
                wfile<<1;
                wfile<<" ";
            }
            else
            {
                wfile<<0;
                wfile<<" ";
                wfile<<0;
                wfile<<" ";
                wfile<<0;
                wfile<<" ";
            }
        }
      wfile <<"\n";
    }
    wfile.close();                           
}

void whichline(int x1, int y1, int x2, int y2)
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
            if(i < 800 && i > -1 && j < 800 && j > -1) finalone[j][i] = 1;
        }
        else {
            if(j < 800 && j > -1 && i < 800 && i > -1) finalone[i][j] = 1;
        }
        if(error >= 0) {
            j += step;
            error -= xdiff;
        }
        error += ydiff;
    }
}


void drawCircle(int xc, int yc, int x, int y, int cc)
{
    if (xc+x<size && yc+y<size && xc+x>=0 && yc+y>=0)  
    {
        finalone[xc+x][yc+y] = cc;
    }
    if (xc-x<size && yc+y<size&& xc-x>=0 && yc+y>=0)
    {
        finalone[xc-x][yc+y] = cc;
    }
    
    if (xc+x<size && yc-y<size && xc+x>=0 && yc-y>=0)
    {
    finalone[xc+x][yc-y] = cc;
    }
    //------------------
    if (xc-x<size && yc-y<size && xc-x>=0 && yc-y>=0)
    {
        finalone[xc-x][yc-y] = cc;
    }
    if (xc+y<size && yc+x<size&& xc+y>=0 && yc+x>=0)
    {
        finalone[xc+y][yc+x] = cc;
    }
    //------------------
    if (xc-y<size && yc+x<size&& xc-y>=0 && yc+x>=0)
    {
        finalone[xc-y][yc+x] = cc;
    }
    if (xc+y<size && yc-x<size&& xc+y>=0 && yc-x>=0)
    {
        finalone[xc+y][yc-x] = cc;
    }
    if (xc-y<size && yc-x<size&& xc-y>=0 && yc-x>=0)
    {
    finalone[xc-y][yc-x] = cc;
    }
}

void cirBres(int xc, int yc, int r,int cc )
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






double dist(point p1, point p2)
{
    return sqrt(((p1.getX() - p2.getX())*(p1.getX() - p2.getX())) + ((p1.getY() - p2.getY())* (p1.getY() - p2.getY())));
}

vector<point> rf (string filename)
{
    
    vector<point> allp;
    double a, b;
    ifstream teheh(filename);
    
    while (teheh>> a >> b)
    {
        
        allp.push_back(point(a,b));
    }
    
    return allp;
    
}

void generateRandomPoints()
{
    
    srand(time(NULL));
    string decision;
    
 //   cout<< "Do you want to generate the points or get information from file? ('Y' or 'N' or 'F' (file input) ) ";
//    cin >> decision;
    decision = "Y";
    if (decision == "Y" )
    {
        int howmany;
        //cout<< "How many points? ";
        //cin>>howmany;
        howmany = 60;
        
        ofstream myfile;
        myfile.open("points.txt");
        myfile<< setprecision(23);
        for(int i = 0; i<howmany; i++)
        {
            myfile << (double)rand() / (double)RAND_MAX<<" "<<(double)rand() / (double)RAND_MAX<<endl ;
        }
        
    }
    else if (decision =="F")
    {
        
        string ffff;
        cout<< "File Name: ";
        cin>>ffff;
        double a, b;
        ifstream fefe(ffff);
        ofstream myfile;
        myfile.open("points.txt");
        myfile<< setprecision(23);
        while (fefe>> a >> b)
        {
           myfile<<a<<" "<<b<<endl;
        }
    }
    

    
}  
double cp(point p1, point p2, point tempch){
    return (p2.getX() - p1.getX()) * (tempch.getY() - p1.getY()) - (tempch.getX() - p1.getX()) * (p2.getY() - p1.getY());
}

double above(point p, point q, point x)
{
    double val = cp(p,q,x);
    if (val>0)
    {
        return 1;
    }
    else if(val<0)
    {
        return -1;
    }
    return 0;
}



double arctan(point p)
{
    return atan2((p.getY() - 0.5), (p.getX() - 0.5));
}

bool acomp(point x, point y)
{
    return arctan(x) < arctan(y);
}





void findHull(vector<point> &allp, point P, point Q,int x)
{
    double index = -1;
    double distance = 0;
    
    for(int i = 0; i <allp.size();i++)
    {
        
        double tempDis = cp(P,Q,allp[i]);
        int side = above(P,Q,allp[i]);
        
        if(side == x && abs(tempDis) > distance)
        {
            index = i;
            distance = abs(tempDis);
        }
        
       
    
    }
    
    
    
     if (index == -1)
     {
            hull.push_back(P);
            hull.push_back(Q);
            return;
     }
      
    
     
     findHull(allp, allp[index], P, -above(allp[index], P, Q));
     findHull(allp, allp[index], Q, -above(allp[index], Q, P));
        
    
}


void quickHull(vector<point> &allp)
{
    ppmSetup();
    vector<point> convexH;
    
    sort(allp.begin(), allp.end(),[](point ii, point jj)
    {  return ii.getX() < jj.getX();  });    
    
    point l = allp[0],r = allp.back();
    
    findHull(allp, l,r,1);
    findHull(allp, l,r,-1);
    
    
    sort(hull.begin(), hull.end(),[](point ii, point jj)
    {  return acomp(ii,jj);  });   
    
    
    for(int i = 0; i<allp.size();i++)
    {
        double cirX = allp[i].getX()*size;
        double cirY = allp[i].getY()*size;
        
        cirBres(cirX,cirY,3,1);
    }
    
    for(int i = 0; i<hull.size();i++)
    {
        int x1 = int(hull[i].getX()*size);
        int y1 = int(hull[i].getY()*size);
        
        int x2 = int(hull[(i + 1) % hull.size()].getX()*size);
        int y2 = int(hull[(i + 1) % hull.size()].getY()*size);
        
        //cout<<"POINT 1: "<<x1<<" "<<y1<<endl;
        //cout<<"POINT 2: "<<x2<<" "<<y2<<endl;
        whichline(x1,y1,x2,y2);
    }
    
    
    ppmFinishUp();
    
}



void part1()
{
    generateRandomPoints();
    vector<point> allp = rf("points.txt");
    quickHull(allp);
}





int main()
{
    part1();
    
    
    
}