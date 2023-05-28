#include "SolveTile.h"

using namespace std;

vector <pair <int, int>> SolveTile::getArea()
{
    vector <pair <int, int>> A;
    for(int i = 0; i < Area.size(); i++){
        A.push_back(pair(Area[i].x(),Area[i].y()));
    }

    return A;
}

pair <pair <int, int>, pair <int, int>> SolveTile::getVectors()
{
    pair <int, int> v1_pair, v2_pair;
    v1_pair = pair(v1.x(), v1.y());
    v2_pair = pair(v2.x(), v2.y());
    pair <pair <int, int>, pair <int, int>> Vectors = pair(v1_pair, v2_pair);
    return Vectors;
}

void SolveTile::setVectors(int x1, int y1, int x2, int y2)
{
    v1 = QVector2D(x1, y1);
    v2 = QVector2D(x2, y2);
}

void SolveTile::setPolygon(vector <pair <int, int>> AreaVector)
{
     QPolygonF A;
     for(size_t i = 0; i < AreaVector.size(); i++){
         A.push_back(QPointF (AreaVector[i].first, AreaVector[i].second));
     }
     Area = A;
}

QPolygonF SolveTile::transfer (QPolygonF P, QVector2D v)
{
    return P.translated(v.toPointF());
}

static double area(QPolygonF polygon){
    double a = 0.0;
    int n = polygon.size();
    for(int i = 0; i < n; i++){
        if (i == 0) {
            a += polygon[i].x()*(polygon[i+1].y() - polygon[n-1].y()); //если i == 0, то y[i-1] заменяем на y[n-1]
        }else if (i == n-1) {
            a += polygon[i].x()*(polygon[0].y() - polygon[i-1].y()); // если i == n-1, то y[i+1] заменяем на y[0]
        }else {
            a += polygon[i].x()*(polygon[i+1].y() - polygon[i-1].y());
        }
    }

    return std::abs(a / 2.0);
}

bool SolveTile::check_intersection ()
{
    QPolygonF fout = transfer(Area, v1);
    QPolygonF p_list;
    p_list = fout.intersected(Area);
    if (area(p_list) != 0)
        return true;

    fout = transfer(Area, v1 + v2);
    p_list = fout.intersected(Area);
    if (area(p_list) != 0)
        return true;

    fout = transfer(Area, v2);
    p_list = fout.intersected(Area);
    if (area(p_list) != 0)
        return true;

    fout = transfer(Area, v2 - v1);
    p_list = fout.intersected(Area);
    if (area(p_list) != 0)
        return true;

    fout = transfer(Area, -v1);
    p_list = fout.intersected(Area);
    if (area(p_list) != 0)
        return true;

    fout = transfer(Area, -v1 - v2);
    p_list = fout.intersected(Area);
    if (area(p_list) != 0)
        return true;

    fout = transfer(Area, -v2);
    p_list = fout.intersected(Area);
    if (area(p_list) != 0)
        return true;

    fout = transfer(Area, v1 - v2);
    p_list = fout.intersected(Area);
    if (area(p_list) != 0)
        return true;

    return false;
}

bool SolveTile::check_hole()
{
    double square = std::abs(v1.x() * v2.y() - v1.y()*v2.x());
    if (square == area(Area)) {
        return false;
    } else {
        return true;
    }
}
