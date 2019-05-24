LOCAL_PATH := $(call my-dir)
SRC_PATH := $(LOCAL_PATH)/../../..

include $(CLEAR_VARS)
include ./defines.mk

LOCAL_MODULE := libdali-core

INCLUDE_PATH=$(LOCAL_PATH)/../../../dali-env/android/include

LOCAL_EXPORT_C_INCLUDES += $(INCLUDE_PATH)

cppfiles :=

public_api_src_dir=$(SRC_PATH)/dali-core/dali/public-api
include $(public_api_src_dir)/file.list

devel_api_src_dir = $(SRC_PATH)/dali-core/dali/devel-api
include $(devel_api_src_dir)/file.list

internal_src_dir=$(SRC_PATH)/dali-core/dali/internal
include $(internal_src_dir)/file.list

platform_abstraction_src_dir=$(SRC_PATH)/dali-core/dali/integration-api
include $(platform_abstraction_src_dir)/file.list

cppfiles += $(public_api_src_files) \
            $(devel_api_src_files) \
            $(internal_src_files) \
            $(platform_abstraction_src_files) \

LOCAL_CFLAGS += -I$(SRC_PATH)/dali-core
LOCAL_CFLAGS += -I$(SRC_PATH)/dali-core/dali
LOCAL_CFLAGS += -I$(SRC_PATH)/dali-core/dali/integration-api
LOCAL_CFLAGS += -I$(SRC_PATH)/dali-core/dali/devel-api
LOCAL_CFLAGS += -I$(SRC_PATH)/dali-core/dali/public-api

LOCAL_SRC_FILES := $(cppfiles)

include $(BUILD_STATIC_LIBRARY)

DALI_PUBLIC_INCLUDES := $(INCLUDE_PATH)/dali


