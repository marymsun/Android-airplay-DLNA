LOCAL_PATH:=$(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE:=libjniinterface

LOCAL_SRC_FILES := \
    com_xindawn_jni_PlatinumJniProxy.cpp

LOCAL_CPPFLAGS := -fpermissive
LOCAL_CFLAGS   := -fpermissive
    
LOCAL_SHARED_LIBRARIES:= libmediaserver
  

LOCAL_LDLIBS += -llog -landroid


    
include $(BUILD_SHARED_LIBRARY)

