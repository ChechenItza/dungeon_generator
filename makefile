FLAGS=-std=c++11 -I $(LIB_PATH) -o $(OUTPUT_PATH)
LIB_PATH=Headers/
OUTPUT_PATH=Output/dungeon
MAIN_PATH=Source/example_main.cpp

dungeon:$(MAIN_PATH)
	clang++ $(FLAGS) $(MAIN_PATH)
