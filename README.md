# Software Development for Algorithmic Problems
### Project 1 2021-2022
**Searching and Clustering Vectors (C++)**

### Our team:
**Ioannis  Georgopoulos**, sdi1800026

**Vasileios Markopoulos**, sdi1800108

This project contains programs that find a vector's nearest vectors in space, given an input file containing vectors. The search is performed by utilizing probabilistic algorithms *LSH* and *Hypercube Projection* respectively. It also includes a program that clusters vectors in discrete groups, using either exhaustive search (*LLLoyd's algorithm*) or reverse range search with the probabilistic algorithms mentioned above.

## Project Structure:
- **bin**
  - exec files
- **cluster**
  - inc
  - src
- **hypercube**
  - inc
  - src
- **inputFiles**
- **lib**
- **lsh**
  - inc
  - src
- makefile
- **outputFiles**
- README

The **bin** folder contains the executable files created by the makefile.

The **cluster**, **hypercube** and **lsh** folders contain headers and source files for their respective programs.

The **inputFiles**  folder ideally contains input files used by the application, to reduce clutter.

The **outputFiles** folder ideally contains output files produced by the application, to reduce clutter.

The **lib** folder contains header and source files for utilities (functions, classes and structs) used in all 3 programs.

## Compilation
From the main project folder, the included *makefile* can be used.
- *make [lsh.out | cube.out | cluster.out]* produces the filename given in the *bin* folder.
- *make* (or *make all*) produces all three files in the *bin* folder.
- *make clean* deletes files in the bin folder, as well as output files produced.

## Using the programs
- Commands to use the 3 programs, with parameters tailored for the input_small_files:
  - lsh:
  - `./bin/lsh.out -i inputFiles/input_small_id -q inputFiles/query_small_id -k 4 -L 5 -o outputFiles/output -N 1 -R 300`
  - hypercube:
  - `./bin/cube.out -i inputFiles/input_small_id -q inputFiles/query_small_id -k 14 -M 10 -probes 2 -o outputFiles/output -N 1 -R 300`
  - cluster:
  - `./bin/cluster.out -i inputFiles/input_small_id -c inputFiles/cluster.conf -o outputFiles/output -m Classic`

#### Detailed information for each program:
- The LSH program is launched using
  - `./lsh -i <input_file> -q <query_file> -k <int> -L <int> -o <output_file> -N <number of nearest> -R <radius>`.
- The Hypercube program is launched using
  - `./cube -i <input_file> -q <query_file> -k <int> -M <int> -probes <int> -o <output_file> -N <number of nearest> -R <radius>`.
- The cluster program is launched using
  - `./cluster -i <input_file> -c <config_file> -o <output_file> -complete (optional) -m <Classic | LSH | Hypercube>`.
- If an argument is not given, the user is asked to provide it when the program launches, or to press enter to continue with the default value.
- When programs LSH and Hypercube are completed, the user is asked if they want to rerun with different arguments, typing CONT, or terminate the program by typing TERM.

#### Input File Structure
- For the input file, every input vector is represented by a line. The first word is the vector's ID (can be string or int), and it is followed by numbers (integers or floating point numbers) separated by spaces, which represent the vector's value in each dimension. The vector's dimension is implied by the amount of numbers provided in the line (excluding the first ID string, if it was also an int). The query file follows the same pattern.
