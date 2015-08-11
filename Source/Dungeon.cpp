#include "Dungeon.h"

using namespace dng;

char** dng::genDungeon()
{
//Allocate memory for array
    char **dungeon = new char*[D_HEIGHT];
    for (int i = 0; i < D_HEIGHT; i++)
        dungeon[i] = new char[D_WIDTH];

//Draw dungeon on it
    Dungeon dung(dungeon);
    dung.genStart();

    return dungeon;
}


void dng::clrDungeon(char** dungeon)
{
    //De-Allocate memory
    for (int i = 0; i < D_HEIGHT; ++i)
        delete [] dungeon[i];
    delete [] dungeon;
}


Dungeon::Dungeon(char** dungeon)
{
//Initialize the dungeon {{{
    this->dungeon = dungeon;
    for (int y = 0; y<D_HEIGHT; y++) {
        for (int x = 0; x<D_WIDTH; x++) {
            this->dungeon[y][x] = NOTHING;
        }
    }
//}}}

//Starting point of the first room
    y_pos = rnd::randomize(D_HEIGHT);
    x_pos = rnd::randomize(D_WIDTH);

//This is needed for genRoom() (recursive calls)
    counter = 1;
    is_executed = false;
}


void Dungeon::genStart()
{
    //Generate rooms and passages between them
    while (!genRoom());
    genPassages();
}


bool Dungeon::genRoom()
{
//Room width and height {{{
    constexpr int MIN_WIDTH = 4;
    constexpr int MIN_HEIGHT = 4;
    constexpr int MAX_WIDTH = D_WIDTH / 9;
    constexpr int MAX_HEIGHT = D_HEIGHT / 5;
    int width = rnd::randomize(MIN_WIDTH, MAX_WIDTH);
    int height = rnd::randomize(MIN_HEIGHT, MAX_HEIGHT);

    static_assert(MAX_WIDTH >= MIN_WIDTH && MAX_HEIGHT >= MIN_HEIGHT
            , "MAX_WIDTH and MAX_HEIGHT has to be more than or equal to MIN_WIDTH/MIN_HEIGHT");
    static_assert(MIN_WIDTH >= 3 && MIN_WIDTH >= 3
            , "MIN_WIDTH and MIN_HEIGHT has to be more than or equal to 3");
//}}}

//s_e - south east; s_w - south west; n_e - north east; n_w - north west;
    dir_t s_e = dir_t::s_e;  dir_t n_e = dir_t::n_e;
    dir_t s_w = dir_t::s_w;  dir_t n_w = dir_t::n_w;

//Store possible directions in %dir_vec vector {{{
    std::vector<dir_t> dir_vec;
    if (check(s_e, width, height)) {
        dir_vec.push_back(s_e);
    }
    if (check(s_w, width, height)) {
        dir_vec.push_back(s_w);
    }
    if (check(n_e, width, height)) {
        dir_vec.push_back(n_e);
    }
    if (check(n_w, width, height)) {
        dir_vec.push_back(n_w);
    }
//}}}

//Do a little trick if there is no possible directions and less than MIN_ROOM_NUM rooms {{{
    //!!! It is not guaranteed that the number of rooms will be equal to this
    constexpr int MIN_ROOM_NUM = 30;
    if (dir_vec.empty() && room_vec.size() < MIN_ROOM_NUM) {
        if (room_vec.size() - counter > 0) {
            x_pos = room_vec[room_vec.size() - counter].end_x;
            y_pos = room_vec[room_vec.size() - counter].end_y;
            counter++;
            while (!genRoom());
            while (!genRoom());
        } else if (!is_executed && room_vec.size() - counter == 0) {
            x_pos = room_vec[0].start_x;
            y_pos = room_vec[0].start_y;
            is_executed = true; //This condition should be executed only ONCE
            genRoom();
        }
    }
//}}}

//Break if no possible directions
    if (dir_vec.empty()) return true;

//Make room in randomly selected direction {{{
    dir_t rnd_dir = dir_vec[rnd::randomize(dir_vec.size() - 1)];
    switch (rnd_dir) {
    case dir_t::s_e:{
        for (int y = y_pos; y < y_pos + height; y++) {
            for (int x = x_pos; x < x_pos + width; x++) {
                if (y == y_pos || y == y_pos + (height-1)
                        || x == x_pos || x == x_pos + (width-1)) {
                    dungeon[y][x] = WALL;
                } else {
                    dungeon[y][x] = FLOOR;
                }
            }
        }
        //Keep track of all rooms
        Room r {x_pos, y_pos, x_pos + (width-1), y_pos + (height-1), s_e};
        room_vec.push_back(r);
        //Set y&&x position to the opposite corner
        y_pos += (height - 1);
        x_pos += (width - 1);
    }
        break;
    case dir_t::s_w: {
        for (int y = y_pos; y < y_pos + height; y++) {
            for (int x = x_pos; x > x_pos - width; x--) {
                if (y == y_pos || y == y_pos + (height-1)
                        || x == x_pos || x == x_pos - (width-1)) {
                    dungeon[y][x] = WALL;
                } else {
                    dungeon[y][x] = FLOOR;
                }
            }
        }
        Room r {x_pos, y_pos, x_pos - (width-1), y_pos + (height-1), s_w};
        room_vec.push_back(r);
        y_pos += (height - 1);
        x_pos -= (width - 1);
    }
        break;
    case dir_t::n_e: {
        for (int y = y_pos; y > y_pos - height; y--) {
            for (int x = x_pos; x < x_pos + width; x++) {
                if (y == y_pos || y == y_pos - (height-1)
                        || x == x_pos || x == x_pos + (width-1)) {
                    dungeon[y][x] = WALL;
                } else {
                    dungeon[y][x] = FLOOR;
                }
            }
        }
        Room r {x_pos, y_pos, x_pos + (width-1), y_pos - (height-1), n_e};
        room_vec.push_back(r);
        y_pos -= (height - 1);
        x_pos += (width - 1);
    }
        break;
    case dir_t::n_w: {
        for (int y = y_pos; y > y_pos - height; y--) {
            for (int x = x_pos; x > x_pos - width; x--) {
                if (y == y_pos || y == y_pos - (height-1)
                        || x == x_pos || x == x_pos - (width-1)) {
                    dungeon[y][x] = WALL;
                } else {
                    dungeon[y][x] = FLOOR;
                }
            }
        }
        Room r {x_pos, y_pos, x_pos - (width-1), y_pos - (height-1), n_w};
        room_vec.push_back(r);
        y_pos -= (height - 1);
        x_pos -= (width - 1);
    }
        break;
    }
//}}}

//Signal that there is still possible directions left
    return false;
}


