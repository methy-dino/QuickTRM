# QuickTRM
QuickTRM is a utility tool for your terminal, able to save and load terminal windows.

## How does it work?
let's suppose you have a little project in the folder ```/home/user/Hello World```, and you use the terminal to handle it's compilation, you'd notice it's annoying to navigate to that folder manually, that's the problem quickTRM aims to solve!\
after installation it handles all the work by itself!\

###  Initialize your QuickTRM
**1.** download the QuickTRM raw executable from this repo (beware that this uses the <unistd.h> library and is exclusive to UNIX compliant systems).\
**2.** set up a bash variable to the executable, on most linux systems you can open ~/.bashrc and insert this shell command on it's end ```alias quickTRM=```, followed by the filepath (inside of double quotes i.e. ```alias quickTRM="/~/apps/quickTRM"```) of the QuickTRM you installed.\
**3.** run "quickTRM" in your terminal, it should automatically create the things needed for it to function.\

### the QuickTRM commands

#### create <name>
running ```quickTRM create <name>``` will attempt to create a shell save in it's internal folder (~/.quickTRM), if a save already exists with that name, it will ask confirmation to edit it.\
Then, it will open the file (which has basic structure to run a new terminal, and cd to the directory of the terminal it was run on) on the text editor specified on the configuration file (which can be edited with the ```quickTRM editor <command>``` and ```quickTRM terminal <command>``` commands).\

#### delete <names>
deletes all files listed, which are separated by spaces, if it fails to delete any, it will throw an error and continue.\

#### load <name>
loads the specified within the name, throws an error if it doesn't exist.\

#### list
lists all the saved files on the .quickTRM directory, also runs if called ```quickTRM view```.\

#### clone 
clones the current terminal by opening another terminal in the same directory.\

#### editor <name>
changes the editor on the configurations file (default is ```vim ```), don't forget to quote the name and put a space in the end.\

#### terminal <name>
changes the terminal on the configurations file (default is ```gnome-terminal -- ```), don't forget to quote the name and put a space in the end, beware that it's needed to append the syntax to load a script on startup (in gnome it's ``` -- ```).\

#### settings
display the currently set terminal, and editor, between quotes
