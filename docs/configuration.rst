Configuration
=============

To customize the behavior of **knm::transforms_tree**, you can define the following
macros before including ``knm_transforms_tree.hpp`` (note: how you want to organize
this configuration in your code is left to you):

.. code:: cpp

    #define KNM_TRANSFORMS_TREE_USE_GLM

Tell **knm::transforms_tree** to use the **GLM library** (*OpenGL Mathematics*,
https://github.com/g-truc/glm).

Note that since this is the only library supported at the moment, this
macro is defined by default


.. code:: cpp

    #define KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE <typename>

By default, **knm::transforms_tree** provides a class called ``Transformable``,
and instances of this class can be associated with the ``Transforms`` (to
indicates that the ``Transforms`` affect this object in particular).

It is then expected that your classes inherit from ``Transformable``.

If you want to use another class instead, define this macro to the
fully-qualified name of your class.

You must also ensure that this class is declared before the
``#include <knm_transforms_tree.hpp>`` line (or at least use a forward
declaration).


.. code:: cpp

    #define KNM_TRANSFORMS_TREE_USE_VOID_PTR_TRANSFORMABLE

If you don't want to use a common base class for all the objects with
transforms, define this macro. The transformable will then be
represented as *void pointers* (``void*``), that you can then cast to
whatever type you want.
