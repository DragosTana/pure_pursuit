#ifndef TEST_SEGMENT_QUELLO_COL_MIO_CODICE_ORRENDO
#define TEST_SEGMENT_QUELLO_COL_MIO_CODICE_ORRENDO

#include "testbase.hpp"
#include "../geometry.hpp"

void test_segment_intersects() {
    test_suite(segment circle intersects) {
        const double root_two = sqrt(2);
        const double root_two_half = root_two/2;

        test_case(guarda guarda sto segmento del cazzo che non fa niente) {
            Segment s = Segment(Point(17.2344, 91.9884),
                                Point(27.9448, 92.7779));
            Point c = Point(14.9834, 91.3054);
            double r = 6;
            std::vector<Point> intersects = s.circle_intersections(c, r);

            ass_eq(intersects.size(), 1);
            exp_eq_float(intersects[0].x, 20.90711, 0.0001);
            exp_eq_float(intersects[0].y, 92.25912, 0.0001);
            std::cout<<"got here"<<std::endl;

            exp_eq_float(c.distance(intersects[0]), r, 0.00001);
            exp_eq_float(intersects[0].distance(c), r, 0.00001);
        }

        test_case(base interseca due punti) {
            double r = 1;
            Point c = Point(0,0);
            auto s = Segment(Point(-1, -1), Point(1, 1));
            auto intersects = s.circle_intersections(c, r);
            ass_eq(intersects.size(), 2);
            Point a = intersects[0];
            Point b = intersects[1];
            if (a.x >= b.x)
                swap(a,b);

            exp_eq_float(a.x, -root_two_half, EPS);
            exp_eq_float(a.y, -root_two_half, EPS);

            exp_eq_float(b.x, root_two_half, EPS);
            exp_eq_float(b.y, root_two_half, EPS);

            exp_eq_float(a.distance(c), r, EPS);
            exp_eq_float(c.distance(a), r, EPS);

            exp_eq_float(b.distance(c), r, EPS);
            exp_eq_float(c.distance(b), r, EPS);
        }

        test_case(base non interseca) {
            auto s = Segment(Point(2, 2), Point(1, 1));
            auto intersects = s.circle_intersections(Point(0,0), 1);
            ass_eq(intersects.size(), 0);
        }

        test_case(base interseca un punto) {
            double r = 2;
            Point c = Point(0,0);
            auto s = Segment(Point(2, 2), Point(1, 1));
            auto intersects = s.circle_intersections(c, r);
            ass_eq(intersects.size(), 1);
            exp_eq_float(intersects[0].x, root_two, EPS);
            exp_eq_float(intersects[0].y, root_two, EPS);

            exp_eq_float(intersects[0].distance(c), r, EPS);
            exp_eq_float(c.distance(intersects[0]), r, EPS);
        }

        test_case(molto verticaloso interseca un punto) {
            double r = 2;
            Point c = Point(0,0);
            auto s = Segment(Point(0, 0), Point(0.0001, 1000));
            auto intersects = s.circle_intersections(c, r);
            ass_eq(intersects.size(),1);

            exp_eq_float(intersects[0].x, 0, 0.00001);
            exp_eq_float(intersects[0].y, 2, 0.00001);

            exp_eq_float(intersects[0].distance(c), r, EPS);
            exp_eq_float(c.distance(intersects[0]), r, EPS);
        }

        test_case(verticaloso interseca due punti) {
            double r = 1.5;
            Point c = Point(0,1.5);
            auto s = Segment(Point(0, 0), Point(0.0001, 1000));
            auto intersects = s.circle_intersections(c, r);
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

        test_case(verticaloso ma non interseca che sta troppo in alto) {
            ass_t(true);
        }

        test_case(verticaloso ma non interseca che sta troppo in basso) {
            ass_t(true);
        }

        test_case(verticaloso ma non interseca che sta troppo a destra) {
            ass_t(true);
        }

        test_case(verticaloso ma non interseca che sta troppo a sinistra) {
            ass_t(true);
        }
    }
}

#endif
