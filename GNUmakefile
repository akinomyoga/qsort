# -*- mode: makefile-gmake -*-

all:
.PHONY: all

optimization_type := L3N
ifeq ($(optimization_type),L0)
  optimization_flags := -O0
else ifeq ($(optimization_type),L1)
  optimization_flags := -O1
else ifeq ($(optimization_type),L2)
  optimization_flags := -O2
else ifeq ($(optimization_type),L3)
  optimization_flags := -O3
else ifeq ($(optimization_type),L3N)
  optimization_flags := -O3 -march=native
endif

use_random := mt19937
ifeq ($(use_random),mt19937)
  def_random := -DRANDOM_TYPE=1
else
  def_random := -DRANDOM_TYPE=0
endif

BUILD_DIRECTORY   := build/$(optimization_type)
INSTALL_DIRECTORY := bin
DIRS += \
  $(BUILD_DIRECTORY) \
  $(INSTALL_DIRECTORY)

CPPFLAGS = -MD -MP -MF $(@:.o=.dep) -DQSORT_MEASURE_DISABLE_MID
-include $(wildcard $(BUILD_DIRECTORY)/*.dep)

CC       := gcc
CXX      := g++
CFLAGS   := -std=c99   $(optimization_flags)
CXXFLAGS := -std=c++14 $(optimization_flags)

#------------------------------------------------------------------------------

measure_OBJS += \
  $(BUILD_DIRECTORY)/measure.o

measure_OBJS += \
  $(BUILD_DIRECTORY)/mm88c.o \
  $(BUILD_DIRECTORY)/die.o \
  $(BUILD_DIRECTORY)/global_variable.o

measure_OBJS += \
  $(BUILD_DIRECTORY)/qs9e17.o \
  $(BUILD_DIRECTORY)/qs10a5.o \
  $(BUILD_DIRECTORY)/qs10a5m.o

#------------------------------------------------------------------------------
# GPL qsorts を組み込む場合

measure_OBJS += \
  $(BUILD_DIRECTORY)/glibc-msort.o \
  $(BUILD_DIRECTORY)/glibc-qsort.o

measure_OBJS += \
  $(BUILD_DIRECTORY)/newlib-qsort.o

#------------------------------------------------------------------------------

.SECONDARY:

$(BUILD_DIRECTORY)/%.o: %.cpp | $(BUILD_DIRECTORY)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(def_random) -c -o $@ $<
$(BUILD_DIRECTORY)/%.o: %.c | $(BUILD_DIRECTORY)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(def_random) -c -o $@ $<
$(BUILD_DIRECTORY)/%.o: lib/%.c | $(BUILD_DIRECTORY)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(def_random) -c -o $@ $<

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
	./measure.sh

$(DIRS):
	mkdir -p $@
