TARGET = DNS_Client
CC = g++
PREF_SRC = ./src/
PREF_OBJ = ./obj/

SRC = $(wildcard $(PREF_SRC)/*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp,$(PREF_OBJ)%.o,$(SRC))

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lpq

$(PREF_OBJ)%.o: $(PREF_SRC)%.cpp 
	$(CC) -c $< -o $@


clean:
	rm -f $(TARGET) $(PREF_OBJ)*.o 

