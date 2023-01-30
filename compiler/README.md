# Work in Progress
To test the current state of the compiler use the following steps

Build:

```
mkdir bin
g++ -std=c++11 src/compiler.cpp -o bin/compiler
```

Run:

```
./bin/compiler ./data/binary_search
```

Observe the contents of the data file (pequin T_I and viper TiTe should be complete so far)
The `data` directory should now contain

```
binary_search.vpex  
binary_search_te.c  
binary_search_TeTs.vpr  
binary_search_ti.c  
binary_search_TiTe.vpr  
binary_search_ts.c  
binary_search_exo.c  
binary_search_v_inp_gen.h  
```