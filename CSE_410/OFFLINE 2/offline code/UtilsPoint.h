#include<iostream>
#include<math.h>
#include<vector>
#include<string>
#include<fstream>
#include <cstdlib>

using namespace std;

double deg2rad(double deg){
    return deg * M_PI / 180.0;
}

class Point{
public:
    double x, y, z;
    Point(){
        x = 0;
        y = 0;
        z = 0;
    }

    Point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Point(double x, double y){
        this->x = x;
        this->y = y;
        this->z = 0;
    }

    Point operator+(const Point &p){
        Point p1;
        p1.x = x + p.x;
        p1.y = y + p.y;
        p1.z = z + p.z;
        return p1;
    }

    Point operator-(const Point &p){
        Point p1;
        p1.x = x - p.x;
        p1.y = y - p.y;
        p1.z = z - p.z;
        return p1;
    }

    Point operator*(const double &d){
        Point p1;
        p1.x = x * d;
        p1.y = y * d;
        p1.z = z * d;
        return p1;
    }

    Point operator/(const double &d){
        Point p1;
        p1.x = x / d;
        p1.y = y / d;
        p1.z = z / d;
        return p1;
    }

    Point operator=(const Point &p){
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }

    Point normalize(){
        double norm = sqrt(x*x + y*y + z*z);
        x /= norm;
        y /= norm;
        z /= norm;
        return *this;
    }

    Point operator-(){
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    double dot(const Point &p){
        return x*p.x + y*p.y + z*p.z;
    }

    Point cross(const Point &p){
        Point p1;
        p1.x = y*p.z - z*p.y;
        p1.y = z*p.x - x*p.z;
        p1.z = x*p.y - y*p.x;
        return p1;
    }

    double length(){
        return sqrt(x*x + y*y + z*z);
    }

    
    friend ostream &operator<<( ostream &output, const Point &p ) { 
        output<< p.x << " " << p.y << " " << p.z << endl;
         return output;            
      }

    friend istream &operator>>( istream  &input, Point &p ) { 
        input >> p.x >> p.y >> p.z;
        return input;            
    }

    static Point RodriguesFormula(Point x, Point a, double angle){
        angle = deg2rad(angle);
        Point p1 = x * cos(angle);
        Point p2 = a * a.dot(x) * (1 - cos(angle));
        Point p3 = x.cross(a) * sin(angle);
        return p1 + p2 + p3;
    }

    void print(){
        cout<<"( x: "<<x<<", y: "<<y<<", z: "<<z<<")"<<endl;
    }
};

