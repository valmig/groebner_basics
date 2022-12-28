CXXFLAGS = -O2 -Wall -pedantic-errors -pedantic -std=c++14  `val-config --cflags` 
LDFLAGS = -s `val-config --libs`
BIN = bin
CXX = g++

OBJECTS = dehomogenize eliminationmatrix groebner groebnerwalk hilbertconversion hilbertpolynomial homgroebner \
			homogenize idealroots intersectideals isgroebner isinG minpol mod_minpolo radicalideal
  
.PHONY ALL:
 
ALL: create_dirs $(OBJECTS)

create_dirs:
	mkdir -p $(BIN)

$(OBJECTS): % : src/%.cpp
	${CXX} $(CXXFLAGS) $(LDFLAGS) src/$@.cpp -o ${BIN}/$@


.PHONY clean:

clean:
	rm -r -f $(BIN)

