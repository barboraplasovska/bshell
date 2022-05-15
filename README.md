The parser and commands part can be compiled by typing the command "make" or 
"make all" into the teminal.

There are two ways to use the Bshell, one of them is interactive and can be found
in the main directory, the other takes in one command per execution and can be
found in the test directory.

list of commands with options for first defense:

    -ls : No options. ls: lists directory contents. Lists the current directory's
                          content  whenever no arguments were passed

    -cat: No options. cat: prints the file's content

    -echo: [-e,-n,-E]. echo: displays a line of text.
           The option -n removes the trailing newline.

    -clear: No options. No arguments either. clear: clears the terminal's screen.

    -cp: No options. cp: copy files and directories.

    -touch: No options. touch: creates new files by taking their names as arguments

    -mv: No options. mv: move (rename) files.
                    case 1: mv file1 file2 : renames file1 to file2
                    case 2: mv file1 dir1/ : moves file1 to dir1/
                    case 3: mv dir1/ dir2/ : (if dir2/ doesn't exist) mv renames
                                             dir1/ to dir2/
                    case 4: mv file1 file2 ... filen dir1/: move multiple files
                                                            to dir1/
    -procstatus: No options.