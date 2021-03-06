# All you need is here
# =========================================================
PROJECT_NAME := sevec

source-dir := src
test-dir := tests

C_CXX_FLAGS := -Werror -Wall -Wextra -Wundef -Wpedantic -g -Os \
	       -Wunreachable-code \
	       -Wmissing-field-initializers
CFLAGS := $(C_CXX_FLAGS) -Wmissing-prototypes -Wstrict-prototypes -std=c89
CXXFLAGS := $(C_CXX_FLAGS) -std=c++11

LDFLAGS :=

# Rest of the Makefile (most likely you won't need it)
# =========================================================

obj-y :=
src := $(source-dir)
tests := $(test-dir)

include $(source-dir)/Makefile
objects := $(source-dir)/$(obj-y)
include $(test-dir)/Makefile
objects += $(test-dir)/$(obj-y)

MAKEFLAGS += -rR

CC = gcc
CXX = g++

ifeq ("$(origin FORCE_CC)", "command line")
  BUILD_FORCE_CC = 1
  override CXX = $(CC)
else ifeq ("$(origin FORCE_CXX)", "command line")
  BUILD_FORCE_CXX = 1
  override CC = $(CXX)
endif
ifndef BUILD_FORCE_CC
  BUILD_FORCE_CC = 0
endif
ifndef BUILD_FORCE_CXX
  BUILD_FORCE_CXX = 0
endif

ifeq ($(BUILD_FORCE_CC), 1)
  CXXFLAGS = $(CFLAGS)
else ifeq ($(BUILD_FORCE_CXX), 1)
  CFLAGS = $(CXXFLAGS)
endif

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(objects)
	$(CC) $(objects) -o $(PROJECT_NAME) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

docs: FORCE
	@echo Building documentation
	make html -C docs/sphinx/

run: $(PROJECT_NAME)
	./$(PROJECT_NAME)

clean:
	rm -f $(PROJECT_NAME)
	rm -f $(PROJECT_NAME).exe
	rm -f $(wildcard ./**/*.o)
	rm -rf docs/sphinx/build
	rm -rf docs/doxygen/build

FORCE: ;
