# Team-A
Team A

DOTA 2 Analyzer
======
Made by Hsiang-Yin(Sean) Hsieh, Tinh La, Jamella Pescasio, Sky Wu, Wyland Lau
### What is it?

This is a web application that allows you to search and find various analytics about dota 2, such as winrates, trueskill and more.

### Requirements

* Node.js
* React
* Express
* MinGW

### How to compile
Before doing anything specific to Ubuntu or Windows...
1. Go into the file directory called *api* and run `npm install --save`
2. Go into the file directory called *frontend* and run `npm install --save`
3. Open 3 terminal/command prompt windows or use VSCode

You should have the *npm package manager* to use this project

`git clone https://github.com/ucr-cs180-spring21/Team-A.git` to download the project

You will automatically start with the Linux Version, use `git checkout Windows-Platform` to get the Windows Version

#### For Ubuntu/Linux
2. Locate *api* and *frontend* in two of your terminals
3. Run `npm start` in those two terminals
4. Locate *backend/src/app* in your last terminal
5. Run `cmake .` in the terminal and wait for the makefile
6. Run `make` and then run `.\backend.out`
7. The application should now be open and running on your preferred web browser

#### For Windows
1. Locate *api* and *frontend* in two of your terminals
2. Run `npm start` in those two terminals
3. Locate *backend/src/app* in your last terminal
4. Run `cmake .` in the terminal and wait for the makefile
5. Run `cmake --build .` and then run `.\Debug\backend.out.exe`
   * You may need Visual Studio Build Tools for this step
   * Go to https://visualstudio.microsoft.com/downloads/
   * Under **Tools for Visual Studio 2019**, look for and download **Build Tools for Visual Studio 2019**
7. The application should now be open and running on your preferred web browser
