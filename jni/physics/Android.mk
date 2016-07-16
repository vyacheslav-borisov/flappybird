#-------------------------------------------------------
# math module
#-------------------------------------------------------
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LS_CPP=$(subst $(1)/,,$(wildcard $(1)/*.cpp))

LOCAL_MODULE := pegas_physics
LOCAL_SRC_FILES := $(call LS_CPP,$(LOCAL_PATH))
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM
LOCAL_STATIC_LIBRARIES := android_native_app_glue png
#LOCAL_CFLAGS := -g -ggdb -O0

include $(BUILD_STATIC_LIBRARY)
$(call import-module, android/native_app_glue)
$(call import-module, libpng)
