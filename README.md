# PythonChess2.0
Work in progress, see a similar version implemented fully in python at https://github.com/TylerRLowe/PythonChess
#How it works#
The interface between python an c relies only on a 64 bit long, the programs both determine based on any changes in that what move was made.
Similarly, the C engine determines color based on if the first board passed has any peices moved.
In C, each piece has a uint64 with a 1 if there is a piece there and a 0 if free. The Python is based on a combanation between my previous code and the pygame tutorial, with the engine interface added.
