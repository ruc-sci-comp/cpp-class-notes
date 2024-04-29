CXX:=g++
STD:=-std=c++20
CFLAGS:=-Wall -Werror -Wextra -Weffc++ -pedantic
DEFINITIONS:= -DFMT_HEADER_ONLY -fPIC
OPTIMIZATION_FLAGS:=-O3 -ffast-math

PYTHON:=python3

SRC:=$(wildcard src/*.cpp)
INC:=$(wildcard include/*.h)
OBJ:=$(SRC:src/%.cpp=obj/%.o)
LIBOBJ:=$(subst obj/main.o,,${OBJ})
OBJ:=$(subst obj/pymodel.o,,${OBJ})
OBJ:=$(subst obj/mc_dashboard_model.o,,${OBJ})

DEP:=$(SRC:src/%.cpp=dep/%.d)
TGRT_DASHBOARD:=dashboard/model$(shell $(PYTHON)-config --extension-suffix)

ifeq ($(SRC),)
$(error No source files found, there is nothing to build!)
endif

INCLUDES:=-I../include -Iinclude

PYTHON_INCLUDES:=$(shell $(PYTHON)-config --includes)
PYTHON_LIB_DIR:=$(shell $(PYTHON)-config --prefix)/lib
PYTHON_LIBS:=$(shell $(PYTHON)-config --libs --embed)
PYTHON_LD_FLAGS:=-Wl,-rpath,$(PYTHON_LIB_DIR) -L$(PYTHON_LIB_DIR) $(PYTHON_LIBS)

dashboard: setup $(LIBOBJ) obj/pymodel.o
	$(CXX) $(LIBOBJ) obj/pymodel.o $(PYTHON_LD_FLAGS) -shared -o $(TGRT_DASHBOARD)
	panel serve dashboard

setup:
	@mkdir -p obj
	@mkdir -p dep

setup_bin:
	@mkdir -p bin

-include $(DEP)

obj/pymodel.o: dashboard/pymodel.cpp $(INC)
	$(CXX) $(STD) $(OPTIMIZATION_FLAGS) $(CFLAGS) $(DEFINITIONS) $(INCLUDES) $(PYTHON_INCLUDES) -MF $(<:dashboard/%.cpp=dep/%.d) -MMD -c $< -o $@

obj/%.o: src/%.cpp $(INC)
	$(CXX) $(STD) $(OPTIMIZATION_FLAGS) $(CFLAGS) $(DEFINITIONS) $(INCLUDES) $(PYTHON_INCLUDES) -MF $(<:src/%.cpp=dep/%.d) -MMD -c $< -o $@

clean:
	rm -rf obj dep bin lib $(TGRT_DASHBOARD) __pycache__

.PHONY:all dashboard setup clean