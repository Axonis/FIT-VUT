project(IFJ)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -std=c99 -DDEBUG=0 ")

set(SOURCE_FILES instruction_list.c instruction_list.h instructions.h interpret.c interpret.h
        stack.c stack.h)

set(str_test str.c str.h str_test.c ial.c error.c)


set(scanner scanner.c scanner-unit.c str.c)
set(interpret-expr-test interpret.c interpret-expr-test.c str.c error.c instruction_list.c stack.c ial.c)
set(stable_test stable-test.c ial.c stack.h str.c error.c)
set(interpret-str-test interpret-str-test.c str.c interpret.c stack.c error.c instruction_list.c instructions.h ial.c)
set(interpre-rel-test interpret.c interpret-relation-test.c str.c error.c instruction_list.c stack.c ial.c)
set(op_parser op-parser.c dynamic_stack.c error.c ial.c scanner.c str.c
        interpret.c instruction_list.c stack.c)
set(parser str.c ial.c scanner.c parser.c error.c instruction_list.c interpret.c stack.c op-parser.c
        dynamic_stack.c)
set(fib interpret-fib.c interpret.c stack.c ial.c instruction_list.c instructions.h ial.c str.c error.c)

add_executable(main ${SOURCE_FILES})

add_executable(op_parser ${op_parser})
add_executable(str_test ${str_test})
add_executable(scanner ${scanner})
add_executable(interpret-expr-test ${interpret-expr-test})
add_executable(stable-test ${stable_test})
add_executable(parser ${parser})
add_executable(interpret-str-test ${interpret-str-test})
add_executable(interpret-rel-test ${interpre-rel-test})
add_executable(fib ${fib})
