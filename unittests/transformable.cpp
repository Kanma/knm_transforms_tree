#include <catch.hpp>
#include "common.h"

using namespace knm::tr2;


#ifdef KNM_TRANSFORMS_TREE_USE_TRANSFORMABLE

TEST_CASE( "Transformable creation", "[transformable]" )
{
    Transformable obj;

    REQUIRE( obj.getTransforms()->getParent() == nullptr );
    REQUIRE( obj.getTransforms()->getChildren().size() == 0 );
    REQUIRE( obj.getTransforms()->getTransformable() == &obj );
}


TEST_CASE( "Transformable set parent", "[transformable]" )
{
    Transformable p;
    Transformable c;

    c.setParent(&p);

    REQUIRE( p.getTransforms()->getParent() == nullptr );
    REQUIRE( p.getTransforms()->getChildren().size() == 1 );
    REQUIRE( p.getTransforms()->getChildren()[0] == c.getTransforms() );
    REQUIRE( p.getTransforms()->getTransformable() == &p );

    REQUIRE( c.getTransforms()->getParent() == p.getTransforms() );
    REQUIRE( c.getTransforms()->getChildren().size() == 0 );
    REQUIRE( c.getTransforms()->getTransformable() == &c );
}


TEST_CASE( "Transformable set null parent", "[transformable]" )
{
    Transformable c;

    c.setParent(nullptr);

    REQUIRE( c.getTransforms()->getParent() == nullptr );
    REQUIRE( c.getTransforms()->getChildren().size() == 0 );
    REQUIRE( c.getTransforms()->getTransformable() == &c );
}


TEST_CASE( "Transformable remove parent", "[transformable]" )
{
    Transformable p;
    Transformable c;

    c.setParent(&p);
    c.setParent(nullptr);

    REQUIRE( p.getTransforms()->getParent() == nullptr );
    REQUIRE( p.getTransforms()->getChildren().size() == 0 );
    REQUIRE( p.getTransforms()->getTransformable() == &p );

    REQUIRE( c.getTransforms()->getParent() == nullptr );
    REQUIRE( c.getTransforms()->getChildren().size() == 0 );
    REQUIRE( c.getTransforms()->getTransformable() == &c );
}


TEST_CASE( "Transformable switch parent", "[transformable]" )
{
    Transformable p1;
    Transformable p2;
    Transformable c;

    c.setParent(&p1);
    c.setParent(&p2);

    REQUIRE( p1.getTransforms()->getParent() == nullptr );
    REQUIRE( p1.getTransforms()->getChildren().size() == 0 );
    REQUIRE( p1.getTransforms()->getTransformable() == &p1 );

    REQUIRE( p2.getTransforms()->getParent() == nullptr );
    REQUIRE( p2.getTransforms()->getChildren().size() == 1 );
    REQUIRE( p2.getTransforms()->getChildren()[0] == c.getTransforms() );
    REQUIRE( p2.getTransforms()->getTransformable() == &p2 );

    REQUIRE( c.getTransforms()->getParent() == p2.getTransforms() );
    REQUIRE( c.getTransforms()->getChildren().size() == 0 );
    REQUIRE( c.getTransforms()->getTransformable() == &c );
}

#else

namespace Tests
{
    class CustomTransformable
    {
    public:
        CustomTransformable()
        : transforms(this)
        {
        }


    public:
        knm::tr2::Transforms transforms;
    };
}

#if KNM_TRANSFORMS_TREE_USE_VOID_PTR_TRANSFORMABLE

TEST_CASE( "Void pointer transformable creation", "[transformable]" )
{
    Tests::CustomTransformable obj;

    REQUIRE( obj.transforms.getParent() == nullptr );
    REQUIRE( obj.transforms.getChildren().size() == 0 );
    REQUIRE( obj.transforms.getTransformable() == (void*) &obj );
    REQUIRE( obj.transforms.getTransformable<Tests::CustomTransformable*>() == &obj );
}

#else

TEST_CASE( "Custom transformable creation", "[transformable]" )
{
    Tests::CustomTransformable obj;

    REQUIRE( obj.transforms.getParent() == nullptr );
    REQUIRE( obj.transforms.getChildren().size() == 0 );
    REQUIRE( obj.transforms.getTransformable() == &obj );
}

#endif

#endif
