# 0D-Tidal-Range (formerly BarraCUDA)

## 0D Modelling of tidal range schemes.

### Compilation with g++
The easiest way to compile the model on Mac/Linux, available on Windows as well (other options are available). All source files are in the src/ directory.
	
	$g++ -std=c++11 main.cpp -o BarraCUDA.out

(Use BarraCUDA.exe on Windows)

### To Do:
	- Improve documentation...
	- Better example models.
	- Develop ancillary services operation.

### Origins
Based off Jingjings rebuild of Reza's 0D model, but in C++ as opposed to Fortran with the aim of making a modern built and stored project.

### Notes:
  1. This model is mainly a learning project for me, testing the varius bits aong the way. 
  2. Try to use the word 'scheme' not 'Barrage' or 'Lagoon'
