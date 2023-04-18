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


double n[7];
int finalone[800][800];
int size = 800;
int points[60];

double ideal1;
double ideal;
double ideal4;
double t1; 
double t2;


struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
 
        if (hash1 != hash2)
        {
            return hash1 ^ hash2;             
        }
         

          return hash1;
    }
};





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
            //stringstream s;
            //s << "(" << setprecision(17) << getX() << "," << setprecision(17) << getY() << ")";
            cout<<setprecision(17);
            cout<<"(" << getX() << "," << getY() << ")";

        }
        
};

point iax; point iay;
point jax; point jay;


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


double dist(point p1, point p2)
{
    return sqrt(((p1.getX() - p2.getX())*(p1.getX() - p2.getX())) + ((p1.getY() - p2.getY())* (p1.getY() - p2.getY())));
}


bool part0()
{
    
    srand(time(NULL));
    string decision;
    
    cout<< "Do you want to generate the points or get information from file? ('Y' or 'N' or 'F' (file input) ) ";
    cin >> decision;
    
    if (decision == "Y" )
    {
        int howmany;
        cout<< "How many points? ";
        cin>>howmany;
        
        ofstream myfile;
        myfile.open("points.txt");
        myfile<< setprecision(23);
        for(int i = 0; i<howmany; i++)
        {
            myfile << (double)rand() / (double)RAND_MAX<<" "<<(double)rand() / (double)RAND_MAX<<endl ;
        }
        
        return true;
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
    
        
        
        
     
        return true;
        
    }
    else
    {
        return false;
            
    }
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
    
    freopen("points.txt", "r", stdin);
    while (cin>> a >> b)
    {
        allp.push_back(point(a,b));
    }
    
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();
    
    double minDis = 1000;
    point x;
    point y;
     
    for(list<point>::iterator it1 = allp.begin(); it1 != allp.end(); ++it1)
    {
        list<point>::iterator it2 = it1; it2++;
        for(list<point>::iterator it = it2; it != allp.end(); ++it)
        {
            
            point b = *it1;  point c = *it;        
            double temp = dist(b,c);
            
            if (temp<minDis)
            {
                minDis = temp;
                ideal1 = minDis;
                x = b; y = c;
            }    
            temp = 0;
        }
            
    } 
    iax = x;
    iay = y;
    cout << "Closest Points: ";
    x.toS();
    cout << " and ";
    y.toS();
    cout<< " Distance: "<<dist(x,y)<<endl;
    
   
    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
   
    t1 = elapsed_seconds.count();
    cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    cout << endl;
    
    
    
    
     
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



double msmall(vector<point> &allp, double left, double right, int BET)
{ 


    if ((right-left +1)<= 3)
    {
        double minArea = 10000;
        for(int i = left; i< right + 1; i++)
        {
            for(int j = i+1; j<right+1; j++)
            {
                double len = dist(allp[i],allp[j]);
                
                minArea = min(minArea,len );
                if(len < ideal)
                {
                    jax = allp[i];
                    jay = allp[j];
                    ideal = len;
                }
            }
          
        }
        return minArea;  
    }
    double mid = (left + right)/2;
    
    double ll = msmall(allp, left, mid,BET);
    double rr = msmall(allp, mid + 1, right,BET);
    double def = min(ll, rr);
    
    
    vector<point> cheepy;
    for(int i = left; i<= right; i++ )
    {
        if (abs(allp[i].getX() - allp[mid].getX()) < def)
        {
            cheepy.push_back(allp[i]);
        }
    }
      
    
    if (BET == 0)
    {
    double intervalmax = 1000;
    for (int i = 0; i<int(cheepy.size());i++)
    {
        for(int j = i+1; j<int(cheepy.size());j++)
        {
            double len = dist(cheepy[i],cheepy[j]);
            intervalmax = min(intervalmax, len);
            
            if(len < ideal){
                jax = cheepy[i];
                jay = cheepy[j];
                ideal = len;
            }
        }
    }
    
    return min(def, intervalmax); 
    }
    
    
    if (BET == 1)
    {
    
    
    sort(cheepy.begin(), cheepy.end(),[](point ii, point jj)
    {  return ii.getY() < jj.getY();  });    
     
    double intervalmax = 1000;
    for (int i = 0; i<int(cheepy.size());i++)
    {
        for(int j = i+1; j<min(i+7,int(cheepy.size()));j++)
        {
            double len = dist(cheepy[i],cheepy[j]);
            intervalmax = min(intervalmax, len);
            
            if(len < ideal){
                jax = cheepy[i];
                jay = cheepy[j];
                ideal = len;
            }
        }
    }
    

    return min(def, intervalmax); 
    }
    return -1;
} 















  



void part2()
{
    ideal = 1000;
    vector<point> allp;
    double a, b;
    ifstream teheh("points.txt");
    
    while (teheh>> a >> b)
    {
        allp.push_back(point(a,b));
    }
    
    point x;
    point y;
    

    sort(allp.begin(), allp.end(),[](point ii, point jj)
    {  return ii.getX() < jj.getX();  });

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();
    

    msmall(allp, 0, allp.size() - 1,0);
    
    
    x = jax;
    y = jay;
    iax = x;
    iay = y;
    ideal1 = ideal;
    cout<<setprecision(17);
    cout << "Part 2 Recursive: Closest Points: ";
    x.toS();
    cout<< " and ";
    y.toS();
    cout<< " Distance: "<<ideal<<endl;

  
    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    t1 = elapsed_seconds.count();
    cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    cout << endl;
    
    
 
}   




void part3()
{
    ideal = 1000;
    vector<point> allp;
    double a, b;
    ifstream teheh("points.txt");
    
    while (teheh>> a >> b)
    {
        allp.push_back(point(a,b));
    }
    
    point x;
    point y;
    

    sort(allp.begin(), allp.end(),[](point ii, point jj)
    {  return ii.getX() < jj.getX();  });

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();
    

    msmall(allp, 0, allp.size() - 1,1);
    
    
    x = jax;
    y = jay;
    cout<<setprecision(17);
    cout << "Part 3 Recursive: Closest Points: ";
    x.toS();
    cout << " and ";
    y.toS();
    cout << " Distance: "<<ideal<<endl;

  
    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    t1 = elapsed_seconds.count();
    cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    cout << endl;
 
    iax = x;
    iay = y;
    ideal1 = ideal;
   
}

vector<point> shuffle (vector<point> allp)
{
   int vlen = allp.size()-1;
    
    int idx;
    for(int i = 0; i<vlen;i++)
    {
        idx =int( i + ( rand() % (  vlen - i + 1 ) ));
        swap(allp[i],allp[idx]);
    
    }
    return allp;
}

vector<point> rf ()
{
    
    vector<point> allp;
    double a, b;
    ifstream teheh("points.txt");
    
    while (teheh>> a >> b)
    {
        
        allp.push_back(point(a,b));
    }
    
    return allp;
    
}

tuple <point, point,double> randomAlgo(vector<point> &allp)
{
  
    
    double dis = dist(allp[0],allp[1]);
    double cha = dis/2;
    
    point best1 = allp[0];
    point best2 = allp[1];
    
    unordered_map<pair<long long, long long>, point, hash_pair> subsq;
    
    subsq[make_pair((long long)(allp[0].getX() / cha),
    (long long)(allp[0].getY() / cha))] = allp[0];
    
    subsq[make_pair((long long)(allp[1].getX() / cha),
    (long long)(allp[1].getY() / cha))] = allp[1];
    

    
    
    for(int i = 0; i<allp.size();i++)
    {
        double tempX = (long long)(allp[i].getX() / cha);
        double tempY = (long long)(allp[i].getY() / cha);
        double tempDis = dis;
        
        if (i<2)
        {
            subsq[make_pair(tempX,tempY)] = allp[i];
        } 
        
        else
        {
            for(int j = -2; j<3; j++)
            {
                for (int k = -2; k<3;k++)
                {
                    
                    long long squareX = tempX + j;
                    long long squareY = tempY + k;
                    
                    pair<long long, long long> squareT = make_pair(squareX,squareY);
                    if ((subsq.count(squareT)>=1))
                    {
                        
                        double bigger = dist(subsq[squareT], allp[i]);
                        if (bigger < tempDis)
                        {
                            best1 = allp[i];
                            best2 = subsq[squareT];
                            
                            tempDis = bigger;
                        }
                    }
                    
                }
            }
            
            if (tempDis<dis)
            {
                dis = tempDis;
                subsq.clear();
                cha = tempDis/2;
               
                for (int n = 0; n<i+1;n++)
                {
                    long long xxx = (long long)(allp[n].getX()/cha);
                    long long yyy = (long long)(allp[n].getY()/cha);
                    subsq[make_pair(xxx,yyy)] = allp[n];
                    
                }
            }
            
            else
            {
                subsq[make_pair(tempX,tempY)] = allp[i];
                
            }
            
        }
      
        
        
    }
       
    
    
    
    
    
    return make_tuple(best1,best2,dis);
    
}


void part4()
{
    vector<point> allp = rf();
    allp = shuffle(allp);  
    
    chrono::time_point<chrono::system_clock> start;
    start = chrono::system_clock::now();
    
    tuple <point, point,double> best = randomAlgo(allp);
    
    chrono::time_point<chrono::system_clock> end;
    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    t2 = elapsed_seconds.count();
    
    cout<<setprecision(17);
    cout << "Part 4 Algorithm: Closest Points: ";
    (get<0>(best)).toS();
    cout<< " and ";
    (get<1>(best)).toS();
    cout<< " Distance: "<<(get<2>(best))<<endl;
    
    ideal = elapsed_seconds.count();
    cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    
    
    jax = get<0>(best);
    iay = get<1>(best);
    ideal4 = get<2>(best) ;
    
    
    
    
}







int main()
{
    

        //Change value of number in part0 to determine amount of points generated
       
        part0();
       //part1();
       //part2();
        part3();
        part4();
   
        ofstream gfile;
        gfile.open("results.txt");
        
        gfile << setprecision(17);
        gfile << "(Full-Recursive) Closest Points: ";
        gfile<<"(" << iax.getX() << "," << iax.getY() << ")";
        gfile << " and ";
        gfile<<"(" << iay.getX() << "," << iay.getY() << ")";
        gfile<< " Distance: "<<ideal1<<endl;
        gfile << "elapsed time: " << t1 << "s\n";
    
        gfile << "(Algorithm) Closest Points: ";
        gfile<<"(" << jax.getX() << "," << jax.getY() << ")";
        gfile<< " and ";
        gfile<<"(" << jay.getX() << "," << jay.getY() << ")";
        gfile<< " Distance: "<<ideal4<<endl;
        gfile << "elapsed time: " << t2 << "s\n";
            
       
        
    
}