bool Dungeon::check(const dir_t& dir, int width, int height) const
{
    //Check if it's possible to make room in the direction(%dir) that was passed
    switch(dir) {
    case dir_t::s_e:
        if (y_pos + height <= D_HEIGHT && x_pos + width <= D_WIDTH) {
            for (int y = y_pos; y < y_pos + height; y++) {
                for (int x = x_pos; x < x_pos + width; x++) {
                    if (y == y_pos || y == y_pos + (height-1)
                            || x == x_pos || x == x_pos + (width-1)) continue; //Ignore wall collision
                    if (dungeon[y][x] != NOTHING) return false;
                }
            }
        } else return false;
        return true;
    case dir_t::s_w:
        if (y_pos + height <= D_HEIGHT && x_pos - width >= 0) {
            for (int y = y_pos; y < y_pos + height; y++) {
                for (int x = x_pos; x > x_pos - width; x--) {
                    if (y == y_pos || y == y_pos + (height-1)
                            || x == x_pos || x == x_pos - (width-1)) continue;
                    if (dungeon[y][x] != NOTHING) return false;
                }
            }
        } else return false;
        return true;
    case dir_t::n_e:
        if (y_pos - height >= 0 && x_pos + width <= D_WIDTH) {
            for (int y = y_pos; y > y_pos - height; y--) {
                for (int x = x_pos; x < x_pos + width; x++) {
                    if (y == y_pos || y == y_pos - (height-1)
                            || x == x_pos || x == x_pos + (width-1)) continue;
                    if (dungeon[y][x] != NOTHING) return false;
                }
            }
        } else return false;
        return true;
    case dir_t::n_w:
        if (y_pos - height >= 0 && x_pos - width >= 0) {
            for (int y = y_pos; y > y_pos - height; y--) {
                for (int x = x_pos; x > x_pos - width; x--) {
                    if (y == y_pos || y == y_pos - (height-1)
                            || x == x_pos || x == x_pos - (width-1)) continue;
                    if (dungeon[y][x] != NOTHING) return false;
                }
            }
        } else return false;
        return true;
    }
}


void Dungeon::genPassages()
{
    //Make passage between rooms
    for (int i = 1; i < room_vec.size(); ++i) {
        for (int n = 1; n <= i; ++n) {
            if (room_vec[i-n].end_y == room_vec[i].start_y
                    && room_vec[i-n].end_x == room_vec[i].start_x) {
                switch (room_vec[i-n].dir) {
                case dir_t::s_e :
                    if (room_vec[i].dir == dir_t::s_e) {  //Because nested switches look ugly
                        genVestibule(dir_t::s_e, i);
                    } else if (room_vec[i].dir == dir_t::s_w) {
                        dungeon[room_vec[i].start_y][room_vec[i].start_x - 1] = FLOOR;
                    } else if (room_vec[i].dir == dir_t::n_e) {
                        dungeon[room_vec[i].start_y - 1][room_vec[i].start_x] = FLOOR;
                    }
                    break;
                case dir_t::s_w :
                    if (room_vec[i].dir == dir_t::s_e) {
                        dungeon[room_vec[i].start_y][room_vec[i].start_x + 1] = FLOOR;
                    } else if (room_vec[i].dir == dir_t::s_w) {
                        genVestibule(dir_t::s_w, i);
                    } else if (room_vec[i].dir == dir_t::n_w) {
                        dungeon[room_vec[i].start_y - 1][room_vec[i].start_x] = FLOOR;
                    }
                    break;
                case dir_t::n_e :
                    if (room_vec[i].dir == dir_t::s_e) {
                        dungeon[room_vec[i].start_y + 1][room_vec[i].start_x] = FLOOR;
                    } else if (room_vec[i].dir == dir_t::n_e) {
                    genVestibule(dir_t::n_e, i);
                    } else if (room_vec[i].dir == dir_t::n_w) {
                        dungeon[room_vec[i].start_y][room_vec[i].start_x - 1] = FLOOR;
                    }
                    break;
                case dir_t::n_w :
                    if (room_vec[i].dir == dir_t::s_w) {
                        dungeon[room_vec[i].start_y + 1][room_vec[i].start_x] = FLOOR;
                    } else if (room_vec[i].dir == dir_t::n_e) {
                        dungeon[room_vec[i].start_y][room_vec[i].start_x + 1] = FLOOR;
                    } else if (room_vec[i].dir == dir_t::n_w) {
                        genVestibule(dir_t::n_w, i);
                    }
                    break;
                }
            }
        }
    }
}


