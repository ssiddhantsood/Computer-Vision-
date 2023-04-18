#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <list>

using namespace std;


double n[7];
int finalone[800][800];
int size = 800;
int points[60];
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
        string toS()
        {
            //stringstream s;
            //s << "(" << setprecision(17) << getX() << "," << setprecision(17) << getY() << ")";
            string s = "(" + to_string(getX()) + "," + to_string(getY()) +")";
            return s;
        }
        
};

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
            if(i < 800 && i > -1 && j < 800 && j > -1) finalone[j][i] = 0;
        }
        else {
            if(j < 800 && j > -1 && i < 800 && i > -1) finalone[i][j] = 0;
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





bool part0()
{
    srand(time(NULL));
    string decision;
    
    cout << "Do you want to genrerate the points? (Y or N)"; 
    cin >> decision;
    ofstream myfile;
    myfile.open("points.txt");
    myfile<< setprecision(17);
    
    if (decision == "Y")
    {
        for(int i = 0; i<60; i++)
        {
            myfile << (float)rand() / (float)RAND_MAX<<" "<<(float)rand() / (float)RAND_MAX<<endl ;
        }
        
        return true;
    }
    else
    {
        cout<<"Ok, have a nice day!";
        return false;
            
    }
}

double dist(point p1, point p2)
{
    return sqrt(((p1.getX() - p2.getX())*(p1.getX() - p2.getX())) + ((p1.getY() - p2.getY())* (p1.getY() - p2.getY())));
}

void part1()
{
    
    
     for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            finalone[i][j] = 1;
        }
    }
    
    
    
   
    list<point> allp;
    double a, b;
    
    freopen("points.txt","r",stdin);
    
    while (cin>> a >> b)
    {
        allp.push_back(point(a,b));
    }
    
    double minDis = 1000;
    point x;
    point y;
     
    for(list<point>::iterator it1 = allp.begin(); it1 != allp.end(); ++it1)
    {
        list<point>::iterator it2 = it1; it2++;
        for(list<point>::iterator it = it2; it != allp.end(); ++it)
        {
            
            point b = *it1;
            point c = *it;
            
            
            
            double temp = dist(b,c);
            
            if (temp<minDis)
            {
                minDis = temp;
                x = b;
                y = c;
            }    
            
            temp = 0;
        }
            
     } 
     cout << "Closest Points: " <<  x.toS()<< " and "<< y.toS()<<endl;
    
    
   
    for(list<point>::iterator it = allp.begin(); it != allp.end(); ++it)
    {
      point z = *it;
          
      if ((round(x.getX()*800) == round(z.getX()*800)) || (round(y.getX()*800) == round(z.getX()*800)))
      {   
           cirBres(round(z.getX()*800),round(z.getY()*800),3,255);            
          cirBres(round(z.getX()*800),round(z.getY()*800),2,255);        
      }
      else
      {
           cirBres(round(z.getX()*800),round(z.getY()*800),2,0);
          cirBres(round(z.getX()*800),round(z.getY()*800),3,0);
      }
    }
        
        
       
            
             
    ofstream wfile;
    wfile.open("points.ppm");
    wfile <<"P3 800 800 1 "<<endl;
    for(int i = 0; i < size; i++)
    {
       
        for (int j = 0; j < size; j++)
        {
            if (finalone[i][j] != 1)
            {
                wfile<<finalone[i][j];
                wfile<<" ";
                wfile<<0;
                wfile<<" ";
                wfile<<0;
                wfile<<" ";
            }
            else
            {
                wfile<<1;
                wfile<<" ";
                wfile<<1;
                wfile<<" ";
                wfile<<1;
                wfile<<" ";
            }
            
        }
      wfile <<"\n";
    }
    wfile.close();
    
                                 
    
                                 
                                  
}                        



int main()
{
    part0();
    part1();
        
}