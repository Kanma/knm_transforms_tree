#include <catch.hpp>
#include "common.h"

using namespace knm::tr2;


TEST_CASE( "No tag", "[tags]" )
{
    Transforms t;

    REQUIRE( t.getTags().empty() );
    REQUIRE( !t.hasTag("unknown") );
}


TEST_CASE( "Tag addition", "[tags]" )
{
    Transforms t;

    t.addTag("tag1");
    t.addTag("tag2");
    t.addTag("tag3");

    REQUIRE( t.getTags().size() == 3 );
    REQUIRE( t.hasTag("tag1") );
    REQUIRE( t.hasTag("tag2") );
    REQUIRE( t.hasTag("tag3") );
    REQUIRE( !t.hasTag("unknown") );
}


TEST_CASE( "Tag removal", "[tags]" )
{
    Transforms t;

    t.addTag("tag1");
    t.addTag("tag2");
    t.addTag("tag3");

    t.removeTag("tag2");

    REQUIRE( t.getTags().size() == 2 );
    REQUIRE( t.hasTag("tag1") );
    REQUIRE( t.hasTag("tag3") );
    REQUIRE( !t.hasTag("tag2") );
}
