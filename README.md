# ASCII Dungeon Generator
C++ program that generates two dimensional array of chars that looks like a dungeon. 

## Usage
There is two functions in the `Dungeon.h` header: `genDungeon()` and `clrDungeon()`. `genDungeon()` returns `char** dungeon` and `clrDungeon()` deallocates `dungeon` from the memory.  
Size of the dungeon is specified in the `Constants.h` header by `D_WIDTH` and `D_HEIGHT` constant variables. Characters that represent `Wall`, `Floor` and `Nothing` are also defined there.  
  
`C++11` is required.

## rand branch
Instead of `rnd::randomize()`, standart `rand()` function is being used if possible.  
If unsure or not satisfied with the looks, use rand branch.

## Examples
### Master branch
![Screenshot1](https://github.com/7Y3RPXK3ETDCNRDD/dungeon_generator/blob/screenshots/Screenshots/example.png)

### rand branch
![Screenshot2](https://github.com/7Y3RPXK3ETDCNRDD/dungeon_generator/blob/screenshots/Screenshots/2015-08-14-1439496514.png)
