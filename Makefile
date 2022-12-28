CXXFLAGS = -O2 -Wall -pedantic-errors -pedantic -std=c++14  `val-config --cflags` 
LDFLAGS = -s `val-config --libs`
OBJ = obj
BIN = bin
CXX = g++

OBJECTS = dehomogenize eliminationmatrix groebner groebnerwalk hilbertconversion hilbertpolynomial homgroebner \
			homogenize idealroots intersectideals isgroebner isinG minpol mod_minpolo radicalideal
  

$(BIN)/ALL: create_dirs $(OBJECTS)

create_dirs:
	mkdir -p $(OBJ) $(BIN)

$(OBJECTS) : % : src/%.cpp
	${CXX} -c $(CXXFLAGS) src/$@.cpp -o ${OBJ}/$@.o
	${CXX} -o ${BIN}/$@ ${OBJ}/$@.o $(LDFLAGS)


.PHONY clean:

clean:
	rm -r -f $(OBJ) $(BIN)

