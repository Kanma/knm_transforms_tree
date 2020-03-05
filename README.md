# knm::transforms_tree

Implementation of a hierarchy of 3D transforms nodes (*position/orientation/scale*),
allowing to move objects relatively to each other using a parent/child relationship
and to retrieve their final, world-space transforms.

**knm::transforms_tree** is a single-file, header-only C++ library. It relies on other
libraries to implement vectors, quaternions and matrices.

In this version, only the **GLM library** (*OpenGL Mathematics*,
[https://github.com/g-truc/glm](https://github.com/g-truc/glm)) is supported.

Everything is defined in the `knm::tr2::` namespace (`knm` for my username, *Kanma*,
and `tr2` for *TRansforms_TRee*).


## Usage

1. Copy the `knm_transforms_tree.hpp` file in a convenient place for your project

2. In *one* C++ file, add the following code to create the implementation:

```cpp
#define KNM_TRANSFORMS_TREE_IMPLEMENTATION
#include <knm_transforms_tree.hpp>
```

In other files, just use `#include <knm_transforms_tree.hpp>`


A simple usage would be to create a hierarchy of transforms:

```cpp
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
```


A more usual usage would be to have *Transforms* attributes in your classes (see
the *Transformable* class) and to manage their relationships through them.


## Configuration

To customize the behavior of **knm::transforms_tree**, you can define the following
macros before including `knm_transforms_tree.hpp` (note: how you want to organize
this configuration in your code is left to you):

### `KNM_TRANSFORMS_TREE_USE_GLM`

```cpp
#define KNM_TRANSFORMS_TREE_USE_GLM
```

Tell **knm::transforms_tree** to use the **GLM library** (*OpenGL Mathematics*,
[https://github.com/g-truc/glm](https://github.com/g-truc/glm)).

Note that since this is the only library supported at the moment, this
macro is defined by default


### `KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE`

```cpp
#define KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE <typename>
```

By default, **knm::transforms_tree** provides a class called *Transformable*,
and instances of this class can be associated with the *Transforms* (to
indicates that the *Transforms* affect this object in particular).

It is then expected that your classes inherit from *Transformable*.

If you want to use another class instead, define this macro to the
fully-qualified name of your class.

You must also ensure that this class is declared before the
`#include <knm_transforms_tree.hpp>` line (or at least use a forward
declaration).


### `KNM_TRANSFORMS_TREE_USE_VOID_PTR_TRANSFORMABLE`

```cpp
#define KNM_TRANSFORMS_TREE_USE_VOID_PTR_TRANSFORMABLE
```

If you don't want to use a common base class for all the objects with
transforms, define this macro. The transformable will then be
represented as void pointers (void*), that you can then cast to
whatever type you want.


## License

knm::transforms_tree is made available under the MIT License.

Copyright (c) 2020 Philip Abbet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
