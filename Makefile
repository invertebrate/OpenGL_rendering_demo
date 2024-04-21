GPP = gcc
NAME = OpenGL_demo
DIR_SRC = ./src
DIR_OBJ = temp
DEBUGFLAG = -g

# Linux and MacOS specific includes & libs & library check files
# ====================
UNAME := $(shell uname)
TYPE := $(shell uname -m)
ifeq ($(UNAME),Linux)
	ifeq ($(TYPE),x86_64)
		SDL_FLAGS = `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
		GL_FLAGS = -lGL
		LIB_MATH = -lm
		LIBSDL2FILE = /usr/lib/x86_64-linux-gnu/libSDL2.a
		LIBSDL2IMAGEFILE = /usr/lib/x86_64-linux-gnu/libSDL2_image.a
		LIBSDL2TTFFILE = /usr/lib/x86_64-linux-gnu/libSDL2_ttf.a
		LIBSDL2MIXERFILE = /usr/lib/x86_64-linux-gnu/libSDL2_mixer.a
	endif
else ifeq ($(UNAME),Darwin)
	SDL_FLAGS = `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
	GL_FLAGS = -framework OpenGL
	LIBSDL2FILE = /usr/local/lib/libSDL2.a
	LIBSDL2IMAGEFILE = /usr/local/lib/libSDL2_image.a
	LIBSDL2TTFFILE = /usr/local/lib/libSDL2_ttf.a
	LIBSDL2MIXERFILE = /usr/local/lib/libSDL2_mixer.a
else
	TYPE="invalid"
endif
# ====================

LIBS = $(SDL_FLAGS) $(GL_FLAGS) $(LIB_PTHRTEAD) $(LIB_MATH)

INCLUDES = 	-I ./include \

# CFLAGS = -Wall -Wextra -Werror -O3 -flto
CFLAGS = -O3 -flto
SOURCES =	main.c \
			window/window.c \
			events/events.c \
			events/camera.c \
			opengl/init_gl.c \
			opengl/objects_gl.c \
			opengl/shaders.c \
			opengl/texture_gl.c \
			rendering/rendering.c \
			rendering/shadows.c \
			file_reading/object_reading.c \
			file_reading/bitmap_reading.c \
			file_reading/file_utils.c \
			matrix/matrix.c \
			assets/asset_handling.c \
			assets/skybox.c \
			assets/lights.c


OBJS = $(addprefix $(DIR_OBJ)/,$(SOURCES:.c=.o))

#DEPENDS = $(patsubst %.c,%.d,$(SOURCES))

#-include $(DEPENDS)


all:
ifneq ($(TYPE),invalid)
	@make intro && make $(DIR_OBJ) && make install_sdl && make $(NAME) && make usage
else
	$(error "OS not supported by this compilation.")
endif

$(NAME): $(OBJS)
# @make libs
	@printf "\033[32;1mCompiling app...\n\033[0m"
	$(GPP) -o $@ $^ $(LIBS) $(CFLAGS)

#libs:
# @printf "\033[32;1mCompiling libs...\n\033[0m"
# make -C $(LIB3D)

intro:
	@printf "\033[32;1mBegin scop compilation...\n\033[0m"

usage:
	@printf "\033[32;1mUsage:\n./OpenGL_demo for default demo view\n"

$(DIR_OBJ):
	@printf "\033[32;1mCreate temp directories...\n\033[0m"
	@mkdir -p temp
	@mkdir -p temp/window
	@mkdir -p temp/events
	@mkdir -p temp/opengl
	@mkdir -p temp/rendering
	@mkdir -p temp/file_reading
	@mkdir -p temp/matrix
	@mkdir -p temp/assets

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@printf "\033[32;1m$<\n\033[0m"
	@$(GPP) -c -o $@ $< $(CFLAGS) $(INCLUDES)

install_sdl:
ifeq ($(UNAME),Linux)
	@printf "\033[32;1mCheck SDL2 on Linux...\n\033[0m"
endif
ifeq ($(UNAME),Darwin)
	@printf "\033[32;1mCheck SDL2 on MacOS...\n\033[0m"
ifeq (,$(shell which brew))
	$(error "No brew found, install homebrew on MacOS to compile")
endif
endif
ifeq ($(OS),Windows_NT)
	$(error "Can only compile on MacOS and Linux")
endif
	@if ! test -e $(LIBSDL2FILE); then\
		echo "SDL2 missing";\
		./install.sh;\
	elif ! test -e $(LIBSDL2IMAGEFILE); then\
		echo "SDL2 image missing";\
		./install.sh;\
	elif ! test -e $(LIBSDL2TTFFILE); then\
		echo "SDL2 ttf missing";\
		./install.sh;\
	elif ! test -e $(LIBSDL2MIXERFILE); then\
		echo "SDL2 mixer missing";\
		./install.sh;\
	fi
	@printf "\033[32;1mCompiling...\n\033[0m"

clean:
	@/bin/rm -f $(OBJS)
	@/bin/rm -rf $(DIR_OBJ)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: all, $(DIR_OBJ), clean, fclean, intro, usage
