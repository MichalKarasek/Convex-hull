#include "algorithms.h"
#include "sortbyxasc.h"
#include "sortbyyasc.h"
#include <stack>
#include <QDebug>
#include <chrono>

QPoint algorithms::pivot;

algorithms::algorithms()
{

}

int algorithms::getPosition(QPoint &q,QPoint &a, QPoint &b, bool choice)
{
    double eps = 1.0e-11;
    double ux = b.x() - a.x();
    double uy = b.y() - a.y();
    double vx = q.x() - a.x();
    double vy = q.y() - a.y();

    double det = (ux * vy - uy * vx);

    if(det > eps)
        return 1;

    if(choice == true && det < -eps)
    {
        return 0;
    }
    else if(choice == false && det < eps)
    {
        return 0;
    }

    return -1;
}

double algorithms::getAngle(QPoint &p1,QPoint &p2,QPoint &p3, QPoint &p4)
{
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    double s = ux * vx + uy * vy;
    double normU = sqrt(ux * ux + uy * uy);
    double normV = sqrt(vx * vx + vy * vy);

    return std::abs(acos(s/(normU * normV)))*(180/M_PI);
}

QPolygon algorithms::jarvisCH(std::vector<QPoint> &points)
{
    const double AEPS = 1.0e-5;
    QPolygon ch;

    // Sort points by Y
    std::sort (points.begin(),points.end(),sortByYAsc());

    //Find pivot
    QPoint q = points[0];

    //Ad pivot to ch
    ch.push_back(q);

    //Find left-most point
    std::sort(points.begin(),points.end(),sortByXAsc());

    QPoint s = points[0];

    //Assign initial points
    QPoint pj = q;
    QPoint pj_1(s.x(),q.y()); //p(j-1)

    if(s.x() == q.x())
        pj_1.setX(q.x()-10);

    do{
        int imax = -1 ;
        double dmax = 0;
        double amax = 0;

        //Find max angle
        for(size_t i = 0; i < points.size(); ++i){

            double angle=getAngle(pj,points[i],pj,pj_1);

            //Common point
            if(angle > amax)
            {
                amax = angle;
                imax = i;
            }

            //Singular point
            else if (std::fabs(angle - amax) <AEPS )
            {
                double dist = distance(points[i],pj);
                if (dist > dmax)
                {
                    dmax = dist;
                    imax = i;
                }
            }
        }

        //Add point to ch
        ch.push_back(points[imax]);

        //Assign points
        pj_1 = pj;
        pj = points[imax];
    }while(pj != q);

    return ch;
}

QPolygon algorithms::qhull(std::vector<QPoint> &points){

    QPolygon ch;
    std::vector<QPoint> uh;
    std::vector<QPoint> lh;

    //Find left-most and right-most points
    std::sort(points.begin(), points.end(),sortByXAsc());
    QPoint q1 = points[0];
    QPoint q3 = points[points.size()-1];

    uh.push_back(q1);
    uh.push_back(q3);

    lh.push_back(q1);
    lh.push_back(q3);

    for(size_t i = 1; i < points.size()-1; ++i)
    {
        int pos = getPosition(points[i],q1,q3);

        //Add to UH
        if((pos==1) || (pos==-1))
        {
           uh.push_back(points[i]);
        }

        //Add to LH
        else if((pos==0) || (pos==-1))
        {
           lh.push_back(points[i]);
        }
    }

    ch.push_back(q3);
    qh(uh,ch,1,0);

    ch.push_back(q1);
    qh(lh,ch,0,1);

    return ch;
}

void algorithms::qh(std::vector<QPoint> &points, QPolygon &ch, int s, int e)
{

    double dmax = -1;
    int imax = -1;

    for(size_t i = 2 ; i < points.size() ; i++)
    {
        int result = getPosition(points[i],points[s],points[e]);
        if (result == 0) {
            double dist = getPointLineDist(points[i],points[s],points[e]);
            if (dist > dmax){
                imax = i;
                dmax = dist;
            }
        }
    }

    //Point found in the right halfplane
    if(imax != -1){
        //First segment
        qh(points, ch, s, imax);

        //Add point to ch
        ch.push_back(points[imax]);

        //Second segment
        qh(points, ch, imax, e);
    }
}

double algorithms::getPointLineDist(QPoint &a, QPoint &p1, QPoint &p2)
{
    double xa = a.x();
    double ya = a.y();

    double x1 = p1.x();
    double y1 = p1.y();

    double x2 = p2.x();
    double y2 = p2.y();

    double dist = distance(p1,p2);
    double value = xa*(y1-y2)+x1*(y2-ya)+x2*(ya-y1);

    double dist_1 = value/dist;

    return std::abs(dist_1);
}

