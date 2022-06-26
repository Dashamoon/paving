#include<iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Vector_2.h>
#include <list>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2                                   Point_2;
typedef Kernel::Vector_2                                  Vector_2;
typedef CGAL::Polygon_2<Kernel>                           Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>                Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>                   Pwh_list_2;
//typedef CGAL::GeneralPolygonWithHoles_2                   GPWH_2;


using namespace std;

Polygon_2 transfer (Polygon_2 P, Vector_2 v){
    for (int i = 0; i < P.size(); i++){
        P[i] += v;
    }

    return P;
}

bool check_intercetion (Polygon_2 f, Vector_2 v1, Vector_2 v2) {
    Polygon_2 fout = transfer(f, v1);
    Pwh_list_2 p_list;
    CGAL::intersection (f, fout, std::back_inserter(p_list));
    if (!p_list.empty())
        return true;

    fout = transfer(f, v1 + v2);
    CGAL::intersection (f, fout, std::back_inserter(p_list));
    if (!p_list.empty())
        return true;
    
    fout = transfer(f, v2);
    CGAL::intersection (f, fout, std::back_inserter(p_list));
    if (!p_list.empty())
        return true;

    fout = transfer(f, v2 - v1);
    CGAL::intersection (f, fout, std::back_inserter(p_list));
    if (!p_list.empty())
        return true;

    fout = transfer(f, -v1);
    CGAL::intersection (f, fout, std::back_inserter(p_list));
    if (!p_list.empty())
        return true;

    fout = transfer(f, -v1 - v2);
    CGAL::intersection (f, fout, std::back_inserter(p_list));
    if (!p_list.empty())
        return true;

    fout = transfer(f, -v2);
    CGAL::intersection (f, fout, std::back_inserter(p_list));
    if (!p_list.empty())
        return true;

    fout = transfer(f, v1 - v2);
    CGAL::intersection (f, fout, std::back_inserter(p_list));
    if (!p_list.empty())
        return true;

    return false;
}

bool check_hole (Polygon_2 f, Vector_2 v1, Vector_2 v2){
    Polygon_with_holes_2 q, q_;
    if(CGAL::join(f, transfer(f, v1), q_)){
        q = q_;
        if(CGAL::join(q, transfer(f,v2), q_)){
            q = q_;
            if(CGAL::join(q, transfer(f, v2 + v1), q_)){
                if(q_.holes_begin() == q_.holes_end()){
                    return false;
                }else{
                    return true;
                }
            }else{
                return true;
            }
        }else{
           return true; 
        }
    }else{
        return true;
    }
}

// bool has_holes(Polygon_2 p1, Vector_2 v1, Vector_2 v2) {
//     Polygon_with_holes_2 q;
//     if (!CGAL::join(p1, transfer(p1, v1), q))
//         return true;
//     if (q.holes_begin() != q.holes_end())
//         return true;
//     // ....
//     return false;
// 

int main(){
    Polygon_2 P, Q;
    Polygon_with_holes_2 F;
    int n;
    cout << "кол-во фигур" << endl;
    cin >> n;
    int x, y, m;
    cout << "кол-во вершин в фигуре" << endl;
    cin >> m;
    for(int j = 0; j < m; j++){
        cin >> x >> y;
        Q.push_back(Point_2(x, y));
    }
    if (!Q.is_counterclockwise_oriented())
        Q.reverse_orientation();
    int i = 1;
    while (i < n){
        cout << "кол-во вершин в фигуре" << endl;
        cin >> m;
        for(int j = 0; j < m; j++){
            cin >> x >> y;
            P.push_back(Point_2(x, y));
        }
        if (!P.is_counterclockwise_oriented())
             P.reverse_orientation();

        if (CGAL::join(P, Q, F))
            cout << "join_YES" << endl;

        if (F.holes_begin() == F.holes_end()){
            if (!F.is_unbounded()){
                Q = F.outer_boundary();
                cout << "bounded()" << endl;
                i++;
            }else{
                cout << "unbounded()" << endl;
                i = n;
            }
        }else{
            cout << "holes" << endl;
            i = n;
        }
    }
    int a, b;
    cout << "vector" << endl;
    cin >> x >> y >> a >> b;
    if (!check_intercetion(Q, Vector_2(x, y), Vector_2(a, b)))
        cout << "intercetion no" << endl;
    if (!check_hole(Q, Vector_2(x, y), Vector_2(a, b)))
        cout << "hole no" << endl;
    //  if (!P.is_counterclockwise_oriented())
    //         P.reverse_orientation();
    return 0;
}