#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
using namespace std;

double n[7];
int finalone[800][800];
int size = 800;

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



void drawCircle(int xc, int yc, int x, int y)
{
    if (xc+x<size && yc+y<size && xc+x>=0 && yc+y>=0)
    {
        finalone[xc+x][yc+y] = 0;
    }
    if (xc-x<size && yc+y<size&& xc-x>=0 && yc+y>=0)
    {
        finalone[xc-x][yc+y] = 0;
    }
    
    if (xc+x<size && yc-y<size && xc+x>=0 && yc-y>=0)
    {
    finalone[xc+x][yc-y] = 0;
    }
    //------------------
    if (xc-x<size && yc-y<size && xc-x>=0 && yc-y>=0)
    {
        finalone[xc-x][yc-y] = 0;
    }
    if (xc+y<size && yc+x<size&& xc+y>=0 && yc+x>=0)
    {
        finalone[xc+y][yc+x] = 0;
    }
    //------------------
    if (xc-y<size && yc+x<size&& xc-y>=0 && yc+x>=0)
    {
        finalone[xc-y][yc+x] = 0;
    }
    if (xc+y<size && yc-x<size&& xc+y>=0 && yc-x>=0)
    {
        finalone[xc+y][yc-x] = 0;
    }
    if (xc-y<size && yc-x<size&& xc-y>=0 && yc-x>=0)
    {
    finalone[xc-y][yc-x] = 0;
    }
}

void cirBres(int xc, int yc, int r)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(xc, yc, x, y);
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
        drawCircle(xc, yc, x, y);
    }
}


double area(double x1,double y1,double x2,double y2,double x3,double y3)
{
    double a = abs( 0.5*(x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2)));
    return a;
}

bool checkColin(double x1,double y1,double x2,double y2,double x3,double y3)
{
    double a = area(x1,y1,x2,y2,x3,y3);
    if (-0.001<a && a<0.001)
    {
        return true;
    }
    return false;
}

bool checkTriangle(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4)
{
    if (checkColin(x1,y1,x2,y2,x4,y4) || checkColin(x3,y3,x2,y2,x4,y4)|| checkColin(x2,y2,x3,y3,x4,y4))
    {  return false;  }
    
    float base = area(x1,y1,x2,y2,x3,y3);
    float combined = area(x1,y1,x2,y2,x4,y4) + area(x1,y1,x3,y3,x4,y4) + area(x3,y3,x2,y2,x4,y4);
    if ((combined - base)<0.001 && (combined-base>0.001))
    {
        return true;
    }
    
    
    return false;
}

void part1()
{
    srand(time(NULL));
    ofstream myfile;
    ofstream mfile;
    myfile.open("points.txt");
    mfile.open("log.txt");
    myfile<< setprecision(17) << fixed;
    mfile<< setprecision(17) << fixed;
    
    bool colin = true;
    while(colin==true)
    {
        for (int i = 0; i < 6; ++i) {
            n[i] =   (float)rand() / (float)RAND_MAX;
        }
        colin = checkColin(n[0],n[1],n[2],n[3],n[4],n[5]);
    
    }
    
    
    cout << setprecision(3) << fixed;
    cout<<"\n"<<"("<<n[0]<<","<<n[1]<<")"<<", "<<"("<<n[2]<<","<<n[3]<<")"<<", "<<"("<<n[4]<<","<<n[5]<<")\n";
    
    mfile<<"First 3 points: "<<"("<<n[0]<<","<<n[1]<<") , " <<"("<<n[2]<<","<<n[3]<<") , " << "("<<n[4]<<","<<n[5]<<")"<<"\n";
    
    
    bool trianglesWork = false;
    while (trianglesWork == false)
    {

        n[6] = (double)rand()/RAND_MAX;
        n[7] = (double)rand()/RAND_MAX;
        
        cout<<"Testing point: "<<"("<<n[6]<<","<<n[7]<<")"<< "\n";
        mfile<<"Testing point: "<<"("<<n[6]<<","<<n[7]<<")"<< "\n";
        
        if (checkTriangle(n[0],n[1],n[2],n[3],n[4],n[5],n[6],n[7])==false)
        {
            trianglesWork = true;
        }
        
    
    }
    cout<<"("<<n[6]<<","<<n[7]<<")"<<"\n\n";
    
    myfile<<"("<<n[0]<<","<<n[1]<<") , " <<"("<<n[2]<<","<<n[3]<<") , " << "("<<n[4]<<","<<n[5]<<") , " <<"("<<n[6]<<","<<n[7]<<")";
    
    
        
}






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
            stringstream s;
            s << "(" << setprecision(17) << getX() << "," << setprecision(17) << getY() << ")";
            return s.str();
        }
        
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
       

