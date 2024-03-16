# countdown

## Description

From the british game show "Countdown", or "Des chiffres et des lettres" in france.

This is a brute force recursive algorithm used to solve a problem involving N integers and a target number, where you're trying to find all possible arrangements of these integers with combinations of arithmetic operations (-, +, \*, /) to reach the target number or a number close to it.

It is a good example of exhaustive search and backtracking algorithms, suitable for classroom exercises. However, due to the exponential growth of the search space with the input size and the absence of specific pruning strategies to expedite the search, it is not recommended for problems involving more than 6 integers and a target. For larger problems, one may consider using the `-f` option and possibly modifying the code to employ a faster search strategy. This last resort could allow for solving larger instances within a reasonable time frame, although it may not necessarily guarantee finding the solution with the minimum number of operations.


## Algorithm

Given N integers and a target number, recursively try all arrangements of 2 to N number assorted with all possible combinations of the four operations -, +, \* and /.

```
./lceb [-f] 134 int1 ... intN
    -f: stop at the first expression that matches the target number
```

## Example:

```
$ ./lceb 2341 5 6 25 9 3 4
Obtain 2341, using 5 6 25 9 3 4
    73: ( 5 6 + 25 + 9 * 3 4 + * )
    71: ( 5 6 + 25 * 3 - 4 - 9 * )
    26: ( 5 6 + 25 * 3 4 * - 9 * )
     2: ( 5 6 + 25 9 * 3 4 * - * )
     1: ( 5 6 * 25 4 + 3 * 9 - * )
     0: ( 5 6 * 3 * 4 + 25 * 9 - )
```


Note that it returns the expression in reverse polish (postfix) notation[1](https://en.wikipedia.org/wiki/Reverse_Polish_notation). Converting the last expression to the common infix notation gives `2341 = 5 * 6 * 3 * 4 - 25 * 9`.

The first column is the absolute error. It is non-zero if the target cannot be found using the provided numbers. For instance, substituting _2346_ for the target in the previous example will find `2: ( 5 9 + 6 * 25 3 + * 4 - )` or _2348_ (error 2).

## Build

You need a c++20 compatible compiler, or c++17 if you modify the code to remove the std::format.

`git clone` this repos and `make` will generate a `./lceb` executable.

