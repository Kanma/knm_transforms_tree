#include <catch.hpp>
#include "common.h"

using namespace knm::tr2;


TEST_CASE( "Set orientation", "[orientation]" )
{
    Transforms t;

    REQUIRE( equals(t.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(t.getWorldOrientation(), QUAT_IDENTITY) );

    t.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getWorldOrientation()) );
}


TEST_CASE( "Set orientation of parent and child", "[orientation]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( equals(p.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(p.getWorldOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(c.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(c.getWorldOrientation(), QUAT_IDENTITY) );

    p.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getWorldOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), c.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI, VEC3_UNIT_Y), c.getWorldOrientation()) );
}


TEST_CASE( "No orientation inheritance", "[orientation]" )
{
    Transforms p;
    Transforms c;

    c.setInheritOrientation(false);

    c.setParent(&p);

    REQUIRE( equals(p.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(p.getWorldOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(c.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(c.getWorldOrientation(), QUAT_IDENTITY) );

    p.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getWorldOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), c.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), c.getWorldOrientation()) );
}


TEST_CASE( "Set direction (local)", "[orientation]" )
{
    Transforms t;

    REQUIRE( equals(t.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(t.getWorldOrientation(), QUAT_IDENTITY) );

    t.setDirection(VEC3_UNIT_X, TS_LOCAL, VEC3_NEGATIVE_UNIT_Z);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getWorldOrientation()) );
}


TEST_CASE( "Set direction (parent)", "[orientation]" )
{
    Transforms t;

    REQUIRE( equals(t.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(t.getWorldOrientation(), QUAT_IDENTITY) );

    t.setDirection(VEC3_UNIT_X, TS_PARENT, VEC3_NEGATIVE_UNIT_Z);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getWorldOrientation()) );
}


TEST_CASE( "Set direction (world)", "[orientation]" )
{
    Transforms t;

    REQUIRE( equals(t.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(t.getWorldOrientation(), QUAT_IDENTITY) );

    t.setDirection(VEC3_UNIT_X, TS_WORLD, VEC3_NEGATIVE_UNIT_Z);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), t.getWorldOrientation()) );
}


TEST_CASE( "Set direction (local) of child", "[orientation]" )
{
    Transforms p;
    Transforms c;
    
    c.setParent(&p);

    REQUIRE( equals(p.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(p.getWorldOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(c.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(c.getWorldOrientation(), QUAT_IDENTITY) );

    p.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.setDirection(VEC3_UNIT_X, TS_LOCAL, VEC3_NEGATIVE_UNIT_Z);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getWorldOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), c.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI, VEC3_UNIT_Y), c.getWorldOrientation()) );
}


TEST_CASE( "Set direction (parent) of child", "[orientation]" )
{
    Transforms p;
    Transforms c;
    
    c.setParent(&p);

    REQUIRE( equals(p.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(p.getWorldOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(c.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(c.getWorldOrientation(), QUAT_IDENTITY) );

    p.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.setDirection(VEC3_UNIT_X, TS_PARENT, VEC3_NEGATIVE_UNIT_Z);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getWorldOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), c.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI, VEC3_UNIT_Y), c.getWorldOrientation()) );
}


TEST_CASE( "Set direction (world) of child", "[orientation]" )
{
    Transforms p;
    Transforms c;
    
    c.setParent(&p);

    REQUIRE( equals(p.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(p.getWorldOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(c.getOrientation(), QUAT_IDENTITY) );
    REQUIRE( equals(c.getWorldOrientation(), QUAT_IDENTITY) );

    p.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    c.setDirection(VEC3_UNIT_X, TS_WORLD, VEC3_NEGATIVE_UNIT_Z);

    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), p.getWorldOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(0.0f, VEC3_UNIT_Y), c.getOrientation()) );
    REQUIRE( equals(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y), c.getWorldOrientation()) );
}

