#include "Dungeon.h"
#include "Rand.h"
#include <cstdio>


char Dungeon::wall = '#';
char Dungeon::floor = '-';
char Dungeon::nothing = '.';


void Dungeon::setMin(int height, int width)
{
    if (height < 3 || height > D_HEIGHT
	    || width < 3 || width > D_WIDTH)
	perror("Wrong setMin() parameters. It has to be more than 2 and less than or equal to D_HEIGHT/D_WIDTH");
    Dungeon::r_min_height = height;
    Dungeon::r_min_width = width;
}


void Dungeon::setMax(int height, int width)
{
    if (height < r_min_height || height > D_HEIGHT
	    || width < r_min_width || width > D_WIDTH)
	perror("Wrong setMax() parameters. It should be more than r_min_height/r_min_width and less than or equal to D_HEIGHT/D_WIDTH");
    Dungeon::r_max_height = height;
    Dungeon::r_max_width = width;
}


void Dungeon::setMinRoomNum(int num)
{
    if (num < 0)
	perror("Wrong setMinRoomNum() parameter. It has to be more than or equal to 0");
    Dungeon::min_room_num = num;
}


Dungeon::Dungeon(int height, int width) : D_HEIGHT{height}, D_WIDTH{width}
{
//Set the default parameters
    setMin(4,4);
    setMax(D_HEIGHT/4, D_WIDTH/7);
    setMinRoomNum(30);

//Make the "canvas" {{{
    dungeon_ = new char*[D_HEIGHT];
    for (int i = 0; i < D_HEIGHT; i++)
        dungeon_[i] = new char[D_WIDTH];

    for (int y = 0; y < D_HEIGHT; y++) {
        for (int x = 0; x < D_WIDTH; x++) {
            dungeon_[y][x] = nothing;
        }
    }
//}}}

//Starting point of the first room
    y_pos_ = rnd::randomize(D_HEIGHT);
    x_pos_ = rnd::randomize(D_WIDTH);

//This is needed for genRoom() (recursive calls)
    counter_ = 1;
    is_executed_ = false;
}


Dungeon::~Dungeon()
{
    //De-Allocate memory
    for (int i = 0; i < D_HEIGHT; ++i)
        delete [] dungeon_[i];
    delete [] dungeon_;
}


void Dungeon::generate()
{
    //Draw the "dungeon" on a "canvas"
    while(!genRoom());
    genPassages();
}


char* Dungeon::operator[](int y)
{
    return dungeon_[y];
}


