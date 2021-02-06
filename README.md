# ASCII Dungeon Generator
C++ program that generates two dimensional array of chars that looks like a dungeon. 

## Usage
Simply, pass the dungeon's desired height and width to the constructor when
instantiating `Dungeon` object. After object is created, you can set the dungeon's room parameters by calling following methods:  
* `setMin(height,width)` sets the minimum possible height and width of the room. `4,4` is default.  
* `setMax(height,width)` sets the maximum possible height and width of the room. `D_HEIGHT/4, D_WIDTH/7` is default.  
* `setChars(wall,floor,nothing)` sets the characters that will represent walls, floor and nothing. `'#', '-', '.'` is default.  
* `setMinRoomNum(num)` sets the minimum possible number of rooms (!It is not guaranteed that number of rooms will be equal to this). `30` is default.  

After all parameters are set, call `generate` function to generate the dungeon. Further, treat an object as you would treat two dimensional array.    
  
`C++11` is required.

## Examples
![Screenshot1](https://github.com/ChechenItza/dungeon_generator/blob/screenshots/Screenshots/example.png)

![Screenshot2](https://github.com/ChechenItza/dungeon_generator/blob/screenshots/Screenshots/2016-05-25-1464121531.png)

![Screenshot3](https://github.com/ChechenItza/dungeon_generator/blob/screenshots/Screenshots/2016-05-25-1464118403.png)
