## Grammar

``` ebnf
progam : (declarations_list | functions_list)+
declarations_list: declaration ';' | declarations_list declaration ';'
declaration : type id
functions_list: function | functions_list function
function : type id '(' param_types ')' '{' declarations_list? statement_list? '}'
statement_list: statement | statement_list statement
statement: assignment ';' | return expr ';'
assignment: id '=' expr
expr: expr binop expr | expr relop expr | id | integer
binop: '+' | '-' | '*' | '/'
relop: '==' | '!=' | '<=' | '<' | '>' | '>='
```
