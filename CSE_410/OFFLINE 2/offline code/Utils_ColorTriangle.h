#include "UtilsMatrix.h"
#include "bitmap_image.hpp"
#include<time.h>



class Color{
    public:
        int r, g, b;
        Color(int r, int g, int b){
            this->r = r;
            this->g = g;
            this->b = b;
        }
        Color(){
            r = rand()%256;
            g = rand()%256;
            b = rand()%256;
        }

        friend ostream& operator<<(ostream& os, const Color& c){
            os<<c.r<<" "<<c.g<<" "<<c.b<<endl;
            return os;
        }
};

class Triangle{

public:
    Point points[3];
    Color color;

    Triangle(Point p1, Point p2, Point p3, Color c){
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
        color = c;
    }
    Triangle(Point p1, Point p2, Point p3){
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
        color = Color();
    }

    Triangle(){
        points[0] = Point();
        points[1] = Point();
        points[2] = Point();
        color = Color();
    }

    double max_y(){
        double max = points[0].y;
        for(int i=1; i<3; i++){
            if(points[i].y > max){
                max = points[i].y;
            }
        }
        return max;
    }

    double min_y(){
        double min = points[0].y;
        for(int i=1; i<3; i++){
            if(points[i].y < min){
                min = points[i].y;
            }
        }
        return min;
    }

    double max_x(){
        double max = points[0].x;
        for(int i=1; i<3; i++){
            if(points[i].x > max){
                max = points[i].x;
            }
        }
        return max;
    }

    double min_x(){
        double min = points[0].x;
        for(int i=1; i<3; i++){
            if(points[i].x < min){
                min = points[i].x;
            }
        }
        return min;
    }

    double min_z(){
        double min = points[0].z;
        for(int i=1; i<3; i++){
            if(points[i].z < min){
                min = points[i].z;
            }
        }
        return min;
    }

    friend ostream& operator<<(ostream &out, Triangle &t){
        out<<"Triangle:"<<endl;
        out<<t.points[0]<<t.points[1]<<t.points[2]<<endl;
        out<<"Colors:"<<endl;
        out<<t.color<<endl;
        return out;
    }

    friend istream& operator>>(istream &in, Triangle &t){
        in>>t.points[0]>>t.points[1]>>t.points[2];
        return in;
    } 
};

