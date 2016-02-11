#ifndef DUNGEON_H
#define DUNGEON_H

#include <vector>


class Dungeon {
public:
    Dungeon(int, int);
    ~Dungeon();
    char* operator[](int);
    void generate();

    void setMin(int,int);
    void setMax(int,int);
    void setChars(char,char,char);
    void setMinRoomNum(int);

private:
    //User-defined types
    enum class dir_t {s_e, s_w, n_e, n_w};
    struct Room {
        int start_x, start_y;
        int end_x, end_y;
        dir_t dir;

        Room(int x, int y, int xx, int yy, const dir_t &d)
            : start_x{x}, start_y{y}, end_x{xx}, end_y{yy}, dir{d} {};
    };

    //Methods
    bool genRoom();
    bool check(const dir_t &, int, int) const;
    void genPassages();
    void genVestibule(const dir_t &, int);

    //Variables
    const int D_HEIGHT_, D_WIDTH_;
    std::vector<Room> room_vec_;
    int r_min_height_, r_min_width_;
    int r_max_height_, r_max_width_;
    char wall_, floor_, nothing_;
    int x_pos_, y_pos_;
    int counter_;
    int min_room_num_;
    char** dungeon_;
    bool is_executed_;
};

#endif //DUNGEON_H
