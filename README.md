# Interpreter
 Interpreter for a simple programming language  
 This is an simple interpreter for the language EXPR.  
 It takes .txt files as programs to be executed.  
 It works only with numbers in the range of the c++'s unsigned long int and has the following grammar:  

>**Line →** Var = Expr **|** Fun[Var] = Expr **|** print Expr **|** read Var  
>**Fun →** A **|** B **|** … **|** Z **|** AFun **|** BFun **|** … **|** ZFun  
>**Var →** a **|** b **|** … **|** z **|** aVar **|** bVar **|** … **|** zVar  
>**Num →** 0 **|** … **|** 9 **|** 1Num **|** … **|** 9Num  
>**Expr →** Expr + Term **|** Expr - Term **|** Term  
>**Term →** Term * Factor **|** Term / Factor **|** Term % Factor **|** Factor  
>**Factor →** Var **|** Num **|** (Expr) **|** Fun[Expr]  