#include "Dungeon.h"
#include "Constants.h"
#include "Rand.h"
#include <assert.h>

using namespace dng;

Dungeon::Dungeon(int height, int width) : D_HEIGHT_{height}, D_WIDTH_{width}
{
//Make the "canvas" {{{
    dungeon_ = new char*[D_HEIGHT_];
    for (int i = 0; i < D_HEIGHT_; i++)
        dungeon_[i] = new char[D_WIDTH_];

    for (int y = 0; y < D_HEIGHT_; y++) {
        for (int x = 0; x < D_WIDTH_; x++) {
            dungeon_[y][x] = NOTHING;
        }
    }
//}}}

//Starting point of the first room
    y_pos_ = rnd::randomize(D_HEIGHT_);
    x_pos_ = rnd::randomize(D_WIDTH_);

//This is needed for genRoom() (recursive calls)
    counter_ = 1;
    is_executed_ = false;

//Draw the "dungeon" on a "canvas" {{{
    while(!genRoom());
    genPassages();
//}}}
}


Dungeon::~Dungeon()
{
    //De-Allocate memory
    for (int i = 0; i < D_HEIGHT_; ++i)
        delete [] dungeon_[i];
    delete [] dungeon_;
}


char* Dungeon::operator[](int y)
{
    return dungeon_[y];
}


