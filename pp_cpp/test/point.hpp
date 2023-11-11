#ifndef TEST_DAS_MOTHERFUCKING_POINT_WEEEEEEE
#define TEST_DAS_MOTHERFUCKING_POINT_WEEEEEEE

#include "testbase.hpp"
#include "../geometry.hpp"

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
    }
  }
}

#endif
