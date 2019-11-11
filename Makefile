NAME := date
SHARED := lib$(NAME).so
ARCHIVE := lib$(NAME).a

CXX := /usr/bin/g++
AR := /usr/bin/ar


DEPENDENCY := \
./inc/Date.h

BINDIR := ./build/
OBJDIR := $(BINDIR)object/
SOURCEDIR := ./src/
LIBDIR := $(BINDIR)lib/

INCLUDE := \
-I./inc

CPPFLAGS := -std=c++1z $(INCLUDE) $(LIBS) $(DEFINES) -Wall -Werror
LD_FLAGS := -fPIC

TEST := $(BINDIR)$(NAME)_test


sources := $(wildcard $(SOURCEDIR)*.cpp)
objects := $(addprefix $(OBJDIR),$(notdir $(sources:.cpp=.o)))

.PHONY:all clean debug

all:$(TEST) shared_lib archive_lib

shared_lib:$(objects)
	@mkdir -p $(LIBDIR)
	$(CXX) --shared $(CPPFLAGS) $(LD_FLAGS) $^ -o $(LIBDIR)$(SHARED)

archive_lib:$(objects)
	@mkdir -p $(LIBDIR)
	$(AR) rcs $(LIBDIR)$(ARCHIVE) $^

$(OBJDIR)%.o: $(SOURCEDIR)%.cpp $(DEPENDENCY)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(LD_FLAGS) -c $<  -o $@

$(TEST):$(objects)
	@mkdir -p $(BINDIR)
	$(CXX) $(LD_FLAGS) $^ -o $@

clean:
	@rm -rf build/
debug:
	@echo $(TEST)