#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <chrono>
#include <QErrorMessage>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "draw.h"
#include "graphic.h"

typedef std::chrono::high_resolution_clock::time_point hrctp;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void on_analyze_clicked();


    void on_mbg_clicked();

    void on_typeCombo_currentIndexChanged(int index);

private:

    Ui::Widget *ui;
    QErrorMessage msg;
    QGraphicsItem *item;
    std::vector<QPoint> dataset;
    std::vector<QPoint> zero_vector;
    QPolygon zero_polygon;
    QPolygon convex_hull;
    QPolygon mbr;
    QGraphicsScene *scene = nullptr;

    //QGraphicsScene *scene;

    void Duration(hrctp, hrctp);
    void paintData();
    void checkData();
    void checkHull();
    void paintHull();
    void paintMBG();
    int tryCluster(int);
};

#endif // WIDGET_H
