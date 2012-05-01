cmake_minimum_required(VERSION 2.4)
set(LISP_SRC
		tokenize.c
		print_node.c
		free_node.c
		copy_node.c
		Calc_and_Comp.c
		eval.c
		hash.c
		vm_codegen.c
		vm_run.c
		start.c
		main.c
   )

include_directories("${PROJECT_SOURCE_DIR}")

add_executable(lisp ${LISP_SRC})
set(CMAKE_C_FLAGS "-Wall -O0 -g3")
target_link_libraries(lisp readline)
