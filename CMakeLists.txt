cmake_minimum_required(VERSION 2.4)
set(LISP_SRC
		tokenize.c
		main.c
   )

include_directories("${PROJECT_SOURCE_DIR}")

add_executable(lisp ${LISP_SRC})
target_link_libraries(lisp readline)
