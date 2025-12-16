program -> defines
         | function
         | **ε**

stmts -> stmt **;** stmts
       | **ε**

stmt -> block
      | **if (** expr **)** stmt
      | **else** stmt
      | **while (** expr **)** stmt
      | **do** stmt **while (** expr **)**
      | **for (** define **;** relation **;** expr **)** stmt
      | **return** expr **;**
      | **id (** parameters **) ;** *// function call*
      | define **;**

block -> **{** stmts **}**

expr -> ... ***Handle with Pratt Parser

relation -> ... ***Handle with Pratt Parser

function -> type **id (** defines **)** block

type -> **type** pl

pl -> * pl
    | **ε**

defines -> define **;** defines
       | **ε**

define -> type **id** assign

assign -> **=** expr
       | **ε**
