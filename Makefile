
SHELL=/bin/sh

LD      := $(CXX)
MKDIR   := mkdir -p
OBJ_DIR := .obj

.SUFFIXES:

$(OBJ_DIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

TARGET = quixey

CXXFLAGS := -std=c++11 -pedantic -W -Wall -O2 -g3
LDFLAGS  := $(CXXFLAGS)

CXX_FILES := \
	array_variable.cc \
	char_variable.cc \
	function.cc \
	function_variable.cc \
	int_variable.cc \
	script_engine.cc \
	string_variable.cc \
	token.cc \
	variable.cc \
	main.cc

H_FILES := \
	array_variable.h \
	char_variable.h \
	error.h \
	function.h \
	function_variable.h \
	int_variable.h \
	library_adaptor.h \
	runtime_error.h \
	script_engine.h \
	script_types.h \
	string_util.h \
	string_variable.h \
	syntax_error.h \
	token.h \
	variable_base.h \
	variable.h \

O_FILES := $(patsubst %.cc, $(OBJ_DIR)/%.o, $(CXX_FILES))
D_FILES := $(O_FILES:.o=.d)

SOURCEFILES :=	$(H_FILES) $(C_FILES)
.PRECIOUS:	$(SOURCEFILES)
.PHONY:     all clean

all: $(TARGET)

$(O_FILES): | $(OBJ_DIR) 

$(D_FILES): | $(OBJ_DIR)

$(OBJ_DIR) :
	@$(MKDIR) $@

$(TARGET): $(O_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	$(RM) $(O_FILES) $(D_FILES) $(TARGET)

-include $(D_FILES)
