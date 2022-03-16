# matplotlib-cpp Style Guide
## Indentations and spacing
Indents should consist of four (4) spaces (taken from the original spacing of `matplotlib-cpp`).

## Nested preprocessor statements
matplotlib-cpp makes extensive use of preprocessor statements.

Nested preprocessor statements should be declared at the beginning of a new line, but the contents should be indented for enhanced readability.
```
#ifndef WITHOUT_NUMPY
#    if PY_MAJOR_VERSION > 3
#        define PyString_FromString PyUnicode_FromString
#    endif
#endif
```
There should be a full indentation (4 spaces) from the `#`.

## Opening Braces for all blocks
Opening curly braces are on their own newlines.
```
int main(int argc, char** argv)
{
    return 0;
}
```

## Control statements
There should be a space between the reserved keyword and the opening parenthesis.

```
for(...) // No

for (...) // Yes
```

### `if` statements containing one statement
`if` statements with only one statement can be written on one line, as long as it
doesn't pass 80 characters.
They should not have curly braces.
```
if (x == true) return 1; // Ok

if (x == true) { return 1; } // No

if (!PyObject_IsTrue(fig_exists)) { fig = PyObject_CallObject(detail::_interpreter::get().s_python_function_figure, detail::_interpreter::get().s_python_empty_tuple) } // statement should be on a newline, and do not require curly braces
```

`if-else` statements should be split up.
```
if (x == true) return 1; else return 0; // No

if (x == true) return 1; // Ok
else return 0;
```

### `for` loops with only one statement
Single-statement `for` should not have curly braces.
```
for (int i=0; i<max; ++i)
    count++;
```

## Functions
### Declarations over 80 characters
Function declarations that pass the 80 character limit should place offending parameters
on newlines.
They should be lined up with the first character of the preceeding parameter.
A comma should be on the previous line.

### Address-of operator
`&` should be included in the same string as the type of the parameter.
```
bool boxplot(const std::vector<Numeric>& data,
             const std::map<std::string, std::string>& keywords = {})
{
    ...
}
```

## Classes
### Access labels
Access labels should be on the same identation as the opening brace.
```
class Plot
{
public:
    Plot();
private:
    ...
}
```

## History
As of the fork, `matplotlibcpp`'s style is inconsistent.
A function's opening brace was on a newline, but an if clause had its opening brace on the same line.
The latter wasn't always held up either.
Nested preprocessor statements were dense and nearly unreadable, especially if they contained functions, variables, etc.
The detected spacing was not always respected.

Being a header-only library, style is incredibly important to retain sanity during development.
The style choices were taken directly from existing code.