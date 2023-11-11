/**
 * questo file contiene tutti i test che utilizzo per vedere se e come
 * sia andato tutto a puttane stavolta
 *
 * al momento basta anche un solo file, non sto facendo troppi test
 */


#include "../utils.hpp"
#include "quadratic.hpp"
#include "point.hpp"
#include "line.hpp"
#include "segment.hpp"
#include "the_cooler_segment.hpp"

int main() {
    test_quadratic();
    test_point();
    test_line();
    test_line_intersects();
    test_segment_intersects();
    cooler_test_segment_intersects();
    std::cout<<std::endl;
    std::cout<<"se non è esploso niente allora passato tutto"<<std::endl;
    std::cout<<"se è esploso qualcosa credo che tu l'abbia già visto"<<std::endl;
}
