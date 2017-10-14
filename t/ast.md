# ast

`$ clang -Xclang -cc1 -Xclang -ast-dump malloc-struct.c`

```
...
    |     `-CallExpr 0x559eda5daf10 <col:18, col:43> 'void *'
    |       |-ImplicitCastExpr 0x559eda5daef8 <col:18> 'void *(*)(unsigned long)' <FunctionToPointerDecay>
    |       | `-DeclRefExpr 0x559eda5dae60 <col:18> 'void *(unsigned long)' Function 0x559eda5cf670 'malloc' 'void *(unsigned long)'
    |       `-UnaryExprOrTypeTraitExpr 0x559eda5daea8 <col:25, col:42> 'unsigned long' sizeof 'struct foo':'struct foo'
    |-DeclStmt 0x559eda5db0e8 <line:14:2, col:35>
    | `-VarDecl 0x559eda5daf98 <col:2, col:34> col:14 z 'struct foo *' cinit
    |   `-ImplicitCastExpr 0x559eda5db0d0 <col:18, col:34> 'struct foo *' <BitCast>
    |     `-CallExpr 0x559eda5db0a0 <col:18, col:34> 'void *'
    |       |-ImplicitCastExpr 0x559eda5db088 <col:18> 'void *(*)(unsigned long)' <FunctionToPointerDecay>
    |       | `-DeclRefExpr 0x559eda5daff8 <col:18> 'void *(unsigned long)' Function 0x559eda5cf670 'malloc' 'void *(unsigned long)'
    |       `-UnaryExprOrTypeTraitExpr 0x559eda5db068 <col:25, col:33> 'unsigned long' sizeof
    |         `-ParenExpr 0x559eda5db048 <col:31, col:33> 'struct foo':'struct foo' lvalue
    |           `-DeclRefExpr 0x559eda5db020 <col:32> 'struct foo':'struct foo' lvalue Var 0x559eda5dad00 'x' 'struct foo':'struct foo'
    |-DeclStmt 0x559eda5db7c8 <line:16:2, col:39>
    | `-VarDecl 0x559eda5db128 <col:2, col:38> col:14 a 'struct foo *' cinit
    |   `-ImplicitCastExpr 0x559eda5db7b0 <col:18, col:38> 'struct foo *' <BitCast>
    |     `-CallExpr 0x559eda5db780 <col:18, col:38> 'void *'
    |       |-ImplicitCastExpr 0x559eda5db768 <col:18> 'void *(*)(unsigned long)' <FunctionToPointerDecay>
    |       | `-DeclRefExpr 0x559eda5db188 <col:18> 'void *(unsigned long)' Function 0x559eda5cf670 'malloc' 'void *(unsigned long)'
    |       `-BinaryOperator 0x559eda5db740 <col:25, col:37> 'unsigned long' '*'
    |         |-UnaryExprOrTypeTraitExpr 0x559eda5db1f8 <col:25, col:33> 'unsigned long' sizeof
    |         | `-ParenExpr 0x559eda5db1d8 <col:31, col:33> 'struct foo':'struct foo' lvalue
    |         |   `-DeclRefExpr 0x559eda5db1b0 <col:32> 'struct foo':'struct foo' lvalue Var 0x559eda5dad00 'x' 'struct foo':'struct foo'
    |         `-ImplicitCastExpr 0x559eda5db238 <col:37> 'unsigned long' <IntegralCast>
    |           `-IntegerLiteral 0x559eda5db218 <col:37> 'int' 5
...
```
