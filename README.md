# Repository with orthodox file manager implementation
#### Team: Lebyak Markiyan, Katya Detsyk
#### KManager (Katya Marki manager)
![](icons/logo_100.png)

### About
Orthodox file managers (sometimes abbreviated to "OFM") or command-based file managers are text-menu based file managers,
that commonly have three windows (two panels and one command line window). Orthodox file managers are one of the longest 
running families of file managers, preceding graphical user interface-based types.
Moreover, this file manager is cross-platform and works fine on:
- Windows 10
- MacOS Big Sur
- Ubuntu 20.04


### Dependencies
```
Qt5
Python
Libarchive
```

### Build && Run :
Build:
```
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release  -G"Unix Makefiles" ..
$ make
```
Then, to run file manager:
```
$ ./KManager
```

### Build && Run in QtCreator:
If you are running this file manager in QtCreator you need to change one line in archives.h:
```
std::string name = "../zip.py ";
```
to:
```
std::string name = "../File_Manager_OS-master/zip.py ";
```
