#include <catch.hpp>
#include "common.h"

using namespace knm::tr2;


TEST_CASE( "Creation", "[creation]" )
{
    Transforms* t = nullptr;

    SECTION("without arguments")
    {
        t = new Transforms();

        REQUIRE( t->getTransformable() == nullptr );
        REQUIRE( t->getTags().empty() );
    }

    SECTION("with null transformable")
    {
        t = new Transforms(nullptr);

        REQUIRE( t->getTransformable() == nullptr );
        REQUIRE( t->getTags().empty() );
    }

    REQUIRE( t->getParent() == nullptr );
    REQUIRE( t->getChildren().size() == 0 );

    delete t;
}