vector<point> readFile(string file, vector<point> allp)
{
    ifstream input(file);
    string s1 = "";
    string s2 = "";
    for (string line; getline(input, line);)
    {
        s1 = s1 + line;
    }
    for (char i : s1)
    {
        if (s2.size() == 0 && i != '(')
        {
        }
        else if (i != ')')
        {
            s2 += i;
        }
        if (i == ')')
        {
            int loc = s2.find(",", 0);
            
                        
            allp.push_back(point(stod(s2.substr(1, loc - 1)), stod(s2.substr(loc + 1, s2.size()))));
            s2.erase();
        }
    }
    return allp;
}


double dist(point p1, point p2)
{
    return sqrt(((p1.getX() - p2.getX())*(p1.getX() - p2.getX())) + ((p1.getY() - p2.getY())* (p1.getY() - p2.getY())));
}

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
Line getPerp(Line x,point p){
    if(x.getdy() == 0)
    {
        double y = (x.getchange() - x.getdy() * p.getX()) / x.getdx();
        return Line(point(p.getX(), y), p);
    }
    else if(x.getdx() == 0)
    {
        double ee = (x.getchange() - x.getdx() * p.getY()) / x.getdy();
        return Line(point(ee, p.getY()), p);
    }
    else
    {
        double m = x.getdx() / x.getdy();
        return Line(point(p.getX() + 1, p.getY() + m), p);
    }
}

vector<point> square(point a, point b, point c, point d)
{
    vector<point> sq;
    
    Line line1 = Line(a, c);
    Line perp = getPerp(line1,b);
    
    double m = -1 * (perp.getdy() / perp.getdx());
    point e = point(b.getX() + dist(c, a) / sqrt(pow(m, 2) + 1), b.getY() + m * (dist(c, a) / sqrt(pow(m, 2) + 1)));
    
    Line lineE = Line(e, d);
    
    Line perp1 =  getPerp(lineE,a);
    Line perp2 =  getPerp(lineE,c);
    Line perp3 = getPerp(perp1,b);
    
    point pA = intersect(lineE,perp1);
    point pB = intersect(lineE, perp2);
    point pC = intersect(perp3,perp1);
    point pD = intersect(perp3,perp2);
   
    sq.push_back(pA);
    sq.push_back(pB);
    sq.push_back(pC);
    sq.push_back(pD);
    
    return sq;
}
vector<int> extendLine(point a, point b)
{
    vector<int> she;
    double slope = (a.getY()-b.getY())/(a.getX()-b.getX());
    

  
    double yintercept = (a.getY() - slope*a.getX());
    double xint = (0-yintercept)/slope;
    double x1 = (1 - yintercept)/slope;
    double y1 = slope + yintercept;
    
    point p1;
    point p2;
    
    if(slope>0)
    {
        
    
    if (yintercept >= 0)
    {
       p1 = point(yintercept, 0);
    }
    else
    {
       p1 = point(0, xint);
    }
    
    if(x1 <1)
    {
       p2 = point(1,x1);
    }
    
    else
    {
       p2 = point(1,x1);
    }
    }
    
    else{
        if (yintercept <1)
        {
            p1 = point(yintercept, 0);
        }
        else
        {
            p1 = point(1, x1);
        }
        if (xint<1)
        {
            p2 = point(0, xint);
        }
        else
        {
            p2 = point(y1, 1);
        }
    }
    
    
    she.push_back(int(p1.getX()*800));
    she.push_back(int(p1.getY()*800));
    she.push_back(int(p2.getX()*800));
    she.push_back(int(p2.getY()*800));
    
    cout << p1.getX() << " " << p1.getY() << " " <<p2.getX() << " " << p2.getY() << endl;
    return she;
}



