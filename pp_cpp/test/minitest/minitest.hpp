#include<iostream>

std::string suite_name;
std::string test_name;
bool case_failed= 0;  //did a test fail in the current case? 
bool suite_failed= 0; //did a test fail in the current suite? 

/* all test macros are big ass loops because
 * breaking out of loops is the best flow control I could think of
 * given how do{}while(false) gets executed only once
 * and has all the good break shit I used that
 */

/* sythax with and without curly braces is supported,
 * mostly by accident,
 * this thing was magnificiently designed
 */

//test cases
#define test_case(name)test_name = #name;       \
    case_failed = 0;                            \
    do{

#define end_case }while (false);                            \
    if (case_failed)                                        \
        std::cout<<"in test case "<<test_name<<'\n';        \
    /* refresh the "expect fail" flag after test case */    \


//suites
#define test_suite(name) suite_name = #name;    \
    /*initialize the fail flags*/               \
    case_failed = 0;                            \
    suite_failed = 0;                           \
    if(true){

#define end_suite }                                                     \
        if(suite_failed)std::cout<<"in suite "<<suite_name<<std::endl;


/* the following have been put in a useless if to limit
 * the scope of the variables inside
 * because I'm reusing the same code everywhere and don't want
 * variable name conflicts    
 *
 * and I ain't making fucking gensyms in c++,
 * they'd ruin the output, and be fucking stupid
 */

#define exp_eq(exp, act)if(true){                               \
        auto exp_val = (exp);                                   \
        auto act_val = (act);                                   \
        if(exp_val != act_val){                                 \
            std::cout<<#act<<" is not equal to "<<#exp<<'\n';   \
            std::cout<<#exp<<" is "<<exp_val<<'\n';             \
            std::cout<<#act<<" is "<<act_val<<'\n';             \
            case_failed = 1;                                    \
            suite_failed = 1;                                   \
        }                                                       \
    }

#define ass_eq(exp, act)if(true){                                   \
        auto exp_val = (exp);                                       \
        auto act_val = (act);                                       \
        if(exp_val != act_val){                                     \
            std::cout<<#act<<" is not equal to "<<#exp<<'\n';       \
            std::cout<<#exp<<" is "<<exp_val<<'\n';                 \
            std::cout<<#act<<" is "<<act_val<<'\n';                 \
            case_failed = 1;                                        \
            suite_failed = 1;                                       \
            break; /* failing an assertion aborts the test case*/   \
        }                                                           \
    }

#define exp_eq_float(exp, act, delta)if(true){                  \
        auto exp_val = (exp);                                   \
        auto act_val = (act);                                   \
        if(fabs(exp-act)>delta){                                \
            std::cout<<#act<<" is not equal to "<<#exp<<'\n';   \
            std::cout<<#exp<<" is "<<exp_val<<'\n';             \
            std::cout<<#act<<" is "<<act_val<<'\n';             \
            case_failed = 1;                                    \
            suite_failed = 1;                                   \
        }                                                       \
    }

#define ass_eq_float(exp, act, delta)if(true){                  \
    auto exp_val = (exp);                                       \
    auto act_val = (act);                                       \
    if(fabs(exp-act)>delta){                                    \
        std::cout<<#act<<" is not equal to "<<#exp<<'\n';       \
        std::cout<<#exp<<" is "<<exp_val<<'\n';                 \
        std::cout<<#act<<" is "<<act_val<<'\n';                 \
        case_failed = 1;                                        \
        suite_failed = 1;                                       \
        break; /* failing an assertion aborts the test case*/   \
    }                                                           \
    }

#define exp_t(condition) if(!(condition)){                              \
        std::cout<<#condition<<" was supposed to be true but is false\n"; \
        case_failed = 1;                                                \
        suite_failed = 1;                                               \
    }

#define ass_t(condition) if(!(condition)){                              \
        std::cout<<#condition<<" was supposed to be true but is false\n"; \
        case_failed = 1;                                                \
        suite_failed = 1;                                               \
        break; /* failing an assertion aborts the test case*/           \
    }
