#include "widget.h"
#include"algorithms.h"
#include "sets.h"
#include <QApplication>
#include <QDebug>
#include <stack>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    /*std::vector<QPoint> data;
    QPolygon result;

   for(int i=0; i <10; ++i)
    {
        data = sets::rasterSet(1000000);
        auto begin = std::chrono::high_resolution_clock::now();
        result = algorithms::incr(data);
        auto end = std::chrono::high_resolution_clock::now();
        double durat = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
        durat *= 1e-9;
        qDebug()<<durat<<" ";
    }
    return 0;*/
    return a.exec();
}
