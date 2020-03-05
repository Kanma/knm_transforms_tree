#include <catch.hpp>
#include "common.h"

using namespace knm::tr2;


TEST_CASE( "Rotation (local)", "[rotation]" )
{
    Transforms t;

    REQUIRE( equals(t.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(t.getWorldOrientation(), QUAT_IDENTITY) );

    t.rotate(VEC3_UNIT_Y, -PI / 2.0f, TS_LOCAL);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getWorldOrientation()) );
}


TEST_CASE( "Rotation (parent)", "[rotation]" )
{
    Transforms t;

    REQUIRE( equals(t.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(t.getWorldOrientation(), QUAT_IDENTITY) );

    t.rotate(VEC3_UNIT_Y, -PI / 2.0f, TS_PARENT);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getWorldOrientation()) );
}


TEST_CASE( "Rotation (world)", "[rotation]" )
{
    Transforms t;

    REQUIRE( equals(t.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(t.getWorldOrientation(), QUAT_IDENTITY) );

    t.rotate(VEC3_UNIT_Y, -PI / 2.0f, TS_WORLD);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getWorldOrientation()) );
}


TEST_CASE( "Rotation (local) around Y of child", "[rotation]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(QUAT_IDENTITY, p.getOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, p.getWorldOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, c.getOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, c.getWorldOrientation()) );

    p.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.rotate(VEC3_UNIT_Y, -PI / 2.0f, TS_LOCAL);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getWorldOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), c.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI, VEC3_UNIT_Y), c.getWorldOrientation()) );
}


TEST_CASE( "Rotation (parent) around Y of child", "[rotation]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(QUAT_IDENTITY, p.getOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, p.getWorldOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, c.getOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, c.getWorldOrientation()) );

    p.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.rotate(VEC3_UNIT_Y, -PI / 2.0f, TS_PARENT);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getWorldOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), c.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI, VEC3_UNIT_Y), c.getWorldOrientation()) );
}


TEST_CASE( "Rotation (world) around Y of child", "[rotation]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(QUAT_IDENTITY, p.getOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, p.getWorldOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, c.getOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, c.getWorldOrientation()) );

    p.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.rotate(VEC3_UNIT_Y, -PI / 2.0f, TS_WORLD);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getWorldOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), c.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI, VEC3_UNIT_Y), c.getWorldOrientation()) );
}


TEST_CASE( "Rotation (local) around X of child", "[rotation]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(QUAT_IDENTITY, p.getOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, p.getWorldOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, c.getOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, c.getWorldOrientation()) );

    p.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.rotate(VEC3_UNIT_X, -PI / 2.0f, TS_LOCAL);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getWorldOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_X), c.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y) * KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_X),
                    c.getWorldOrientation()) );
}


TEST_CASE( "Rotation (parent) around X of child", "[rotation]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(QUAT_IDENTITY, p.getOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, p.getWorldOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, c.getOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, c.getWorldOrientation()) );

    p.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.rotate(VEC3_UNIT_X, -PI / 2.0f, TS_PARENT);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getWorldOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_X), c.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y) * KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_X),
                    c.getWorldOrientation()) );
}


TEST_CASE( "Rotation (world) around X of child", "[rotation]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(QUAT_IDENTITY, p.getOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, p.getWorldOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, c.getOrientation()) );
    REQUIRE( equals(QUAT_IDENTITY, c.getWorldOrientation()) );

    p.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.rotate(VEC3_UNIT_X, -PI / 2.0f, TS_WORLD);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getWorldOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(PI / 2.0f, VEC3_UNIT_Z), c.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y) * KNM_TRANSFORMS_TREE_ANGLE_AXIS(PI / 2.0f, VEC3_UNIT_Z),
                    c.getWorldOrientation()) );
}
