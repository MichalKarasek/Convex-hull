#include "widget.h"
#include "algorithms.h"
#include "ui_widget.h"
#include "draw.h"
#include "graphic.h"
#include "sets.h"
#include <QErrorMessage>
#include <QDebug>
#include <chrono>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->setSize->setToolTip(tr("Enter int(3, 1e6)"));
    ui->mbg->setToolTip(tr("Tick to display minimum bounding geometry of set in Canvas"));
    ui->methodCombo->setToolTip(tr("Select a method to create a convex hull with"));
    ui->typeCombo->setToolTip(tr("Points distribution in set"));
    ui->duration->setToolTip(tr("Duration in seconds"));
    ui->clusterLine->setToolTip(tr("Enter number of subsets(optional - default 100)"));

    ui->duration->setEnabled(false);
    ui->mbg->setEnabled(false);
    setWindowTitle(tr("Convex hull analyzer 1.0"));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_analyze_clicked()
{
    if(scene!=nullptr)
    {
        scene->clear();
        delete scene;
        scene = nullptr;
    }

    item = new QGraphicsEllipseItem;
    scene = new QGraphicsScene(this);
    ui->Canvas->setScene(scene);
    scene->addItem(item);

    int size = ui->setSize->text().toInt();
    int choice = ui->typeCombo->currentIndex();
    ui->mbg->setChecked(false);

    //check if cluster number is valid
    int size_cluster = tryCluster(size);

    // check input
    if(!size)
    {   
        msg.setModal(true);
        msg.setWindowTitle(tr("Size of set - incorrect input"));
        msg.showMessage(tr("Input must be integer type and can not be empty !"));

        ui->setSize->clear();
        ui->clusterLine->clear();
        scene->clear();
        size = 0;
    }
    else if((size < 3) || (size > 1e6))
    {
        msg.setModal(true);
        msg.setWindowTitle(tr("Size of set - out of bounds"));
        msg.showMessage(tr("Input number must be between 3 and 1 000 000 !"));

        ui->setSize->clear();
        ui->clusterLine->clear();
        scene->clear();
        size = 0;
    }
    else if(size_cluster == -1)
    {
        scene->clear();
        ui->clusterLine->clear();
        ui->setSize->clear();
    }
    else
    {
        //clear previous data
        checkData();

        // select algorithm for generating set of points
        switch(choice)
        {
            case 0:{
                    dataset = sets::clusterCircleSet(size, size_cluster);
                    break;
                    }
            case 1:{
                    dataset = sets::clusterEllipseSet(size, size_cluster);
                    break;
                    }
            case 2:{
                    dataset = sets::clusterSquareSet(size, size_cluster);
                    break;
                    }
            case 3:{
                    dataset = sets::rasterSet(size);
                    break;
                    }
            case 4:{
                    dataset = sets::randomSet(size);
                    break;
                    }
        }

        //paint data in scene
        paintData();

        // number of chosen algorithm for computation of convex hull
        int choice_1 = ui->methodCombo->currentIndex();

        //clear previous data
        checkHull();

        //select algorithm for convex hull
        switch(choice_1)
        {
            case 0:{
                    auto begin = std::chrono::high_resolution_clock::now();
                    convex_hull = algorithms::grahamScan(dataset);
                    auto end = std::chrono::high_resolution_clock::now();
                    Duration(begin, end);
                    break;
                    }
            case 1:{
                    auto begin = std::chrono::high_resolution_clock::now();
                    convex_hull = algorithms::jarvisCH(dataset);
                    auto end = std::chrono::high_resolution_clock::now();
                    Duration(begin, end);
                    break;
                    }
            case 2:{
                    auto begin = std::chrono::high_resolution_clock::now();
                    convex_hull = algorithms::qhull(dataset);
                    auto end = std::chrono::high_resolution_clock::now();
                    Duration(begin, end);
                    break;
                    }
            case 3:{
                    auto begin = std::chrono::high_resolution_clock::now();
                    convex_hull = algorithms::incr(dataset);
                    auto end = std::chrono::high_resolution_clock::now();
                    Duration(begin, end);
                    break;
                    }
        }
        // paint convex hull in scene
        paintHull();
        //allow to paint MBG
        ui->mbg->setEnabled(true);
    }
}

void Widget::checkData()
{
    // clear previous set of points
    if(!dataset.empty())
    {
        dataset.clear();
        dataset.swap(zero_vector);
    }
}

void Widget::checkHull()
{
    //clear previous convex hull
    if(!convex_hull.empty())
    {
        convex_hull.clear();
        convex_hull.swap(zero_polygon);
    }
}

void Widget::Duration(hrctp begin, hrctp end)
{
    // Measure time for convex hull algorithm
    double durat = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    durat *= 1e-9;
    ui->duration->setText(QString::number(durat));
}

void Widget::paintData()
{
    //clear previous content of scene
    scene->clear();

    double rad = 1;

    // add set of points to screen
    for(auto &point:dataset)
    {
        scene->addEllipse(point.x()-1, point.y()-1, rad*4.0, rad*4.0, QPen(Qt::red), QBrush(Qt::green));
    }
}

void Widget::paintHull()
{
    // paitn convex hull to scene
    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::red);
    scene->addPolygon(convex_hull,pen);
}

void Widget::on_mbg_clicked()
{
    // compute MBG of set
    if(ui->mbg->isChecked())
    {
        mbr = algorithms::MinEnclosingBox(dataset);
        //paint MBR in scene
        paintMBG();
    }
}

void Widget::paintMBG()
{
    // paint MBG in scene
    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::darkGreen);
    scene->addPolygon(mbr, pen);
}

void Widget::on_typeCombo_currentIndexChanged(int index)
{
    if( index <= 2)
    {
        ui->clusterLine->setEnabled(true);
    }
    else
    {
        ui->clusterLine->clear();
        ui->clusterLine->setEnabled(false);
    }
}

int Widget::tryCluster(int size)
{
    if(!ui->clusterLine->text().isEmpty())
    {
        int cluster = ui->clusterLine->text().toInt();

        if(cluster >= size || cluster == 0)
        {
            msg.setModal(true);
            msg.setWindowTitle(tr("Incorrect input - number of clusters"));
            msg.showMessage(tr("Insert positive numbers only. No bigger than size of dataset !!!"));
            msg.exec();
            return -1;
        }
        else
        {
            return cluster;
        }
    }
    else
    {
        return 100;
    }
}
