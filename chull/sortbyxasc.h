#ifndef SORTBYXASC_H
#define SORTBYXASC_H
#include <QPoint>

class sortByXAsc
{
public:
    sortByXAsc();
    bool operator()(QPoint &p1, QPoint &p2){
        const double EPS = 1.0e-4;
        if(std::abs(p1.x() - p2.x())<EPS)
            return (p1.y() < p2.y());
        else
            return (p1.x() < p2.x());

    }
};

#endif // SORTBYXASC_H
