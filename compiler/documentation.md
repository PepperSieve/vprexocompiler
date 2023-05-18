# Documentation for `vpex` files:
A `vpex` file must provide everything required for the tool to generate an implementation, a specification, and an efficient checker for a computation. It also contains necessary assertions and loop invariants to conduct refinement proofs between these different versions of the computation, as well as code that generates random inputs for the computation for testing.

## Basic Structure
A `vpex` file can be divided into 11 sections, each must be present (can be empty) and must be listed as the exact order as below:

### Usize Flag:
An option line `UFLAG on` at the beginning of the file. If the line is added, we treat all variable as unsigned (`uint32_t`).

### Macro:
Begins with a single line: `PREAMBLE`. Each non-empty line in this section must be of the C macro format:
```
#define <SYM> <VAL>
```
where `<SYM>` is the name of a constant and `<VAL>` is an integer value.

All non-empty lines are copied verbatim to all C files, and all `<SYM>` are converted to integer input variables of the main method in all Viper files.

### Ti &rarr; Te Proof Prefix:
Begins with a single line: `PRE-TiTe`. Every non-empty line in this section is copied verbatim to beginning of the generated Ti &rarr; Te Viper file.

### Te &rarr; Ts Proof Prefix:
Begins with a single line: `PRE-TeTs`. Every non-empty line in this section is copied verbatim to beginning of the generated Te &rarr; Ts Viper file.

### C Helper Functions:
Begins with a single line: `HELPER`. Every non-empty line in this section is copied verbatim to beginning of all generated C files.


### Input Struct:
Begins with a single line: `IN`. Each non-empty line must be one of the three formats:
```
int <SYM>
arr <SYM> <VAL>
mat <SYM> <VAL> <VAL>
```
where `<SYM>` is the name of a variable and `<VAL>` is either an integer value or a constant variable defined in Macro section.
* `int x` refers to an integer variable `x` in `Struct In` of C files and in parameters of the main method in Viper files
* `arr x, n` refers to an array `x` of size `n` in `Struct In` and method parameter
* `mat x, n, m` refers to a 2-D array `x` of size `n` by `m` in `Struct In` and method parameter

### Output Struct:
Begins with a single line: `OUT`. Each non-empty line must also be one of the three formats described in the section above. Variables listed here appear in `Struct Out` of C files and **do not** appear directly in Viper files. However, we require that for any variable listed in this section, **a local variable with an identical name** must be defined in the main function / method.

### Ti &rarr; Te Proof Requirement:
Begins with a single line: `REQ-TiTe`. Every non-empty line in this section is copied verbatim to the beginning of the main method of the generated Ti &rarr; Te Viper File. Each line should be of the form `requires <EXPRESSION>`.

### Te &rarr; Ts Proof Requirement:
Begins with a single line: `REQ-TeTs`. Every non-empty line in this section is copied verbatim to the beginning of the main method of the generated Te &rarr; Ts Viper File. Syntax is the same as in Ti &rarr; Te proof requirement.

### Ti code:
Begins with a single line: `TI`. Content of this section will appear in generated `ti` C file, `exo` file, and front of Ti &rarr; Te Viper file.

Syntax of this section largely follows Syntax & Tokens section, with two special remarks:
* Ghost variables are allowed.
* Assertions are **not** allowed.

### Te code:
Begins with a single line: `TE`. Content of this section will appear in generated `te` C file, end of Ti &rarr; Te Viper file, and front of Te &rarr; Ts Viper file.

Syntax of this section largely follows Syntax & Tokens section, with two special remarks:
* Ghost variables are **not** allowed.
* Assertions are allowed (and likely required).

### Ts code:
Begins with a single line: `TS`. Content of this section will appear in generated `ts` C file and end of Te &rarr; Ts Viper file.

Syntax of this section largely follows Syntax & Tokens section, with two special remarks:
* Ghost variables are **not** allowed.
* Assertions are allowed (and likely required).

