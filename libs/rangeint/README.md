 Defines to use a ranged type, that implements overflow arithmetic. To use, define 3 macros:
 - `RI_NAME`  - the name of the type, and the prefix for all functions
 - `RI_MAX`   - maximum value of the type, inclusive
 - `RI_MIN`   - minimum value of the type, inclusive

Then include the header. This header will:
- choose the appriopriate type that can represent values between `RI_MAX` and `RI_MIN`
- define a function `<RI_NAME>_from` for converting value to be in range
- define `<RI_NAME>_from_o` that does the same but also has an overflow flag.
- define a bunch of function on operations, like `<RI_NAME>_add` , that implement overflow arithmetic
- `#undef` `RI_NAME`, `RI_MAX` and `RI_MIN`

For examples see tests.
