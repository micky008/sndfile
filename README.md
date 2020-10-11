# sndfile

## Intro :

Programme écrit en C il a été compiler en mingw64 et sur une debian [Buster] x64.  
via msys2 https://www.msys2.org/ package "toolchain" et "sndfile" et "mpg123"

- pour le wav pensez a installer libsndfile-devel et libsndfile  pour compiler et executer
- pour le mp3 pensez a installer libmpg123-dev >= v1.26 et libmpg123 pour compiler et executer



## Pour le wav :

### ligne de compilation
gcc wav.c -l sndfile

## Pour le mp3 :

### ligne de compilation
gcc mp3.c list.c -l mpg123


Le make file viendra plus tard...
