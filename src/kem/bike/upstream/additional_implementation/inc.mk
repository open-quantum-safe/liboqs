#WA for GCC 4.8.5 bug.
EXTRA_FLAGS += -Wno-missing-braces -Wno-missing-field-initializers -maes -mavx

#Always use SHA384 code other versions are not supported
EXTRA_FLAGS +=-DSHA384

ifdef RDTSC
    EXTRA_FLAGS += -DRDTSC
endif

ifdef VERBOSE
    EXTRA_FLAGS += -DVERBOSE=$(VERBOSE)
endif

ifdef VER
    EXTRA_FLAGS += -DBIKE_VER=$(VER)
else
    $(info using BIKE_VER=1 as default.)
    EXTRA_FLAGS += -DBIKE_VER=1
endif

ifdef FIXED_SEED
    EXTRA_FLAGS += -DFIXED_SEED=1
endif

ifdef CHECK_COMPILATION
    EXTRA_FLAGS += -mno-red-zone -fvisibility=hidden -funsigned-char -Wall -Wextra -Werror -Wpedantic 
endif

ifdef BATCH_SIZE
    EXTRA_FLAGS += -DBATCH_SIZE=$(BATCH_SIZE)
endif

ifdef AVX512_SUPPORT
    SUF = _avx512
else
    EXTRA_FLAGS += -mavx2
    SUF = _avx2
endif

ifdef CONSTANT_TIME
    EXTRA_FLAGS += -DCONSTANT_TIME
endif

ifdef VALIDATE_CONSTANT_TIME
    EXTRA_FLAGS += -DVALIDATE_CONSTANT_TIME
endif

ifdef LEVEL
    EXTRA_FLAGS += -DLEVEL=$(LEVEL)
endif