bool Dungeon::genRoom()
{
//Room width and height {{{
    const int MIN_HEIGHT = 4;
    const int MIN_WIDTH = 4;
    const int MAX_HEIGHT = D_HEIGHT_ / 5;
    const int MAX_WIDTH = D_WIDTH_ / 9;
    int width = rnd::randomize(MIN_WIDTH, MAX_WIDTH);
    int height = rnd::randomize(MIN_HEIGHT, MAX_HEIGHT);

    assert(MAX_WIDTH >= MIN_WIDTH && MAX_HEIGHT >= MIN_HEIGHT);
    assert(MIN_WIDTH >= 3 && MIN_WIDTH >= 3);
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
    if (dir_vec.empty() && room_vec_.size() < MIN_ROOM_NUM) {
        if (room_vec_.size() - counter_ > 0) {
            x_pos_ = room_vec_[room_vec_.size() - counter_].end_x;
            y_pos_ = room_vec_[room_vec_.size() - counter_].end_y;
            counter_++;
            while (!genRoom());
            while (!genRoom());
        } else if (!is_executed_ && room_vec_.size() - counter_ == 0) {
            x_pos_ = room_vec_[0].start_x;
            y_pos_ = room_vec_[0].start_y;
            is_executed_ = true; //This condition should be executed only ONCE
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
        for (int y = y_pos_; y < y_pos_ + height; y++) {
            for (int x = x_pos_; x < x_pos_ + width; x++) {
                if (y == y_pos_ || y == y_pos_ + (height-1)
                        || x == x_pos_ || x == x_pos_ + (width-1)) {
                    dungeon_[y][x] = WALL;
                } else {
                    dungeon_[y][x] = FLOOR;
                }
            }
        }
        //Keep track of all rooms
        Room r {x_pos_, y_pos_, x_pos_ + (width-1), y_pos_ + (height-1), s_e};
        room_vec_.push_back(r);
        //Set y&&x position to the opposite corner
        y_pos_ += (height - 1);
        x_pos_ += (width - 1);
    }
        break;
    case dir_t::s_w: {
        for (int y = y_pos_; y < y_pos_ + height; y++) {
            for (int x = x_pos_; x > x_pos_ - width; x--) {
                if (y == y_pos_ || y == y_pos_ + (height-1)
                        || x == x_pos_ || x == x_pos_ - (width-1)) {
                    dungeon_[y][x] = WALL;
                } else {
                    dungeon_[y][x] = FLOOR;
                }
            }
        }
        Room r {x_pos_, y_pos_, x_pos_ - (width-1), y_pos_ + (height-1), s_w};
        room_vec_.push_back(r);
        y_pos_ += (height - 1);
        x_pos_ -= (width - 1);
    }
        break;
    case dir_t::n_e: {
        for (int y = y_pos_; y > y_pos_ - height; y--) {
            for (int x = x_pos_; x < x_pos_ + width; x++) {
                if (y == y_pos_ || y == y_pos_ - (height-1)
                        || x == x_pos_ || x == x_pos_ + (width-1)) {
                    dungeon_[y][x] = WALL;
                } else {
                    dungeon_[y][x] = FLOOR;
                }
            }
        }
        Room r {x_pos_, y_pos_, x_pos_ + (width-1), y_pos_ - (height-1), n_e};
        room_vec_.push_back(r);
        y_pos_ -= (height - 1);
        x_pos_ += (width - 1);
    }
        break;
    case dir_t::n_w: {
        for (int y = y_pos_; y > y_pos_ - height; y--) {
            for (int x = x_pos_; x > x_pos_ - width; x--) {
                if (y == y_pos_ || y == y_pos_ - (height-1)
                        || x == x_pos_ || x == x_pos_ - (width-1)) {
                    dungeon_[y][x] = WALL;
                } else {
                    dungeon_[y][x] = FLOOR;
                }
            }
        }
        Room r {x_pos_, y_pos_, x_pos_ - (width-1), y_pos_ - (height-1), n_w};
        room_vec_.push_back(r);
        y_pos_ -= (height - 1);
        x_pos_ -= (width - 1);
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
        if (y_pos_ + height <= D_HEIGHT_ && x_pos_ + width <= D_WIDTH_) {
            for (int y = y_pos_; y < y_pos_ + height; y++) {
                for (int x = x_pos_; x < x_pos_ + width; x++) {
                    if (y == y_pos_ || y == y_pos_ + (height-1)
                            || x == x_pos_ || x == x_pos_ + (width-1)) continue; //Ignore wall collision
                    if (dungeon_[y][x] != NOTHING) return false;
                }
            }
        } else return false;
        return true;
    case dir_t::s_w:
        if (y_pos_ + height <= D_HEIGHT_ && x_pos_ - width >= 0) {
            for (int y = y_pos_; y < y_pos_ + height; y++) {
                for (int x = x_pos_; x > x_pos_ - width; x--) {
                    if (y == y_pos_ || y == y_pos_ + (height-1)
                            || x == x_pos_ || x == x_pos_ - (width-1)) continue;
                    if (dungeon_[y][x] != NOTHING) return false;
                }
            }
        } else return false;
        return true;
    case dir_t::n_e:
        if (y_pos_ - height >= 0 && x_pos_ + width <= D_WIDTH_) {
            for (int y = y_pos_; y > y_pos_ - height; y--) {
                for (int x = x_pos_; x < x_pos_ + width; x++) {
                    if (y == y_pos_ || y == y_pos_ - (height-1)
                            || x == x_pos_ || x == x_pos_ + (width-1)) continue;
                    if (dungeon_[y][x] != NOTHING) return false;
                }
            }
        } else return false;
        return true;
    case dir_t::n_w:
        if (y_pos_ - height >= 0 && x_pos_ - width >= 0) {
            for (int y = y_pos_; y > y_pos_ - height; y--) {
                for (int x = x_pos_; x > x_pos_ - width; x--) {
                    if (y == y_pos_ || y == y_pos_ - (height-1)
                            || x == x_pos_ || x == x_pos_ - (width-1)) continue;
                    if (dungeon_[y][x] != NOTHING) return false;
                }
            }
        } else return false;
        return true;
    }
    //Something went wrong if program reached this
    return false;
}


void Dungeon::genPassages()
{
    //Make passage between rooms
    for (int i = 1; i < room_vec_.size(); ++i) {
        for (int n = 1; n <= i; ++n) {
            if (room_vec_[i-n].end_y == room_vec_[i].start_y
                    && room_vec_[i-n].end_x == room_vec_[i].start_x) {
                switch (room_vec_[i-n].dir) {
                case dir_t::s_e :
                    if (room_vec_[i].dir == dir_t::s_e) {  //Because nested switches look ugly
                        genVestibule(dir_t::s_e, i);
                    } else if (room_vec_[i].dir == dir_t::s_w) {
                        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x - 1] = FLOOR;
                    } else if (room_vec_[i].dir == dir_t::n_e) {
                        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x] = FLOOR;
                    }
                    break;
                case dir_t::s_w :
                    if (room_vec_[i].dir == dir_t::s_e) {
                        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x + 1] = FLOOR;
                    } else if (room_vec_[i].dir == dir_t::s_w) {
                        genVestibule(dir_t::s_w, i);
                    } else if (room_vec_[i].dir == dir_t::n_w) {
                        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x] = FLOOR;
                    }
                    break;
                case dir_t::n_e :
                    if (room_vec_[i].dir == dir_t::s_e) {
                        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x] = FLOOR;
                    } else if (room_vec_[i].dir == dir_t::n_e) {
                        genVestibule(dir_t::n_e, i);
                    } else if (room_vec_[i].dir == dir_t::n_w) {
                        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x - 1] = FLOOR;
                    }
                    break;
                case dir_t::n_w :
                    if (room_vec_[i].dir == dir_t::s_w) {
                        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x] = FLOOR;
                    } else if (room_vec_[i].dir == dir_t::n_e) {
                        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x + 1] = FLOOR;
                    } else if (room_vec_[i].dir == dir_t::n_w) {
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
        if (dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x + 1] == NOTHING) {
            dungeon_[room_vec_[i].start_y + 2][room_vec_[i].start_x + 1] = WALL;
            dungeon_[room_vec_[i].start_y + 2][room_vec_[i].start_x + 2] = WALL;
            dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x + 2] = WALL;
        }
        if (dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x - 1] == NOTHING) {
            dungeon_[room_vec_[i].start_y - 2][room_vec_[i].start_x - 2] = WALL;
            dungeon_[room_vec_[i].start_y - 2][room_vec_[i].start_x - 1] = WALL;
            dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x - 2] = WALL;
        }

        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x] = FLOOR;
        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x + 1] = FLOOR;
        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x - 1] = FLOOR;
        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x - 1] = FLOOR;
        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x] = FLOOR;
        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x + 1] = FLOOR;
        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x - 1] = FLOOR;
        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x + 1] = FLOOR;
        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x] = FLOOR;
        break;
    case dir_t::s_e :
    case dir_t::n_w :
        if (dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x - 1] == NOTHING) {
            dungeon_[room_vec_[i].start_y + 2][room_vec_[i].start_x - 1] = WALL;
            dungeon_[room_vec_[i].start_y + 2][room_vec_[i].start_x - 2] = WALL;
            dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x - 2] = WALL;
        }
        if (dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x + 1] == NOTHING) {
            dungeon_[room_vec_[i].start_y - 2][room_vec_[i].start_x + 2] = WALL;
            dungeon_[room_vec_[i].start_y - 2][room_vec_[i].start_x + 1] = WALL;
            dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x + 2] = WALL;
        }

        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x] = FLOOR;
        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x + 1] = FLOOR;
        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x - 1] = FLOOR;
        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x - 1] = FLOOR;
        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x] = FLOOR;
        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x + 1] = FLOOR;
        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x - 1] = FLOOR;
        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x + 1] = FLOOR;
        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x] = FLOOR;
        break;
    }
}