void part2()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            finalone[i][j] = 1;
        }
    }
    
    
    
    
    
    vector<point> allp;
    allp = readFile("points.txt", allp);
    
    
    
    
    vector <vector<point>> sq;
    vector <double> areas;
    
    vector<int> nums = {0, 1, 2, 3};
    
    int perms[6][4] = {{0,2,1,3}, {0,1,2,3}, {0,1,3,2},{0,3,1,2} ,{0,3,2,1},{0,2,3,1} };

    double smallval = 10000;
    
   
    int smallindex = 1000;
    for(int i = 0; i<6; i++)
    {
        vector<point> ss= square(allp[perms[i][0]],allp[perms[i][1]],allp[perms[i][2]],allp[perms[i][3]]);
        double aa = dist(ss[0],ss[1]) *  dist(ss[0],ss[1]);
        sq.push_back(ss);
        areas.push_back(aa);
        
        if (aa<smallval)
        {
            smallindex = i;
            smallval = aa;
            
        }
    }
    
     ofstream myfile;
     myfile.open("sampleoutput.txt");
     myfile<<allp[0].toS()<<" , "<<allp[1].toS()<<" , "<<allp[2].toS()<<" , "<<allp[3].toS()<<endl;
    
     myfile<<sq[smallindex][0].toS()<<" , "<<sq[smallindex][1].toS()<<" , "<<sq[smallindex][2].toS()<<" , "<<sq[smallindex][3].toS()<< " Area = " << areas[smallindex];
     
     ofstream thefile;
     thefile.open("output.txt");
     thefile<<setprecision(17);
    
    thefile<<allp[0].toS()<<" , "<<allp[1].toS()<<" , "<<allp[2].toS()<<" , "<<allp[3].toS()<<endl;
    
    
    
     for(int i = 0; i<6;i++)
     {
         for(int j = 0; j<4; j++)
         {
             thefile<<sq[i][j].toS();
               
             if (j == 3)
             {
                 thefile<< " Area = " << areas[i] << "\n";
             }
             else
             {
                 thefile<<" , ";
             }
            
            
                
            
         }
     }
    
    
    
    
    
     //Draw everything
     for(point p: allp)
     {
         cirBres(round(p.getY()*800),round(p.getX()*800),2);
     }
    
     for(point p: sq[smallindex])
     {
        cirBres(round(p.getY()*800), round(p.getX()*800),5);
     }
    
    vector<int> a = extendLine(sq[smallindex][0], sq[smallindex][1]);
    vector<int> b = extendLine(sq[smallindex][3], sq[smallindex][1]);
    vector<int> c = extendLine(sq[smallindex][0], sq[smallindex][2]);
    vector<int> d = extendLine(sq[smallindex][3], sq[smallindex][2]);
    
    whichline((int)a[0],(int)a[1],(int)a[2],(int)a[3]);
    whichline((int)b[0],(int)b[1],(int)b[2],(int)b[3]);
    whichline((int)c[0],(int)c[1],(int)c[2],(int)c[3]);
    whichline((int)d[0],(int)d[1],(int)d[2],(int)d[3]);
   
    ofstream wfile;
    wfile.open("output.ppm");
    wfile <<"P3 800 800 1 "<<endl;
    for(int i = 0; i < size; i++)
    {
       
        for (int j = 0; j < size; j++)
        {
            wfile<<finalone[i][j];
            wfile<<" ";
            wfile<<finalone[i][j];
            wfile<<" ";
            wfile<<finalone[i][j];
            wfile<<" ";
            
        }
      wfile <<"\n";
    }
    wfile.close();
    
}

int main()
{
   
    //part1();
    part2();
    
    return 3;
}
