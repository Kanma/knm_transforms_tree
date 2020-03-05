knm::transforms_tree
====================

Implementation of a hierarchy of 3D transforms nodes (*position/orientation/scale*),
allowing to move objects relatively to each other using a parent/child relationship
and to retrieve their final, world-space transforms.

**knm::transforms_tree** is a single-file, header-only C++ library. It relies on other
libraries to implement vectors, quaternions and matrices.

In this version, only the **GLM library** (*OpenGL Mathematics*,
https://github.com/g-truc/glm) is supported.

Everything is defined in the ``knm::tr2::`` namespace (``knm`` for my username, *Kanma*,
and ``tr2`` for *TRansforms_TRee*).


.. toctree::
   :maxdepth: 2
   :caption: Documentation
   
   usage
   configuration
   license


.. toctree::
   :maxdepth: 2
   :caption: API
   
   api_transforms
   api_transform_space
   api_transformable
   api_math
   api_constants
