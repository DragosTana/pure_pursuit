#ifndef MINITEST_HEADER_MOTHERFUCKER
#define MINITEST_HEADER_MOTHERFUCKER

#include<iostream>

std::string suite_name;
std::string test_name;
bool case_failed= 0;  //did a test fail in the current case? 
bool suite_failed= 0; //did a test fail in the current suite? 
int this_is_a_counter_used_by_the_test_macros_please_dont_use_it_thx = 0;

/* all test macros are big ass loops because
 * breaking out of loops is the best flow control I could think of
 * given how do{}while(false) gets executed only once
 * and has all the good break shit I used that
 */

/* sythax with and without curly braces is supported,
 * mostly by accident,
 * this thing was magnificiently designed
 */

void report_case_and_suite() {
    std::cout << "\nin test case <" << test_name
              << ">\nin test suite <" << suite_name << "> \n";
}

template <typename T, typename U>
void report_inequality(std::string exp_name, std::string act_name,
                       T exp_val, U act_val) {
    std::cout<<exp_name<< " is not equal to " << act_name<<'\n';
    std::cout<<exp_name<< " is <" << exp_val << "> -- "
             << act_name << " is <" << act_val << ">\n";
}

template <typename T, typename U>
void report_equality(std::string exp_name, std::string act_name,
                       T exp_val, U act_val) {
    std::cout<<exp_name<< " is equal to " << act_name<<'\n';
    std::cout<<exp_name<< " is <" << exp_val << "> -- "
             << act_name << " is <" << act_val << ">\n";
}

//test cases
// only way I figured how to make a loop run only once
// (we need a loop construct to make assertions break out of tests)
#define test_case(name)test_name = #name;                               \
    case_failed = 0;                                                    \
    for(this_is_a_counter_used_by_the_test_macros_please_dont_use_it_thx = 0; \
        this_is_a_counter_used_by_the_test_macros_please_dont_use_it_thx == 0; \
        this_is_a_counter_used_by_the_test_macros_please_dont_use_it_thx = 1 ) \

//suites
#define test_suite(name) suite_name = #name;    \
    /*initialize the fail flags*/               \
    if(true)

/* the following have been put in a useless if to limit
 * the scope of the variables inside
 * because I'm reusing the same code everywhere and don't want
 * variable name conflicts    
 *
 * and I ain't making fucking gensyms in c++,
 * they'd ruin the output, and be fucking stupid
 */

#define exp_eq(exp, act) if(true){                              \
        auto exp_val = (exp);                                   \
        auto act_val = (act);                                   \
        if(exp_val != act_val) {                                \
            report_case_and_suite();                            \
            report_inequality(#exp, #act, exp_val, act_val);    \
        }                                                       \
    }

#define ass_eq(exp, act)if(true){                                   \
        auto exp_val = (exp);                                       \
        auto act_val = (act);                                       \
        if(exp_val != act_val){                                     \
            report_case_and_suite();                                \
            report_inequality(#exp, #act, exp_val, act_val);        \
            break; /* failing an assertion aborts the test case*/   \
        }                                                           \
    }

#define exp_neq(exp, act) if(true){                              \
        auto exp_val = (exp);                                   \
        auto act_val = (act);                                   \
        if(exp_val == act_val) {                                \
            report_case_and_suite();                            \
            report_equality(#exp, #act, exp_val, act_val);    \
        }                                                       \
    }

#define ass_neq(exp, act)if(true){                                   \
        auto exp_val = (exp);                                       \
        auto act_val = (act);                                       \
        if(exp_val == act_val){                                     \
            report_case_and_suite();                                \
            report_equality(#exp, #act, exp_val, act_val);        \
            break; /* failing an assertion aborts the test case*/   \
        }                                                           \
    }

#define exp_eq_float(exp, act, delta)if(true){                  \
        auto exp_val = (exp);                                   \
        auto act_val = (act);                                   \
        if(fabs(exp-act)>delta){                                \
            report_case_and_suite();                            \
            report_inequality(#exp, #act, exp_val, act_val);    \
        }                                                       \
    }

#define ass_eq_float(exp, act, delta)if(true){                      \
        auto exp_val = (exp);                                       \
        auto act_val = (act);                                       \
        if(fabs(exp-act)>delta){                                    \
            report_case_and_suite();                                \
            report_inequality(#exp, #act, exp_val, act_val);        \
            break; /* failing an assertion aborts the test case*/   \
        }                                                           \
    }

#define exp_neq_float(exp, act, delta)if(true){                  \
        auto exp_val = (exp);                                   \
        auto act_val = (act);                                   \
        if(fabs(exp-act)<=delta){                                \
            report_case_and_suite();                            \
            report_equality(#exp, #act, exp_val, act_val);    \
        }                                                       \
    }

#define ass_neq_float(exp, act, delta)if(true){                      \
        auto exp_val = (exp);                                       \
        auto act_val = (act);                                       \
        if(fabs(exp-act)<=delta){                                    \
            report_case_and_suite();                                \
            report_equality(#exp, #act, exp_val, act_val);        \
            break; /* failing an assertion aborts the test case*/   \
        }                                                           \
    }

#define exp_t(condition) if(!(condition)){                              \
        report_case_and_suite();                                        \
        std::cout<<#condition<<" was supposed to be true but is false\n"; \
    }

#define ass_t(condition) if(!(condition)){                              \
        report_case_and_suite();                                        \
        std::cout<<#condition<<" was supposed to be true but is false\n"; \
        break; /* failing an assertion aborts the test case*/           \
    }

#define exp_false(condition) if(condition){                              \
        report_case_and_suite();                                        \
        std::cout<<#condition<<" was supposed to be false but is true\n"; \
    }

#define ass_false(condition) if(condition){                              \
        report_case_and_suite();                                        \
        std::cout<<#condition<<" was supposed to be false but is true\n"; \
        break; /* failing an assertion aborts the test case*/           \
    }

#endif
