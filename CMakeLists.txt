cmake_minimum_required (VERSION 3.0)

project(quixey)


set(quixey_SOURCES
    array_variable.cpp 
    array_variable.h 
    char_variable.cpp 
    char_variable.h 
    error.h 
    function.cpp 
    function.h 
    function_variable.cpp 
    function_variable.h 
    int_variable.cpp 
    int_variable.h 
    library_adaptor.h 
    main.cpp
    quixey.cpp 
    quixey.h 
    runtime_error.h 
    script_types.h 
    string_util.h 
    string_variable.cpp 
    string_variable.h 
    syntax_error.h 
    token.cpp 
    token.h 
    variable_base.h 
    variable.cpp 
    variable.h 
)

add_executable(quixey ${quixey_SOURCES})

set_property(TARGET quixey PROPERTY CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -W -Wall -Wmissing-field-initializers -Wunused -Wshadow")
