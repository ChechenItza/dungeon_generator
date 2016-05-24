#include <fstream>
#include <memory>
#include "Dungeon.h"

int main()
{
//Set the file stream
    std::ofstream out("Output/dungeon.txt");
    if(out.fail()){
        perror("dungeon.txt");
        return 1;
    }

//Generate dungeon
    const int Y = 59;
    const int X = 221;
    std::unique_ptr<Dungeon> dungeon(new Dungeon(Y, X));
    dungeon->setMax(Y/5, X/9);
    dungeon->setMinRoomNum(60);
    dungeon->setChars('#','.',' ');
    dungeon->generate();

//Print dungeon to stream
    for (int y = 0; y < Y; y++){
        for (int x = 0; x < X; x++){
            out << (*dungeon)[y][x];
        }
        out << "\n";
    }

    return 0;
}
