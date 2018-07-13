#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QPoint>
#include <QPolygon>
#include <stack>

class algorithms
{

public:

    algorithms();
    static int getPosition(QPoint &p,QPoint &a,QPoint &b, bool choice = true);
    static bool sortByAngle(QPoint &a, QPoint &b);
    static double getAngle(QPoint &,QPoint &,QPoint &, QPoint &);
    static QPolygon jarvisCH(std::vector<QPoint> &points);
    static QPolygon grahamScan(std::vector<QPoint> &points);
    static double getPointLineDist(QPoint &a, QPoint &p1, QPoint &p2);
    static QPolygon strictCH(std::vector<QPoint> ch);

    static double distance(QPoint &a, QPoint &b){

        double dx = b.x() - a.x();
        double dy = b.y() - a.y();
        return sqrt(dx*dx + dy*dy);
    }

    static QPoint nextToTop(std::stack<QPoint> &stack){

        QPoint p = stack.top();
        stack.pop();
        QPoint q = stack.top();
        stack.push(p);
        return q;
    }
    static QPolygon qhull(std::vector<QPoint> &points);
    static QPolygon incr(std::vector<QPoint> &points);
    static void qh(std::vector<QPoint> &points, QPolygon &ch, int s, int e);
    static QPolygon MinEnclosingBox(std::vector<QPoint> CH);

    static double MBR(std::vector<QPoint> CH, double angle, QPolygon &Box, double &delta);

    static double getAngleToXAxis(QPoint &p1, QPoint &p2);
    static double getAngleToYAxis(QPoint &p1, QPoint &p2);


private:

    static QPoint pivot;
};

#endif // ALGORITHMS_H
