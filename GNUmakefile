# -*- mode: makefile-gmake -*-

all:
.PHONY: all

optimization_type := L3N
ifeq ($(optimization_type),L0)
  optimization_flags := -O0 -DNDEBUG
else ifeq ($(optimization_type),L1)
  optimization_flags := -O1 -DNDEBUG
else ifeq ($(optimization_type),L2)
  optimization_flags := -O2 -DNDEBUG
else ifeq ($(optimization_type),L3)
  optimization_flags := -O3 -DNDEBUG
else ifeq ($(optimization_type),L3N)
  optimization_flags := -O3 -march=native -DNDEBUG
endif

use_random := mt19937
ifeq ($(use_random),mt19937)
  use_random_flags := -DRANDOM_TYPE=1
else ifeq ($(use_random),rand)
  use_random_flags := -DRANDOM_TYPE=2
else
  use_random_flags := -DRANDOM_TYPE=0
endif

BUILD_DIRECTORY   := build/$(optimization_type)
INSTALL_DIRECTORY := bin
DIRS += \
  $(BUILD_DIRECTORY) \
  $(INSTALL_DIRECTORY)

CPPFLAGS = -MD -MP -MF $(@:.o=.dep)
-include $(wildcard $(BUILD_DIRECTORY)/*.dep)

CC       := gcc
CXX      := g++
CFLAGS   := -std=c99   $(optimization_flags) $(use_random_flags) -Wall -Wextra -Wno-implicit-fallthrough
CXXFLAGS := -std=c++14 $(optimization_flags) $(use_random_flags) -Wall -Wextra -Wno-implicit-fallthrough

#------------------------------------------------------------------------------

measure_OBJS += \
  $(BUILD_DIRECTORY)/measure.o

measure_OBJS += \
  $(BUILD_DIRECTORY)/die.o \
  $(BUILD_DIRECTORY)/global_variable.o

measure_OBJS += \
  $(BUILD_DIRECTORY)/qs9e17.o \
  $(BUILD_DIRECTORY)/qs10a6.o \
  $(BUILD_DIRECTORY)/qs10a6m.o \
  $(BUILD_DIRECTORY)/qs226ms2.o

#------------------------------------------------------------------------------
# GPL qsorts を組み込む場合

measure_OBJS += \
  $(BUILD_DIRECTORY)/glibc-msort.o \
  $(BUILD_DIRECTORY)/glibc-qsort.o

measure_OBJS += \
  $(BUILD_DIRECTORY)/newlib-qsort.o

#------------------------------------------------------------------------------

.INTERMEDIATE:

$(BUILD_DIRECTORY)/%.o: %.cpp | $(BUILD_DIRECTORY)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
$(BUILD_DIRECTORY)/%.o: %.c | $(BUILD_DIRECTORY)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
$(BUILD_DIRECTORY)/%.o: lib/%.c | $(BUILD_DIRECTORY)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(BUILD_DIRECTORY)/measure.exe: $(measure_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
all: $(BUILD_DIRECTORY)/measure.exe

#------------------------------------------------------------------------------

install:
.PHONY: install
install: $(INSTALL_DIRECTORY)/measure.exe
$(INSTALL_DIRECTORY)/measure.exe: $(BUILD_DIRECTORY)/measure.exe
	cp $< $@

clean:
.PHONY: clean
clean:
	-rm -rf $(BUILD_DIRECTORY)/*.o $(BUILD_DIRECTORY)/*.dep

measure:
.PHONY: all
measure:
	for opt in L0 L1 L2 L3 L3N; do make optimization_type=$$opt all; done
	./measure.sh measure2

$(DIRS):
	mkdir -p $@
