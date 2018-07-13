#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include<QtGui>

class draw : public QWidget
{
    Q_OBJECT
public:
    explicit draw(QWidget *parent = 0);
    void setData(std::vector<QPoint> &);
    virtual void paintEvent(QPaintEvent *e);

signals:

public slots:

private:

    std::vector<QPoint> data;
};

#endif // DRAW_H
