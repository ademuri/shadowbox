#!/bin/bash

# Formats all of the source files using clang-format.
clang-format -i $(find src -name *.[ch]pp)
clang-format -i $(find test -name *.[ch]pp)
