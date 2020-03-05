#include <catch.hpp>
#include "common.h"

using namespace knm::tr2;


TEST_CASE( "Translation (local)", "[translation]" )
{
    Transforms t;

    REQUIRE( equals(VEC3_ZERO, t.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, t.getWorldPosition()) );

    t.translate(10.0f, 20.0f, 30.0f, TS_LOCAL);

    REQUIRE( equals(vec3(10.0f, 20.0f, 30.0f), t.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 20.0f, 30.0f), t.getWorldPosition()) );
}


TEST_CASE( "Translation (parent)", "[translation]" )
{
    Transforms t;

    REQUIRE( equals(VEC3_ZERO, t.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, t.getWorldPosition()) );

    t.translate(10.0f, 20.0f, 30.0f, TS_PARENT);

    REQUIRE( equals(vec3(10.0f, 20.0f, 30.0f), t.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 20.0f, 30.0f), t.getWorldPosition()) );
}


TEST_CASE( "Translation (world)", "[translation]" )
{
    Transforms t;

    REQUIRE( equals(VEC3_ZERO, t.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, t.getWorldPosition()) );

    t.translate(10.0f, 20.0f, 30.0f, TS_WORLD);

    REQUIRE( equals(vec3(10.0f, 20.0f, 30.0f), t.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 20.0f, 30.0f), t.getWorldPosition()) );
}


TEST_CASE( "Translation (local) of child (no rotation)", "[translation]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(VEC3_ZERO, p.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, p.getWorldPosition()) );
    REQUIRE( equals(VEC3_ZERO, c.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, c.getWorldPosition()) );

    p.setPosition(10.0f, 10.0f, 10.0f);
    c.translate(20.0f, 30.0f, 40.0f, TS_LOCAL);

    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getWorldPosition()) );
    REQUIRE( equals(vec3(20.0f, 30.0f, 40.0f), c.getPosition()) );
    REQUIRE( equals(vec3(30.0f, 40.0f, 50.0f), c.getWorldPosition()) );
}


TEST_CASE( "Translation (parent) of child (no rotation)", "[translation]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(VEC3_ZERO, p.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, p.getWorldPosition()) );
    REQUIRE( equals(VEC3_ZERO, c.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, c.getWorldPosition()) );

    p.setPosition(10.0f, 10.0f, 10.0f);
    c.translate(20.0f, 30.0f, 40.0f, TS_PARENT);

    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getWorldPosition()) );
    REQUIRE( equals(vec3(20.0f, 30.0f, 40.0f), c.getPosition()) );
    REQUIRE( equals(vec3(30.0f, 40.0f, 50.0f), c.getWorldPosition()) );
}


TEST_CASE( "Translation (world) of child (no rotation)", "[translation]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(VEC3_ZERO, p.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, p.getWorldPosition()) );
    REQUIRE( equals(VEC3_ZERO, c.getPosition()) );
    REQUIRE( equals(VEC3_ZERO, c.getWorldPosition()) );

    p.setPosition(10.0f, 10.0f, 10.0f);
    c.translate(20.0f, 30.0f, 40.0f, TS_WORLD);

    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getWorldPosition()) );
    REQUIRE( equals(vec3(20.0f, 30.0f, 40.0f), c.getPosition()) );
    REQUIRE( equals(vec3(30.0f, 40.0f, 50.0f), c.getWorldPosition()) );
}


TEST_CASE( "Translation (local) of child (with rotations)", "[translation]" )
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
    c.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.translate(20.0f, 30.0f, 40.0f, TS_LOCAL);

    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getWorldPosition()) );
    REQUIRE( equals(vec3(-40.0f, 30.0f, 20.0f), c.getPosition()) );
    REQUIRE( equals(vec3(-10.0f, 40.0f, -30.0f), c.getWorldPosition()) );
}


TEST_CASE( "Translation (parent) of child (with rotations)", "[translation]" )
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
    c.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.translate(20.0f, 30.0f, 40.0f, TS_PARENT);

    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getWorldPosition()) );
    REQUIRE( equals(vec3(20.0f, 30.0f, 40.0f), c.getPosition()) );
    REQUIRE( equals(vec3(-30.0f, 40.0f, 30.0f), c.getWorldPosition()) );
}


TEST_CASE( "Translation (world) of child (with rotations)", "[translation]" )
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
    c.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.translate(20.0f, 30.0f, 40.0f, TS_WORLD);

    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getWorldPosition()) );
    REQUIRE( equals(vec3(40.0f, 30.0f, -20.0f), c.getPosition()) );
    REQUIRE( equals(vec3(30.0f, 40.0f, 50.0f), c.getWorldPosition()) );
}


TEST_CASE( "Translation (local) of child (with scaling)", "[translation]" )
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
    c.setScale(2.0f);
    c.translate(20.0f, 30.0f, 40.0f, TS_LOCAL);

    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getWorldPosition()) );
    REQUIRE( equals(vec3(20.0f, 30.0f, 40.0f), c.getPosition()) );
    REQUIRE( equals(vec3(50.0f, 70.0f, 90.0f), c.getWorldPosition()) );
}


TEST_CASE( "Translation (parent) of child (with scaling)", "[translation]" )
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
    c.setScale(2.0f);
    c.translate(20.0f, 30.0f, 40.0f, TS_PARENT);

    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getWorldPosition()) );
    REQUIRE( equals(vec3(20.0f, 30.0f, 40.0f), c.getPosition()) );
    REQUIRE( equals(vec3(50.0f, 70.0f, 90.0f), c.getWorldPosition()) );
}


TEST_CASE( "Translation (world) of child (with scaling)", "[translation]" )
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
    c.setScale(2.0f);
    c.translate(20.0f, 30.0f, 40.0f, TS_WORLD);

    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getPosition()) );
    REQUIRE( equals(vec3(10.0f, 10.0f, 10.0f), p.getWorldPosition()) );
    REQUIRE( equals(vec3(10.0f, 15.0f, 20.0f), c.getPosition()) );
    REQUIRE( equals(vec3(30.0f, 40.0f, 50.0f), c.getWorldPosition()) );
}
