#include <catch.hpp>
#include "common.h"

using namespace knm::tr2;


TEST_CASE( "Scaling", "[scaling]" )
{
    Transforms t;

    REQUIRE( equals(VEC3_UNIT_SCALE, t.getScale()) );
    REQUIRE( equals(VEC3_UNIT_SCALE, t.getWorldScale()) );

    t.scale(2.0f, 3.0f, 4.0f);

    REQUIRE( equals(vec3(2.0f, 3.0f, 4.0f), t.getScale()) );
    REQUIRE( equals(vec3(2.0f, 3.0f, 4.0f), t.getWorldScale()) );
}


TEST_CASE( "Scaling of parent and child", "[scaling]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(VEC3_UNIT_SCALE, p.getScale()) );
    REQUIRE( equals(VEC3_UNIT_SCALE, p.getWorldScale()) );
    REQUIRE( equals(VEC3_UNIT_SCALE, c.getScale()) );
    REQUIRE( equals(VEC3_UNIT_SCALE, c.getWorldScale()) );

    p.setScale(2.0f, 3.0f, 4.0f);
    c.scale(2.0f, 3.0f, 4.0f);

    REQUIRE( equals(vec3(2.0f, 3.0f, 4.0f), p.getScale()) );
    REQUIRE( equals(vec3(2.0f, 3.0f, 4.0f), p.getWorldScale()) );
    REQUIRE( equals(vec3(2.0f, 3.0f, 4.0f), c.getScale()) );
    REQUIRE( equals(vec3(4.0f, 9.0f, 16.0f), c.getWorldScale()) );
}
