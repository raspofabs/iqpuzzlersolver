TARGETS=pzsolve
#TARGETS=pzsolve err

CC=g++
#OPTIMISE=-O0
OPTIMISE=-O2
DEBUG=-ggdb3
CFLAGS= -fno-strict-aliasing -DUNIV_LINUX \
	-Werror -Wall -Wextra $(OPTIMISE) $(DEBUG)
#LIBS=-Wl,-Bsymbolic-functions

RAW_TARGETOBJECTS=$(TARGETS:%=%.o)
TARGETOBJECTS=$(RAW_TARGETOBJECTS:%=build/%)
ALLOBJECTS=$(COMMONOBJECTS) $(TARGETOBJECTS) $(OOPOBJECTS)

all: $(TARGETS)

build:
	mkdir -p build

-include $(ALLOBJECTS:.o=.d)

clean:
	rm -f $(TARGETS)
	rm -f build/*

build/%.o: %.cpp | build
	$(CC) $(CFLAGS) -c $*.cpp -o build/$*.o
	$(CC) $(CFLAGS) -MM $*.cpp > build/$*.d
	sed -i '1s/^/build\//' build/$*.d

# testing
pzsolve: build/pzsolve.o $(COMMONOBJECTS) $(CORE) $(TINYJS)
	$(CC) build/pzsolve.o $(COMMONOBJECTS) $(CORE) $(TINYJS) -o pzsolve $(LIBS)
err: build/err.o $(COMMONOBJECTS) $(CORE) $(TINYJS)
	$(CC) build/err.o $(COMMONOBJECTS) $(CORE) $(TINYJS) -o err $(LIBS)

run: $(TARGETS)
	#./pzsolve p71
	./pzsolve
