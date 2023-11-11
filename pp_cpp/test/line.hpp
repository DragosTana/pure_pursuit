#ifndef TEST_THE_LINE__LOVE_ISNT_ALWAYS_ON_TIME
#define TEST_THE_LINE__LOVE_ISNT_ALWAYS_ON_TIME

#include "testbase.hpp"
#include "../geometry.hpp"

void test_line() {
    test_suite(inizializzazione) {
        test_case(punti 0 0) {
            Line l = Line::from_points(Point(0,0), Point(1,1));
            ass_eq_float(l.m, 1, EPS);
            ass_eq_float(l.q, 0, EPS);
        }
    }
}

void test_line_intersects() {
    test_suite(con gli assi) {
        test_case(asse x no intersect) {
            Line l = Line(0, 0);
            std::vector<Point> pv;
            pv = l.circle_intersections(Point(10, 10), 1.0);
            ass_eq(pv.size(), 0);
            pv = l.circle_intersections(Point(5, 5), 1.0);
            ass_eq(pv.size(), 0);
            pv = l.circle_intersections(Point(-5, -2), 1.0);
            ass_eq(pv.size(), 0);
        }

        /*
        test_case(asse x punto doppio intersect) {
            Line l = Line(0, 0);
            std::vector<Point> pv;
            Point p=Point(0,0);
            pv = l.circle_intersections(p=Point(10, 10), 10.0);
            ass_eq(pv.size(), 2);
            ass_eq_float(pv[0].x, 10, EPS); ass_eq_float(pv[0].y, 0, EPS);
            ass_eq_float(pv[1].x, 10, EPS); ass_eq_float(pv[1].y, 0, EPS);

            pv = l.circle_intersections(p=Point(-5, -2), 2.0);
            ass_eq(pv.size(), 2);
            ass_eq_float(pv[0].x, -5.0, EPS); ass_eq_float(pv[0].y, 0.0, EPS);
            ass_eq_float(pv[1].x, -5.0, EPS); ass_eq_float(pv[1].y, 0.0, EPS);
        }
        */

        test_case(asse x normale 1) {
            Line l = Line(0, 0);
            std::vector<Point> pv;
            pv = l.circle_intersections(Point(3,4), 5.0);
            ass_eq(pv.size(), 2);
            Point a = pv[0];
            Point b = pv[1];
            if (a.x >= b.x)
                swap(a,b);

            ass_eq_float(a.x, 0, EPS); ass_eq_float(a.y, 0, EPS);
            ass_eq_float(b.x, 6, EPS); ass_eq_float(b.y, 0, EPS);
        }

        test_case(asse x normale 2) {
            Line l = Line(0, 0);
            std::vector<Point> pv;
            pv = l.circle_intersections(Point(4,3), 5.0);
            ass_eq(pv.size(), 2);
            Point a = pv[0];
            Point b = pv[1];
            if (a.x >= b.x)
                swap(a,b);

            ass_eq_float(a.x, 0, EPS); ass_eq_float(a.y, 0, EPS);
            ass_eq_float(b.x, 8, EPS); ass_eq_float(b.y, 0, EPS);
        }

        test_case(asse y) {
        }
    }

    test_suite(preso da segment circle intersects) {
        const double root_two = sqrt(2);
        const double root_two_half = root_two/2;
        test_case(base interseca due punti) {
            double r = 1;
            Point c = Point(0,0);
            auto l = Line::from_points(Point(-1, -1), Point(1, 1));
            auto intersects = l.circle_intersections(c, r);
            ass_eq(intersects.size(), 2);
            Point a = intersects[0];
            Point b = intersects[1];
            if (a.x >= b.x)
                swap(a,b);

            ass_eq_float(a.x, -root_two_half, EPS);
            ass_eq_float(a.y, -root_two_half, EPS);

            ass_eq_float(b.x, root_two_half, EPS);
            ass_eq_float(b.y, root_two_half, EPS);

            ass_eq_float(a.distance(c), r, EPS);
            ass_eq_float(c.distance(a), r, EPS);

            ass_eq_float(b.distance(c), r, EPS);
            ass_eq_float(c.distance(b), r, EPS);
        }

        test_case(molto verticaloso parte 1) {
            double r = 2;
            Point c = Point(0,0);
            Line l = Line::from_points(Point(0, 0), Point(0.0001, 1000));
            auto intersects = l.circle_intersections(c, r);
            ass_eq(intersects.size(),2);

            Point a = intersects[0];
            Point b = intersects[1];
            if (a.y >= b.y)
                swap(a,b);

            exp_eq_float(a.x, 0, 0.00001);
            exp_eq_float(a.y, -2, 0.00001);

            exp_eq_float(b.x, 0, 0.00001);
            exp_eq_float(b.y, 2, 0.00001);

            exp_eq_float(a.distance(c), r, EPS);
            exp_eq_float(c.distance(a), r, EPS);

            exp_eq_float(b.distance(c), r, EPS);
            exp_eq_float(c.distance(b), r, EPS);
        }

        test_case(molto verticaloso parte due) {
            double r = 1.5;
            Point c = Point(0,1.5);
            Line l  = Line::from_points(Point(0, 0), Point(0.0001, 1000));
            auto intersects = l.circle_intersections(c, r);
            ass_eq(intersects.size(),2);

            Point a = intersects[0];
            Point b = intersects[1];
            if (a.y >= b.y)
                swap(a,b);

            exp_eq_float(a.x, 0, 0.00001);
            exp_eq_float(a.y, 0, 0.00001);

            exp_eq_float(b.x, 0, 0.00001);
            exp_eq_float(b.y, 3, 0.00001);

            exp_eq_float(a.distance(c), r, EPS);
            exp_eq_float(c.distance(a), r, EPS);

            exp_eq_float(b.distance(c), r, EPS);
            exp_eq_float(c.distance(b), r, EPS);
        }
    }

    test_suite(quel cazzo di vertice) {
        test_case(guarda guarda sto segmento del cazzo che non fa niente) {
            Line l = Line::from_points(Point(17.2344, 91.9884),
                                       Point(27.9448, 92.7779));
            Point c = Point(14.9834, 91.3054);
            double r = 6;
            std::vector<Point> intersects = l.circle_intersections(c, r);

            ass_eq(intersects.size(), 2);

            exp_eq_float(c.distance(intersects[0]), r, 0.00001);
            exp_eq_float(intersects[0].distance(c), r, 0.00001);

            exp_eq_float(c.distance(intersects[1]), r, 0.00001);
            exp_eq_float(intersects[1].distance(c), r, 0.00001);
        }
    }
}

#endif
