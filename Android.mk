LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)


LOCAL_CPP_EXTENSION := .cc

LOCAL_SRC_FILES := \
	main.cc \
	sniper.cc

LOCAL_CFLAGS := -w -Wno-unused-parameter


LOCAL_MODULE := sniper

LOCAL_SHARED_LIBRARIES := \
	libbase \
	libbinder \
	libutils

include $(BUILD_EXECUTABLE)
