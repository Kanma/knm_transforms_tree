/** knm::transforms_tree - v1.0.0 - MIT License

DESCRIPTION

    Implementation of a hierarchy of 3D transforms nodes (position/orientation/scale),
    allowing to move objects relatively to each other using a parent/child relationship
    and to retrieve their final, world-space transforms.

    knm::transforms_tree is a single-file, header-only C++ library. It relies on other
    libraries to implement vectors, quaternions and matrices.

    In this version, only the GLM library (OpenGL Mathematics,
    https://github.com/g-truc/glm) is supported.

    Everything is defined in the "knm::tr2" namespace ("knm" for my username, "Kanma",
    and "tr2" for "TRansforms_TRee").


USAGE

    1. Copy this file in a convenient place for your project

    2. In *one* C++ file, add the following code to create the implementation:

        #define KNM_TRANSFORMS_TREE_IMPLEMENTATION
        #include <knm_transforms_tree.hpp>

    In other files, just use #include <knm_transforms_tree.hpp>


    A simple usage would be to create a hierarchy of transforms:

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


    A more usual usage would be to have Transforms attributes in your classes (see
    the Transformable class) and to manage their relationships through them.


CONFIGURATION

    To customize the behavior of knm::transforms_tree, you can define the following
    macros before including knm_transforms_tree.hpp (note: how you want to organize
    this configuration in your code is left to you):

        #define KNM_TRANSFORMS_TREE_USE_GLM

            Tell knm::transforms_tree to use the GLM library (OpenGL Mathematics,
            https://github.com/g-truc/glm).

            Note that since this is the only library supported at the moment, this
            macro is defined by default


        #define KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE <typename>

            By default, knm::transforms_tree provides a class called Transformable,
            and instances of this class can be associated with the Transforms (to
            indicates that the Transforms affect this object in particular).

            It is then expected that your classes inherit from Transformable.

            If you want to use another class instead, define this macro to the
            fully-qualified name of your class.

            You must also ensure that this class is declared before the
            #include <knm_transforms_tree.hpp> line (or at least use a forward
            declaration).


        #define KNM_TRANSFORMS_TREE_USE_VOID_PTR_TRANSFORMABLE

            If you don't want to use a common base class for all the objects with
            transforms, define this macro. The transformable will then be
            represented as void pointers (void*), that you can then cast to
            whatever type you want.


LICENSE

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

*/


#pragma once

#include <string>
#include <algorithm>
#include <cassert>


// We only support GLM at the moment
#ifndef KNM_TRANSFORMS_TREE_USE_GLM
    #define KNM_TRANSFORMS_TREE_USE_GLM
#endif


// If no transformable class is specified, use Transformable
#ifndef KNM_TRANSFORMS_TREE_USE_VOID_PTR_TRANSFORMABLE
    #ifndef KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE
        #define KNM_TRANSFORMS_TREE_USE_TRANSFORMABLE
        #define KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE Transformable
    #endif
#else
    #define KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE void
#endif


#ifdef KNM_TRANSFORMS_TREE_USE_GLM
    #include <glm/vec3.hpp>
    #include <glm/mat4x4.hpp>
    #include <glm/mat3x3.hpp>
    #include <glm/gtc/quaternion.hpp>
    #include <glm/gtc/type_precision.hpp>
    #include <glm/gtx/norm.hpp>
    #include <glm/gtx/matrix_decompose.hpp>
#endif


#define KNM_TRANSFORMS_TREE_VERSION_MAJOR 1
#define KNM_TRANSFORMS_TREE_VERSION_MINOR 0
#define KNM_TRANSFORMS_TREE_VERSION_PATCH 0



namespace knm {
namespace tr2 {


#ifdef KNM_TRANSFORMS_TREE_USE_GLM
    typedef glm::vec3 vec3;
    typedef glm::quat quat;
    typedef glm::mat4 mat4;
    typedef glm::mat3 mat3;
    typedef glm::float32_t real;

    #define KNM_TRANSFORMS_TREE_DOT(a, b)               glm::dot(a, b)
    #define KNM_TRANSFORMS_TREE_CROSS(a, b)             glm::cross(a, b)
    #define KNM_TRANSFORMS_TREE_NORMALISE(a)            glm::normalize(a)
    #define KNM_TRANSFORMS_TREE_INVERSE(a)              glm::inverse(a)
    #define KNM_TRANSFORMS_TREE_SQUARED_LENGTH(a)       glm::length2(a)
    #define KNM_TRANSFORMS_TREE_ANGLE_AXIS(angle, axis) glm::angleAxis(angle, axis)
#endif


    extern const vec3 VEC3_ZERO;
    extern const vec3 VEC3_UNIT_X;
    extern const vec3 VEC3_UNIT_Y;
    extern const vec3 VEC3_UNIT_Z;
    extern const vec3 VEC3_NEGATIVE_UNIT_X;
    extern const vec3 VEC3_NEGATIVE_UNIT_Y;
    extern const vec3 VEC3_NEGATIVE_UNIT_Z;
    extern const vec3 VEC3_UNIT_SCALE;

    extern const quat QUAT_ZERO;
    extern const quat QUAT_IDENTITY;

    extern const real PI;


    // Forward declarations
#ifdef KNM_TRANSFORMS_TREE_USE_TRANSFORMABLE
    class KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE;
#endif


    //--------------------------------------------------------------------------------
    /// @brief  Enumeration denoting the spaces which a transform can be relative to
    //--------------------------------------------------------------------------------
    enum transform_space_t
    {
        TS_LOCAL,       ///< Transform is relative to the local space
        TS_PARENT,      ///< Transform is relative to the parent space
        TS_WORLD        ///< Transform is relative to world space
    };


