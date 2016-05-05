# massDelete


This application will delete all files inside a directory given from command line as an argument,

The files are not sorted and deleted as the app seek the file. 

Please use in caution as the app will delete all files without asking for it.

This app is especially useful when you want to delete millions of files inside a directory and you get the error from rm: "argument list too long"
If you suffer from disk empty but you don't have any inodes to use the space, mostly 0 sized millions of files causes this error. Application tested to be faster than rm, rsync and perl alternatives.

# Usage

#### Download the code:
>git clone https://github.com/VeriTeknik/massDelete.git

#### Cd to dir:
>cd massDelete/

#### Compile the code
>make

#### to delete files in /var/lib/session
>./massDelete -d /var/lib/session -v

#### to delete files in /tmp with 2500 microseconds sleep between each deletion
>./massDelete -d /tmp -v -s 2500

#Changelog
Version 0.4
	Added Average deletes per second
Version 0.3
	Added sleep between each delete cycle to drop disk utilization
	
Version 0.2
	Added Ctrl + C handler, now you can see the number of deleted files on hitting CTRL + C
	
Version 0.1
	Initial Release

