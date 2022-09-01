#include "1705027_Point.hpp"


class MidPointAlgo
{
public:
    Point p1, p2;
    Color color;
    // bitmap_image image;
    int width, height;

    MidPointAlgo(Point p1, Point p2, int width, int height, Color color)
    {
        this->p1 = p1;
        this->p2 = p2;
        this->width = width;
        this->height = height;
        this->color = color;
        // this->image = image;
    }

    void drawLine()
    {
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;

        double d = 2 * dy - dx;
        double dE = 2 * dy;
        double dNE = 2 * (dy - dx);
        double x = p1.x;
        double y = p1.y;
        image.set_pixel(x, height - y, color.r, color.g, color.b);

        if (dy <= dx)
        {
            while (x < p2.x)
            {
                if (d <= 0)
                {
                    d += dE;
                    x++;
                }
                else
                {
                    d += dNE;
                    x++;
                    y++;
                }
                // cout << x << " " << y << endl;
                image.set_pixel(x, height - y, color.r, color.g, color.b);
            }
        }
        else
        {
            d = 2 * dx - dy;
            dE = 2 * dx;
            dNE = 2 * (dx - dy);
            x = p1.x;
            y = p1.y;
            image.set_pixel(x, height - y, 0, 0, 0);
            while (y < p2.y)
            {
                if (d <= 0)
                {
                    d += dE;
                    y++;
                }
                else
                {
                    d += dNE;
                    x++;
                    y++;
                }
                // cout << x << " " << y << endl;
                image.set_pixel(x, height - y, color.r, color.g, color.b);
            }
        }
    }
};