#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "HCTree.hpp"
using namespace std;
using namespace testing;

class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    SimpleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        // freqs['a'] = 2;
        // freqs['b'] = 3;
        // added

        freqs['h'] = 2;
        freqs['u'] = 9;
        freqs['m'] = 2;
        freqs['n'] = 2;
        freqs['k'] = 2;
        freqs['a'] = 3;
        freqs['p'] = 1;
        freqs['\''] = 1;

        // end
        tree.build(freqs);
    }
};

TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('m', os);
    tree.encode('u', os);
    tree.encode('n', os);
    ASSERT_EQ(os.str(), "111001101");
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("11000");
    ASSERT_EQ(tree.decode(is), 'p');
}
