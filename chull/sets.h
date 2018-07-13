#ifndef SETS_H
#define SETS_H

#include <QPoint>

class sets
{
public:
    sets();
    static std::vector<QPoint> rasterSet(int);
    static std::vector<QPoint> randomSet(int);
    static std::vector<QPoint> clusterCircleSet(int y, int x);
    static std::vector<QPoint> clusterEllipseSet(int y, int x);
    static std::vector<QPoint> clusterSquareSet(int y, int x);
};

#endif // SETS_H
