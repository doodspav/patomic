The file repeat.h contains a massive macro.
Trying to load this macro on a modern IDE will likely freeze the system.

You can use the command `python gen.py n` to resize the macro to something
manageable by your IDE (n=2 is a good bet).

Make sure to set it back to its original value (probably 0x4000) before
compiling, since tsx will only support operations on widths up to n.