    //------------------------------------------------------------------------------------
    /// @brief  Defines a transforms space, by a position, an orientation and a scale
    ///
    /// Transforms can be organized in a hierarchy, where the position/orientation/scale
    /// of a child Transforms are relative to the transforms space defined by its parent
    /// (also called its origin).
    ///
    /// The "final" (world-space) position/orientation/scale of a Transforms is then the
    /// combination of all those above it in the hierarchy, and can be easily retrieved.
    /// For efficiency, they are only computed when necessary.
    ///
    /// User-supplied objects can be associated with Transforms. They are refered to as a
    /// Transformable. This class makes no assumption about what a Transformable is, it
    /// only stores a pointer to it.
    ///
    /// For convenience, "tags" can also be set on a Transforms. This allows to perform
    /// a tag-based search on a hierarchy of Transforms.
    ///
    /// It is expected that all the memory management is done by the user code. This class
    /// works with pointers (to parent/child Transforms, to Transformable objects), but
    /// doesn't allocate or deallocate them.
    //------------------------------------------------------------------------------------
    class Transforms
    {
        //_____ Construction / Destruction __________
    public:
    /// @name Construction / Destruction
    /// @{
        //--------------------------------------------------------------------------------
        /// @brief  Constructor
        //--------------------------------------------------------------------------------
        Transforms();

        //--------------------------------------------------------------------------------
        /// @brief  Constructor
        //--------------------------------------------------------------------------------
        Transforms(KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE* transformable);

        //--------------------------------------------------------------------------------
        /// @brief  Destructor
        //--------------------------------------------------------------------------------
        ~Transforms();
    /// @}


        //_____ Transformable __________
    public:
    /// @name Transformable retrieval
    /// @{
        //--------------------------------------------------------------------------------
        /// @brief  Gets the associated Transformable
        //--------------------------------------------------------------------------------
        inline KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE* getTransformable() const
        {
            return m_transformable;
        }

        //--------------------------------------------------------------------------------
        /// @brief  Gets the associated Transformable, casted to the type T
        //--------------------------------------------------------------------------------
        template <typename T> inline T getTransformable() const
        {
            return static_cast<T>(m_transformable);
        }
    /// @}


        //_____ Parent-children relationship __________
    public:
    /// @name Parent-children relationship
    /// @{
        //--------------------------------------------------------------------------------
        /// @brief  Sets the parent Transforms (to be used as the origin of this one)
        ///
        /// @param  transforms     The parent
        //--------------------------------------------------------------------------------
        void setParent(Transforms* transforms);

        //--------------------------------------------------------------------------------
        /// @brief  Gets the parent Transforms
        //--------------------------------------------------------------------------------
        inline Transforms* getParent() const
        {
            return m_parent;
        }

        //--------------------------------------------------------------------------------
        /// @brief  Gets the children Transforms (the ones directly under this one in the
        ///         hierarchy)
        //--------------------------------------------------------------------------------
        inline std::vector<Transforms*> getChildren() const
        {
            return m_children;
        }

        //--------------------------------------------------------------------------------
        /// @brief  Find the children Transforms having a given tag
        ///
        /// @param  tag         The tag to search for
        /// @param  recursive   If true, will search in all the Transforms below this one.
        ///                     Otherwise, will only search among the direct children.
        //--------------------------------------------------------------------------------
        std::vector<Transforms*> findChildren(const std::string& tag,
                                              bool recursive=false) const;
    /// @}


        //_____ Tags __________
    public:
    /// @name Tags managment
    /// @{
        //--------------------------------------------------------------------------------
        /// @brief  Add a tag to the Transforms
        ///
        /// @param  tag     The tag
        //--------------------------------------------------------------------------------
        inline void addTag(const std::string& tag)
        {
            assert(!tag.empty());
            m_tags.push_back(tag);
        }

        //--------------------------------------------------------------------------------
        /// @brief  Remove a tag from the Transforms
        ///
        /// @param  tag     The tag
        //--------------------------------------------------------------------------------
        inline void removeTag(const std::string& tag)
        {
            assert(!tag.empty());

            auto iter = std::find(m_tags.begin(), m_tags.end(), tag);
            if (iter != m_tags.end())
                m_tags.erase(iter);
        }

        //--------------------------------------------------------------------------------
        /// @brief  Gets all the tags
        //--------------------------------------------------------------------------------
        inline const std::vector<std::string>& getTags() const
        {
            return m_tags;
        }

        //--------------------------------------------------------------------------------
        /// @brief  Indicates if the Transforms has a specific tag
        ///
        /// @param  tag     The tag
        //--------------------------------------------------------------------------------
        inline bool hasTag(const std::string& tag) const
        {
            assert(!tag.empty());
            return (std::find(m_tags.begin(), m_tags.end(), tag) != m_tags.end());
        }
    /// @}


        //_____ Matrix conversions __________
    public:
    /// @name Matrix conversions
    /// @{
        //--------------------------------------------------------------------------------
        /// @brief  Set the position/orientation/scale of the Transforms relative to its
        ///         origin using a matrix
        ///
        /// @param  matrix     The matrix
        //--------------------------------------------------------------------------------
        void set(const mat4& matrix);

        //--------------------------------------------------------------------------------
        /// @brief  Gets a matrix containing the world position/orientation/scale of the
        ///         Transforms
        //--------------------------------------------------------------------------------
        mat4 getWorldMatrix();

        //--------------------------------------------------------------------------------
        /// @brief  Gets a matrix containing the position/orientation/scale of the
        ///         Transforms
        //--------------------------------------------------------------------------------
        mat4 getMatrix();
    /// @}


        //_____ Position __________
    public:
    /// @name Position-related methods
    /// @{
        //--------------------------------------------------------------------------------
        /// @brief  Sets the position of the Transforms relative to its origin
        ///
        /// @param  pos     The position vector
        //--------------------------------------------------------------------------------
        void setPosition(const vec3& pos);

