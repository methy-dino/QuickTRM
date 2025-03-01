# QuickTRM
QuickTRM is a utility tool for your terminal, able to save and load terminal windows.

## How does it work?
let's suppose you have a little project in the folder ```/home/user/Hello World```, and you use the terminal to handle it's compilation, you'd notice it's annoying to navigate to that folder manually, that's the problem quickTRM aims to solve!\
after installation it handles all the work by itself!

###  Initialize your QuickTRM
#### Global method
This method allows you to install quickTRM and for it to be able as any User, albeit each user will have it's own saves folder. \
**1.** download the QuickTRM raw executable from this repo (beware that this uses the <unistd.h> library and is exclusive to UNIX compliant systems).\
**2.** use a command to move or copy the file to your "bin" folder (i.e. ```cp <quickTRM path> /bin``` for copying, and ```mv <quickTRM path> /bin```.\
**3.** running "quickTRM" in the terminal will create the files needed for function on the logged user's storage.
#### User exclusive method
In case you want quickTRM to be used only on a specific user you can follow these steps: \
**1.** download the QuickTRM raw executable from this repo (beware that this uses the <unistd.h> library and is exclusive to UNIX compliant systems).\
**2.** set up a bash variable to the executable, on most linux systems you can open ~/.bashrc and insert this shell command on it's end ```alias quickTRM=```, followed by the filepath (inside of double quotes i.e. ```alias quickTRM="/~/apps/quickTRM"```) of the QuickTRM you installed.\
**3.** run "quickTRM" in your terminal, it should automatically create the files needed for it to function.

### the QuickTRM commands

#### ```create <name>```
running ```quickTRM create <name>``` will attempt to create a shell save in it's internal folder (~/.quickTRM), if a save already exists with that name, it will ask confirmation to edit it.\
Then, it will open the file (which has basic structure to run a new terminal, and cd to the directory of the terminal it was run on) on the text editor specified on the configuration file (which can be edited with the ```quickTRM editor <command>``` and ```quickTRM terminal <command>``` commands).

#### ```delete <names>```
deletes all saves listed, which are separated by spaces, if it fails to delete any, it will throw an error and continue. \
can also be run using ```delete "ALL SAVES"``` to clear all the saves stored (PS. there is no undoing).

#### ```load <names>```
loads the specified terminals within names, separated by spaces, throws an error for each that is non-existant. \
can also be run using ```load "ALL SAVES"``` to load all the terminals stored, beware of the quantity! 

#### ```load local <names>```
loads the specified terminals within names, exactly as load does, except it does not open new terminal windows.
can also be run using ```load local "ALL SAVES"``` to load all the terminals stored, might be useful if you're making saves to do menial tasks. \
the load keyword can be omitted, in that case, the program will check if the first argument is valid, allowing for calls like ```quickTRM <names>``` and ```quickTRM local <names>```

#### ```list```
lists all the saved files on the .quickTRM directory, also runs if called ```quickTRM view```.

#### ```clone```
clones the current terminal by opening another terminal in the same directory.

#### ```editor <name>```
changes the editor on the configurations file (default is ```vim ```), don't forget to quote the name and put a space in the end.

#### ```terminal <name>```
changes the terminal on the configurations file (default is ```gnome-terminal -- ```), don't forget to quote the name and put a space in the end, beware that it's needed to append the syntax to load a script (ps always uses ```working-directory=""``` to clone terminals) on startup (in gnome it's ``` -- ```).

#### ```settings```
display the currently set terminal, and editor, between quotes.
#### ```export <fPath>```
copies all the saved data of quickTRM to the specified directory
#### ```import <fPath>```
imports the shell files from <fPath> to quickTRM's internal folder.
#### ```help```
displays information about all the quickTRM arguments and commands.

