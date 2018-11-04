#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>


#include "random.hpp"
#include <boost/shared_ptr.hpp>

void func(Random *a){
    boost::shared_ptr<Random> A(a);
}

BOOST_AUTO_TEST_CASE(randomTest){
    Random *a = new Random();
    func(a);
}
