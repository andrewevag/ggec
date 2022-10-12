# Semantics

## Types
### Basic Types
- int (2 bytes)
- char (1 byte)
- double (10 bytes)
- bool (bool)
### Type Constructors
- Pointer t* (2 bytes)

## Semantic Constraints
- Every program should have defined void main ()
- Pointer variables defined as arrays cannot have their values changed. Assignements do not work.
  ```c
  int a[100], b[200];
  a = b; //Wrong
  ```
- Pascal's scopes
- Every expressions has unique type (! except NULL which has type t* ($\forall \tau, \mathrm{type}(\tau) \rArr \mathrm{NULL} : \tau *$), and every expression can be evaluated to their type.
  - results of evaluation can be l-values or r-values
- *(NULL) is forbidden
- Binary Operation operrands calculated from left to right.
- Only [] and * operators can give l-value


## LVAL and RVAL are sets over expressions (predicates)

## L-value Rules
$\Gamma$ context. $\mathrm{LVAL}$ predicate

1. $\Gamma \vdash x : \tau \land (\mathrm{VARIABLE}(x) \lor \mathrm{PARAMETER}(x)) \rArr \mathrm{LVAL}(x, \tau)$
   1. $\Gamma \vdash x : \tau \land \mathrm{VARIABLE}(x) \rArr \mathrm{LVAL}(x, \tau)$
   2. $\Gamma \vdash x : \tau \land \mathrm{PARAMETER}(x) \rArr \mathrm{LVAL}(x, \tau)$
2. $\Gamma \vdash x : \mathrm{Pointer}(t) \rArr *x : t \land \mathrm{LVAL}(*x, t)$
3. $\Gamma \vdash x : \mathrm{Pointer}(t) \land \Gamma \vdash e : \mathrm{int} \rArr x[e] : t \land \mathrm{LVAL}(x[e], t)$

## R-Values
$\mathrm{RVAL}$ predicate
1. $\Gamma \vdash \mathrm{IntConst} : \mathrm{int}$
2. $\Gamma \vdash \mathrm{true} : \mathrm{bool}$
3. $\Gamma \vdash \mathrm{false} : \mathrm{bool}$
2. $\Gamma \vdash \mathrm{DoubleConst} : \mathrm{double}$
2. $\Gamma \vdash \mathrm{CharConst} : \mathrm{char}$
3. $\forall \tau, \Gamma \vdash \mathrm{NULL} : \mathrm{Pointer}(\tau)$
2. $\Gamma \vdash \mathrm{StringConst} : \mathrm{Pointer}(\mathrm{char})$

1. $\Gamma \vdash \mathrm{LVALUE}(l, t) \rArr \Gamma \vdash \&l : \mathrm{Pointer}(t) \land \mathrm{RVAL}(\&l, \mathrm{Pointer}(t))$
2. $\Gamma \vdash x : \mathrm{int} \rArr \Gamma \vdash +x : \mathrm{int}$
3. $\Gamma \vdash x : \mathrm{int} \rArr \Gamma \vdash -x : \mathrm{int}$
4. $\Gamma \vdash x : \mathrm{double} \rArr \Gamma \vdash +x : \mathrm{double}$
5. $\Gamma \vdash x : \mathrm{double} \rArr \Gamma \vdash -x : \mathrm{double}$
6. $\Gamma \vdash x : \mathrm{bool} \rArr \Gamma \vdash !x : \mathrm{bool}$
7. $\forall \circ \in \{+, -, *, /, \%\}, \Gamma \vdash x : \mathrm{int} \land \Gamma \vdash y : \mathrm{int} \rArr \Gamma \vdash x \circ y : \mathrm{int}$
8. $\forall \circ \in \{+, -, *, /\}, \Gamma \vdash x : \mathrm{double} \land \Gamma \vdash y : \mathrm{double} \rArr \Gamma \vdash x \circ y : \mathrm{double}$
9. $\forall t\forall \circ \in \{+, -\}, \Gamma \vdash x : \mathrm{Pointer}(t) \land \Gamma \vdash y : \mathrm{int} \rArr \Gamma \vdash x \circ y : \mathrm{Pointer}(t)$
10. $\forall t\forall \circ \in \{==, !-, >, <, >=, <=\}, \Gamma \vdash x : t \land \Gamma \vdash y : t \rArr \Gamma \vdash x \circ y : \mathrm{bool}$
11. $\forall \circ \in \{||, \&\&\}, \Gamma \vdash x : \mathrm{bool} \land \Gamma \vdash y : \mathrm{bool} \rArr \Gamma \vdash x \circ y : \mathrm{bool}$
12. $\forall p, \forall q, \Gamma \vdash x : p \land \Gamma \vdash y : q \rArr \Gamma \vdash x, y : q$
13. $\forall t, \Gamma \vdash e : \mathrm{bool} \land \Gamma \vdash x : t \land \Gamma \vdash y : t \rArr \Gamma \vdash e ? x :  y : t$
14. $\forall t, \Gamma \vdash \mathrm{LVAL}(x, t) \land \Gamma \vdash y : t \rArr \Gamma \vdash x = y : t$
15. $\forall op \in \{+, -, *, /, \%\}, \Gamma \vdash \mathrm{LVAL}(x, int) \land \Gamma \vdash y : int \rArr \Gamma \vdash x\ op= y : int$
16. $\forall op \in \{+, -, *, /\}, \Gamma \vdash \mathrm{LVAL}(x, \mathrm{double}) \land \Gamma \vdash y : \mathrm{double} \rArr \Gamma \vdash x\ op= y : \mathrm{double}$
17. $\forall op \in \{+, -\}\forall t, \Gamma \vdash \mathrm{LVAL}(x, \mathrm{Pointer}(t)) \land \Gamma \vdash y : \mathrm{int} \rArr \Gamma \vdash x\ op= y : \mathrm{Pointer}(t)$
18. $\forall op \in \{++, --\}, \Gamma \vdash \mathrm{LVAL}(x, \mathrm{int}) \rArr \Gamma \vdash x\ op: \mathrm{int} \land \Gamma \vdash op\ x: \mathrm{int}$
19. $\forall op \in \{++, --\}, \Gamma \vdash \mathrm{LVAL}(x, \mathrm{double}) \rArr \Gamma \vdash x\ op: \mathrm{double} \land \Gamma \vdash x\ op: \mathrm{double}$
20. $\forall op \in \{++, --\}\forall t, \Gamma \vdash \mathrm{LVAL}(x, \mathrm{Pointer}(t)) \rArr \Gamma \vdash x\ op: \mathrm{Pointer}(t) \land \Gamma \vdash op\ x: \mathrm{Pointer}(t)$
21. $\forall n, \forall t_n, \forall a_n, \forall q, (\Gamma \vdash f : (a_n\ t_n \rarr q), (a_i = \mathrm{byref} \rArr \Gamma \vdash \mathrm{LVAL(x_i, t_i)}), (a_i = \mathrm{bycall} \rArr \Gamma \vdash x_i : t_i) ) \rArr \Gamma \vdash f(x_1, x_2, ..., x_n) : q$

# Statements

<!-- # $\Gamma \vdash x_1 :t, x_2 : t , (f : (t, t) \rArr q) \rArr f(x_1, x_2) : q$
$t^n, q, a_i \in \{\mathrm{bycall}, \mathrm{byref}\} \rArr (a_1\ t_1,a_2\ t_2, ...,a_n\ t_n) \rArr q$

$constr(n, \{n\ elems\}, \{n\ elems\}, q) \rArr (a_1\ t_1, a_2\ t_2, ..., a_n\ t_n) \rArr q$ -->
```C
struct type {
	enum whatType { int, bool, double, char, pointer, function };
	union {
		struct int_type {};
		struct bool_type {};
		struct double_type {};
		struct char_type {};
		struct pointer_type {
			type* t;
		};
		struct function_type {
			int n;
			type* inputs[];
			enum calling_conv {byref, bycall} calls[];
			type* res;
		}
	}
};

context Γ === {(name, type)}



					Type
	   /   /    /     |       \        \
	   Int Bool Char  Double Pointer  Function
	   						 (type*)   (type* res, int n, calling_conv [], type* inputs[])

typecheck(*E) -> 

classPrefixUnAss {
	...
	...
	bool sem(){
		sem(x);

		
		if(prove(x, LVAL, int)){
			this->type = int;
			return;
		};
		if(prove(x, LVAL, double)){
			this->type = int;
			return;
		}
		if(prove(x, LVAL, Pointer*)){
			this->type = x->type;
			return;
		}

	}
	...
}

E -> ++
      |
      x

```

## Symbol Table
1. Name -> Type or FunctionDefinition(result type, parameters types, parameters calling convention)
2. Name -> Type

<!-- ```C
//catholic program (i, i) 
x > 100 αν θα τερματισει 0 < x < 100 αν δεν τερματισει.
int a[100];
a[1] > a[x]; // if 0 < x < 99
``` -->