        //--------------------------------------------------------------------------------
        /// @brief  Sets the position of the Transforms relative to its origin
        ///
        /// @param  x   The position on the x-axis
        /// @param  y   The position on the y-axis
        /// @param  z   The position on the z-axis
        //--------------------------------------------------------------------------------
        void setPosition(real x, real y, real z);

        //--------------------------------------------------------------------------------
        /// @brief  Sets the position of the Transforms relative to its origin
        ///
        /// @param  pos     The position vector
        //--------------------------------------------------------------------------------
        void setPosition(real* pos);

        //--------------------------------------------------------------------------------
        /// @brief  Moves the Transforms along the cartesian axes
        ///
        /// @param  d           Vector with x,y,z values representing the translation
        /// @param  relativeTo  The space which this translation is relative to
        //--------------------------------------------------------------------------------
        void translate(const vec3& d, transform_space_t relativeTo = TS_LOCAL);

        //--------------------------------------------------------------------------------
        /// @brief  Moves the Transforms along the cartesian axes
        ///
        /// @param  x, y, z     Values representing the translation
        /// @param  relativeTo  The space which this translation is relative to
        //--------------------------------------------------------------------------------
        void translate(real x, real y, real z, transform_space_t relativeTo = TS_LOCAL);

        //--------------------------------------------------------------------------------
        /// @brief  Moves the Transforms along the cartesian axes
        ///
        /// @param  d           Vector with x,y,z values representing the translation
        /// @param  relativeTo  The space which this translation is relative to
        //--------------------------------------------------------------------------------
        void translate(real* d, transform_space_t relativeTo = TS_LOCAL);

        //--------------------------------------------------------------------------------
        /// @brief  Gets the world-space position of the Transforms
        //--------------------------------------------------------------------------------
        vec3 getWorldPosition();

        //--------------------------------------------------------------------------------
        /// @brief  Gets the position of the Transforms relative to its origin
        //--------------------------------------------------------------------------------
        inline vec3 getPosition() const
        {
            return m_position;
        }
    /// @}


        //_____ Orientation __________
    public:
    /// @name Orientation-related methods
    /// @{
        //--------------------------------------------------------------------------------
        /// @brief  Sets the direction vector (ie the local -Z)
        ///
        /// @remark Note that the 'up' vector for the orientation will automatically be
        ///         recalculated based on the current 'up' vector (i.e. the yaw will
        ///         remain the same)
        ///
        /// @param  vec                     The direction vector
        /// @param  relativeTo              The space in which this direction vector is
        ///                                 expressed
        /// @param  localDirectionVector    The vector which normally describes the
        ///                                 natural direction of the Transforms,
        ///                                 usually -Z
        //--------------------------------------------------------------------------------
        void setDirection(const vec3& vec, transform_space_t relativeTo = TS_LOCAL,
                          const vec3& localDirectionVector = VEC3_NEGATIVE_UNIT_Z);

        //--------------------------------------------------------------------------------
        /// @brief  Points the local -Z direction at a point in space
        ///
        /// @param  targetPoint             The look at point
        /// @param  relativeTo              The space in which the point is expressed
        /// @param  localDirectionVector    The vector which normally describes the
        ///                                 natural direction of the Transforms,
        ///                                 usually -Z
        //--------------------------------------------------------------------------------
        void lookAt(const vec3& targetPoint, transform_space_t relativeTo = TS_LOCAL,
                    const vec3& localDirectionVector = VEC3_NEGATIVE_UNIT_Z);

        //--------------------------------------------------------------------------------
        /// @brief  Sets the orientation of the Transforms via a quaternion
        ///
        /// @remark Orientations, unlike other transforms, are not always inherited by
        ///         children. Whether or not orientations affect the orientation
        ///         of the child depends on the setInheritOrientation() option
        ///         of the child.
        ///         The default is to inherit as with other transforms.
        ///
        /// @param  q   The orientation quaternion
        //--------------------------------------------------------------------------------
        void setOrientation(const quat& q);

        //--------------------------------------------------------------------------------
        /// @brief  Sets the orientation of the Transforms via quaternion parameters
        ///
        /// @remark Orientations, unlike other transforms, are not always inherited by
        ///         children. Whether or not orientations affect the orientation
        ///         of the child depends on the setInheritOrientation() option
        ///         of the child.
        ///         The default is to inherit as with other transforms.
        ///
        /// @param  w   The w parameter
        /// @param  x   The x parameter
        /// @param  y   The y parameter
        /// @param  z   The z parameter
        //--------------------------------------------------------------------------------
        void setOrientation(real w, real x, real y, real z);

        //--------------------------------------------------------------------------------
        /// @brief  Sets the orientation of the Transforms via quaternion parameters
        ///
        /// @remark Orientations, unlike other transforms, are not always inherited by
        ///         children. Whether or not orientations affect the orientation
        ///         of the child depends on the setInheritOrientation() option
        ///         of the child.
        ///         The default is to inherit as with other transforms.
        ///
        /// @param  q   The orientation quaternion parameters
        //--------------------------------------------------------------------------------
        void setOrientation(real* q);

        //--------------------------------------------------------------------------------
        /// @brief  Rotate the Transforms around the X-axis
        ///
        /// @param  angle       Angle of the rotation, in radians
        /// @param  relativeTo  The space which this rotation is relative to
        //--------------------------------------------------------------------------------
        void roll(real angle, transform_space_t relativeTo = TS_LOCAL);

        //--------------------------------------------------------------------------------
        /// @brief  Rotate the Transforms around the Y-axis
        ///
        /// @param  angle       Angle of the rotation, in radians
        /// @param  relativeTo  The space which this rotation is relative to
        //--------------------------------------------------------------------------------
        void pitch(real angle, transform_space_t relativeTo = TS_LOCAL);

