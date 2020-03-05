#include <catch.hpp>
#include "common.h"

using namespace knm::tr2;


TEST_CASE( "Set position", "[position]" )
{
    Transforms t;

    REQUIRE( equals(VEC3_ZERO, t.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, t.getWorldPosition()) );

    t.setPosition(10.0f, 20.0f, 30.0f);

    REQUIRE( equals(vec3(10.0f, 20.0f, 30.0f), t.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 20.0f, 30.0f), t.getWorldPosition()) );
}


TEST_CASE( "Set position of parent and child", "[position]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(VEC3_ZERO, p.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, p.getWorldPosition()) );
    REQUIRE( equals(VEC3_ZERO, c.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, c.getWorldPosition()) );

    p.setPosition(10.0f, 10.0f, 10.0f);
    c.setPosition(20.0f, 30.0f, 40.0f);

    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getWorldPosition()) );
    REQUIRE( equals(vec3(20.0f, 30.0f, 40.0f), c.getPosition()) );
    REQUIRE( equals(vec3(30.0f, 40.0f, 50.0f), c.getWorldPosition()) );
}


TEST_CASE( "Set position with rotated parent", "[position]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(VEC3_ZERO, p.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, p.getWorldPosition()) );
    REQUIRE( equals(VEC3_ZERO, c.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, c.getWorldPosition()) );

    p.setPosition(10.0f, 10.0f, 10.0f);
    p.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.setPosition(20.0f, 30.0f, 40.0f);

    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getWorldPosition()) );
    REQUIRE( equals(vec3(20.0f, 30.0f, 40.0f), c.getPosition()) );
    REQUIRE( equals(vec3(-30.0f, 40.0f, 30.0f), c.getWorldPosition()) );
}


TEST_CASE( "Set position with scaled parent", "[position]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(VEC3_ZERO, p.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, p.getWorldPosition()) );
    REQUIRE( equals(VEC3_ZERO, c.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, c.getWorldPosition()) );

    p.setPosition(10.0f, 10.0f, 10.0f);
    p.setScale(2.0f);
    c.setPosition(20.0f, 30.0f, 40.0f);

    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getWorldPosition()) );
    REQUIRE( equals(vec3(20.0f, 30.0f, 40.0f), c.getPosition()) );
    REQUIRE( equals(vec3(50.0f, 70.0f, 90.0f), c.getWorldPosition()) );
}
