/**
 * questo file contiene tutti i test che utilizzo per vedere se e come
 * sia andato tutto a puttane stavolta
 *
 * al momento basta anche un solo file, non sto facendo troppi test
 */

#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include<iostream>
#include <sstream>
#include <string>
#include <thread>
#include <limits>
#include <assert.h>
#include "../utils.hpp"
#include "../geometry.hpp"
#include "minitest/minitest.hpp"

#define EPS (0.000001)

#define swap(a, b) {                            \
        auto c = a;                             \
        a = b;                                  \
        b = c;                                  \
    }


void test_quadratic() {
  test_suite(soluzioni esistono) {
    test_case(base 1) {
        std::vector<double> s;
        s = quadratic_solutions(1,2,1);
        ass_eq(s.size(), 2);
        ass_eq_float(s[0], -1.0, EPS);
        ass_eq_float(s[1], -1.0, EPS);
    }end_case;

    test_case(base 2) {
        std::vector<double> s;
        s = quadratic_solutions(1,3,2);
        ass_eq(s.size(), 2);
        ass_eq_float(s[0], -1.0, EPS);
        ass_eq_float(s[1], -2.0, EPS);
    }end_case;
  }end_suite;

  test_suite(soluzioni non esistono) {
    test_case(gauss momento) {
        std::vector<double> s = quadratic_solutions(1, 0, 1);
        ass_eq(s.size(), 0);
    }end_case;
  }end_suite;
}

void test_point() {
  test_suite(point) {
    test_case(distanza) {
        // terne pitagoriche alla cazzo
        ass_eq_float(Point(0,0).distance(Point(3,4)), 5.0, EPS);
        ass_eq_float(Point(3,4).distance(Point(0,0)), 5.0, EPS);

        ass_eq_float(Point(0,0).distance(Point(4,3)), 5.0, EPS);
        ass_eq_float(Point(4,3).distance(Point(0,0)), 5.0, EPS);

        ass_eq_float(Point(0,0).distance(Point(12,5)), 13.0, EPS);
        ass_eq_float(Point(12,5).distance(Point(0,0)), 13.0, EPS);

        ass_eq_float(Point(0,0).distance(Point(5,12)), 13.0, EPS);
        ass_eq_float(Point(5,12).distance(Point(0,0)), 13.0, EPS);
    }end_case;
  }end_suite;
}

void test_line() {
    test_suite(inizializzazione) {
        test_case(punti 0 0) {
            Line l = Line::from_points(Point(0,0), Point(1,1));
            ass_eq_float(l.m, 1, EPS);
            ass_eq_float(l.q, 0, EPS);
        }end_case;
    }end_suite;
}

void test_line_intersects() {
    test_suite(con gli assi) {
        test_case(asse x no intersect) {
            Line l = Line(0, 0);
            std::vector<Point> pv;
            Point p=Point(0,0);
            pv = l.circle_intersections(p=Point(10, 10), 1.0);
            ass_eq(pv.size(), 0);
            pv = l.circle_intersections(p=Point(5, 5), 1.0);
            ass_eq(pv.size(), 0);
            pv = l.circle_intersections(p=Point(-5, -2), 1.0);
            ass_eq(pv.size(), 0);
        }end_case;

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
        }end_case;
        */

        test_case(asse x normale 1) {
            Line l = Line(0, 0);
            std::vector<Point> pv;
            Point p=Point(0,0);
            pv = l.circle_intersections(p=Point(3,4), 5.0);
            ass_eq(pv.size(), 2);
            Point a = pv[0];
            Point b = pv[1];
            if (a.x >= b.x)
                swap(a,b);

            ass_eq_float(a.x, 0, EPS); ass_eq_float(a.y, 0, EPS);
            ass_eq_float(b.x, 6, EPS); ass_eq_float(b.y, 0, EPS);
        }end_case;

        test_case(asse x normale 2) {
            Line l = Line(0, 0);
            std::vector<Point> pv;
            Point p=Point(0,0);
            pv = l.circle_intersections(p=Point(4,3), 5.0);
            ass_eq(pv.size(), 2);
            Point a = pv[0];
            Point b = pv[1];
            if (a.x >= b.x)
                swap(a,b);

            ass_eq_float(a.x, 0, EPS); ass_eq_float(a.y, 0, EPS);
            ass_eq_float(b.x, 8, EPS); ass_eq_float(b.y, 0, EPS);
        }end_case;

        test_case(asse y) {
        }end_case;
    }end_suite;
}

void test_segment_intersects() {
    return;
}

int main() {
    test_quadratic();
    test_point();
    test_line();
    test_line_intersects();
    test_segment_intersects();
    std::cout<<"se non è esploso niente allora passato tutto"<<std::endl;
    std::cout<<"se è esploso qualcosa credo che tu l'abbia già visto"<<std::endl;
}
