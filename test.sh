#!/bin/sh
set -e # Exit early if any commands fail

(
  cd "$(dirname "$0")" # Ensure compile steps are run within the repository directory
  cmake -DCMAKE_BUILD_TYPE=Debug -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
  cmake --build ./build
)

(
    set +e -x
    $(dirname $0)/build/interpreter testchunk
    $(dirname $0)/build/interpreter testvm
    $(dirname $0)/build/interpreter testhash
    $(dirname $0)/build/interpreter tokenize tests/empty.lox
    $(dirname $0)/build/interpreter tokenize tests/tokens.lox
    $(dirname $0)/build/interpreter tokenize tests/numbers.lox
    $(dirname $0)/build/interpreter tokenize tests/strings.lox
    $(dirname $0)/build/interpreter tokenize tests/identifiers.lox
    $(dirname $0)/build/interpreter tokenize tests/keywords.lox
    $(dirname $0)/build/interpreter evaluate tests/value.lox
    $(dirname $0)/build/interpreter evaluate tests/expr.lox
    $(dirname $0)/build/interpreter run tests/print.lox
    $(dirname $0)/build/interpreter run tests/vars.lox
    $(dirname $0)/build/interpreter run tests/scopes.lox
    $(dirname $0)/build/interpreter run tests/if.lox
    $(dirname $0)/build/interpreter run tests/while.lox
    $(dirname $0)/build/interpreter run tests/for.lox
    $(dirname $0)/build/interpreter run tests/logical.lox
    $(dirname $0)/build/interpreter run tests/native.lox
    $(dirname $0)/build/interpreter run tests/fun.lox
    $(dirname $0)/build/interpreter run tests/closure.lox
    $(dirname $0)/build/interpreter run tests/class.lox
    $(dirname $0)/build/interpreter run tests/inheritance.lox
    $(dirname $0)/build/interpreter run tests/invoke.lox
) > tests/output.log 2>&1

diff --color=auto tests/base.log tests/output.log