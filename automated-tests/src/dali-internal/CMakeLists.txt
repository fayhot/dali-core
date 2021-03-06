SET(PKG_NAME "dali-internal")

SET(EXEC_NAME "tct-${PKG_NAME}-core")
SET(RPM_NAME "core-${PKG_NAME}-tests")

SET(CAPI_LIB "dali-internal")

SET(TC_SOURCES
        utc-Dali-Internal-ActorObserver.cpp
        utc-Dali-Internal-Core.cpp
        utc-Dali-Internal-FixedSizeMemoryPool.cpp
        utc-Dali-Internal-FrustumCulling.cpp
        utc-Dali-Internal-Handles.cpp
        utc-Dali-Internal-MemoryPoolObjectAllocator.cpp
        utc-Dali-Internal-OwnerPointer.cpp
)

LIST(APPEND TC_SOURCES
        ../dali/dali-test-suite-utils/mesh-builder.cpp
        ../dali/dali-test-suite-utils/test-harness.cpp
        ../dali/dali-test-suite-utils/test-actor-utils.cpp
        ../dali/dali-test-suite-utils/dali-test-suite-utils.cpp
        ../dali/dali-test-suite-utils/test-application.cpp
        ../dali/dali-test-suite-utils/test-gl-abstraction.cpp
        ../dali/dali-test-suite-utils/test-gl-sync-abstraction.cpp
        ../dali/dali-test-suite-utils/test-native-image.cpp
        ../dali/dali-test-suite-utils/test-platform-abstraction.cpp
        ../dali/dali-test-suite-utils/test-render-controller.cpp
        ../dali/dali-test-suite-utils/test-render-surface.cpp
        ../dali/dali-test-suite-utils/test-trace-call-stack.cpp
)

PKG_CHECK_MODULES(${CAPI_LIB} REQUIRED
     dali-core
)

ADD_COMPILE_OPTIONS( -O0 -ggdb --coverage -Wall -Werror )
ADD_COMPILE_OPTIONS( ${${CAPI_LIB}_CFLAGS_OTHER} )

FOREACH(directory ${${CAPI_LIB}_LIBRARY_DIRS})
    SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -L${directory}")
ENDFOREACH(directory ${CAPI_LIB_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(
    ../../..
    .
    ${${CAPI_LIB}_INCLUDE_DIRS}
    ../dali/dali-test-suite-utils
    ../dali-devel
)

ADD_EXECUTABLE(${EXEC_NAME} ${EXEC_NAME}.cpp ${TC_SOURCES})
TARGET_LINK_LIBRARIES(${EXEC_NAME}
    ${${CAPI_LIB}_LIBRARIES}
    --coverage
)

INSTALL(PROGRAMS ${EXEC_NAME}
    DESTINATION ${BIN_DIR}/${EXEC_NAME}
)
