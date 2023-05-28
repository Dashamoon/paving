#ifndef SLOVETILE_H
#define SLOVETILE_H

#include <list>
#include <vector>

#include <QPolygonF>
#include <QVector2D>

class SolveTile
{
public:
    std::pair <std::pair <int, int>, std::pair <int, int>> getVectors();
    std::vector <std::pair <int, int>> getArea();

    void setVectors(int x1, int y1, int x2, int y2);
    void setPolygon(std::vector <std::pair<int, int>> AreaVector);
    bool check_intersection();
    bool check_hole();

private:
    QPolygonF transfer (QPolygonF P, QVector2D v);

private:
     QVector2D v1,v2;
     QPolygonF Area;
};

#endif // SLOVETILE_H
