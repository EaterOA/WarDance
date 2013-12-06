ROOTDIR = $(shell readlink -f '..')
NAME = WarDance
SRC = Actor.cpp App.cpp Fighter.cpp GameGraphics.cpp GameGUI.cpp GameMechanics.cpp Grunt.cpp Player.cpp Projectile.cpp RegularBullet.cpp SplittingBullet.cpp Sprinkler.cpp Util.cpp Config.cpp
DEPS = Actors.hpp App.hpp GameGraphics.hpp GameGUI.hpp GameMechanics.hpp Util.hpp Config.hpp
OBJ = ${SRC:.cpp=.o}
CFLAGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
LDFLAGS = -lm -lsfml-graphics -lsfml-system -lsfml-window

.PHONY: all clean distclean

all: debug

debug: CFLAGS += -g
debug: $(NAME)

release: CFLAGS += -O2
release: $(NAME)

%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME) $(LDFLAGS)
	cp $(NAME) $(ROOTDIR)

clean:
	@ rm $(NAME)
	@ rm $(ROOTDIR)/$(NAME)
	@ rm $(OBJ)
