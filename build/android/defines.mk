# Dali defines
LOCAL_CFLAGS += -DNON_POWER_OF_TWO_TEXTURES

# Warnings, optimisation and debug flags
# --------------------------------------
LOCAL_CFLAGS += -w -Wall
LOCAL_CFLAGS += -g
ifeq ($(APP_OPTIM),debug)
#LOCAL_CFLAGS += -DDALI_GL_ERROR_CHECK                             # Extra GL error checking / reporting in Dali renderer
LOCAL_CFLAGS += -DDEBUG_ENABLED                                   # Generic flag to enable additional debug in code (toolkit, adaptor, dali, and also libxml)
LOCAL_CFLAGS += -O0
ifeq ($(BUILD_NATIVE_TRACE), 1)
LOCAL_CFLAGS += -DBUILD_NATIVE_TRACE
$(info Native Android systrace enabled)
endif
else
LOCAL_CFLAGS += -Os
endif

ifeq ($(DALI_LOGGING), 1)
LOCAL_CFLAGS += -DENABLE_DALI_LOGGING
endif

# C++ language features
# ---------------------
LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti
