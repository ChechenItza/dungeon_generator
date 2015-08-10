# ASCII Dungeon Generator
C++ program that generates two dimensional array of chars that looks like a dungeon. 

## How does it work?
There is two functions in the `Dungeon.h` header: `genDungeon()` and `clrDungeon()`. `genDungeon()` returns `char** dungeon` and `clrDungeon()` deallocates `dungeon` from the memory.  
Size of the dungeon is specified in the `Constants.h` header by `D_WIDTH` and `D_HEIGHT` constant variables. Characters that represent `Wall`, `Floor` and `Nothing` are also defined there.  
  
`C++11` is required.

## Example
![Screenshot1](https://github.com/7Y3RPXK3ETDCNRDD/dungeon_generator/blob/screenshots/Screenshots/example.png)
