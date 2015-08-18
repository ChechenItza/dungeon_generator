#include <fstream>
#include "Dungeon.h"
#include "Constants.h"

int main()
{
//Set the file stream
    std::ofstream out("dungeon.txt");
    if(out.fail()){
        perror("dungeon.txt");
        return 1;
    }

//Generate dungeon
    dng::Dungeon dungeon(36,96);

//Print dungeon to stream
    for (int y = 0; y < 36; y++){
        for (int x = 0; x < 96; x++){
            out << dungeon[y][x];
        }
        out << "\n";
    }

    return 0;
}
