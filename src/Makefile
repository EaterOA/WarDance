ROOTDIR = $(shell readlink -f '..')
NAME = WarDance
SRC = $(wildcard *.cpp) $(wildcard */*.cpp)
DEPS = $(wildcard *.hpp) $(wildcard */*.hpp)
OBJ = ${SRC:.cpp=.o}
CFLAGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused -Wno-long-long -pipe -fno-exceptions
LDFLAGS = -lm -lsfml-graphics -lsfml-system -lsfml-window -Wl,-O1 -Wl,--no-undefined
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
