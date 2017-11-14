all: report

.PHONY: report
report:
	scan-build -o report -enable-checker alpha.security.KHeapFabric make test 

.PHONY: view
view: clean report
	scan-view /home/joe/kheap-fabric/report/* --host 45.63.39.81 --allow-all-hosts

test: t/malloc-struct.c
	$(CC) -o t/$@ $^

.PHONY: clean
clean:
	$(RM) -r report/* t/test
