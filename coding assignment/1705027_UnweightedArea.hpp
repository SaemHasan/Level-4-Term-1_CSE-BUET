#include "1705027_MidPointAlgo.hpp"

class UnweightedAreaSampling{
    public:
        Point p1, p2;
        Color color;
        int width, height;

        UnweightedAreaSampling(Point p1, Point p2, int width, int height, Color color)
        {
            this->p1 = p1;
            this->p2 = p2;
            this->width = width;
            this->height = height;
            this->color = color;
        }
        // draw line using Unweighted Area Sampling Antialiasing technique
        void drawLine(){
            
        }

};