        //--------------------------------------------------------------------------------
        /// @brief  Rotate the Transforms around the Z-axis
        ///
        /// @param  angle       Angle of the rotation, in radians
        /// @param  relativeTo  The space which this rotation is relative to
        //--------------------------------------------------------------------------------
        void yaw(real angle, transform_space_t relativeTo = TS_LOCAL);

        //--------------------------------------------------------------------------------
        /// @brief  Rotate the Transforms around an arbitrary axis
        ///
        /// @param  axis        The axis around which the rotation will occur
        /// @param  angle       The angle, in radians
        /// @param  relativeTo  The space in which the rotation axis is expressed
        //--------------------------------------------------------------------------------
        void rotate(const vec3& axis, real angle, transform_space_t relativeTo = TS_LOCAL);

        //--------------------------------------------------------------------------------
        /// @brief  Rotate the Transforms around an arbitrary axis using a quaternion
        ///
        /// @param  q           The quaternion representing the rotation
        /// @param  relativeTo  The space in which the rotation axis is expressed
        //--------------------------------------------------------------------------------
        void rotate(const quat& q, transform_space_t relativeTo = TS_LOCAL);

        //--------------------------------------------------------------------------------
        /// @brief  Resets the orientation (local axes as world axes, no rotation)
        //--------------------------------------------------------------------------------
        void resetOrientation();

        //--------------------------------------------------------------------------------
        /// @brief  Tells the Transforms whether it should inherit orientation from its
        ///         parent
        ///
        /// @remark Orientations, unlike other transforms, are not always inherited by
        ///         children. Whether or not orientations affect the orientation
        ///         of the child depends on the setInheritOrientation() option
        ///         of the child.
        ///         The default is to inherit as with other transforms.
        ///
        /// @param  bInherit    If true, this Transforms's orientation will be affected
        ///                     by its parent's orientation. If false, it will not be
        ///                     affected.
        //--------------------------------------------------------------------------------
        void setInheritOrientation(bool bInherit);

        //--------------------------------------------------------------------------------
        /// @brief  Indicates if the Transforms inherit orientation from its parent
        //--------------------------------------------------------------------------------
        inline bool inheritOrientation() const
        {
            return m_bInheritOrientation;
        }

        //--------------------------------------------------------------------------------
        /// @brief  Gets the world-space orientation of the Transforms
        //--------------------------------------------------------------------------------
        quat getWorldOrientation();

        //--------------------------------------------------------------------------------
        /// @brief  Returns a quaternion representing the Transforms's orientation
        ///         relative to its origin
        //--------------------------------------------------------------------------------
        inline quat getOrientation() const
        {
            return m_orientation;
        }
    /// @}


        //_____ Scale __________
    public:
    /// @name Scale-related methods
    /// @{
        //--------------------------------------------------------------------------------
        /// @brief  Sets the scaling factor applied to this Transforms
        ///
        /// @remark Scaling factors, unlike other transforms, are not always inherited by
        ///         children. Whether or not scalings affect both the size and position
        ///         of the child transforms depends on the setInheritScale option of the
        ///         child.
        ///         The default is to inherit as with other transforms. Note that like
        ///         rotations, scalings are oriented around the Transforms's origin.
        ///
        /// @param  scale   The scaling factor
        //--------------------------------------------------------------------------------
        void setScale(const vec3& scale);

        //--------------------------------------------------------------------------------
        /// @brief  Sets the scaling factor applied to this Transforms
        ///
        /// @remark Scaling factors, unlike other transforms, are not always inherited by
        ///         children. Whether or not scalings affect both the size and position
        ///         of the child transforms depends on the setInheritScale option of the
        ///         child.
        ///         The default is to inherit as with other transforms. Note that like
        ///         rotations, scalings are oriented around the Transforms's origin.
        ///
        /// @param  x   The scaling factor on the x-axis
        /// @param  y   The scaling factor on the y-axis
        /// @param  z   The scaling factor on the z-axis
        //--------------------------------------------------------------------------------
        void setScale(real x, real y, real z);

        //--------------------------------------------------------------------------------
        /// @brief  Sets the scaling factor applied to this Transforms
        ///
        /// @remark Scaling factors, unlike other transforms, are not always inherited by
        ///         children. Whether or not scalings affect both the size and position
        ///         of the child transforms depends on the setInheritScale option of the
        ///         child.
        ///         The default is to inherit as with other transforms. Note that like
        ///         rotations, scalings are oriented around the Transforms's origin.
        ///
        /// @param  scale   The scaling factor
        //--------------------------------------------------------------------------------
        void setScale(real scale);

        //--------------------------------------------------------------------------------
        /// @brief  Sets the scaling factor applied to this Transforms
        ///
        /// @remark Scaling factors, unlike other transforms, are not always inherited by
        ///         children. Whether or not scalings affect both the size and position
        ///         of the child transforms depends on the setInheritScale option of the
        ///         child.
        ///         The default is to inherit as with other transforms. Note that like
        ///         rotations, scalings are oriented around the Transforms's origin.
        ///
        /// @param  scales  The scaling factors
        //--------------------------------------------------------------------------------
        void setScale(real* scales);

        //--------------------------------------------------------------------------------
        /// @brief  Scales the Transforms, combining its current scale with the passed in
        ///         scaling factor
        ///
        /// @remark This method applies an extra scaling factor to the transforms's
        ///         existing scale, (unlike setScale which overwrites it) combining its
        ///         current scale with the new one. E.g. calling this method twice with
        ///         vec3(2, 2, 2) would have the same effect as setScale(vec3(4, 4, 4))
        ///         if the existing scale was 1. Note that like rotations, scalings are
        ///         oriented around the Transforms's origin.
        ///
        /// @param  scale   The scaling factor
        //--------------------------------------------------------------------------------
        void scale(const vec3& scale);

