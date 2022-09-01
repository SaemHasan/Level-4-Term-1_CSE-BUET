#include<iostream>
#include<cmath>
#include "bitmap_image.hpp"
using namespace std;

bitmap_image image;

class Point{
    public:
        double x, y;
        Point(double x, double y){
            this->x = x;
            this->y = y;
        }
        Point(){
            this->x = 0;
            this->y = 0;
        }
        Point operator+(Point p){
            return Point(this->x + p.x, this->y + p.y);
        }
        Point operator-(Point p){
            return Point(this->x - p.x, this->y - p.y);
        }
        Point operator*(double d){
            return Point(this->x * d, this->y * d);
        }
        Point operator/(double d){
            return Point(this->x / d, this->y / d);
        }

        friend ostream& operator<<(ostream& os, const Point& p){
            os << "(" << p.x << ", " << p.y << ")";
            return os;
        }
        
        friend istream& operator>>(istream& is, Point& p){
            is >> p.x >> p.y;
            return is;
        }

        double distance(Point p){
            return sqrt(pow(this->x - p.x, 2) + pow(this->y - p.y, 2));
        }

};

class Color{
    public:
        int r, g, b;
        Color(int r, int g, int b){
            this->r = r;
            this->g = g;
            this->b = b;
        }
        Color(){
            this->r = 0;
            this->g = 0;
            this->b = 0;
        }

        friend ostream& operator<<(ostream& os, const Color& c){
            os << "(" << c.r << ", " << c.g << ", " << c.b << ")";
            return os;
        }
        
        friend istream& operator>>(istream& is, Color& c){
            is >> c.r >> c.g >> c.b;
            return is;
        }
};
