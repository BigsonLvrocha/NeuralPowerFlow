
#define BOOST_TEST_MODULE teste_principal
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

BOOST_AUTO_TEST_CASE(segundo_test)
{
  int i = 1;
  BOOST_TEST(i);
  BOOST_TEST(i == 1);
}


