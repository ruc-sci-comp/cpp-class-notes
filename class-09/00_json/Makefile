CXX:=g++
STD:=-std=c++20
CFLAGS:=-Wall -Werror -Wextra -Weffc++ -pedantic
DEFINITIONS:= -DFMT_HEADER_ONLY
DEBUG_FLAGS:=-g

PYTHON:=python3

SRC:=$(wildcard src/*.cpp)
INC:=$(wildcard include/*.h)
OBJ:=$(SRC:src/%.cpp=obj/%.o)
DEP:=$(SRC:src/%.cpp=dep/%.d)
TRGT_EXE:=bin/main.exe

ifeq ($(SRC),)
$(error No source files found, there is nothing to build!)
endif

INCLUDES:=-I../include -Iinclude

$(TRGT_EXE): setup setup_bin $(OBJ)
	$(CXX) $(OBJ) $(PYTHON_LD_FLAGS) -o $@

dashboard: setup $(TRGT_EXE)
	python -m panel serve --allow-websocket-origin=* dashboard

setup:
	@mkdir -p obj
	@mkdir -p dep

setup_bin:
	@mkdir -p bin

-include $(DEP)

obj/%.o: src/%.cpp $(INC)
	$(CXX) $(STD) $(DEBUG_FLAGS) $(CFLAGS) $(DEFINITIONS) $(INCLUDES) $(PYTHON_INCLUDES) -MF $(<:src/%.cpp=dep/%.d) -MMD -c $< -o $@

clean:
	rm -rf obj dep bin lib $(TRGT_EXE) $(TGRT_DASHBOARD) test_output __pycache__ $(HTML) 

.PHONY:all dashboard setup clean
