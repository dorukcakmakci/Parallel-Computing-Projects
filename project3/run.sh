for k in 1 2 5 7 10
do
    # run sequential code here
    # echo "For K = $k"
    echo -e "\n-------------------\nlbp_seq for K = $k\n-------------------\n" >> doruk_cakmakci.output
    ./lbp_seq $k >> doruk_cakmakci.output
    for num_of_threads in 1 2 4 6 8 16
    do
        # run parallel code here
        # echo -e "\tFor num_of_threads = $num_of_threads"
        echo -e "\n---------------------------------------\nlbp_omp for K = $k, num_of_threads = $num_of_threads\n---------------------------------------\n" >> doruk_cakmakci.output
        OMP_NUM_THREADS=$num_of_threads
        ./lbp_omp $k >> doruk_cakmakci.output
    done
done