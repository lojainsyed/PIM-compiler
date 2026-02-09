
# Scanner part 1 - Lexical Specification


## Input language: 
Specify the input language of your compiler here.

## Token Categories/Types:

List ALL the tokens that your language will use. For example:


### Keywords: 

These are reserved words in your language 

- while
- class
- true

### Identifiers: 
Names of variables, functions, and classes. Identifiers are arbitrary sequences of letters, digits, and the underscore character (_). They cannot contain spaces and must not begin with a digit.


### Numbers: could be formatted as integers, or floating point values, or fractions, or in alternate bases such as binary, octal or hexadecimal.

These may be formatted as integers, floating-point values, fractions, or numbers in alternate bases such as binary, octal, or hexadecimal.

**Note:** 
Read Section 3.1 for more information on tokens. Do not forget to include mathematical and logical operators, special symbols, punctuation, comments, and whitespace.


## Formal Specification:

For each token category, write a regular expression (using the basic regular expressions learned in class) that precisely defines how the token is formed from the input character stream.

For example:


### Keywords:

```
(while)|(class)|(true)
```

## Examples: 


Include clear, runnable examples for each token type, showing how they appear in source code. For example:


### Keywords: 

```
while(true){
	break;
}
```

### Identifiers: 

```
class X{
	int y;
};
```

### Numbers: 

```
int z = 0b0101;
```



Your examples should be comprehensive —not as incomplete as the short list shown here.

