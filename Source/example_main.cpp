#include <fstream>
#include "Dungeon.h"

int main()
{
//Set the file stream
    std::ofstream out("dungeon.txt");
    if(out.fail()){
        perror("dungeon.txt");
        return 1;
    }

//Generate dungeon
    const int Y = 36;
    const int X = 96;
    Dungeon dungeon(Y, X);
    dungeon.generate();

//Print dungeon to stream
    for (int y = 0; y < Y; y++){
        for (int x = 0; x < X; x++){
            out << dungeon[y][x];
        }
        out << "\n";
    }

    return 0;
}
