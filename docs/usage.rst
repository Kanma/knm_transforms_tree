Usage
=====

1. Copy the ``knm_transforms_tree.hpp`` file in a convenient place for your project

2. In *one* C++ file, add the following code to create the implementation:

.. code:: cpp

    #define KNM_TRANSFORMS_TREE_IMPLEMENTATION
    #include <knm_transforms_tree.hpp>


In other files, just use ``#include <knm_transforms_tree.hpp>``


A simple usage would be to create a hierarchy of transforms:

.. code:: cpp

    using namespace knm::tr2;

    Transforms a;
    Transforms b;
    Transforms c;

    b.setParent(&a);
    c.setParent(&b);

    a.setPosition(10.0f, 0.0f, 0.0f);
    a.setOrientation(0.707f, 0.0f, 0.707f. 0.0f);
    a.setScale(1.0f, 2.0f, 1.0f);

    b.translate(0.0f, 5.0f, 1.0f);

    c.roll(0.2f, TS_WORLD);

    ... = c.getWorldPosition();
    ... = c.getWorldOrientation();
    ... = c.getWorldScale();


A more usual usage would be to have ``Transforms`` attributes in your classes (see
the ``Transformable`` class) and to manage their relationships through them.
