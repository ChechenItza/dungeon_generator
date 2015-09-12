# ASCII Dungeon Generator
C++ program that generates two dimensional array of chars that looks like a dungeon. 

## Usage
Simply, pass the dungeon's desired height and width to the constructor when
instantiating an `dng::Dungeon` object. Further, treat an object as you would
treat an two dimensional array.    
Also, you can change the characters that will represent walls, floor and nothing before contructing the dungeon. They're all static so you access them like this: `Dungeon::wall` (default is '#'), `Dungeon::floor` (default is '-') and `Dungeon::nothing` (default is '.')  
  
`C++11` is required.

## rand branch
Instead of `rnd::randomize()`, standart `rand()` function is being used where it's possible.  
If unsure or not satisfied with the looks, use rand branch.

## Examples
### Master branch
![Screenshot1](https://github.com/7Y3RPXK3ETDCNRDD/dungeon_generator/blob/screenshots/Screenshots/example.png)

### rand branch
![Screenshot2](https://github.com/7Y3RPXK3ETDCNRDD/dungeon_generator/blob/screenshots/Screenshots/2015-08-14-1439496514.png)