### INP
Begins with a single line: `INP`. Content of this section will appear in generated `input_generation` header files and are used to generate random inputs in Pequin for verification.

Default syntax in this section is:
```
    for (int i = 0; i < num_inputs; i++) {
        mpq_set_ui(input_q[i], rand(), 1);
    }
```

### END
`vpex` files terminates with a single line: `END`.

## Line Prefixes
In all of Ti, Exo, Te, and Ts sections, we can add prefixes to lines so that they only appear in certain locations. The prefixes are:
* `%`: The line only appears in relevant Viper files.
* `%c`: The line only appears in relevant C files, including the `exo` file.
* `%exo`: The line only appears in `exo` C file and relevent Viper files.
* `%vie`: The line only appears in Ti &rarr; Te Viper file.
* `%ves`: The line only appears in Te &rarr; Ts Viper file.

Note that adding `%ves` in Ti section would cause the compiler to skip the line. Since contents in Ti section do not appear in Te &rarr; Ts Viper file. Simlarly, `%vie` should not appear in Ts section and `%exo` should not appear in anywhere but Ti section.

## Syntax & Tokens
Syntax of all of Ti, Exo, Te, and Ts sections are based on conventional C format, with the following changes:
* No semicolons at the end of line. No two statements on the same line.

* No brackets (`{`, `}`) for scoping. 

* If / else statements are of the form:
```
if <CONDITION>
    <BODY>
end
```
Note that brackets are not required around the condition.

* For loops are of the form:
```
loop <ITERATOR> <TERMINATOR>
    <BODY>
endloop
```
where `<ITERATOR>` must be a **unique** identifier that has not been defined in the current function, and `<TERMINATOR>` is either an integer value, a constant variable defined in macro section, or a computation that can be statically evaluated **with
no spaces in between**. Loop count will always start from 0.

* While loops are of the form:
```
while <CONDITION>
    <BODY>
end
```
Any while loop should only appear in Ti section and should be proceeded by a **loop bound** statement of the form
```
buffet <BOUND>
```
where `<BOUND>` is pre-determined value (similar to `<TERMINATOR>` in for loop) that states the maximum number of iterations allowed for the while loop.

The only exception is when the while loop appears in a nested loop. In this case, a loop bound statement should appear right
before the outermost loop statement (regardless of while or for loop).

* Variables in `Struct In` and `Struct Out` should be referred as `input->X` and `output->X` respectively, with no spaces in between.

* Array definition is of the form `arr <SYM> <VAL>`, and are accessed using the expression `slot( <SYM>, <VAL> ).val`. Note that a space should always present between elements, and between elements and the brackets, and there needs to be **NO** spaces within an element or between elements and `,` (i.e. `slot( A , k - 1 ).val` is not allowed).

* Matrix definition is of the form `mat <SYM> <VAL> <VAL>`, and are accessed using the expression `mat_slot( <SYM>, <VAL>, <VAL> ).val`. Note that like array accesses, elements and brackets should be separated by a space, and **NO** spaces within an element or between elements and `,`.

* Ghost variables are defined of the form:
```
ghostInt <SYM>
ghostArr <SYM> <VAL>
ghostMat <SYM> <VAL> <VAL>
```
They can only be defined in Ti section, and behave similarly to regular variables, except that they are outputs of exo compute in `te` C file, and their values can be accessed in the `te` section of Ti &rarr; Te Viper file.

* Assertions are of the form `assert_zero <EXPR>`. Note that for the assertion to pass, `<EXPR>` should either be evaluated to 0 or `false`, which means that they are the negation of whatever we are trying to assert.

## Other Rules & Known Bugs
* The right shift operator `>>` must have a space between it and the proceding value / variable. i.e. `>>1` is not allowed, need to use `>> 1`.
* Skipping a line after `assert_zero` seems to result in segmentation fault.