        //--------------------------------------------------------------------------------
        /// @brief  Scales the transforms, combining its current scale with the passed in
        ///         scaling factor
        ///
        /// @remark This method applies an extra scaling factor to the transforms's
        ///         existing scale, (unlike setScale which overwrites it) combining its
        ///         current scale with the new one. E.g. calling this method twice with
        ///         (2, 2, 2) would have the same effect as setScale(4, 4, 4)
        ///         if the existing scale was 1. Note that like rotations, scalings are
        ///         oriented around the transforms's origin.
        ///
        /// @param  x   The scaling factor on the x-axis
        /// @param  y   The scaling factor on the y-axis
        /// @param  z   The scaling factor on the z-axis
        //--------------------------------------------------------------------------------
        void scale(real x, real y, real z);

        //--------------------------------------------------------------------------------
        /// @brief  Scales the Transforms, combining its current scale with the passed in
        ///         scaling factor
        ///
        /// @remark This method applies an extra scaling factor to the transforms's
        ///         existing scale, (unlike setScale which overwrites it) combining its
        ///         current scale with the new one. E.g. calling this method twice with
        ///         (2, 2, 2) would have the same effect as setScale(4, 4, 4)
        ///         if the existing scale was 1. Note that like rotations, scalings are
        ///         oriented around the Transforms's origin.
        ///
        /// @param  scales  The scaling factors
        //--------------------------------------------------------------------------------
        void scale(real* scales);

        //--------------------------------------------------------------------------------
        /// @brief  Tells the Transforms whether it should inherit scaling factors from
        ///         its parent
        ///
        /// @remark Scaling factors, unlike other transforms, are not always inherited by
        ///         children. Whether or not scalings affect both the size and position
        ///         of the child transforms depends on the setInheritScale option of the
        ///         child.
        ///         The default is to inherit as with other transforms. Note that like
        ///         rotations, scalings are oriented around the Transforms's origin.
        ///
        /// @param  bInherit    If true, this Transforms's scale and position will be
        ///                     affected by its parent's scale. If false, it will not be
        ///                     affected.
        //--------------------------------------------------------------------------------
        void setInheritScale(bool bInherit);

        //--------------------------------------------------------------------------------
        /// @brief  Indicates if the Transforms inherit scaling factors from its parent
        //--------------------------------------------------------------------------------
        inline bool inheritScale() const
        {
            return m_bInheritScale;
        }

        //--------------------------------------------------------------------------------
        /// @brief  Gets the world-space scale of the Transforms
        //--------------------------------------------------------------------------------
        vec3 getWorldScale();

        //--------------------------------------------------------------------------------
        /// @brief  Gets the scaling factor of this Transforms, relative to its origin
        //--------------------------------------------------------------------------------
        inline vec3 getScale() const
        {
            return m_scale;
        }
    /// @}


        //_____ Private methods __________
    private:
        void needUpdate();
        void update();


        //_____ Attributes __________
    private:
        // Parent-children relationship
        Transforms* m_parent;
        std::vector<Transforms*> m_children;

        // Transformable
        KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE* m_transformable;

        // Tags
        std::vector<std::string> m_tags;

        // Relative transforms
        vec3 m_position;
        quat m_orientation;
        vec3 m_scale;

        // Full (world) transforms
        vec3 m_fullPosition;
        quat m_fullOrientation;
        vec3 m_fullScale;

        // Flags
        bool m_bDirty;
        bool m_bInheritOrientation;
        bool m_bInheritScale;
    };



#ifdef KNM_TRANSFORMS_TREE_USE_TRANSFORMABLE

    //------------------------------------------------------------------------------------
    /// @brief  Instances of this class can be associated with the Transforms (to
    ///         indicates that the Transforms affect this object in particular)
    ///
    /// It is expected that your classes inherit from Transformable.
    ///
    /// If you want to use another class instead, define the
    /// KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE macro to the fully-qualified name of your
    /// class.
    ///
    /// If you don't want to use a common base class for all the objects with
    /// transforms, define the KNM_TRANSFORMS_TREE_USE_VOID_PTR_TRANSFORMABLE macro. The
    /// transformable will then be represented as void pointers (void*), that you can
    /// then cast to whatever type you want.
    //------------------------------------------------------------------------------------
    class Transformable
    {
        //_____ Construction / Destruction __________
    public:
        //--------------------------------------------------------------------------------
        /// @brief  Constructor
        //--------------------------------------------------------------------------------
        Transformable()
        : transforms(this)
        {
        }


        //_____ Methods __________
    public:
        //--------------------------------------------------------------------------------
        /// @brief  Sets a Transformable as parent of this one
        ///
        /// Note that the Transformable class doesn't manage a hierarchy itself. This
        /// method is just a proxy for the corresponding one in Transformable.
        //--------------------------------------------------------------------------------
        inline void setParent(Transformable* transformable)
        {
            assert(transformable != this);

            transforms.setParent(transformable ? &transformable->transforms : nullptr);
        }

        //--------------------------------------------------------------------------------
        /// @brief  Gets the Transforms affecting this Transformable
        //--------------------------------------------------------------------------------
        inline Transforms* getTransforms()
        {
            return &transforms;
        }


        //_____ Attributes __________
    protected:
        Transforms transforms;
    };

#endif


