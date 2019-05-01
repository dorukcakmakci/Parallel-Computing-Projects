gcc -pg -o lbp_seq util.c lbp_seq.c -lm
gcc -pg -o lbp_omp -fopenmp util.c lbp_omp.c -lm