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

The AST nodes for this language generally match up with the non-terminals for this language, and their members / what they track are the non-resolved terminals and non-terminals on the RHS of their production(s).

For example:

The $func\_decl$ non-terminal corresponds to the `FuncDeclNode` AST node. This AST node has members `id`, `params`, `returnType`, and `body`. These members correspond wiht the non-resolved components of the RHS of the $func\_decl$ production - `IDENTIFIER`, `opt_param_list`, `type`, and `block`. `FUNC_KEYWORD`, `LPAREN_DELIMITER`, `RPAREN_DELIMITER`, and `COLON_DELIMITER` already have a single set token they can be, so they don't need to be tracked.

However, there are some non-terminals that don't have a corresponding AST node. For the list non-terminals - `decl_list`, `code_item_list`, `param_list`, `arg_list` - their productions just use left-recursion to create a list. Since we create a vector for these lists, we just need to assign a member of their parent node. For example, the vector for `decl_list` is added to the member `declarations` in `ProgramNode`. Another example: the vector for `code_item_list` is added to the `statements` member in `BlockNode`.

Other non-terminals that don't have a corresponding AST node include:
- Optional Non-Terminals: `opt_param_list`, `opt_arg_list`
    - The use of a vector captures the optional nature of these lists already. If the vectors are empty they represent the empty "optional" state of the param_list.
- Grouping Non-Terminals: `stmt`, `decl`, `code_item`
    - Inheritance already takes care of the union types in these grammar rules. `decl` can be a `func_decl` or a `var_decl`. We don't need a corresponding AST node because when defining the `FuncDeclNode` and `VarDeclNode`, we can inherit from `DeclNode` which captures the information we need.

## 3.  The challenges you faced while designing or coding the parser.
The biggest hurdle was getting started. Learning how yacc/bison worked was a significant hurdle - it was essentially a new language, and running into error messages that were tough to interpret didn't help. Plus, this language had so many productions - I counted over 20 - so it took a significant amount of time just to get the basic test case working. This means there were a lot of places to go wrong. I'm on a WSL machine, so I had to download `flex` and `bison` before anything worked. This didn't take that long to figure out, but it all adds up. 

To add to the learning curve, flex was also new to me. So this took reading and rereading how flex and bison worked before getting started. This took several hours. When I did eventually get to writing code, I would run into a bunch of syntax errors, and I had to utilize Google and the docs a lot to debug what was going on. After things mostly worked, I had to figure out how `yylloc` and `%destructor` and `%precedence` worked.

Actually designing the language for the parser was straightforward. This type of task has been done many times, in homeworks and lecture and it was what we had learned in class.

I'd say 99% of the difficulty / challenges of this project were figuring out the syntax, and how `flex` / `bison` / the C++ files interacted with each other.

## 4.  Reflections on flex vs the hand-written scanner in assignment 1.


## 5.  What did you learn during this assignment?


## 6.  Provide at least three distinct sample test cases you created in addition to the provided samples. These do not have to be semantically correct, but should go beyond the examples in the assignment.