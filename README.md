# massDelete


This application will delete all files and directories inside a directory given from command line as an argument, **including recursive deletion of subdirectories**.

The files and directories are not sorted and deleted as the app seeks them. 

Please use with caution as the app will delete all files and directories without asking for confirmation.

This app is especially useful when you want to delete millions of files inside a directory and you get the error from rm: "argument list too long"
If you suffer from disk being empty but you don't have any inodes to use the space, mostly 0-sized millions of files causes this error. Application tested to be faster than rm, rsync and perl alternatives.

**Supports both Linux and macOS with full recursive directory deletion.**

# Usage

Download the code:

```git clone https://github.com/VeriTeknik/massDelete.git```

cd to dir:

```cd massDelete```

compile the code:

```make```

or one can simply use gcc:

```gcc massDelete.c -o massDelete```

to delete all files and directories in ```/var/lib/session``` recursively:

```./massDelete -d /var/lib/session -v```

The ```-v``` means verbose mode, it is optional.

to delete all files and directories in ```/tmp``` with 2500 microseconds sleep between each deletion:

```./massDelete -d /tmp -v -s 2500```

By default, the program removes the empty directory afterwards. To preserve the directory, use the ```-p``` option:

```./massDelete -d /tmp -p```

**New in version 0.6:** The program now recursively deletes directories and their contents by default. This means it can handle complex directory structures like `node_modules`, `.next`, `build` folders, etc.

## Command Line Options

- ```-d <directory>``` - **Required**. Path to directory to delete contents from
- ```-v``` - **Optional**. Verbose mode - shows each file/directory as it's deleted
- ```-s <microseconds>``` - **Optional**. Sleep duration between deletions (useful to reduce disk I/O)
- ```-p``` - **Optional**. Preserve the target directory after deleting all contents
- ```-r``` - **Optional**. Recursive deletion (now enabled by default)

## Examples

Delete everything in a Next.js build directory:
```./massDelete -d /path/to/project/.next -v```

Delete node_modules with verbose output and slower pace:
```./massDelete -d /path/to/project/node_modules -v -s 1000```

Clear cache directory but keep the directory itself:
```./massDelete -d /var/cache/myapp -p```


# Changelog

Version 0.6 (2024)
    **Major Update**: Added recursive directory deletion capability (now default behavior)
    Added macOS support (cross-platform compatibility)
    Enhanced performance for complex directory structures
    Improved error handling and progress reporting
    Now supports deleting directories like node_modules, .next, build folders with millions of files

Version 0.5
    Now removes the empty directory afterwards. If the users wants to keep the empty directory, has to explicitly provide the -p option.

Version 0.4
	Added Average deletes per second
	
Version 0.3
	Added sleep between each delete cycle to drop disk utilization
	
Version 0.2
	Added Ctrl + C handler, now you can see the number of deleted files on hitting CTRL + C
	
Version 0.1
	Initial Release

