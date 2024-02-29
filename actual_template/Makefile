# this Makefile is set for Mingw compilator in release type under windows;
# edit variables "COMPILATOR" | "COMPILATOR_DIR" | "DIRECTORY_INCLUDE" | "STATIC_LIBS_DIR" to your environment;
# you are required to have the following external libraries:
#	SDL2
#	OpenGL
#	FreeType
#	Assimp
#	Glew

COMPILATOR 			:= g++
COMPILATOR_DIR 		:= C:/MinGW
DIRECTORY_INCLUDE 		:= ${COMPILATOR_DIR}/your_include_dir
STATIC_LIBS_DIR 			:= ${COMPILATOR_DIR}/your_library_.a_dir
STATIC_LIBS 			:= -lmingw32 -lSDL2main -lSDL2  -lassimp -lzlibstatic -lopengl32 -lglew32 -lfreetype -static-libstdc++ -static-libgcc
STANDARD_FLAGS		:= -Wall -O2 -s

FINALE_OUTPUT 		:= program
DIRECTORY_OBJ 	:= obj
DIRECOTRY_SRC_CODE	:= source

# run makefile with command: "make mode=run" or "mingw32-make mode=run" to run program after compilation;

PROGRAM_ARGS 	:= 0

#=================================================================================================================================

FINALE_FLAGS := -I${DIRECTORY_INCLUDE} -L${STATIC_LIBS_DIR} ${STANDARD_FLAGS} ${STATIC_LIBS}

SRC_FILES_CPP := $(wildcard *.cpp) $(wildcard ${DIRECOTRY_SRC_CODE}/*.cpp)
SRC_FILES_C := $(wildcard *.c) $(wildcard ${DIRECOTRY_SRC_CODE}/*.c)

BINARY_SRC_FILES := $(SRC_FILES_CPP:%.cpp=${DIRECTORY_OBJ}/%.o) $(SRC_FILES_C:%.c=${DIRECTORY_OBJ}/%.o)
BINARY_SRC_FILES := $(subst ${DIRECOTRY_SRC_CODE}/,,$(BINARY_SRC_FILES))
BINARY_SRC_FLAGS := ${STANDARD_FLAGS} -I${DIRECTORY_INCLUDE} -L${STATIC_LIBS_DIR} ${STATIC_LIBS}


${FINALE_OUTPUT}: ${BINARY_SRC_FILES}
	@echo --------------------------------------------------------------------
	@echo linking program in progress...
	@${COMPILATOR} $^ ${FINALE_FLAGS} -o ${FINALE_OUTPUT}
	@echo program has been linked;
ifeq "${mode}" "run"
	@echo program is running;
	${FINALE_OUTPUT} ${PROGRAM_ARGS}
endif

${DIRECTORY_OBJ}/%.o: ${DIRECOTRY_SRC_CODE}/%.c
	@echo --------------------------------------------------------------------
	@echo compiling "$<" file in progress...
	@${COMPILATOR} $< -c ${BINARY_SRC_FLAGS} -o $@
	@echo file "$@" has been created;

${DIRECTORY_OBJ}/%.o: ${DIRECOTRY_SRC_CODE}/%.cpp
	@echo --------------------------------------------------------------------
	@echo compiling "$<" file in progress...
	@${COMPILATOR} $< -c ${BINARY_SRC_FLAGS} -o $@
	@echo file "$@" has been created;

${DIRECTORY_OBJ}/%.o: %.c
	@echo --------------------------------------------------------------------
	@echo compiling "$<" file in progress...
	@${COMPILATOR} $< -c ${BINARY_SRC_FLAGS} -o $@
	@echo file "$@" has been created;

${DIRECTORY_OBJ}/%.o: %.cpp
	@echo --------------------------------------------------------------------
	@echo compiling "$<" file in progress...
	@${COMPILATOR} $< -c ${BINARY_SRC_FLAGS} -o $@
	@echo file "$@" has been created;


#=====================================================================================================================
DELETE_OBJ := $(wildcard ${DIRECTORY_OBJ}/*.o)
DELETE_OBJ :=  $(subst /,\,$(DELETE_OBJ))
.PHONY: clean
clean:
ifneq   "${DELETE_OBJ}" ""
	@del /f /q  ${DELETE_OBJ}
	@echo ${DIRECTORY_OBJ}/*.o files has been deleted;
else
	@echo there is nothing to delete;
endif
