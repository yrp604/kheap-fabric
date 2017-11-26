# kheap-fabric

## Install

`$ make`

## Usage

```sh
$ scan-build \
	-load-plugin <path/to/KHeapFabric.so> \
	-enable-checker alpha.security.KHeapFabric \
	-disable-checker core,unix,deadcode,nullability \
	make <your make target>
```
