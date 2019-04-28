#!/bin/bash

find . \( -name "*.h" -or -name "*.cpp" -or -name "*.inl" \) | xargs clang-format -i

