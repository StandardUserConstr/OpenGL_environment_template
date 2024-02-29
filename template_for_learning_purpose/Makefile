# this Makefile is set for Mingw compilator in release type under windows;
# edit variables "COMPILATOR" | "COMPILATOR_DIR" | "DIRECTORY_INCLUDE" | "STATIC_LIBS_DIR" to your environment;
# edit "PROGRAM_ARG_FROM_0_TO_3" variable to change program behavior;
# you are required to have the following external libraries:
#	SDL2
#	OpenGL
#	FreeType
#	Assimp
#	Glew

PROGRAM_ARG_FROM_0_TO_3	:= 0

COMPILATOR 			:= g++
COMPILATOR_DIR 		:= C:/MinGW
DIRECTORY_INCLUDE 		:= ${COMPILATOR_DIR}/your_include_dir
STATIC_LIBS_DIR 			:= ${COMPILATOR_DIR}/your_library_.a_dir
STATIC_LIBS 			:= -lmingw32 -lSDL2main -lSDL2  -lassimp -lzlibstatic -lopengl32 -lglew32 -lfreetype -static-libstdc++ -static-libgcc
STANDARD_FLAGS		:= -Wall -O2 -s

FINALE_OUTPUT 		:= program
DIRECTORY_OBJ 	:= obj
DIRECOTRY_SRC_CODE	:= source

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
	@echo program is running with the argument: ${PROGRAM_ARG_FROM_0_TO_3} 
	@${FINALE_OUTPUT} ${PROGRAM_ARG_FROM_0_TO_3}

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
