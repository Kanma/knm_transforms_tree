#include <catch.hpp>
#include "common.h"

using namespace knm::tr2;


TEST_CASE( "Set parent", "[hierarchy]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);

    REQUIRE( p.getParent() == nullptr );
    REQUIRE( p.getChildren().size() == 1 );
    REQUIRE( p.getChildren()[0] == &c );

    REQUIRE( c.getParent() == &p );
    REQUIRE( c.getChildren().size() == 0 );
}


TEST_CASE( "Set null parent", "[hierarchy]" )
{
    Transforms c;

    c.setParent(nullptr);

    REQUIRE( c.getParent() == nullptr );
    REQUIRE( c.getChildren().size() == 0 );
}


TEST_CASE( "Remove parent", "[hierarchy]" )
{
    Transforms p;
    Transforms c;

    c.setParent(&p);
    c.setParent(nullptr);

    REQUIRE( p.getParent() == nullptr );
    REQUIRE( p.getChildren().size() == 0 );

    REQUIRE( c.getParent() == nullptr );
    REQUIRE( c.getChildren().size() == 0 );
}


TEST_CASE( "Switch parent", "[hierarchy]" )
{
    Transforms p1;
    Transforms p2;
    Transforms c;

    c.setParent(&p1);
    c.setParent(&p2);

    REQUIRE( p1.getParent() == nullptr );
    REQUIRE( p1.getChildren().size() == 0 );

    REQUIRE( p2.getParent() == nullptr );
    REQUIRE( p2.getChildren().size() == 1 );

    REQUIRE( c.getParent() == &p2 );
    REQUIRE( c.getChildren().size() == 0 );
}


TEST_CASE( "Find children by tag", "[hierarchy]" )
{
    Transforms a;
    Transforms b;
    Transforms c;
    Transforms d;

    a.addTag("tag");
    b.addTag("tag");
    d.addTag("tag");

    b.setParent(&a);
    c.setParent(&a);
    d.setParent(&b);

    auto children = a.findChildren("tag");

    REQUIRE( children.size() == 1 );
    REQUIRE( children[0] == &b );
}


TEST_CASE( "Find children by tag (recursive)", "[hierarchy]" )
{
    Transforms a;
    Transforms b;
    Transforms c;
    Transforms d;

    a.addTag("tag");
    b.addTag("tag");
    d.addTag("tag");

    b.setParent(&a);
    c.setParent(&a);
    d.setParent(&b);

    auto children = a.findChildren("tag", true);

    REQUIRE( children.size() == 2 );
    REQUIRE( children[0] == &b );
    REQUIRE( children[1] == &d );
}


TEST_CASE( "Find children by unknown tag", "[hierarchy]" )
{
    Transforms a;
    Transforms b;
    Transforms c;
    Transforms d;

    a.addTag("tag");
    b.addTag("tag");
    d.addTag("tag");

    b.setParent(&a);
    c.setParent(&a);
    d.setParent(&b);

    auto children = a.findChildren("unknown");

    REQUIRE( children.size() == 0 );
}
