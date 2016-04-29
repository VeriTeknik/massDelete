# massDelete


This application will delete all files inside a directory given from command line as an argument,

The files are not sorted and deleted as the app seek the file.

Please use in caution as the app will delete all files without asking for it.

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
Version 0.3
	Added sleep between each delete cycle to drop disk utulization
Version 0.2
	Added Ctrl + C handler, now you can see the number of deleted files on hitting CTRL + C
	
Version 0.1
	Initial Release

