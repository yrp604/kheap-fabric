CXXFLAGS = $(shell llvm-config --cxxflags)
LDFLAGS  = $(shell llvm-config --ldflags)
LIBS     = -lclang

all: KHeapFabric.so

src/KHeapFabric.cpp.o: src/KHeapFabric.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

KHeapFabric.so: src/KHeapFabric.cpp.o
	$(CXX) $(LDFLAGS) -shared -o $@ $^ $(LIBS)

.PHONY: report
report: KHeapFabric.so
	scan-build \
		-o report \
		-load-plugin ./KHeapFabric.so \
		-enable-checker alpha.security.KHeapFabric \
		make test

.PHONY: view
view: clean report
	scan-view $(shell pwd)/report/* --host 45.63.39.81 --allow-all-hosts

test: t/malloc-struct.c
	$(CC) -o t/$@ $^

.PHONY: clean
clean:
	$(RM) -r report/*
	$(RM) t/test
	$(RM) src/KHeapFabric.cpp.o
	$(RM) KHeapFabric.so
