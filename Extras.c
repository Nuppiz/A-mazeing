//random stuff only used for testing
#include "Shared.h"

#define M_PI                3.14159265358979323846264338327950288
#define degToRad(degree)    (degree * (M_PI / 180.0))

void draw_circle()
{
    int radius = 50;
    int degree = 0;
    double degs = 0;
    int center_x = 55;
    int center_y = 100;
    int offset_x = 0;
    int offset_y = 0;

    while (degree < 360)
    {
        degs = degToRad(degree);
        offset_y = sin(degs) * radius;
        offset_x = cos(degs) * radius;
        screen_buf[(center_y + offset_y) * SCREEN_WIDTH + (center_x + offset_x)]=14;
        degree++;
    }
}

void draw_circle_oct()
{
    int radius = 50;
    int center_x = 55;
    int center_y = 100;
    int offset_x;
    int offset_y;

    offset_y = 0;
    do
    {
        offset_x = cos(asin((float)(offset_y)/radius)) * radius;
        SET_PIXEL(center_x + offset_x, center_y + offset_y, 14); // lower right octant
        SET_PIXEL(center_x - offset_x, center_y + offset_y, 14); // lower left octant
        SET_PIXEL(center_x + offset_x, center_y - offset_y, 14); // upper right octant
        SET_PIXEL(center_x - offset_x, center_y - offset_y, 14); // upper left octant
        SET_PIXEL(center_x + offset_y, center_y + offset_x, 14); // bottom right octant
        SET_PIXEL(center_x - offset_y, center_y + offset_x, 14); // bottom left octant
        SET_PIXEL(center_x + offset_y, center_y - offset_x, 14); // top right octant
        SET_PIXEL(center_x - offset_y, center_y - offset_x, 14); // top left octant

        offset_y++;
    }
    while (offset_y < offset_x);
}

void disco_ball()
{
    uint8_t color = 12;
    int b_radius = 50;
    int b_degree = 0;
    double b_degs = 0;
    int b_center_x = 265;
    int b_center_y = 100;
    int b_offset_x = 0;
    int b_offset_y = 0;
    int limit_x = 0;
    int limit_y = 0;

    while (b_degree < 360)
    {
        b_degs = degToRad(b_degree);
        limit_y = sin(b_degs) * b_radius;
        limit_x = cos(b_degs) * b_radius;
        if (limit_y >= 0 && limit_x >= 0)
        {
            while (b_offset_y < limit_y)
            {
                while(b_offset_x < limit_x)
                {
                    SET_PIXEL(b_center_x + b_offset_x, b_center_y + b_offset_y, color + TICKS*2);
                    b_offset_x++;
                    color++;
                }
                b_offset_x = 0;
                b_offset_y++;
            }
            b_offset_y = 0;
            b_degree++;
        }
        else if (limit_y < 0 && limit_x < 0)
        {
            while (b_offset_y > limit_y)
            {
                while(b_offset_x > limit_x)
                {
                    SET_PIXEL(b_center_x + b_offset_x, b_center_y + b_offset_y, color + TICKS*2);
                    b_offset_x--;
                    color++;
                }
                b_offset_x = 0;
                b_offset_y--;
            }
            b_offset_y = 0;
            b_degree++;
        }
        else if (limit_y < 0 && limit_x >= 0)
        {
            while (b_offset_x < limit_x)
            {
                while(b_offset_y > limit_y)
                {
                    SET_PIXEL(b_center_x + b_offset_x, b_center_y + b_offset_y, color + TICKS*2);
                    b_offset_y--;
                    color++;
                }
                b_offset_y = 0;
                b_offset_x++;
            }
            b_offset_x = 0;
            b_degree++;
        }
        else if (limit_y >= 0 && limit_x < 0)
        {
            while (b_offset_x > limit_x)
            {
                while(b_offset_y < limit_y)
                {
                    SET_PIXEL(b_center_x + b_offset_x, b_center_y + b_offset_y, color + TICKS*2);
                    b_offset_y++;
                    color++;
                }
                b_offset_y = 0;
                b_offset_x--;
            }
            b_offset_x = 0;
            b_degree++;
        }
    }
}