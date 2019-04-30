gcc -pg -o lbp_omp -fopenmp util.c lbp_omp.c -lm
for num_of_threads in 1 2 4 6 8 16
do
    # run parallel code here
    # echo -e "\tFor num_of_threads = $num_of_threads"
    echo -e "\n---------------------------------------\nProfile for K = 10, num_of_threads = $num_of_threads\n---------------------------------------\n" >> prof_omp.txt
    OMP_NUM_THREADS=$num_of_threads
    ./lbp_omp 10 
    gprof -b lbp_omp gmon.out >> prof_omp.txt
done