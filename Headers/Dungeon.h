#ifndef DUNGEON_H
#define DUNGEON_H

#include <vector>

//READ THIS:
//The look of outcoming result depends on a few variables that you can adjust to your needs:
//
// ||| Dungeon.h |||
// MIN_WIDTH           //NOTE:
// MIN_HEIGHT          //If you change one of these consider adjusting others too
// MAX_WIDTH           //for the better-looking result
// MAX_HEIGHT
// MIN_ROOM_NUM
//
// ||| Constants.h |||
// D_HEIGHT
// D_WIDTH

namespace dng{
    class Dungeon {
    public:
        Dungeon(const int, const int);
        ~Dungeon();
        char* operator[](const int);

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
        std::vector<Room> room_vec_;
        const int D_HEIGHT_,D_WIDTH_;
        int x_pos_, y_pos_;
        int counter_;
        char** dungeon_;
        bool is_executed_;
    };
}

#endif //DUNGEON_H
