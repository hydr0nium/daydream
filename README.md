# daydream
Daydream is a dynamically typed scripting language. It was created just for fun. For now its not optimized at all so don't tell me the code is bad. I know that. If you want to toy around with it you are welcome to do so. If you spot any errors or issues please let me know.

# Intro to daydream

## Statements

A hello world programm is really simple, because in daydream printing is equivalent to just writing a line with a statement in it. A statement in daydream is every combination of arithmetic, logical and functional terms. Lets start with a hello world programm:

```
'hello world'
```
This will print `hello world`.

```
5
```
This will print `5`.

```
40+2
```
This will print `42`.

## Variables 

Variables in daydream are dynamically typed. This means the interpreter tries to determine the types of the variables while evaluating. Lets see how this works:

```
string = 'hello world'
```
This defined the variable `string` with the value of `hello world`.

Let's try to print the variable with the stuff we learned before:

```
string = "Hello world"
string
```
This prints `hello world`.

## If Expressions

An if is similar to python. Let's take a look at it:

```
if 5==5:
  'hello'
end
```
This will print `hello`

As you can see if expressions need to be ended with the `end`keyword. This was a deliberate choice such that daydream is free to use any indentation.

So this:
```
if 5==5:
'hello'
end
```
is the same as above.

## For Expressions

Similar to an if expression we can define a for expression:

```
for (i = 0; i < 10; i = i+1):
  i
end
```
This will print `0123456789`
Note that daydream current does not support the `++` or `--` operator.

## While Expressions

Again similar to the for we can define a while expression:

```
a = 0
while (a==0):
  'hello world'
  a = a + 1
end
```
This will print `hello world`

## Single Line Expression

It is possible to do expressions like if, while and for in a single line:

```
while true: "hello"
for (i = 0; i<10;i = i+1): i
if (true): "is_true"
```

## Function Definitions

To define a function we will use the def keyword as follows:

```
def print_var(var):
  var
end
```

In daydream functions are defined 1. by their name, 2. by their number of arguments. This means

```
def test(a):
end

def test(a, b):
end
```
are two different functions.

## Break and Return

The `break` and `return` keywords work the same as in any other programming language. If you put `break` in a while or for loop it will break out of it. If you use `return` you will return the statement that is followed after that. Let's see some examples:

```
i == 0
while(true):
  i = i+1
  if (i==5):
    break
  end
end
```
This will return after i reached the value of 5.

```
def add_five(number):
  return number+5
end
add_five(4)
```
This will print `9`.

## "Advanced Features"

If you use the `.` between an statement and a function then this will be treated as a type method. This means the expression to the left of the dot will be evaluated and then passed as the first parameter to a special type mangled method:

```
def __string__print(this): this + "\n"

my_string = "hello world"
my_string.print()
 
```

This will print `hello world`. Note that the method that is called is mangled with the type. So my_string will be evaluated as a string and hence the print method needs to be mangled with `__string__`. Similarly you can mangle ints with `__int__` and bools with `__bool__`. Also note that the first parameter as stated before is the expression that called the function. The name of this variable does not matter. Here is another example:

```
def __bool__is_true(some_var):
  if some_var: "This is true\n"
  if not some_var: "This is false\n"
end
true.is_true()
```
This will output `This is true`.

## Other Stuff

- If you add two strings they get concatenated.
- If you add a int and a string you concat the two.
- You can use a logical and with `and` and a logical or with `or`
- The bools are `true` and `false`
- Variables inside a function definition do not leak to the outside and are bound to the function
- You can use the ** for as a power operator.
- The keyword `debug` can be used to print the variables that are currently in scope at that point
- `not` is the logical negation

## Some Caution

Currently daydream does not support floats.

# TODO

- Implement Library Support
- Implement Syscall Support
- Implement Libraries
- Implement Floats
- Implement Lists / Arrays
