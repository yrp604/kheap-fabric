# ast

`$ clang -Xclang -cc1 -Xclang -ast-dump malloc-struct.c`

```
...
    |-DeclStmt 0x55b4252502c8 <line:18:2, col:44>
    | `-VarDecl 0x55b425250170 <col:2, col:43> col:14 used y 'struct foo *' cinit
    |   `-ImplicitCastExpr 0x55b4252502b0 <col:18, col:43> 'struct foo *' <BitCast>
    |     `-CallExpr 0x55b425250280 <col:18, col:43> 'void *'
    |       |-ImplicitCastExpr 0x55b425250268 <col:18> 'void *(*)(unsigned long)' <FunctionToPointerDecay>
    |       | `-DeclRefExpr 0x55b4252501d0 <col:18> 'void *(unsigned long)' Function 0x55b4252317a0 'malloc' 'void *(unsigned long)'
    |       `-UnaryExprOrTypeTraitExpr 0x55b425250218 <col:25, col:42> 'unsigned long' sizeof 'struct foo':'struct foo'
    |-DeclStmt 0x55b425250458 <line:20:2, col:35>
    | `-VarDecl 0x55b425250308 <col:2, col:34> col:14 used z 'struct foo *' cinit
    |   `-ImplicitCastExpr 0x55b425250440 <col:18, col:34> 'struct foo *' <BitCast>
    |     `-CallExpr 0x55b425250410 <col:18, col:34> 'void *'
    |       |-ImplicitCastExpr 0x55b4252503f8 <col:18> 'void *(*)(unsigned long)' <FunctionToPointerDecay>
    |       | `-DeclRefExpr 0x55b425250368 <col:18> 'void *(unsigned long)' Function 0x55b4252317a0 'malloc' 'void *(unsigned long)'
    |       `-UnaryExprOrTypeTraitExpr 0x55b4252503d8 <col:25, col:33> 'unsigned long' sizeof
    |         `-ParenExpr 0x55b4252503b8 <col:31, col:33> 'struct foo':'struct foo' lvalue
    |           `-DeclRefExpr 0x55b425250390 <col:32> 'struct foo':'struct foo' lvalue Var 0x55b425250070 'x' 'struct foo':'struct foo'
    |-DeclStmt 0x55b425250648 <line:22:2, col:39>
    | `-VarDecl 0x55b425250498 <col:2, col:38> col:14 used a 'struct foo *' cinit
    |   `-ImplicitCastExpr 0x55b425250630 <col:18, col:38> 'struct foo *' <BitCast>
    |     `-CallExpr 0x55b425250600 <col:18, col:38> 'void *'
    |       |-ImplicitCastExpr 0x55b4252505e8 <col:18> 'void *(*)(unsigned long)' <FunctionToPointerDecay>
    |       | `-DeclRefExpr 0x55b4252504f8 <col:18> 'void *(unsigned long)' Function 0x55b4252317a0 'malloc' 'void *(unsigned long)'
    |       `-BinaryOperator 0x55b4252505c0 <col:25, col:37> 'unsigned long' '*'
    |         |-UnaryExprOrTypeTraitExpr 0x55b425250568 <col:25, col:33> 'unsigned long' sizeof
    |         | `-ParenExpr 0x55b425250548 <col:31, col:33> 'struct foo':'struct foo' lvalue
    |         |   `-DeclRefExpr 0x55b425250520 <col:32> 'struct foo':'struct foo' lvalue Var 0x55b425250070 'x' 'struct foo':'struct foo'
    |         `-ImplicitCastExpr 0x55b4252505a8 <col:37> 'unsigned long' <IntegralCast>
    |           `-IntegerLiteral 0x55b425250588 <col:37> 'int' 5
    |-DeclStmt 0x55b4252508c8 <line:24:2, col:56>
    | `-VarDecl 0x55b425250710 <col:2, col:55> col:14 used c 'struct bar *' cinit
    |   `-ImplicitCastExpr 0x55b4252508b0 <col:18, col:55> 'struct bar *' <BitCast>
    |     `-CallExpr 0x55b425250880 <col:18, col:55> 'void *'
    |       |-ImplicitCastExpr 0x55b425250868 <col:18> 'void *(*)(unsigned long)' <FunctionToPointerDecay>
    |       | `-DeclRefExpr 0x55b425250770 <col:18> 'void *(unsigned long)' Function 0x55b4252317a0 'malloc' 'void *(unsigned long)'
    |       `-BinaryOperator 0x55b425250840 <col:25, col:54> 'unsigned long' '*'
    |         |-UnaryExprOrTypeTraitExpr 0x55b4252507e0 <col:25, col:33> 'unsigned long' sizeof
    |         | `-ParenExpr 0x55b4252507c0 <col:31, col:33> 'struct foo':'struct foo' lvalue
    |         |   `-DeclRefExpr 0x55b425250798 <col:32> 'struct foo':'struct foo' lvalue Var 0x55b425250070 'x' 'struct foo':'struct foo'
    |         `-UnaryExprOrTypeTraitExpr 0x55b425250820 <col:37, col:54> 'unsigned long' sizeof 'struct bar':'struct bar'
    |-DeclStmt 0x55b425250a28 <line:26:2, col:30>
    | `-VarDecl 0x55b425250908 <col:2, col:29> col:14 used b 'struct foo *' cinit
    |   `-ImplicitCastExpr 0x55b425250a10 <col:18, col:29> 'struct foo *' <BitCast>
    |     `-CallExpr 0x55b4252509c8 <col:18, col:29> 'void *'
    |       |-ImplicitCastExpr 0x55b4252509b0 <col:18> 'void *(*)(unsigned long)' <FunctionToPointerDecay>
    |       | `-DeclRefExpr 0x55b425250968 <col:18> 'void *(unsigned long)' Function 0x55b4252317a0 'malloc' 'void *(unsigned long)'
    |       `-ImplicitCastExpr 0x55b4252509f8 <col:25> 'unsigned long' <IntegralCast>
    |         `-IntegerLiteral 0x55b425250990 <col:25> 'int' 65
...
```
