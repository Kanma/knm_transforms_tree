include_directories(
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/dependencies"
)


set(SRCS
    main.cpp
    creation.cpp
    transformable.cpp
    tags.cpp
    hierarchy.cpp
    position.cpp
    orientation.cpp
    scale.cpp
    translation.cpp
    rotation.cpp
    scaling.cpp
    matrix.cpp
    helpers.h
)


# Using GLM
set(DEFINITIONS
    KNM_TRANSFORMS_TREE_USE_GLM
    KNM_TRANSFORMS_TREE_KEEP_HELPER_FUNCTIONS
)

add_executable(tests-glm ${SRCS})
set_property(TARGET tests-glm PROPERTY COMPILE_DEFINITIONS ${DEFINITIONS})



# Using GLM and a custom Transformable class
set(DEFINITIONS
    KNM_TRANSFORMS_TREE_USE_GLM
    KNM_TRANSFORMS_TREE_TRANSFORMABLE_TYPE=Tests::CustomTransformable
    KNM_TRANSFORMS_TREE_KEEP_HELPER_FUNCTIONS
)

add_executable(tests-glm-custom-transformable ${SRCS})
set_property(TARGET tests-glm-custom-transformable PROPERTY COMPILE_DEFINITIONS ${DEFINITIONS})


# Using GLM and void pointers for Transformable
set(DEFINITIONS
    KNM_TRANSFORMS_TREE_USE_GLM
    KNM_TRANSFORMS_TREE_USE_VOID_PTR_TRANSFORMABLE
    KNM_TRANSFORMS_TREE_KEEP_HELPER_FUNCTIONS
)

add_executable(tests-glm-voidptr-transformable ${SRCS})
set_property(TARGET tests-glm-voidptr-transformable PROPERTY COMPILE_DEFINITIONS ${DEFINITIONS})



# Run the unit tests
set(WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

add_custom_target(Run-UnitTests-using-glm ALL tests-glm
                  DEPENDS tests-glm
                  WORKING_DIRECTORY ${WORKING_DIRECTORY}
                  COMMENT "Unit testing: using glm..." VERBATIM)

add_custom_target(Run-UnitTests-using-glm-custom-transformable ALL tests-glm-custom-transformable
                  DEPENDS tests-glm-custom-transformable
                  WORKING_DIRECTORY ${WORKING_DIRECTORY}
                  COMMENT "Unit testing: using glm and custom transformable class..." VERBATIM)

add_custom_target(Run-UnitTests-using-glm-voidptr-transformable ALL tests-glm-voidptr-transformable
                  DEPENDS tests-glm-voidptr-transformable
                  WORKING_DIRECTORY ${WORKING_DIRECTORY}
                  COMMENT "Unit testing: using glm and void* transformable..." VERBATIM)
