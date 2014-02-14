ROOTDIR = $(shell readlink -f '..')
NAME = WarDance
SRC = Actor.cpp App.cpp Fighter.cpp GameGraphics.cpp GameGUI.cpp GameMechanics.cpp Grunt.cpp Player.cpp Projectile.cpp RegularBullet.cpp SplittingBullet.cpp Sprinkler.cpp Util.cpp Config.cpp Alien.cpp Item.cpp Medkit.cpp GameScript.cpp Grenade.cpp Wiper.cpp Laser.cpp GrenadeWiper.cpp RoundBullet.cpp GameConfig.cpp
DEPS = Actors.hpp App.hpp GameGraphics.hpp GameGUI.hpp GameMechanics.hpp Util.hpp Config.hpp GameScript.hpp GameConfig.hpp
OBJ = ${SRC:.cpp=.o}
CFLAGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused -Wno-long-long
LDFLAGS = -lm -lsfml-graphics -lsfml-system -lsfml-window
INCFLAGS = 
LNKFLAGS = 

.PHONY: all clean debug release

all: debug

debug: CFLAGS += -g
debug: $(NAME)

release: CFLAGS += -O2
release: $(NAME)

%.o: %.cpp $(DEPS)
	$(CXX) $(INCFLAGS) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME) $(LNKFLAGS) $(LDFLAGS)
	cp $(NAME) $(ROOTDIR)

clean:
	@ rm $(OBJ)
	@ rm $(NAME)
	@ rm $(ROOTDIR)/$(NAME)
