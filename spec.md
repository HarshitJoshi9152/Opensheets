# Sheets Specification 0.1


> this is a document describing the sheets file format specification
> for the [Opensheets](https://github.com/HarshitJoshi9152/Opensheets) parser.

1. The file extension does not matter, the data does.
2. The functions will be like SUM(), SUB(), MUL(), DIV() and 
   will use whitespace as delimiter for arguments.

## Functions Implemented

1. `SUM`: takes a list of numbers and adds them together.
2. `MUL`: takes a list of numbers and multiplies them together.
3. `SUB`: takes a list of numbers and subtracts the rest from the first.
4. `DIV`: takes a list of numbers and recursively divides the first number by the rest.
5. `DATE`: takes no arguments and returns the Date in localtime with format `day month date hh:mm:ss yyy`
