#include "../include/bbwidgets/globals.hpp"


namespace bbwidgets {


    static_assert(cyclic_adapt(0, 0, 3) == 0);
    static_assert(cyclic_adapt(1, 0, 3) == 1);
    static_assert(cyclic_adapt(2, 0, 3) == 2);
    static_assert(cyclic_adapt(3, 0, 3) == 0);
    static_assert(cyclic_adapt(4, 0, 3) == 1);
    static_assert(cyclic_adapt(5, 0, 3) == 2);
    static_assert(cyclic_adapt(6, 0, 3) == 0);
    static_assert(cyclic_adapt(7, 0, 3) == 1);
    static_assert(cyclic_adapt(-1, 0, 3) == 2);
    static_assert(cyclic_adapt(-2, 0, 3) == 1);
    static_assert(cyclic_adapt(-3, 0, 3) == 0);
    static_assert(cyclic_adapt(-4, 0, 3) == 2);
    static_assert(cyclic_adapt(-5, 0, 3) == 1);
    static_assert(cyclic_adapt(-6, 0, 3) == 0);
    static_assert(cyclic_adapt(-7, 0, 3) == 2);


}
