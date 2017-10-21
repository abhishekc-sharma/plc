# PLC

An interpreter for a Pure Lambda Calculus implemented in C

## Grammar

```
term ::=
  var           Variable Access
  (term term)   Application
  \var.term     Abstraction

var ::=
  (a-z|A-Z|0-9)+
```

## Compilation

```
make
```

## Running

```
./bin/plc # Start REPL

./bin/plc <.plc file> # Interpret File
```