void Dungeon::genVestibule(const dir_t& dir, int i)
{
    //This belongs to genPassages()
    //Have put this in separate method for the sake of clarity
    switch (dir) {
    case dir_t::s_w :
    case dir_t::n_e :
        //Draw the walls if this vestibule is not collapsing with other rooms
        if (dungeon[room_vec[i].start_y + 1][room_vec[i].start_x + 1] == NOTHING) {
            dungeon[room_vec[i].start_y + 2][room_vec[i].start_x + 1] = WALL;
            dungeon[room_vec[i].start_y + 2][room_vec[i].start_x + 2] = WALL;
            dungeon[room_vec[i].start_y + 1][room_vec[i].start_x + 2] = WALL;
        }
        if (dungeon[room_vec[i].start_y - 1][room_vec[i].start_x - 1] == NOTHING) {
            dungeon[room_vec[i].start_y - 2][room_vec[i].start_x - 2] = WALL;
            dungeon[room_vec[i].start_y - 2][room_vec[i].start_x - 1] = WALL;
            dungeon[room_vec[i].start_y - 1][room_vec[i].start_x - 2] = WALL;
        }

        dungeon[room_vec[i].start_y - 1][room_vec[i].start_x] = FLOOR;
        dungeon[room_vec[i].start_y - 1][room_vec[i].start_x + 1] = FLOOR;
        dungeon[room_vec[i].start_y - 1][room_vec[i].start_x - 1] = FLOOR;
        dungeon[room_vec[i].start_y + 1][room_vec[i].start_x - 1] = FLOOR;
        dungeon[room_vec[i].start_y + 1][room_vec[i].start_x] = FLOOR;
        dungeon[room_vec[i].start_y + 1][room_vec[i].start_x + 1] = FLOOR;
        dungeon[room_vec[i].start_y][room_vec[i].start_x - 1] = FLOOR;
        dungeon[room_vec[i].start_y][room_vec[i].start_x + 1] = FLOOR;
        dungeon[room_vec[i].start_y][room_vec[i].start_x] = FLOOR;
        break;
    case dir_t::s_e :
    case dir_t::n_w :
        if (dungeon[room_vec[i].start_y + 1][room_vec[i].start_x - 1] == NOTHING) {
            dungeon[room_vec[i].start_y + 2][room_vec[i].start_x - 1] = WALL;
            dungeon[room_vec[i].start_y + 2][room_vec[i].start_x - 2] = WALL;
            dungeon[room_vec[i].start_y + 1][room_vec[i].start_x - 2] = WALL;
        }
        if (dungeon[room_vec[i].start_y - 1][room_vec[i].start_x + 1] == NOTHING) {
            dungeon[room_vec[i].start_y - 2][room_vec[i].start_x + 2] = WALL;
            dungeon[room_vec[i].start_y - 2][room_vec[i].start_x + 1] = WALL;
            dungeon[room_vec[i].start_y - 1][room_vec[i].start_x + 2] = WALL;
        }

        dungeon[room_vec[i].start_y - 1][room_vec[i].start_x] = FLOOR;
        dungeon[room_vec[i].start_y - 1][room_vec[i].start_x + 1] = FLOOR;
        dungeon[room_vec[i].start_y - 1][room_vec[i].start_x - 1] = FLOOR;
        dungeon[room_vec[i].start_y + 1][room_vec[i].start_x - 1] = FLOOR;
        dungeon[room_vec[i].start_y + 1][room_vec[i].start_x] = FLOOR;
        dungeon[room_vec[i].start_y + 1][room_vec[i].start_x + 1] = FLOOR;
        dungeon[room_vec[i].start_y][room_vec[i].start_x - 1] = FLOOR;
        dungeon[room_vec[i].start_y][room_vec[i].start_x + 1] = FLOOR;
        dungeon[room_vec[i].start_y][room_vec[i].start_x] = FLOOR;
        break;
    }
}
