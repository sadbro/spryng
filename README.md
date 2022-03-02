
# SPRYNG

A python-like implementation for strings(in this case, char array) in C.
the functions are named and implemented according to python3 convention.

## How To

Include the header file "spryng.h" in your file with it's path.

## Examples

```
count("123-456-789", '-') will return (int) 2.
split("123-456-789", '-') will return (char**) {"123", "456", "789"}.
join({"123", "456", "789"}, 3, '-') will return "123-456-789".
```
