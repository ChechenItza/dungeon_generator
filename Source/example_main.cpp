#include <fstream>
#include "Dungeon.h"


int main()
{
//Set the file stream {{{
    std::ofstream out("dungeon.txt");
    if(out.fail()){
        perror("dungeon.txt");
        return 1;
    }
//}}}

//Generate dungeon
    auto dungeon = dng::genDungeon();

//Print dungeon to stream {{{
    for (int y = 0; y<D_HEIGHT; y++){
        for (int x = 0; x<D_WIDTH; x++){
            out << dungeon[y][x];
        }
        out << "\n";
    }
//}}}

    dng::clrDungeon(dungeon); //De-Allocate memory
    return 0;
}
