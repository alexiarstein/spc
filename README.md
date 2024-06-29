# spc
Lists files within a given directory sorted by size in descending order

## Usage

spc <directory> <number of rows to list>

e.g: ```spc /tmp 5``` 

Would list the files within tmp sorted by size in descending order


## Install: 
```sudo mv spc /usr/local/bin; chmod a+x /usr/local/bin/spc```

## Compile & Install (Requires GCC):
```gcc src/spc.c -o spc; sudo mv spc /usr/local/bin; chmod a+x /usr/local/bin/spc```

Author: Alexia RS <alexia@rockylinux.org>
