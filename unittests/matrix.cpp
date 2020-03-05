#include <catch.hpp>
#include "common.h"

using namespace knm::tr2;


TEST_CASE( "Check matrix containing position", "[matrix]" )
{
    Transforms t;

    t.setPosition(10.0f, 20.0f, 30.0f);

    mat4 matrix = t.getMatrix();

    Transforms t2;
    t2.set(matrix);

    REQUIRE( equals(t.getPosition(), t2.getPosition()) );
    REQUIRE( equals(t.getOrientation(), t2.getOrientation()) );
    REQUIRE( equals(t.getScale(), t2.getScale()) );
}


TEST_CASE( "Check matrix containing orientation", "[matrix]" )
{
    Transforms t;

    t.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));

    mat4 matrix = t.getMatrix();

    Transforms t2;
    t2.set(matrix);

    REQUIRE( equals(t.getPosition(), t2.getPosition()) );
    REQUIRE( equals(t.getOrientation(), t2.getOrientation()) );
    REQUIRE( equals(t.getScale(), t2.getScale()) );
}


TEST_CASE( "Check matrix containing scale", "[matrix]" )
{
    Transforms t;

    t.setScale(10.0f, 20.0f, 30.0f);

    mat4 matrix = t.getMatrix();

    Transforms t2;
    t2.set(matrix);

    REQUIRE( equals(t.getPosition(), t2.getPosition()) );
    REQUIRE( equals(t.getOrientation(), t2.getOrientation()) );
    REQUIRE( equals(t.getScale(), t2.getScale()) );
}


TEST_CASE( "Check complete matrix", "[matrix]" )
{
    Transforms t;

    t.setPosition(10.0f, 20.0f, 30.0f);
    t.setOrientation(KNM_TRANSFORMS_TREE_ANGLE_AXIS(-PI / 2.0f, VEC3_UNIT_Y));
    t.setScale(10.0f, 20.0f, 30.0f);

    mat4 matrix = t.getMatrix();

    Transforms t2;
    t2.set(matrix);

    REQUIRE( equals(t.getPosition(), t2.getPosition()) );
    REQUIRE( equals(t.getOrientation(), t2.getOrientation()) );
    REQUIRE( equals(t.getScale(), t2.getScale()) );
}
