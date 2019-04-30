gcc -pg -o lbp_seq util.c lbp_seq.c -lm
./lbp_seq 10
gprof -b lbp_seq gmon.out >> prof_sequential.txt