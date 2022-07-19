# Compiler detials and flags
CXX = clang
CXX_FLAGS = --std=c11 -g -Wall # -fsanitize=address -fno-omit-frame-pointer 

# Executable details and flags
BUILD_DIR = build
EXEC_DIR = bin

# Stateful signatures directory 
SIG_STFL_DIR = sig_stfl
SIG_STFL_SRC_FILES = $(wildcard $(SIG_STFL_DIR)/*.c)
SIG_STFL_HDR_FILES = $(wildcard $(SIG_STFL_DIR)/*.h)

# Library details and flags
LDFLAGS = -L../liboqs/build/lib
LDLIBS = -lcrypto -loqs -lm
INC_FLAGS = -I../liboqs/build/include -I./sig_stfl/xmss/external

# XMSS implementation directory and files
XMSS_IMPL_DIR = $(SIG_STFL_DIR)/xmss/external
XMSS_IMPL_SRC_FILES = $(wildcard $(XMSS_IMPL_DIR)/*.c)
XMSS_IMPL_HDR_FILES = $(wildcard $(XMSS_IMPL_DIR)/*.h) 

# XMSS object directory and files
XMSS_OBJ_BUILD_DIR = $(SIG_STFL_DIR)/xmss
XMSS_OBJ_BUILD_SRC_FILES = $(wildcard $(XMSS_OBJ_BUILD_DIR)/*.c)
XMSS_OBJ_BUILD_HDR_FILES = $(wildcard $(XMSS_OBJ_BUILD_DIR)/*.h)

# Tests to performs
TESTS = $(EXEC_DIR)/test_struct # $(BUILD_DIR)/test_fast $(BUILD_DIR)/test_multi  $(BUILD_DIR)/test_subkeys

tests: $(TESTS)

.PHONY: clean test

# Building the test executable(s)
$(EXEC_DIR)/test_struct: tests/new_structure_tester.c $(XMSS_IMPL_SRC_FILES) $(XMSS_IMPL_HDR_FILES) $(XMSS_OBJ_BUILD_SRC_FILES)  $(XMSS_OBJ_BUILD_HDR_FILES) $(SIG_STFL_SRC_FILES)  $(SIG_STFL_HDR_FILES) 
	$(CXX) $(CXX_FLAGS) -o $@ $(SIG_STFL_SRC_FILES) $(XMSS_IMPL_SRC_FILES) $(XMSS_OBJ_BUILD_SRC_FILES) $< $(LDLIBS) $(LDFLAGS) $(INC_FLAGS) 


# Regular test executables (now defunct cause no secret_key.h and secret_key.c)
$(EXEC_DIR)/test_fast: tests/full_tester.c $(XMSS_IMPL_SRC_FILES) $(OBJS) $(XMSS_IMPL_HDR_FILES)
	$(CXX) $(CXX_FLAGS) -o $@ $(XMSS_IMPL_SRC_FILES) $< $(LDLIBS) $(LDFLAGS) $(INC_FLAGS) 

$(EXEC_DIR)/test_subkeys: tests/subkeys_tester.c $(XMSS_IMPL_SRC_FILES) $(OBJS) $(XMSS_IMPL_HDR_FILES)
	$(CXX) $(CXX_FLAGS) -o $@ $(XMSS_IMPL_SRC_FILES) $< $(LDLIBS) $(LDFLAGS) $(INC_FLAGS) 

$(EXEC_DIR)/test_multi: tests/multithreaded_tester.c $(XMSS_IMPL_SRC_FILES) $(OBJS) $(XMSS_IMPL_HDR_FILES)
	$(CXX) $(CXX_FLAGS) -o $@ $(XMSS_IMPL_SRC_FILES) $< $(LDLIBS) $(LDFLAGS) $(INC_FLAGS) 

clean:
	-$(RM) $(TESTS)
