# DESIGN.md

## 1.  What grammar rules did you define for the language?
$$
\begin{align}
program &\rightarrow decl\_list \ \texttt{END\_OF\_FILE} \\
decl\_list &\rightarrow decl\_list \ decl \\
decl\_list &\rightarrow decl \\
decl &\rightarrow func\_decl \\
decl &\rightarrow var\_decl \\
\\
func\_decl &\rightarrow \texttt{FUNC\_KEYWORD} \ \texttt{IDENTIFIER} \ \texttt{(} \ opt\_param\_list \ \texttt{)} \ \texttt{:} \ type \ block \\
var\_decl &\rightarrow \texttt{VAR\_KEYWORD} \ \texttt{IDENTIFIER} \ \texttt{:} \ type \ \texttt{=} \ expr \ \texttt{;} \\
var\_decl &\rightarrow \texttt{LET\_KEYWORD} \ \texttt{IDENTIFIER} \ \texttt{:} \ type \ \texttt{=} \ expr \ \texttt{;}
\\
type &\rightarrow \texttt{INT\_KEYWORD} \\
type &\rightarrow \texttt{FLOAT\_KEYWORD} \\
type &\rightarrow \texttt{BOOL\_KEYWORD} \\
\\
block &\rightarrow \texttt{\{} \ code\_item\_list \ \texttt{\}} \\
block &\rightarrow \texttt{\{} \ \texttt{\}} \\
code\_item\_list &\rightarrow code\_item\_list \ code\_item \\
code\_item\_list &\rightarrow code\_item \\
code\_item &\rightarrow stmt \\
code\_item &\rightarrow decl \\
stmt &\rightarrow assignment\_stmt \ | \ if\_stmt \ | \ while\_stmt \ | \ print\_stmt \ | \ return\_stmt \\
assignment\_stmt &\rightarrow \texttt{IDENTIFIER} \ \texttt{=} \ expr \ \texttt{;} \\
if\_stmt &\rightarrow \texttt{IF\_KEYWORD} \ \texttt{(} \ expr \ \texttt{)} \ block \\
if\_stmt &\rightarrow \texttt{IF\_KEYWORD} \ \texttt{(} \ expr \ \texttt{)} \ block \ \texttt{ELSE\_KEYWORD} \ block \\
while\_stmt &\rightarrow \texttt{WHILE\_KEYWORD} \ \texttt{(} \ expr \ \texttt{)} \ block \\
print\_stmt &\rightarrow \texttt{PRINT\_KEYWORD} \ \texttt{(} \ expr \ \texttt{)} \ \texttt{;} \\
return\_stmt &\rightarrow \texttt{RETURN\_KEYWORD} \ expr \ \texttt{;}\\
\\
expr &\rightarrow literal \\
expr &\rightarrow \texttt{IDENTIFIER} \\
expr &\rightarrow func\_call \\
expr &\rightarrow \texttt{(} \ expr \ \texttt{)} \\
expr &\rightarrow \texttt{-} \ expr \ \text{(unary minus)} \\
expr &\rightarrow expr \ \texttt{*} \ expr \\
expr &\rightarrow expr \ \texttt{/} \ expr \\
expr &\rightarrow expr \ \texttt{+} \ expr \\
expr &\rightarrow expr \ \texttt{-} \ expr \\
expr &\rightarrow expr \ \texttt{<} \ expr \\
expr &\rightarrow expr \ \texttt{>} \ expr \\
expr &\rightarrow expr \ \texttt{<=} \ expr \\
expr &\rightarrow expr \ \texttt{>=} \ expr \\
expr &\rightarrow expr \ \texttt{==} \ expr \\
expr &\rightarrow expr \ \texttt{!=} \ expr \\
literal &\rightarrow \texttt{INTEGER\_LITERAL} \ | \ \texttt{FLOAT\_LITERAL} \ | \ \texttt{BOOL\_LITERAL} \\
func\_call &\rightarrow \texttt{IDENTIFIER} \ \texttt{(} \ opt\_arg\_list \ \texttt{)}\\
\\
opt\_param\_list &\rightarrow param\_list \\
opt\_param\_list &\rightarrow \epsilon \\
param\_list &\rightarrow param\_list \ \texttt{,} \ param \\
param\_list &\rightarrow param \\
param &\rightarrow \texttt{IDENTIFIER} \ \texttt{:} \ type \\
\\
opt\_arg\_list &\rightarrow arg\_list \\
opt\_arg\_list &\rightarrow \epsilon \\
arg\_list &\rightarrow arg\_list \ \texttt{,} \ expr \\
arg\_list &\rightarrow expr

\end{align}
$$

## 2.  What types of AST nodes did you design for the language? What are the members of these AST nodes? What is each AST node supposed to track?


## 3.  The challenges you faced while designing or coding the parser.


## 4.  Reflections on flex vs the hand-written scanner in assignment 1.


## 5.  What did you learn during this assignment?


## 6.  Provide at least three distinct sample test cases you created in addition to the provided samples. These do not have to be semantically correct, but should go beyond the examples in the assignment.