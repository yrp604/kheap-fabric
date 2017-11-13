# kheap-fabric

## Install

Assuming your `llvm` directory is in `$LLVM`:
- `$ ln -s src/KHeapFabric.cpp $LLVM/tools/clang/lib/StaticAnalyzer/Checkers/`
- edit `$LLVM/tools/clang/include/clang/StaticAnalyzer/Checkers/Checkers.td`
and add
```
def KHeapFabricChecker : Checker<"KHeapFabric">,
  HelpText<"Generate a list of structs and their alloc sites">,
  DescFile<"KHeapFabric.cpp">;
```
- edit `$LLVM/tools/clang/lib/StaticAnalyzer/Checkers/CMakeLists.txt` and add
`KHeapFabric.cpp`
