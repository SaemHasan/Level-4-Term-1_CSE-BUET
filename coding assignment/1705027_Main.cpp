#include "1705027_MidPointAlgo.hpp"
#include <fstream>

void clearImage(int width, int height)
{
    // image.clear();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image.set_pixel(j, i, 255, 255, 255);
        }
    }
}

int main()
{
    ifstream input("input.txt");
    int width, height, n;
    input >> width >> height;
    input >> n;
    image = bitmap_image(width, height);
    clearImage(width, height);
    
    Point p1Arr[n], p2Arr[n];
    Color color[n];


    for (int i = 0; i < n; i++)
    {
        Point p1, p2;
        Color c;
        input >> p1 >> p2;
        input >> c;
        p1Arr[i] = p1;
        p2Arr[i] = p2;
        color[i] = c;
    }
    // mid point algo
    for(int i = 0; i < n; i++){
        MidPointAlgo mpa(p1Arr[i], p2Arr[i], width, height, color[i]);
        mpa.drawLine();
    }
    image.save_image("1705027_1.bmp");
    clearImage(width, height);

    input.close();
    return 0;
}
