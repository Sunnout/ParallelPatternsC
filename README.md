
# README #

### What is this repository for? ###

* This is the base code for the cp2019-20_project_50095_51910 for CP 2019-20

### Note to professor ###
There are two implementations of Scatter. You should add the following macros to unit.c and pattern.h:
```
#define scatter(dest, src, n, size, filter) scatterAtomic(dest, src, n, size, filter)
#define scatter(dest, src, n, size, filter) scatterNotAtomic(dest, src, n, size, filter)
```
Then comment the one you don't want to test.

### How do I get set up? ###

```
git clone https://github.com/AndreOA/cp2019-20_project_50095_51910.git
cd cp2019-20_project_50095_51910
cd code
make
```

### How do I run tests? ###
* Testing: `./main -t [testNumber] [inputSize]`
* Testing with debug: `./main -d -t [testNumber] [inputSize]`
* Testing with validation: `./main -v [inputSize]`
* Testing with sequential version: `./main -s -t [testNumber] [inputSize]`

There are several test sets, ranging from **0-6** and **8-10**.

To vary the thread number just set the OMP variable like so:
* Testing with threads: `OMP_NUM_THREADS=[numberOfThreads] ./main -t [testNumber] [inputSize]`


### How do I run bash scripts? ###
To run tests using our bash scripts do:
*	```
     cd bash
    ```
* Bash with sequential version: `./test0_seq.sh`
* Bash with parallel version: `./test0_par.sh`

There are several bash scripts:
* **test0-2** and **4-10** have **sequential and parallel** versions;
* **test3** only has a **parallel** version.

### How do I run python scripts? ###
*	```
     cd python
     python testScanStairEffect_plots.py
    ```
There are several python scripts:
* testSpeedupMapReduce_plots.py
* testSpeedupFarm_plots.py
* testSpeedupGatherScatter_plots.py
* testSpeedupPipelineLight_plots.py
* testSpeedupScan_plots.py
* testScanStairEffect_plots.py
* testSpeedupPipelineHeavy_plots.py

### How do I access the plots? ###
*	```
     cd plots
    ```

### Who do I talk to? ###
* Project developers:
    * Ema Vieira, 50095. er.vieira@campus.fct.unl.pt
    * André Atalaia, 51910. a.atalaia@campus.fct.unl.pt
