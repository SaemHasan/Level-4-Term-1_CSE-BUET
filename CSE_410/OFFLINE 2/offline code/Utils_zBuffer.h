#include "UtilsMatrix.h"
#include "bitmap_image.hpp"
#include<time.h>

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
double LEFT_LIMIT, RIGHT_LIMIT, BOTTOM_LIMIT, TOP_LIMIT, FRONT_Z, REAR_Z;
double dx, dy, Top_Y, Left_X, Right_X, Bottom_Y;
bitmap_image image;

class Triangle;
vector<Triangle> triangles;
vector< vector<double> > zBuffer;

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

void initializeBuffers(){
    //dx,dy
    dx = (RIGHT_LIMIT-LEFT_LIMIT)/(SCREEN_WIDTH*1.0);
    dy = (TOP_LIMIT-BOTTOM_LIMIT)/(SCREEN_HEIGHT*1.0);
    //Top_Y, Left_X
    Top_Y = TOP_LIMIT - dy/2.0;
    Bottom_Y = BOTTOM_LIMIT + dy/2.0;
    Left_X = LEFT_LIMIT + dx/2.0;
    Right_X = RIGHT_LIMIT - dx/2.0;

    //z buffer
    zBuffer.resize(SCREEN_WIDTH);
    for(int i=0; i<SCREEN_WIDTH; i++){
        zBuffer[i].resize(SCREEN_HEIGHT);
        for(int j=0; j<SCREEN_HEIGHT; j++){
            zBuffer[i][j] = REAR_Z;
        }
    }

    //create image
    image= bitmap_image(SCREEN_WIDTH, SCREEN_HEIGHT);
    for(int i=0; i<SCREEN_WIDTH; i++){
        for(int j=0; j<SCREEN_HEIGHT; j++){
            image.set_pixel(i,j,0,0,0);
        }
    }

    // image.save_image("output/out.bmp");
}

double computeZvalue(Triangle t, Point p){
    double z = 0;
    
    return z;
}

int rowNumberFromY(double y){
    return SCREEN_HEIGHT-((y-BOTTOM_LIMIT)/dy);
}

int columnNumberFromX(double x){
    return (x-LEFT_LIMIT)/dx;
}

bool isInTriangle(Point p, Triangle t){
    if(p.x < t.min_x() || p.x > t.max_x() || p.y < t.min_y() || p.y > t.max_y()){
        return false;
    }
    return true;
}

void zBufferAlgorithm(){
    for (int k=0; k<triangles.size(); k++){
        Triangle triangle = triangles[k];
        // cout<<triangle<<endl;
        //clipping
        double top_scanline = min(triangle.max_y(), Top_Y);
        double bottom_scanline = min(triangle.min_y(), Bottom_Y);
        double left_scanline = min(triangle.min_x(), Left_X);
        double right_scanline = min(triangle.max_x(), Right_X);

        cout<<top_scanline<<" "<<bottom_scanline<<" "<<left_scanline<<" "<<right_scanline<<endl;

        //row and column number
        int top_row = rowNumberFromY(top_scanline);
        int bottom_row = rowNumberFromY(bottom_scanline);
        int left_column = columnNumberFromX(left_scanline);
        int right_column = columnNumberFromX(right_scanline);

        cout<<top_row<<" "<<bottom_row<<" "<<left_column<<" "<<right_column<<endl;

        for(int i=top_row; i<=bottom_row; i++){
            for(int j=left_column; j<=right_column; j++){
                //Top_Y- row_no*dy, Left_X+col_no*dx
                double x = Left_X + j*dx;
                double y = Top_Y - i*dy;
                Point p(x,y);
                if(isInTriangle(p, triangle)){
                    // cout<<"here i am 1\n";
                    double z = computeZvalue(triangle, p);
                    if(z < zBuffer[j][i]){
                        zBuffer[j][i] = z;
                        image.set_pixel(j,i,triangle.color.r,triangle.color.g,triangle.color.b);
                        cout<<"here i am 2\n";
                    }
                }
                else{
                    // cout<<"here i am 3\n";
                }
            }
        }
        
    }
    image.save_image("output/out.bmp");

}


void printDetails()
{
    cout<<"SCREEN_WIDTH: "<<SCREEN_WIDTH<<endl;
    cout<<"SCREEN_HEIGHT: "<<SCREEN_HEIGHT<<endl;
    cout<<"LEFT_LIMIT: "<<LEFT_LIMIT<<endl;
    cout<<"RIGHT_LIMIT: "<<RIGHT_LIMIT<<endl;
    cout<<"BOTTOM_LIMIT: "<<BOTTOM_LIMIT<<endl;
    cout<<"TOP_LIMIT: "<<TOP_LIMIT<<endl;
    cout<<"FRONT_Z: "<<FRONT_Z<<endl;
    cout<<"REAR_Z: "<<REAR_Z<<endl;

    cout<<"#triangles: "<<triangles.size()<<endl;
    for(int i=0; i<triangles.size(); i++){
        cout<<"Triangle "<<i+1<<":"<<endl;
        cout<<triangles[i];
    }
}