QPolygon algorithms::incr(std::vector<QPoint> &points)
{
    //Amount of  points
    int m = points.size();
    //bool choice = false;
    bool skip = true;

    //if(option == 3)
      //  choice = true;

    QPolygon ch;

    //Lists of previous and next indices
    std::vector<int> p(m);
    std::vector<int> n(m);

    //Sort by x
    std::sort(points.begin(),points.end(),sortByXAsc());

    // initial approximation of CH
    p[0]=1;
    p[1]=0;
    n[0]=1;
    n[1]=0;

    int start=2;

    //Position of q[2] according to q[0], q[1]. Remove all colinear cases.

    do{
        if(getPosition(points[start],points[0],points[start-1]) == 1)
        {
            n[start-1] = start;
            n[start] = 0;
            p[start] = start-1;
            p[0] = start;
            ++start;
            skip = false;
        }
        else if(getPosition(points[start],points[0],points[start-1]) == 0)
        {
            n[0] = start;
            n[start] = start-1;
            p[start] = 0;
            p[start-1] = start;
            ++start;
            skip = false;
        }
        else
        {
            ++start;
        }
    }while(skip);

    for(int i = start; i < m; i++){

        //Point q[i] in upper half-plane
        //Connect q[i] with its previous and next points
        if(points[i].y() > points[i-1].y())
        {
            p[i] = i-1;
            n[i] = n[i-1];

        }
        //Point q[i] in lower half-plane
        //Connect q[i] with its previous and next points
        else
        {
            p[i]=p[i-1];
            n[i]=i-1;
        }

        //Connect point next to q[i] with q[i]
        //Connect point previous to q[i] with q[i]
        n[p[i]]=i;
        p[n[i]]=i;

        //Find upper tangent
        while(getPosition(points[n[n[i]]], points[i], points[n[i]], false) == 0)
        {
            p[n[n[i]]] = i;
            n[i] = n[n[i]];
        }

        //Find lower tangent
        while(getPosition(points[p[p[i]]], points[p[i]], points[i], false) == 0)
        {
            n[p[p[i]]] = i;
            p[i] = p[p[i]];
        }
    }

    // add the left most point to ch
    ch.push_back(points[0]);

    // convert circular list to list of points
    int index = n[0];

    do{
        ch.push_back(points[index]);
        index = n[index];
    }while(index != 0);

    return ch;
}

QPolygon algorithms::grahamScan(std::vector<QPoint> &points)
{
    std::stack<QPoint> gs;
    QPolygon convex_hull;

    if(points.size()==3)
    {
        for(size_t i = 0; i < points.size(); ++i)
        {
            convex_hull.push_back(points[i]);
        }
        return convex_hull;
    }

    // Sort by Y
    std::sort(points.begin(), points.end(), sortByYAsc());

    // Pivot
    gs.push(points[0]);
    pivot = points[0];
    points.erase(points.begin());
    convex_hull.push_back(pivot);

    // Sort by angle
    std::sort(points.begin(), points.end(), sortByAngle);

    // Erase points with same polar angle within the most distant one
    for(size_t i = 1; i < points.size(); ++i)
    {
        while (i < points.size()-1 && getPosition(points[i+1],points[0],points[i])==-1)
        {
            ++i;
        }
        convex_hull.push_back(points[i]);
    }

    gs.push(convex_hull[1]);

    //Search for convex hull points
    for(int i = 2; i < convex_hull.size(); ++i)
    {
        QPoint p1 = nextToTop(gs);

        while (getPosition(convex_hull[i], p1, gs.top())!=1) {
            gs.pop();
            p1 = nextToTop(gs);
        }
        gs.push(convex_hull[i]);
    }

    //empty polygon
    convex_hull.clear();

    // copy points from stack to vector
    while(!gs.empty())
    {
        convex_hull.push_back(gs.top());
        gs.pop();
    }

    //close polygon
    points.push_back(pivot);
    return convex_hull;
}

bool algorithms::sortByAngle(QPoint &a, QPoint &b)
{
    const double EPS = 1.0e-11;
    QPoint pivot2(pivot.x()-10, pivot.y());

    // get polar angle with respect to the pivot
    double angle_1 = getAngle(pivot, pivot2, pivot, a);
    double angle_2 = getAngle(pivot, pivot2, pivot, b);

    // sort by distance from pivot if angles are identical
    if(std::abs(angle_1-angle_2) < EPS)
    {
        double dist_1 = distance(pivot, a);
        double dist_2 = distance(pivot, b);

        return dist_1 < dist_2;
    }
    return angle_1 >  angle_2;
}

double algorithms::getAngleToXAxis(QPoint &p1, QPoint &p2 )
{
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();

    return atan( dy / -dx );
}

double algorithms::getAngleToYAxis(QPoint &p1, QPoint &p2 )
{
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();

    return atan( dx / dy );
}

