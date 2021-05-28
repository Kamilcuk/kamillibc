#include "test.t.h"

TEST(Logical, not){
    EXPECT(kuint128_not(kuint128_init_u(0xffffffff)) == false);
    EXPECT(kuint128_not(kuint128_init_u(0)) == true);
}

int main(){
	RUNTEST(Logical, not);
}