bool Dungeon::genRoom()
{
//Room width and height 
    int width = rnd::randomize(r_min_width, r_max_width);
    int height = rnd::randomize(r_min_height, r_max_height);

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

//Do a little trick if there is no possible directions and less than %min_room_num rooms {{{
    //!!! It is not guaranteed that the number of rooms will be equal to %min_room_num
    if (dir_vec.empty() && room_vec_.size() < min_room_num) {
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
                    dungeon_[y][x] = wall;
                } else {
                    dungeon_[y][x] = floor;
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
                    dungeon_[y][x] = wall;
                } else {
                    dungeon_[y][x] = floor;
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
                    dungeon_[y][x] = wall;
                } else {
                    dungeon_[y][x] = floor;
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
                    dungeon_[y][x] = wall;
                } else {
                    dungeon_[y][x] = floor;
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
        if (y_pos_ + height <= D_HEIGHT && x_pos_ + width <= D_WIDTH) {
            for (int y = y_pos_; y < y_pos_ + height; y++) {
                for (int x = x_pos_; x < x_pos_ + width; x++) {
                    if (y == y_pos_ || y == y_pos_ + (height-1)
                            || x == x_pos_ || x == x_pos_ + (width-1)) continue; //Ignore wall collision
                    if (dungeon_[y][x] != nothing) return false;
                }
            }
        } else return false;
        return true;
    case dir_t::s_w:
        if (y_pos_ + height <= D_HEIGHT && x_pos_ - width >= 0) {
            for (int y = y_pos_; y < y_pos_ + height; y++) {
                for (int x = x_pos_; x > x_pos_ - width; x--) {
                    if (y == y_pos_ || y == y_pos_ + (height-1)
                            || x == x_pos_ || x == x_pos_ - (width-1)) continue;
                    if (dungeon_[y][x] != nothing) return false;
                }
            }
        } else return false;
        return true;
    case dir_t::n_e:
        if (y_pos_ - height >= 0 && x_pos_ + width <= D_WIDTH) {
            for (int y = y_pos_; y > y_pos_ - height; y--) {
                for (int x = x_pos_; x < x_pos_ + width; x++) {
                    if (y == y_pos_ || y == y_pos_ - (height-1)
                            || x == x_pos_ || x == x_pos_ + (width-1)) continue;
                    if (dungeon_[y][x] != nothing) return false;
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
                    if (dungeon_[y][x] != nothing) return false;
                }
            }
        } else return false;
        return true;
    }

    //Something went wrong if program reached this
    perror("Something wrong in check() function");
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
                        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x - 1] = floor;
                    } else if (room_vec_[i].dir == dir_t::n_e) {
                        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x] = floor;
                    }
                    break;
                case dir_t::s_w :
                    if (room_vec_[i].dir == dir_t::s_e) {
                        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x + 1] = floor;
                    } else if (room_vec_[i].dir == dir_t::s_w) {
                        genVestibule(dir_t::s_w, i);
                    } else if (room_vec_[i].dir == dir_t::n_w) {
                        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x] = floor;
                    }
                    break;
                case dir_t::n_e :
                    if (room_vec_[i].dir == dir_t::s_e) {
                        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x] = floor;
                    } else if (room_vec_[i].dir == dir_t::n_e) {
                        genVestibule(dir_t::n_e, i);
                    } else if (room_vec_[i].dir == dir_t::n_w) {
                        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x - 1] = floor;
                    }
                    break;
                case dir_t::n_w :
                    if (room_vec_[i].dir == dir_t::s_w) {
                        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x] = floor;
                    } else if (room_vec_[i].dir == dir_t::n_e) {
                        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x + 1] = floor;
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
        if (dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x + 1] == nothing) {
            dungeon_[room_vec_[i].start_y + 2][room_vec_[i].start_x + 1] = wall;
            dungeon_[room_vec_[i].start_y + 2][room_vec_[i].start_x + 2] = wall;
            dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x + 2] = wall;
        }
        if (dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x - 1] == nothing) {
            dungeon_[room_vec_[i].start_y - 2][room_vec_[i].start_x - 2] = wall;
            dungeon_[room_vec_[i].start_y - 2][room_vec_[i].start_x - 1] = wall;
            dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x - 2] = wall;
        }

        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x] = floor;
        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x + 1] = floor;
        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x - 1] = floor;
        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x - 1] = floor;
        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x] = floor;
        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x + 1] = floor;
        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x - 1] = floor;
        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x + 1] = floor;
        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x] = floor;
        break;
    case dir_t::s_e :
    case dir_t::n_w :
        if (dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x - 1] == nothing) {
            dungeon_[room_vec_[i].start_y + 2][room_vec_[i].start_x - 1] = wall;
            dungeon_[room_vec_[i].start_y + 2][room_vec_[i].start_x - 2] = wall;
            dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x - 2] = wall;
        }
        if (dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x + 1] == nothing) {
            dungeon_[room_vec_[i].start_y - 2][room_vec_[i].start_x + 2] = wall;
            dungeon_[room_vec_[i].start_y - 2][room_vec_[i].start_x + 1] = wall;
            dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x + 2] = wall;
        }

        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x] = floor;
        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x + 1] = floor;
        dungeon_[room_vec_[i].start_y - 1][room_vec_[i].start_x - 1] = floor;
        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x - 1] = floor;
        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x] = floor;
        dungeon_[room_vec_[i].start_y + 1][room_vec_[i].start_x + 1] = floor;
        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x - 1] = floor;
        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x + 1] = floor;
        dungeon_[room_vec_[i].start_y][room_vec_[i].start_x] = floor;
        break;
    }
}
