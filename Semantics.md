# Semantics

## Types
### Basic Types
- int (2 bytes)
- char (1 byte)
- double (10 bytes)
- bool (1 byte)
- void
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
22. $\forall t, \Gamma \vdash e : \mathrm{int} \rArr \Gamma \vdash \mathrm{new}\ t[e] : \mathrm{Pointer}(t)$
23. $\forall t, \Gamma \vdash \mathrm{new}\ t : \mathrm{Pointer}(t)$
24. $\forall t, \Gamma \vdash e : \mathrm{Pointer}(t) \rArr \Gamma \vdash \mathrm{delete}\ e : \mathrm{Pointer}(t)$

# Statement Restrictions
1. $\Gamma \vdash e : \mathrm{bool}$, $\mathrm{if}\ e\ s_1 \mathrm{else}\ s_2$  is ok
2. $for(e_1; e_2, e_3)\ s$, $\Gamma \vdash e_2 : \mathrm{bool}$
   Αν η $e_2$ δεν δίνεται είναι true.
3. Άτυπα το $\mathrm{label}$ θα χρησιμοποιηθεί ως type. Στην ιεραρχία κανονικά. 
   1. Στα forloops κατά την σημασιολογική ανάλυση μπαίνει στο symbol table ως active label. Γίνεται η σημασιολογική ανάλυση του εσωτερικού της loop.
   Όταν τελειώσει η loop το κάνεις inactive στο symbol table (Αυτό γίνεται διότι πρέπει να είναι μοναδικό το όνοα του label στο σώμα μίας συνάρτησης αλλά μπορεί να χρησιμοποιηθεί μόνο εντός της loop στην οποία δηλώθηκε)
   2. Στα break && continue που έχουν label πρέπει να ελεγχθεί αν είναι active το label για να είναι valid. Αλλιώς ορίστηκε σε άλλη λοοπ άκυρη και είναι σημασιολογικό λάθος.
4. $\mathrm{return}\ e;$ Βρες τρέχουσα συνάρτηση. Βρες return_type. Αν είναι void 
   πρέπει η $e$ να παραλείπεται. Αν δεν είναι void πρέπει $e : \mathrm{return\_type}$ 
5. name -> symbol_entry (type, frame offset)
---

# Notes
- Δεν υπάρχουν εκφράσεις που να μπορούν να φτιάξουν void* 
  άρα δεν παίζει σαν type το void*

# Translating to LLVM

## Types $\Longrightarrow$ LLVM Types

| Edsger     | LLVM                  |
|------------|-----------------------|
| int        | i16                   |
| char       | i8                    |
| bool       | i8                    |
| double     | x86_fp80              |
| t*         | t*                    |
| t* (array) | [size * element type] |

## Global Variables
Ta global variables μπορεί να είναι όλοι οι τύποι εκτός από void
γίνονται define αυτούσιοι. Θεωρούνται αυτόματα pointers..
> @var = global type

## Local Variables
> %var = alloca type ; this puts it in stack
  
Άμεσο για int, bool, char, double, t*.
Για σκέτο pointer κάνεις
> %ptrTothatPtr = alloca type
Για array κάνεις
> %ptrtoArray = alloca insideType, insideType size 

ΚΑΙ ΑΥΤΟ ΘΕΩΡΕΙΤΑΙ POINTER ΘΕΛΕΙ DEREFERENCE
(ΦΑΝΤΑΖΟΜΑΙ ΠΩΣ ΣΕ ΤΙ ΑΝΤΙΣΤΟΙΧΟΥΝ ΘΑ ΠΡΕΠΕΙ ΝΑ ΕΙΝΑΙ ΑΠΟΘΗΚΕΥΜΕΝΟ
ΣΤΟ SYMBOL TABLE ΩΣΤΕ ΝΑ ΜΠΟΡΕΙΣ ΑΜΕΣΑ ΝΑ ΤΟ ΚΑΝΕΙΣ DEREFERENCE)
Άρα με το initialization τρέχεις το alloca και βάζεις και στο symbol table που βρίσκεται αυτό. 
## CONSTANTS
Αυτά μπορούν να εμφανιστούν μόνο σε expressions 
Ολα είναι άμεσες σταθερές που παιρνιούνται ως έχουν στο evaluation του παρακάτω. Μόνο τα strings έχουν διαφορά. Πρέπει να κάνεις initialize array στην τιμή και να επιστρέψεις char*. Αυτά τα κάνεις define σαν global variables me private linkage. Και άρα χρησιμοποιείς τον pointer απευθείας και γίνονται initialize απευθείας

Πχ.
>@.str = private unnamed_addr **constant** [13 x i8] c"Hello World!\00", align 1

Βάζεις μόνος σου το 0 στο τέλος. Δεν σε νοιάζει ιδιαίτερα το visibility αφού από την σημασιολογική ανάλυση δεν πρόκειται να το πειράξει κάποιος άλλος.
## CASTS
Καταρχάς πρέπει να αποφασίσουμε τι casts επιτρέπονται.

Participating to a cast : [int, char, double, bool, pointer]
| From    | To      | Type of Cast                   |
|---------|---------|--------------------------------|
| int     | bool    | truncating unsigned downcast   |
| int     | char    | truncating signed downcast     |
| int     | double  | sitofp instruction in llvm                            |
| int     | pointer | bitwise cast (llvm has inttoptr instruction)                  |
| bool    | int     | unsigned upcast (zero-extend)  |
| bool    | char    | unsigned upcast (zero-extend)  |
| bool    | double  | uitofp instruction llvm                            |
| bool    | pointer | unsigned upcast (zero-extend)  |
| char    | int     | signed upcast (sign-extend)    |
| char    | bool    | unsigned downcast (truncating) |
| char    | double  | sitofp instruction llvm                            |
| char    | pointer | unsigned upcast (zero-extend)  |
| double  | int     | fptosi .. to llvm instruction                            |
| double  | bool    | fptoui .. to llvm instruction                            |
| double  | char    | fptosi .. to llvm instruction                            |
| double  | pointer | (can do to int and then to ptr)	                            |
| pointer | int     | bitwise cast (llvm has inttoptr instruction)                   |
| pointer | bool    | unsigned downcast (truncating) |
| pointer | char    | unsigned downcast (truncating) |
| pointer | double  | can do ptrtoint and then uitofp                             |

## Function Prototypes
declare type @FunctionName (parameter list)

## FunctionDefinitions
define [linkage] type @FunctionName (parameter list) {
	...
}


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


```C

void f(){
	int x;
	void g(){
		int y;
		void h(){ 
			int z;
			y = (((x + z) + y) + 10);
			z = x;
		}



		y = h(); (SingleExpression(BinaryAss(ASS, Id(y), IntConst(10))) (scope* -> g)
	}
}

void fun(){

}
```


```C
int f(int x, int y){
	int z, w;
	
	
	z = x;
	w = x + y;
	return z + w;
}
```

w := x + y;
ret := x + w;


GlobalContexts -> Tree [(name, type)]
Context -> [(name, type)]

parameters
---
result type
---
static link
---
return address (possibly can do)
---
locals
---
temps


[Scope tree]		-> [RunTime-Info Tree]
[global]				
 |    \				-> 									+ intermediate code for f -> [f] : return_address (...)  + intermediate_code -> code
[f]   [fun]				[f] : return address (...)									     : locals										 
 |							: locals													 : declarations
[g]						    : declarations												 : parameters
 |							: parameters												 : temporary results
[h]						    					
	