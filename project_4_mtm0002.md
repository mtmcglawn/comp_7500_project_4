# Project 4

Matthew Tyler McGlawn

4/24/2022

## 1. Design

The design that I used was the original concept design by Dr. Qin.

## 2. Performance metrics

Because performance metrics were optional and I have much going on, I decided not to proceed with any performance metrics.

## 3. Implementation

I originally started to write my program in the order of how the driver calls each function.
This turned out to cause me to spend more time debugging than originally planned, so I switched up to writing them as they were declared in the header file instead.
When implementing each function I would run the program to insure that it would still compile, would not crash, and compare the output of the program to the expected output of that was given as an example.

## 4. Difficulties

The main difficulty that I experienced was a mistyped variable - an i should have been a j.
This caused me several hours of debugging to figure out.
Another difficulty that I experienced was that I recently started a new job and am in the middle of moving cities.
This allowed me to only work on the project early in the morning on little sleep, or late at night extremely tired.
I believe this fed into the mistyped variable.

## 5. Sample Running

Sample Input:
```
$ ./cpmRun
```

Sample Output:
```
DIRECTORY LISTING
mytestf1.txt 15874
holefile.txt 1152
shortf.ps 1032
mytestf. 1026
FREE BLOCK LIST: (* means in-use)
  0: * . * . * . * . * . * . * . * . 
 10: * . . . . . . . . . . . . . . . 
 20: * . . . * . . . . . . . . . . . 
 30: * * . . . . . . . . . . . . . . 
 40: . . . . . . . . . . . . . . . . 
 50: . . . . . . . . . . . . . . . . 
 60: . . . . . . . . . . . . . . . . 
 70: . . . . . . . . . . . . . . . . 
 80: . . . . . . . . . . . . . . . . 
 90: . . . . . . . . . . . . . . . . 
 a0: * . . . * . . . . . . . . . . . 
 b0: . . . . . . . . . . . . . . . . 
 c0: . . . . . . . . . . . . . . . . 
 d0: . . . . . . . . . . . . . . . . 
 e0: . . . . . . . . . . . . . . . . 
 f0: . * . * . * . * . * . * . * . * 
DIRECTORY LISTING
mytestf1.txt 15874
holefile.txt 1152
mytestf. 1026
cpmRename return code = 0,
DIRECTORY LISTING
mytest2.tx 15874
holefile.txt 1152
mytestv2.x 1026
FREE BLOCK LIST: (* means in-use)
  0: * . * . * . * . * . * . * . * . 
 10: * . . . . . . . . . . . . . . . 
 20: * . . . * . . . . . . . . . . . 
 30: . . . . . . . . . . . . . . . . 
 40: . . . . . . . . . . . . . . . . 
 50: . . . . . . . . . . . . . . . . 
 60: . . . . . . . . . . . . . . . . 
 70: . . . . . . . . . . . . . . . . 
 80: . . . . . . . . . . . . . . . . 
 90: . . . . . . . . . . . . . . . . 
 a0: * . . . * . . . . . . . . . . . 
 b0: . . . . . . . . . . . . . . . . 
 c0: . . . . . . . . . . . . . . . . 
 d0: . . . . . . . . . . . . . . . . 
 e0: . . . . . . . . . . . . . . . . 
 f0: . * . * . * . * . * . * . * . * 
 ```
