CXX:=g++
STD:=-std=c++20
CFLAGS:=-Wall -Werror -Wextra -Weffc++ -pedantic
DEFINITIONS:= -DFMT_HEADER_ONLY
DEBUG_FLAGS:=-g

PYTHON:=python3

SRC:=$(wildcard src/*.cpp)
INC:=$(wildcard include/*.h)
OBJ:=$(SRC:src/%.cpp=obj/%.o)
LIBOBJ:=$(subst obj/main.o,,${OBJ})
OBJ:=$(subst obj/dashboard_model.o,,${OBJ})

DEP:=$(SRC:src/%.cpp=dep/%.d)
TRGT_EXE:=bin/main.exe
TGRT_DASHBOARD:=dashboard/model$(shell $(PYTHON)-config --extension-suffix)

ifeq ($(SRC),)
$(error No source files found, there is nothing to build!)
endif

INCLUDES:=-I../include -Iinclude

PYTHON_INCLUDES:=$(shell $(PYTHON)-config --includes)
PYTHON_LIB_DIR:=$(shell $(PYTHON)-config --prefix)/lib
PYTHON_LIBS:=$(shell $(PYTHON)-config --libs --embed)
PYTHON_LD_FLAGS:=-Wl,-rpath,$(PYTHON_LIB_DIR) -L$(PYTHON_LIB_DIR) $(PYTHON_LIBS)

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

test: $(TEST) $(TRGT_EXE)
	contest contest.yaml

.PHONY:all dashboard setup clean test
