#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

// I really shouldn't have to do this...  investigate.  (The .lib file
// contains main(), but when we link against it, the error "entry
// point must be defined" appears.
#include <boost/test/impl/unit_test_main.ipp>
