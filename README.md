# Description
## warp:
### A tool to change between directories
### Usage: 
```
warp <arg2>
```
arg1: directory-name  -   .   ..   ~ {null}

## peek: 
### A  tool to peek into files
### Usage: 
```
peek <flags> <arg1>
```
arg1: directory-name  -   .   ..   ~ {null}
flags: -l -a
## pastevents:
### A tool to print past events
### Usage: 
```
pastevents
```
### pastevents purge: removes all past events
```
pastevents purge
```
### pastevents execute i : executes the ith past event
```
pastevents execute i
```
## all system commands are supported:
### for background:
```
sys_com &
```
### for sequential
```
sys_com1 ; sys_com2; ...
```
## proclore
### used to print details of a process
```
proclore p_id
```
### if p_id is not given, detials of current process are printed
### all pastevents can be found in history.txt
## seek
### Used to look for an exact string in file_names or directory names
```
seek <flags> <arg1> <arg2>
```
### flags: -d -e -f
### arg1: target string
### arg2: target directory, if not given, current directory will be searched
# Assumptions
## Multi line commands are not supported