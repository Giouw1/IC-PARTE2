TARGET = gen
CXXFLAGS = -m64 -std=c++11
CPPFLAGS = -pthread -w
LDFLAGS = -L${LIB} 
LIBS = -lm
OBJS = 

all: ${TARGET}

gen : gen.cpp $(OBJS)
	${CXX} ${CXXFLAGS} -o $@ $^ ${CPPFLAGS} ${LIBS}
clean:
	rm -f *.o *~ ${TARGET} core




