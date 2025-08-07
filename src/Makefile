
INCLUDES = -I/usr/local/include -I/opt/local/include

CXX = clang++
# OPT = -g
OPT = -O3  -DNDEBUG
CXXFLAGS = -std=c++20 -Wall -MMD $(INCLUDES) $(OPT) -stdlib=libc++ \
	-isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk 
LIBS = -L/usr/local/lib -rpath /usr/local/lib

OBJECTS = anisohedral.o action.o laves.o shape.o picture.o
DEPENDS = ${OBJECTS:.o=.d}

all: anisohedral

anisohedral: anisohedral.o picture.o laves.o action.o shape.o
	$(CXX) $(LIBS) -o anisohedral anisohedral.o picture.o laves.o action.o shape.o 

picture.o: picture.hpp common.hpp

action.o: action.hpp common.hpp

laves.o: laves.hpp common.hpp

shape.o: shape.hpp earcut.hpp common.hpp

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS}
