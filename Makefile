NAME = Matrix

CXX = c++
CXXFLAGS = -Wall -Wextra -Wno-unknown-pragmas -std=c++17 -g
INCLUDES = -I./includes
SRCS = ./srcs/tests.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f display_linux/proj

fclean: clean
	rm -f $(NAME)

re: fclean all