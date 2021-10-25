# Robotic car movement and pathfinding simulator

### About

Given a 200x200cm area to roam and scan objects, where and how should the robot move? Using this simulator, you can visualise the where and how!

#### Understanding input.txt

An example of the input file is as follows:

5  
3 19 3  
7 13 1  
9 6 2  
17 2 4  
16 17 3

The first line (N) tells the program how many objects it must go to.  
The following N lines tells the program where the objects are and which side of the square the object is at.  
1 = North  
2 = East  
3 = South  
4 = West

#### Understanding pathing output
f = move forward  
b = move back  
r = turn right  
l = turn left  
s = scan object  
1, 2, ... N = object ID (based on input file)

#### Some points

1. There are some invalid locations such as 1 1, 20 20, etc.  
   Basically the corners of the map because the robot cannot move out of the 200x200 area and the robot is about 30x30cm in size.  
2. There are some invalid facing such as 2 3 4, 19 19 2, etc.  
   Basically if the robot needs to move out of the map to scan the object. The robot needs about a 20cm spacing infront of the object to scan it.
3. If for some reason the executable does not run you will need to download Qt (open source version) and open the source code and build it yourself

---

### Technology used

C++, Qt

---

### Features

#### Load objects

- able to load objects from a text file
- shows error if no file is selected

#### Algorithm selection

- Choose from 3 algorithm and see which algo is the best
- Least turns algorithm
- Closest object algorithm
- Closest offset algorithm

---

### Video
