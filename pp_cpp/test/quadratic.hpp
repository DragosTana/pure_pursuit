
#ifndef TEST_THE_FUCKING_QUADRATIC
#define TEST_THE_FUCKING_QUADRATIC

#include"testbase.hpp"
#include"../geometry.hpp"

void test_quadratic() {
  test_suite(soluzioni esistono) {
    test_case(base 1) {
        std::vector<double> s;
        s = quadratic_solutions(1,2,1);
        ass_eq(s.size(), 2);
        ass_eq_float(s[0], -1.0, EPS);
        ass_eq_float(s[1], -1.0, EPS);
    }

    test_case(base 2) {
        std::vector<double> s;
        s = quadratic_solutions(1,3,2);
        ass_eq(s.size(), 2);
        ass_eq_float(s[0], -1.0, EPS);
        ass_eq_float(s[1], -2.0, EPS);
    }
  }

  test_suite(soluzioni non esistono) {
    test_case(gauss momento) {
        std::vector<double> s = quadratic_solutions(1, 0, 1);
        ass_eq(s.size(), 0);
    }
  }
}

#endif
