#include "sets.h"
#include <cmath>
#include <ctime>

sets::sets()
{

}

std::vector<QPoint> sets::rasterSet(int x)
{
    std::vector<QPoint> set;

    size_t size = floor(sqrt(x));

    //Create raster points set
    for(size_t i = 0; i < size; ++i)
    {
        for(size_t j = 0; j < size; ++j)
        {
            set.push_back(QPoint(i*10,j*10));
        }
    }
    return set;
}

std::vector<QPoint> sets::randomSet(int x)
{
    // create random points set
    std::vector<QPoint> set;
    QPoint point;

    srand(time(NULL));

    for(int i = 0; i < x; ++i)
    {
        point.setX(rand()%10000);
        point.setY(rand()%10000);

        set.push_back(point);
    }
    return set;
}

std::vector<QPoint> sets::clusterCircleSet(int y, int x)
{
    // create cluster points set
    std::vector<QPoint> set;
    QPoint point;
    QPoint centr;
    int diameter_max = floor(sqrt(y/M_PI));

    // set maximal diameter to 1
    if(y <= 3)
        diameter_max += 1;

    int diameter;
    int angle;

    srand(time(NULL));

    // set x to 1 if size of set is smaller then number of circle undersets
    if(y <= x)
        x=1;

    for(int i = 0; i < x; ++i)
    {
        // cente of circle
        centr.setX(rand()%10000);
        centr.setY(rand()%10000);

        for(int j = 0; j < (y/x); ++j)
        {
            diameter = rand()%diameter_max;
            angle = rand()%360;

            // point coordinates randomly distributed around center
            point.setX(centr.x() + floor(diameter*cos(angle*M_PI/180)));
            point.setY(centr.y() + floor(diameter*sin(angle*M_PI/180)));

            set.push_back(point);
        }
    }
    return set;
}

std::vector<QPoint> sets::clusterEllipseSet(int y, int x)
{
    // create ellipse point set
    std::vector<QPoint> set;
    QPoint point;
    QPoint centr;

    // use golden ratio to specify a and b ratio
    double golden_ratio = 1.618033988;

    // compute maximum size of ellipse minor axis b
    int b_max = floor(sqrt(y/(M_PI*golden_ratio)));


    if(y < 6)
        b_max +=1;

    // compute maximum size of main axis a
    int a_max = floor(b_max*golden_ratio);
    int a;
    int b;
    int angle;

    srand(time(NULL));

    // set x to 1 if size of set is smaller then number of ellipse undersets
    if(y <= x)
        x=1;

    for(int i = 0; i < x; ++i)
    {
        //centre of ellipse
        centr.setX(rand()%10000);
        centr.setY(rand()%10000);

        for(int j = 0; j < (y/x); ++j)
        {
            // main, minor axis + angle
            a = rand()%a_max;
            b = rand()%b_max;
            angle = rand()%360;

            //point coordinates randomly distributed around center
            point.setX(centr.x() + floor(a*cos(angle*M_PI/180)));
            point.setY(centr.y() + floor(b*sin(angle*M_PI/180)));

            set.push_back(point);
        }
    }
    return set;
}

std::vector<QPoint> sets::clusterSquareSet(int y, int x)
{
    std::vector<QPoint> set;
    QPoint point;
    QPoint low_left_point;

    // compute size of square side
    int a = floor(sqrt(y));
    int x_max;
    int y_max;

    srand(time(NULL));

    // set x to 1 if size of set is smaller then number of square undersets
    if(y <= x)
        x=1;

    for(int i = 0; i < x; ++i)
    {
        // set coordinates of squre low left point
        low_left_point.setX(rand()%10000);
        low_left_point.setY(rand()%10000);

        // set maximum coordinates
        x_max = low_left_point.x() + a;
        y_max = low_left_point.y() + a;

        for(int j = 0; j < (y/x); ++j)
        {
            // point coordinates randomly distributed
            point.setX(low_left_point.x() + rand()%(x_max - low_left_point.x()) + low_left_point.x());
            point.setY(low_left_point.y() + rand()%(y_max - low_left_point.y()) + low_left_point.y());

            set.push_back(point);
        }
    }
    return set;
}