    //------------------------------------------------------------------------------------
    /// @brief  Contains some math utility methods
    //------------------------------------------------------------------------------------
    class Math
    {
    public:
        //--------------------------------------------------------------------------------
        /// @brief  Gets the shortest arc quaternion to rotate one vector to the
        ///         destination vector
        ///
        /// @remark If you call this with a dest vector that is close to the inverse
        ///         of the source vector, we will rotate 180 degrees around the
        ///         'fallbackAxis' (if specified, or a generated axis if not) since in
        ///         this case ANY axis of rotation is valid.
        //--------------------------------------------------------------------------------
        static quat getRotationTo(const vec3& from, const vec3& dest,
                                  const vec3& fallbackAxis = VEC3_ZERO);
    };



#ifdef KNM_TRANSFORMS_TREE_IMPLEMENTATION

    /*********************************** CONSTANTS **************************************/

    const vec3 VEC3_ZERO(0.0f, 0.0f, 0.0f);
    const vec3 VEC3_UNIT_X(1.0f, 0.0f, 0.0f);
    const vec3 VEC3_UNIT_Y(0.0f, 1.0f, 0.0f);
    const vec3 VEC3_UNIT_Z(0.0f, 0.0f, 1.0f);
    const vec3 VEC3_NEGATIVE_UNIT_X(-1.0f, 0.0f, 0.0f);
    const vec3 VEC3_NEGATIVE_UNIT_Y(0.0f, -1.0f, 0.0f);
    const vec3 VEC3_NEGATIVE_UNIT_Z(0.0f, 0.0f, -1.0f);
    const vec3 VEC3_UNIT_SCALE(1.0f, 1.0f, 1.0f);

    const quat QUAT_ZERO(0.0f, 0.0f, 0.0f, 0.0f);
    const quat QUAT_IDENTITY(1.0f, 0.0f, 0.0f, 0.0f);

    const real PI = real(4.0 * atan(1.0));


    /*************************** CONSTRUCTION / DESTRUCTION *****************************/

    Transforms::Transforms()
    : m_parent(nullptr), m_transformable(nullptr),
      m_position(VEC3_ZERO), m_orientation(QUAT_IDENTITY), m_scale(VEC3_UNIT_SCALE),
      m_fullPosition(VEC3_ZERO), m_fullOrientation(QUAT_IDENTITY),
      m_fullScale(VEC3_UNIT_SCALE), m_bDirty(true), m_bInheritOrientation(true),
      m_bInheritScale(true)
    {
    }

    //-----------------------------------------------------------------------

    Transforms::Transforms(KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE* transformable)
    : m_parent(nullptr), m_transformable(transformable),
      m_position(VEC3_ZERO), m_orientation(QUAT_IDENTITY), m_scale(VEC3_UNIT_SCALE),
      m_fullPosition(VEC3_ZERO), m_fullOrientation(QUAT_IDENTITY),
      m_fullScale(VEC3_UNIT_SCALE), m_bDirty(true), m_bInheritOrientation(true),
      m_bInheritScale(true)
    {
#ifdef KNM_TRANSFORMS_TREE_USE_TRANSFORMABLE
        assert((transformable == nullptr) || (transformable->getTransforms() == this));
#endif
    }

    //-----------------------------------------------------------------------

    Transforms::~Transforms()
    {
    }


    /************************** PARENT-CHILDREN RELATIONSHIP ****************************/

    void Transforms::setParent(Transforms* transforms)
    {
        assert(transforms != this);

        if (m_parent)
        {
            m_parent->m_children.erase(
                std::find(m_parent->m_children.begin(), m_parent->m_children.end(), this)
            );
            m_parent = nullptr;
        }

        if (transforms)
        {
            m_parent = transforms;
            transforms->m_children.push_back(this);
        }

        needUpdate();
    }

    //-----------------------------------------------------------------------

    std::vector<Transforms*> Transforms::findChildren(const std::string& tag,
                                                      bool recursive) const
    {
        assert(!tag.empty());

        std::vector<Transforms*> result;

        for (Transforms* child : m_children)
        {
            if (child->hasTag(tag))
                result.push_back(child);

            if (recursive)
            {
                auto result2 = child->findChildren(tag, true);
                result.insert(result.end(), result2.begin(), result2.end());
            }
        }

        return result;
    }


    /******************************** MATRIX CONVERSIONS ********************************/

    void Transforms::set(const mat4& matrix)
    {
        glm::vec3 skew;
        glm::vec4 perspective;

        glm::decompose(matrix, m_scale, m_orientation, m_position, skew, perspective);

        needUpdate();
    }


    mat4 Transforms::getWorldMatrix()
    {
        if (m_bDirty)
            update();

        mat3 rot3x3(m_fullOrientation);

        return mat4(
            m_fullScale.x * rot3x3[0][0], m_fullScale.x * rot3x3[0][1], m_fullScale.x * rot3x3[0][2], 0.0f,
            m_fullScale.y * rot3x3[1][0], m_fullScale.y * rot3x3[1][1], m_fullScale.y * rot3x3[1][2], 0.0f,
            m_fullScale.z * rot3x3[2][0], m_fullScale.z * rot3x3[2][1], m_fullScale.z * rot3x3[2][2], 0.0f,
            m_fullPosition.x, m_fullPosition.y, m_fullPosition.z, 1.0f
        );
    }


    mat4 Transforms::getMatrix()
    {
        mat3 rot3x3(m_orientation);

        return mat4(
            m_scale.x * rot3x3[0][0], m_scale.x * rot3x3[0][1], m_scale.x * rot3x3[0][2], 0.0f,
            m_scale.y * rot3x3[1][0], m_scale.y * rot3x3[1][1], m_scale.y * rot3x3[1][2], 0.0f,
            m_scale.z * rot3x3[2][0], m_scale.z * rot3x3[2][1], m_scale.z * rot3x3[2][2], 0.0f,
            m_position.x, m_position.y, m_position.z, 1.0f
        );
    }


