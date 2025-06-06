#Compilador
CXX = g++

#Define source files and output
SRC = main.cpp maxsetguloso.cpp gera_grafos.cpp solucao.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = scripttodo
#Arquivo final
all: $(TARGET)
#Linka
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)
#Compila
%.o: %.cpp
	$(CXX) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)