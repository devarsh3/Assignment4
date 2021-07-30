## Assignment 4 - Banker's Algorithm

# Project Title:
Banker's Algorithm - CP 386 A4

# Authors / Contributors: 
Devarsh Patwa (190417940)
Mahek Shah (190734000)

# Github 
Devarsh Patwa - devarsh3
Mahek SHah - mahekshah28
Repository - https://github.com/devarsh3/Assignment4


# Installation:
- Open the Linux Terminal and go to the directory where the files are kept using the 'cd' command
- Run the '$ gcc -o Question1 Question1.c -lpthread' command to compile the file
- Run the '$ ./Question1 10 5 7 8' command to run the compiled file

# Motivation: 
The motivation behind this assignment is to apply theortical knowledge taught and apply it into a practical setting. Thus, giving us more hands on expierence when working with different operating systems along with the different concepts required to further strenghten our understanding of the topic in this case the banker's algorithm.

# Features: 
This algorithm will take into consideration requests from "n" customers for "m" resource types, if requests satisfies algorithm it will print state of the system. Uses the process of allocating resources to obtain safest sequences then to add them onto 2 dimensional array. This assignment uses the following commands:
RQ - Requesting resources
RL - Releasing resources
Status - print the information
Run - executes customers as threads in a safe sequence
exit - exits the system

# Screenshots:
![IMG_5906](https://user-images.githubusercontent.com/75551489/127713313-dfe5e8d6-d4c4-459d-a122-22943c591181.jpeg)
![IMG_5907](https://user-images.githubusercontent.com/75551489/127713318-1719452b-db76-493f-a217-fa005f16d94f.jpeg)
![IMG_5908](https://user-images.githubusercontent.com/75551489/127713319-6627661b-d7a0-4ffa-a9be-39b77603c541.jpeg)

# Individual Contribution:

Both of us took the time to actively zoom call over the course of this assignment. We decided to split off some of the work, Devarsh decided to work on the Banker's Algorithm individually and I, Mahek decided to work on the runthread function. The rest of the functions along with the README file were done together during the zoom call over the course of the week. 

# Test Cases:

The commands we tested were:
./Question1 10 5 7 8
RQ 0 1 0 0 1
RQ 1 1 1 1 1
RQ 2 2 2 2 2
RQ 3 1 1 1 1
RQ 4 1 0 0 0
Status
Run
exit

We got the exact same output as expected for the most part except for the available resources.

# About Developers:
Mahek Shah (190734000): 3rd year Computer Science student at Wilfrid Laurier University 

Devarsh Patwa (190417940): 3rd year Computer Science student at Wilfrid Laurier University 

# License: 
MIT License

Copyright (c) 2021 devarsh3

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