    /************************************* POSITION *************************************/

    void Transforms::setPosition(const vec3& pos)
    {
        m_position = pos;
        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::setPosition(real x, real y, real z)
    {
        m_position.x = x;
        m_position.y = y;
        m_position.z = z;
        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::setPosition(real* pos)
    {
        m_position.x = pos[0];
        m_position.y = pos[1];
        m_position.z = pos[2];
        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::translate(const vec3& d, transform_space_t relativeTo)
    {
        switch(relativeTo)
        {
        case TS_LOCAL:
            // Position is relative to parent so transform downwards
            m_position += m_orientation * d;
            break;

        case TS_PARENT:
            m_position += d;
            break;

        case TS_WORLD:
            {
                // Position is relative to parent so transform upwards
                if (m_parent)
                    m_position += (KNM_TRANSFORMS_TREE_INVERSE(m_parent->getWorldOrientation()) * d) / m_parent->getWorldScale();
                else
                    m_position += d;
                break;
            }
        }

        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::translate(real x, real y, real z, transform_space_t relativeTo)
    {
        translate(vec3(x, y, z), relativeTo);
    }

    //-----------------------------------------------------------------------

    void Transforms::translate(real* d, transform_space_t relativeTo)
    {
        translate(vec3(d[0], d[1], d[2]), relativeTo);
    }

    //-----------------------------------------------------------------------

    vec3 Transforms::getWorldPosition()
    {
        if (m_bDirty)
            update();

        return m_fullPosition;
    }


    /*************************************** ORIENTATION ***************************************/

    void Transforms::setDirection(const vec3& vec, transform_space_t relativeTo,
                                  const vec3& localDirectionVector)
    {
        // Do nothing if given a zero vector
        if (vec == VEC3_ZERO)
            return;

        // The direction we want the local direction point to
        vec3 targetDir = KNM_TRANSFORMS_TREE_NORMALISE(vec);

        // Transform target direction to world space
        switch (relativeTo)
        {
        case TS_LOCAL:
            targetDir = getWorldOrientation() * targetDir;
            break;

        case TS_PARENT:
            if (m_bInheritOrientation && m_parent)
                targetDir = m_parent->getWorldOrientation() * targetDir;
            break;

        case TS_WORLD:
            // default orientation
            break;
        }

        // Calculate target orientation relative to world space
        quat targetOrientation;

        // Get current local direction relative to world space
        const quat& currentOrient = getWorldOrientation();
        vec3 currentDir = currentOrient * localDirectionVector;

        if (KNM_TRANSFORMS_TREE_SQUARED_LENGTH(currentDir + targetDir) < 0.00005f)
        {
            // Oops, a 180 degree turn (infinite possible rotation axes)
            // Default to pitch i.e. use current UP
            targetOrientation =
                quat(-currentOrient.y, -currentOrient.z, currentOrient.w, currentOrient.x);
        }
        else
        {
            // Derive shortest arc to new direction
            quat rotQuat = Math::getRotationTo(currentDir, targetDir);
            targetOrientation = rotQuat * currentOrient;
        }

        // Set target orientation, transformed to parent space
        if (m_parent && m_bInheritOrientation)
        {
            quat q = m_parent->getWorldOrientation();
            setOrientation(quat(q.w, -q.x, -q.y, -q.z) * targetOrientation);
        }
        else
        {
            setOrientation(targetOrientation);
        }
    }

    //-----------------------------------------------------------------------

    void Transforms::lookAt(const vec3& targetPoint, transform_space_t relativeTo,
                            const vec3& localDirectionVector)
    {
        // Calculate our self origin relative to the given transform space
        vec3 origin;
        switch (relativeTo)
        {
        case TS_WORLD:
            origin = getWorldPosition();
            break;

        case TS_PARENT:
            origin = m_position;
            break;

        case TS_LOCAL:
            origin = VEC3_ZERO;
            break;
        }

        setDirection(targetPoint - origin, relativeTo, localDirectionVector);
    }

    //-----------------------------------------------------------------------

    void Transforms::setOrientation(const quat& q)
    {
        m_orientation = q;
        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::setOrientation(real x, real y, real z, real w)
    {
        m_orientation.w = w;
        m_orientation.x = x;
        m_orientation.y = y;
        m_orientation.z = z;
        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::setOrientation(real* q)
    {
        m_orientation.w = q[0];
        m_orientation.x = q[1];
        m_orientation.y = q[2];
        m_orientation.z = q[3];
        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::roll(real angle, transform_space_t relativeTo)
    {
        rotate(VEC3_UNIT_X, angle, relativeTo);
    }

    //-----------------------------------------------------------------------

    void Transforms::pitch(real angle, transform_space_t relativeTo)
    {
        rotate(VEC3_UNIT_Y, angle, relativeTo);
    }

    //-----------------------------------------------------------------------

    void Transforms::yaw(real angle, transform_space_t relativeTo)
    {
        rotate(VEC3_UNIT_Z, angle, relativeTo);
    }

    //-----------------------------------------------------------------------

    void Transforms::rotate(const vec3& axis, real angle,
                            transform_space_t relativeTo)
    {
        rotate(KNM_TRANSFORMS_TREE_ANGLE_AXIS(angle, axis), relativeTo);
    }

    //-----------------------------------------------------------------------

    void Transforms::rotate(const quat& q, transform_space_t relativeTo)
    {
        switch (relativeTo)
        {
        case TS_PARENT:
            // Rotations are normally relative to local axes, transform up
            m_orientation = q * m_orientation;
            break;

        case TS_WORLD:
            // Rotations are normally relative to local axes, transform up
            m_orientation = m_orientation * KNM_TRANSFORMS_TREE_INVERSE(getWorldOrientation()) * q * getWorldOrientation();
            break;

        case TS_LOCAL:
            // Note the order of the mult, i.e. q comes after
            m_orientation = m_orientation * q;
            break;
        }

        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::resetOrientation()
    {
        m_orientation = QUAT_IDENTITY;
        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::setInheritOrientation(bool bInherit)
    {
        m_bInheritOrientation = bInherit;
        needUpdate();
    }

    //-----------------------------------------------------------------------

    quat Transforms::getWorldOrientation()
    {
        if (m_bDirty)
            update();

        return m_fullOrientation;
    }


    /**************************************** SCALE *****************************************/

    void Transforms::setScale(const vec3& scale)
    {
        m_scale = scale;
        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::setScale(real x, real y, real z)
    {
        m_scale.x = x;
        m_scale.y = y;
        m_scale.z = z;
        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::setScale(real scale)
    {
        m_scale.x = scale;
        m_scale.y = scale;
        m_scale.z = scale;
        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::setScale(real* scale)
    {
        m_scale.x = scale[0];
        m_scale.y = scale[1];
        m_scale.z = scale[2];
        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::scale(const vec3& scale)
    {
        m_scale = m_scale * scale;
        needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::scale(real x, real y, real z)
    {
        scale(vec3(x, y, z));
    }

    //-----------------------------------------------------------------------

    void Transforms::scale(real* scales)
    {
        scale(vec3(scales[0], scales[1], scales[2]));
    }

    //-----------------------------------------------------------------------

    void Transforms::setInheritScale(bool bInherit)
    {
        m_bInheritScale = bInherit;
        needUpdate();
    }

    //-----------------------------------------------------------------------

    vec3 Transforms::getWorldScale()
    {
        if (m_bDirty)
            update();

        return m_fullScale;
    }


    /********************************* PRIVATE METHODS **********************************/

    void Transforms::needUpdate()
    {
        m_bDirty = true;

        for (Transforms* child : m_children)
            child->needUpdate();
    }

    //-----------------------------------------------------------------------

    void Transforms::update()
    {
        if (!m_bDirty)
            return;

        if (m_parent)
        {
            // Update orientation
            const quat& parentOrientation = m_parent->getWorldOrientation();
            if (m_bInheritOrientation)
            {
                // Combine orientation with that of parent
                m_fullOrientation = KNM_TRANSFORMS_TREE_NORMALISE(parentOrientation * m_orientation);
            }
            else
            {
                // No inheritence
                m_fullOrientation = m_orientation;
            }

            // Update scale
            const vec3& parentScale = m_parent->getWorldScale();
            if (m_bInheritScale)
            {
                // Scale own position by parent scale, NB just combine
                // as equivalent axes, no shearing
                m_fullScale = parentScale * m_scale;
            }
            else
            {
                // No inheritence
                m_fullScale = m_scale;
            }

            // Change position vector based on parent's orientation & scale
            m_fullPosition = parentOrientation * (parentScale * m_position);

            // Add altered position vector to parents
            m_fullPosition += m_parent->getWorldPosition();
        }
        else
        {
            // No parent
            m_fullPosition      = m_position;
            m_fullOrientation   = m_orientation;
            m_fullScale         = m_scale;
        }

        m_bDirty = false;
    }


    /************************************** MATH ****************************************/

    quat Math::getRotationTo(const vec3& from, const vec3& dest,
                             const vec3& fallbackAxis)
    {
        // Based on Stan Melax's article in Game Programming Gems
        quat q;
        vec3 v0 = KNM_TRANSFORMS_TREE_NORMALISE(from);
        vec3 v1 = KNM_TRANSFORMS_TREE_NORMALISE(dest);

        real d = KNM_TRANSFORMS_TREE_DOT(v0, v1);
        // If dot == 1, vectors are the same
        if (d >= 1.0f)
        {
            return QUAT_IDENTITY;
        }
        if (d < (1e-6f - 1.0f))
        {
            if (fallbackAxis != VEC3_ZERO)
            {
                // rotate 180 degrees about the fallback axis
                q = KNM_TRANSFORMS_TREE_ANGLE_AXIS(PI, fallbackAxis);
            }
            else
            {
                // Generate an axis
                vec3 axis = KNM_TRANSFORMS_TREE_CROSS(VEC3_UNIT_X, from);
                if (KNM_TRANSFORMS_TREE_SQUARED_LENGTH(axis) < 1e-6f) // pick another if colinear
                    axis = KNM_TRANSFORMS_TREE_CROSS(VEC3_UNIT_Y, from);
                axis = KNM_TRANSFORMS_TREE_NORMALISE(axis);
                q = KNM_TRANSFORMS_TREE_ANGLE_AXIS(PI, axis);
            }
        }
        else
        {
            real s = sqrt((1.0f + d) * 2.0f);
            real invs = 1.0f / s;

            vec3 c = KNM_TRANSFORMS_TREE_CROSS(v0, v1);

            q.x = c.x * invs;
            q.y = c.y * invs;
            q.z = c.z * invs;
            q.w = s * 0.5f;

            q = KNM_TRANSFORMS_TREE_NORMALISE(q);
        }

        return q;
    }

#endif // KNM_TRANSFORMS_TREE_IMPLEMENTATION


#ifndef KNM_TRANSFORMS_TREE_KEEP_HELPER_FUNCTIONS
    #undef KNM_TRANSFORMS_TREE_DOT
    #undef KNM_TRANSFORMS_TREE_CROSS
    #undef KNM_TRANSFORMS_TREE_NORMALISE
    #undef KNM_TRANSFORMS_TREE_INVERSE
    #undef KNM_TRANSFORMS_TREE_SQUARED_LENGTH
    #undef KNM_TRANSFORMS_TREE_ANGLE_AXIS
#endif

}
}
