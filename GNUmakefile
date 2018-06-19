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
$(BUILD_DIRECTORY)/measure.o: measure.cpp | $(BUILD_DIRECTORY)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

measure_OBJS += \
  $(BUILD_DIRECTORY)/qs9e17.o \
  $(BUILD_DIRECTORY)/qs10a5.o \
  $(BUILD_DIRECTORY)/qs10a5m.o \
  $(BUILD_DIRECTORY)/mm88c.o
$(BUILD_DIRECTORY)/qs9e17.o:  qs9e17.c | $(BUILD_DIRECTORY)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
$(BUILD_DIRECTORY)/qs10a5.o:  qs10a5.c | $(BUILD_DIRECTORY)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
$(BUILD_DIRECTORY)/qs10a5m.o: qs10a5.c | $(BUILD_DIRECTORY)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DMEMCPY -Dqsort10a5=qsort10a5m -c -o $@ $<
$(BUILD_DIRECTORY)/mm88c.o:    mm88c.c | $(BUILD_DIRECTORY)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

#------------------------------------------------------------------------------
# glibc qsort を組み込む場合

measure_OBJS += \
  $(BUILD_DIRECTORY)/glibc-msort.o \
  $(BUILD_DIRECTORY)/glibc-qsort.o
$(BUILD_DIRECTORY)/glibc-qsort.o: lib/glibc-qsort.c | $(BUILD_DIRECTORY)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
$(BUILD_DIRECTORY)/glibc-msort.o: lib/glibc-msort.c | $(BUILD_DIRECTORY)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

#------------------------------------------------------------------------------

$(BUILD_DIRECTORY)/measure.exe: $(measure_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
all: $(BUILD_DIRECTORY)/measure.exe

install:
.PHONY: install

install: $(INSTALL_DIRECTORY)/measure.exe
$(INSTALL_DIRECTORY)/measure.exe: $(BUILD_DIRECTORY)/measure.exe
	cp $< $@

clean:
.PHONY: clean

clean:
	-rm -rf $(BUILD_DIRECTORY)/*.o $(BUILD_DIRECTORY)/*.dep

$(DIRS):
	mkdir -p $@
