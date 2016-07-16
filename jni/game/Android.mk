#-------------------------------------------------------
#game file
#-------------------------------------------------------
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LS_CPP=$(subst $(1)/,,$(wildcard $(1)/*.cpp))

LOCAL_MODULE := game
LOCAL_SRC_FILES := $(call LS_CPP,$(LOCAL_PATH))
LOCAL_WHOLE_STATIC_LIBRARIES := pegas_gfx pegas_core pegas_system pegas_app pegas_gui pegas_physics
#LOCAL_CFLAGS := -g -ggdb -O0

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path,$(LOCAL_PATH)/..)
$(call import-module, core)
$(call import-module, gfx)
$(call import-module, system)
$(call import-module, app)
$(call import-module, gui)
$(call import-module, physics)