QPolygon algorithms::MinEnclosingBox(std::vector<QPoint> CH)
{
    double delta;
    double sum_delta = 0.0;
    double area = 0.01;
    double new_area = 0.0;
    QPolygon minBox(4);
    QPolygon newBox(4);

    // compute area, &BoundaryBox and &delta of given rotation angle(sum_delta)
    area = MBR(CH, sum_delta, minBox, delta);

    while(sum_delta + delta < M_PI_2)
    {
        sum_delta = sum_delta + delta;


        new_area = MBR(CH, sum_delta, newBox, delta);


        if(new_area < area)
        {
            area = new_area;
            minBox = newBox;
        }

    }
    return minBox;
}

double algorithms::MBR(std::vector<QPoint> CH, double angle, QPolygon &Box, double &delta)
{
    double pX, pY;

    //rotate CH by given angle
    for( uint i = 0 ; i < CH.size() ; i++ )
    {
        pX =  CH[i].x() * cos(angle) - CH[i].y() * sin(angle);
        pY =  CH[i].x() * sin(angle) + CH[i].y() * cos(angle);
        CH[i].setX( pX );
        CH[i].setY( pY );
    }

    //find indexes of min max of that rotated CH
    uint iminX = 0;
    uint imaxX = 0;
    uint iminY = 0;
    uint imaxY = 0;

    // Find extreme points (min, max of X and Y)
    for(uint i = 0 ; i < CH.size() ; i++)
    {
        if( CH[i].x() < CH[iminX].x() )
            iminX = i;

        if( CH[i].x() > CH[imaxX].x() )
            imaxX = i;

        if( CH[i].y() < CH[iminY].y() )
            iminY = i;

        if( CH[i].y() > CH[imaxY].y() )
            imaxY = i;
    }

    // area of Minimal Boundary Rectagle
    double area = ( CH[imaxX].x() - CH[iminX].x() ) * ( CH[imaxY].y() - CH[iminY].y() );



    // Minimal enclosing box in local CRS
    Box[0].setX( CH[imaxX].x() );
    Box[0].setY( CH[iminY].y() );

    Box[1].setX( CH[imaxX].x() );
    Box[1].setY( CH[imaxY].y() );

    Box[2].setX( CH[iminX].x() );
    Box[2].setY( CH[imaxY].y() );

    Box[3].setX( CH[iminX].x() );
    Box[3].setY( CH[iminY].y() );



    // transform Box into main CRS
    for( uint i = 0 ; i < 4; i++ )
    {
        pX =  Box[i].x() * cos(-angle) - Box[i].y() * sin(-angle);
        pY =  Box[i].x() * sin(-angle) + Box[i].y() * cos(-angle);
        Box[i].setX( pX );
        Box[i].setY( pY );
    }


    // find value of all deltas
    double delta_i, delta_j, delta_k, delta_l;

    if(iminX == 0)
        delta_i = getAngleToYAxis( CH[iminX] , CH[CH.size() - 1] );
    else
        delta_i = getAngleToYAxis( CH[iminX] , CH[iminX - 1] );

    if(imaxY == 0)
        delta_j = getAngleToXAxis( CH[imaxY] , CH[CH.size() - 1] );
    else
        delta_j = getAngleToXAxis( CH[imaxY] , CH[imaxY - 1] );

    if(imaxX == 0)
        delta_k = getAngleToYAxis( CH[imaxX] , CH[CH.size() - 1] );
    else
        delta_k = getAngleToYAxis( CH[imaxX] , CH[imaxX - 1] );

    if(iminY == 0)
        delta_l = getAngleToXAxis( CH[iminY] , CH[CH.size() - 1] );
    else
        delta_l = getAngleToXAxis( CH[iminY] , CH[iminY - 1] );


    const double EPSA = 1.0e-14;

    // keep only positive value of angel
    while(delta_i < EPSA)
        delta_i += M_PI;
    while(delta_j < EPSA)
        delta_j += M_PI;
    while(delta_k < EPSA)
        delta_k += M_PI;
    while(delta_l < EPSA)
        delta_l += M_PI;


    // find minimum delta
    delta = std::min( std::min(delta_i, delta_j), std::min(delta_k, delta_l));

    return area;
}

QPolygon algorithms::strictCH(std::vector<QPoint> ch)
{
    const double EPS = 1.0e-10;

    // add first two points at the end (becouse of cicle)
    ch.push_back(ch[0]);
    ch.push_back(ch[1]);

    // new strict convex hull
    QPolygon CH;
    //tady se to musi udelat jako graham
    for(size_t i = 0 ; i < ch.size() - 1 ; i++ )
    {
        while( getPointLineDist( ch[i] , ch[i+1] , ch[i+2] ) < EPS )
            CH.push_back(ch[i+1]);
    }

    return CH;
}

