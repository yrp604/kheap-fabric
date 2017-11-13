all: report

.PHONY: report
report:
	scan-build -o report -enable-checker alpha.security.KHeapFabric make test 

test: t/malloc-struct.c
	$(CC) -o t/$@ $^

.PHONY: clean
clean:
	$(RM) -r report/* t/test
