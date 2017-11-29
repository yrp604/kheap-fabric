# kheap-fabric

## Install

`$ make`

## Usage

```sh
$ scan-build \
	-o <output dir> \
	-load-plugin <path/to/KHeapFabric.so> \
	-enable-checker alpha.security.KHeapFabric \
	-disable-checker core,unix,deadcode,nullability \
	make <your make target>
$ ./parse-dumps.py <output dir>/<report dir> > reports
```
