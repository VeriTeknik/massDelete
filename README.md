# massDelete


This application will delete all files inside a directory given from command line as an argument,

The files are not sorted and deleted as the app seek the file.

Please use in caution as the app will delete all files without asking for it.

# Usage

- compile the code
- chmod + x massDelete

## delete files in /var/lib/session

./massDelete -d /var/lib/session -v

#Changelog

Version 0.2
	Added Ctrl + C handler, now you can see the number of deleted files on hitting CTRL + C
	
Version 0.1
	Initial Release

