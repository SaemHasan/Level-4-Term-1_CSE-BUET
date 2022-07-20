#include "Utils_ColorTriangle.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
double LEFT_LIMIT, RIGHT_LIMIT, BOTTOM_LIMIT, TOP_LIMIT, FRONT_Z, REAR_Z;
double dx, dy, Top_Y, Left_X, Right_X, Bottom_Y;
bitmap_image image;

vector<Triangle> triangles;
vector< vector<double> > zBuffer;
vector< vector<Color> > colorBuffer;



//for upto stage 3
Point transformPoint(Matrix m, Point p){
    Matrix pointMatrix = Matrix::pointMatrix(p);
    Matrix result = m * pointMatrix;
    // cout<<fixed<<setprecision(5)<<result<<endl;
    return result.getPoint();
}


Matrix viewTransformationMatrix(Point eye, Point look, Point up){
    Point l = look - eye;
    l.normalize();
    Point r = l.cross(up);
    r.normalize();
    Point u = r.cross(l);

    Matrix T = Matrix::translationMatrix(-eye);
    Matrix R = Matrix::rotationMatrix(r, u, -l);

    Matrix V = R * T;

    return V;
}


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
    zBuffer.resize(SCREEN_HEIGHT);
    for(int i=0; i<SCREEN_HEIGHT; i++){
        zBuffer[i].resize(SCREEN_WIDTH);
        for(int j=0; j<SCREEN_WIDTH; j++){
            zBuffer[i][j] = REAR_Z;
        }
    }

    Color black = Color(0,0,0);
    //color buffer
    colorBuffer.resize(SCREEN_HEIGHT);
    for(int i=0; i<SCREEN_HEIGHT; i++){
        colorBuffer[i].resize(SCREEN_WIDTH);
        for(int j=0; j<SCREEN_WIDTH; j++){
            colorBuffer[i][j] = black;
        }
    }
}

double calcZ(Point p1, Point p2, double ys){
    double z;
    z = p1.z + ((p2.z - p1.z)*1.0) * ((ys-p1.y)/(p2.y-p1.y)*1.0);
    return z;
}

int rowNumberFromY(double y){
    return SCREEN_HEIGHT-((y-BOTTOM_LIMIT)/dy);
}

int columnNumberFromX(double x){
    return (x-LEFT_LIMIT)/dx;
}

bool isInTriangle(Point p, Triangle t){
    if(p.x< t.min_x() || p.x>t.max_x() || p.y<t.min_y() || p.y>t.max_y()){
        return false;
    }
    return true;
}

void zBufferAlgorithm(){
    long long int count = 0;
    for (int k=0; k<triangles.size(); k++){
        //get the triangle
        Triangle triangle = triangles[k];
        // construct line equations for each edge of the triangle
        triangle.prepareLines();
        // cout<<triangle<<endl;
        //clipping
        double top_scanline = min(triangle.max_y(), Top_Y);
        double bottom_scanline = max(triangle.min_y(), Bottom_Y);

        //row number
        int top_row = rowNumberFromY(top_scanline);
        int bottom_row = rowNumberFromY(bottom_scanline);

        for(int i=top_row; i<bottom_row; i++){
            double ys = Top_Y - i*dy;
            double xa,xb,za,zb;
            Point p1, p2, p3;
            if(triangle.pMax.y == triangle.pMid.y){
                xa = triangle.line31.getX(ys);
                xb = triangle.line23.getX(ys);
                za = calcZ(triangle.pMax, triangle.pMin, ys);
                zb = calcZ(triangle.pMid, triangle.pMin, ys);
                
            }
            else if(triangle.pMin.y == triangle.pMid.y){
                xa = triangle.line12.getX(ys);
                xb = triangle.line31.getX(ys);
                za = calcZ(triangle.pMin, triangle.pMax, ys);
                zb = calcZ(triangle.pMid, triangle.pMax, ys);
            }
            else if(ys>=triangle.pMid.y){
                xa = triangle.line12.getX(ys);
                xb = triangle.line31.getX(ys);
                za = calcZ(triangle.pMax, triangle.pMid, ys);
                zb = calcZ(triangle.pMin, triangle.pMax, ys);
            }
            else if(ys<triangle.pMid.y){
                xa = triangle.line23.getX(ys);
                xb = triangle.line31.getX(ys);
                za = calcZ(triangle.pMin, triangle.pMid, ys);
                zb = calcZ(triangle.pMax, triangle.pMin, ys);
            }
            if(xa>xb){
                swap(xa,xb);
                swap(za,zb);
            }

            double za_zb = za - zb;
            double xa_xb = xa - xb;
            double za_zb_xa_xb = (za_zb*1.0) / xa_xb;
            double known_constant = dx * za_zb_xa_xb * 1.0;
            double zp;

            double left_scanline = max(xa, Left_X);
            double right_scanline = min(xb, Right_X);
            int left_column = columnNumberFromX(left_scanline);
            int right_column = columnNumberFromX(right_scanline);

            for(int j=left_column; j<right_column; j++){
                //Top_Y- row_no*dy, Left_X+col_no*dx
                double xp = Left_X + j*dx;
                if(j==left_column){
                    zp = za + (xp-xa)*za_zb_xa_xb;
                }
                else{
                    zp = zp + known_constant;
                }
                
                if(zp>=FRONT_Z && zp < zBuffer[i][j]){
                    zBuffer[i][j] = zp;
                    colorBuffer[i][j] = triangle.color;
                }
                
            }
        }
    }
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