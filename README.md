# Bshell

Bshell is a simple command line with multiple ordinary and unordinary commands
ready for you to use. The syntax is following

    ./bshl [command] [option]... [args]...

Commands can be chained using pipes,

    ./bshl '[command] [option]... [args]... | [command] [option]... [args]...'

or operators...

    ./bshl '[command] [option]... [args]... && [command] [option]... [args]...'

    ./bshl '[command] > [args]...'

The following builtin commands are available.

|  command           |  description                                        |  syntax
|:-------------------|:-----------------------------------------------------|:-----------------------
|   alias               |   create alias for a  command                        |       ./bshl alias "[command]='[alias]'" ...
 |   alias -p                     | print aliases                                         |   ./bshl alias -p
| cat                  |   concatenate and print content of files              |     ./bshl cat [option].. [file]..
|   clear                |   clear the terminal                                  |     ./bshl clear
|   cp                   |   copy source to dest                                 |     ./bshl cp [source].. [dest]
|   date                 |   displays the current time and date                  |     ./bshl date
 |  echo                 |   write arguments onto STDOUT                         |     ./bshl echo [option].. [file]..
  | grep                 |   search for a pattern in file(s)                      |    ./bshl grep [option].. [file]..
   |head                 |   prints first 10 (by default) lines of file            |   ./bshl head [option].. [file]..
|   help                 |   prints out a syntax summary of commands              |   ./bshl help
|   ls                   |   lists information about file(s)                       |   ./bshl ls [file]..
|   mkdir                |   creates directory(ies)                                |   ./bshl mkdir [directoryname]..
|   mv                   |   rename or move files/directories                      |   ./bshl mv [source].. [dest]
|   rm                   |   removes file(s)                                       |   ./bshl rm [option].. [file]..
|   rmdir                |   removes empty directory(ies)                          |   ./bshl rmdir [directory]..
|   sort                  |  write sorted concantenation of file(s) into STDOUT     |  ./bshl sort [option].. [file]..
|   tac                   |  concatenate and print content of files in reverse      |  ./bshl tac [option].. [file]..
|   tail                  |  prints last 10 (by default) lines of file              |  ./bshl tail [option].. [file]..
|   touch                 |  creates file(s)                                        |  ./bshl touch [file]..
|   unalias               |  remove alias(es)                                       |  ./bshl unalias [aliasname]..
   
The following external commands are available.

|  command           |  description                                        |  syntax
|:-------------------|:-----------------------------------------------------|:-----------------------
 |  procstatus            |  Shows the status of a project such as the number of threds running and the number of allowed cpus       | ./bshl procstatus [PID]
|   proclist              |  Shows a list of PID's of running processes             |  ./bshl proclist
|   prockill              |  Kills a process                                        |  ./bshl prockill [PID]
|   cpuinfo               |  Shows the model name of the cpu and the number of  processors in it.    |  ./bshl cpuinfo
|   history               |  Opens a user interface to navigate through the    history of commands and allows command execution      |  ./bshl history
|   clearhistory          |  Clears the history                                     |  ./bshl clearhistory
 |  createanimation        | Opens a user interface to create animation frames using text to represent each frame. Includes the option to override an animation     |  ./bshl createanimation [name]
|   removeanimation      |   Removes the animation from the list of animations      |  ./bshl removeanimation [name]
|   listanimations       |   Shows the list of existing animations                  |  ./bshl listanimations
|   clearanimations      |   Deletes all existing animations                        |  ./bshl clearanimations
|   animationframecount   |  Shows the number of frames in an animation             |  ./bshl animationframecount
|   runanimation          |  Runs an animation. (Use -l to loop the animation)      |  ./bshl runanimation [option] [name]
|   exec                  | Executes a process or runs shell script                |  ./bshl exec [cmnd]

## Installation procedure

Click on code in the right corner and then download zip. Then, you unzip the file and you will get a repository called bshell with all the libraries and source files inside.

```c
unzip bshell-main.zip && clear && rm -rf bshell-main.zip
```

## Running the app

To start the app simply run the following command in the terminal

```c
make && clear && ./bshl help
```

In the terminal you will see the syntax and all the commands that are available in the bshell terminal. 

### Help command

It allows you to see the available commands. If you have any other issues or inquiries, try checking out the project report.

```
./bshl help
````

To close the app, run the following command

```c
make clean && clear
```

## Uninstallation procedure

To uninstall bshell, simply remove it by using this command

```c
rm -rf bshell-main
```


