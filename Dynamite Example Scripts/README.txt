This is a demo of Dynamite 0.1.6

This build uses the first version of the Dynamite Virtual machine written in Golang.

Dynamite 0.2.0 and later use a C++ virtual machine. The virtual machine code
for the C++ machine is mostly backwards compatible with the machine code for this build.

However, the libraries and extensions written in Golang that give this build
- Random number generation
- Multithreading with channels
- UDP server client chat capabilities
- Email sending capabilities
- File IO
- Keyboard IO with a timeout
- Webview
Are not available on the C++ virtual machine

The C++ virtual machine is smaller, faster, and has more builtin types
and better error handling and more descriptive error messages. It is also
easier to maintain.

If you're curious, you can look at the code for C++ virtual machine, but I havent
built a compiler for Dynamite 0.2.0 yet, so you won't be able to run any of the
example Dynamite scripts.