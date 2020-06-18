# ENGR101-Project-3
Github for team project 3 for ENGR101

## TEAM 3 PROJECT PLAN.

#### User info and roles:

[Project Manager]
RN: Sam Milburn, UN: milbursamu, ID: 300509843, E: sammilburn1916@gmail.com,
GitHub: the-hacker-known-as-anonymous

[Software Architect]
RN: Reilly Oldham UN: oldhamreil, ID: 300534051, E: oldhamreil@myvuw.ac.nz, 
Github: ProRedCat

[Developer]
RN: Tyler Andrews, UN: andrewtyle, ID: 300425252, E: tyler.andrews.home@gmail.com, 
GitHub: Got-any-pizza

[Developer]
RN: Joud Asfari, UN: asfarijoud, ID: 300448284, E: joudzasfari@gmail.com, 
GitHub: cucumber-turtle

#### Plan Timeline

| Date | Objectives | Due date | Item due | Conflicts | Tasks|
| --- | --- | --- | --- | --- | --- |
| ... | Starting and planning of the project. | 4th June | Testing code | COMP112 6-7 or 7. | | [All] Complete AVC plan   \n
Sam M - SFML install testing. Write test cases for the team.
Sam M- Ensures Plan is done. Help to install SFML for the team.
Reilly- Ensure Robot is built.
Reilly- Get/save pictures from the input. |
|4th June| Efficiently and uniformly implementing Robot behaviour to specifications. | 11th June | Core & Completion code | CONFLICTS HERE... |
[ALL] Uniform input and effort.
Reilly - General structure and algorithm building.
Sam - Organizing progress and sorting the documentation. 
Tyler - documenting and recording progress, extending algorithmic work.
Joud - documenting and recording progress, extending algorithmic work. |
| 11th June | Project functionality & completion | 17th June | Challenge Code | CONFLICTS HERE... |
[ALL]
Working on completeness of the project for all specifications.
Reilly - extending algorithmic functionality.
Sam - Ensuring completeness of the project and conclusion.
Tyler - Implementing further functionality.
Joud - Implementing further functionality. |  
| 17th June | Report write-up| 19th June | Report | CONFLICTS HERE... | 
[ALL] Writing report collectively through discord. |







## Guide to installation and running the Project.

#### Step (1)
Download the contents of this GitHub and unpack them in to a file.

#### Step (2)
Download and install the MingW64 compiler. This should be Linux and Windows compatible.
The hyperlink is at: https://mingw-w64.org/doku.php/download

#### Step (3)
Download and unpack the SFML library in to the same directory as "AVC_Robot" and "AVC_Server".
The hyperlink is at: https://www.sfml-dev.org/download.php
Version as of writing this guide is 2.5.1. The directory should be called therefore: SFML-2.5.1

#### Step (4)
Compile server3.cpp in "AVC_Server" and robot.cpp in "AVC_Robot".
The MingW command should be g++ [server3.cpp or robot.cpp] -I SFML-2.5.1/include -L SFML-2.5.1/lib -l sfml-window -l sfml-audio -l sfml-graphics -l sfml-system -l sfml-network -o [server3.exe or robot.exe]
Also provided is a precompiled .exe in this GitHub, if that's easier.


#### Step (5)
Navigate to AVC_Server and start server3.exe **first**, and then navigate to AVC_Robot and start robot.exe after that.
Find the opened executable called "Global view". This should run the the maze-navigating robot!
Have fun!
