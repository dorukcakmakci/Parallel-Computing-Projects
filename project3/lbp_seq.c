#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "util.h"
#include <math.h>   
#include <limits.h>
#include <time.h>

void create_histogram(int * hist, int **img, int num_rows, int num_cols) {

    for (int i = 0; i < 255; i++) {
        hist[i] = 0;
    }

    int lbp[8];

    // first process non border pixels
    for(int r = 1; r < num_rows - 2; r++) {
        for(int c = 1; c < num_cols - 2; c++) {
            
            int center = img[r][c]; // 
            lbp[0] = (center < img[r-1][c-1] ? 1 : 0);
            lbp[1] = (center < img[r-1][c] ? 1 : 0);
            lbp[2] = (center < img[r-1][c+1] ? 1 : 0);
            lbp[3] = (center < img[r][c+1] ? 1 : 0);
            lbp[4] = (center < img[r+1][c+1] ? 1 : 0);
            lbp[5] = (center < img[r+1][c] ? 1 : 0);
            lbp[6] = (center < img[r+1][c-1] ? 1 : 0);
            lbp[7] = (center < img[r][c-1] ? 1 : 0);

            //manually convert binary string to decimal
            int index = 0;

            for(int i = 0; i <= 7; i++) {
                index += pow(2,(7 - i)) * lbp[i];
            }

            hist[index]++;
        }
    }

    

    // process r = 0 case
    for (int c = 1; c < num_cols - 1; c++) {
        int center = img[0][c];
        lbp[0] = 0;
        lbp[1] = 0;
        lbp[2] = 0;
        lbp[3] = (center < img[0][c+1] ? 1 : 0);
        lbp[4] = (center < img[1][c+1] ? 1 : 0);
        lbp[5] = (center < img[1][c] ? 1 : 0);
        lbp[6] = (center < img[1][c-1] ? 1 : 0);
        lbp[7] = (center < img[0][c-1] ? 1 : 0);

        //manually convert binary string to decimal
        int index = 0;

        for(int i = 0; i <= 7; i++) {
            index += pow(2,(7 - i)) * lbp[i];
        }

        hist[index]++;
    }

    // process r = img_height - 1 case
    for (int c = 1; c < num_cols - 1; c++) {
        int center = img[num_rows - 1][c];
        lbp[0] = (center < img[num_rows - 2][c-1] ? 1 : 0);
        lbp[1] = (center < img[num_rows - 2][c] ? 1 : 0);
        lbp[2] = (center < img[num_rows - 2][c+1] ? 1 : 0);
        lbp[3] = (center < img[num_rows - 1][c+1] ? 1 : 0);
        lbp[4] = 0;
        lbp[5] = 0;
        lbp[6] = 0;
        lbp[7] = (center < img[num_rows - 1][c-1] ? 1 : 0);

        //manually convert binary string to decimal
        int index = 0;

        for(int i = 0; i <= 7; i++) {
            index += pow(2,(7 - i)) * lbp[i];
        }

        hist[index]++;
    }

    // process c = 0 case
    for (int r = 1; r < num_rows - 1; r++) {
        int center = img[r][0];
        lbp[0] = 0;
        lbp[1] = (center < img[r-1][0] ? 1 : 0);
        lbp[2] = (center < img[r-1][1] ? 1 : 0);
        lbp[3] = (center < img[r][1] ? 1 : 0);
        lbp[4] = (center < img[r+1][1] ? 1 : 0);
        lbp[5] = (center < img[r+1][0] ? 1 : 0);
        lbp[6] = 0;
        lbp[7] = 0;

        //manually convert binary string to decimal
        int index = 0;

        for(int i = 0; i <= 7; i++) {
            index += pow(2,(7 - i)) * lbp[i];
        }

        hist[index]++;
    }

    // process c = img_width - 1 case
    for (int r = 1; r < num_rows - 1; r++) {
        int center = img[r][num_cols - 1];
        lbp[0] = (center < img[r-1][num_cols - 2] ? 1 : 0);
        lbp[1] = (center < img[r-1][num_cols - 1] ? 1 : 0);
        lbp[2] = 0;
        lbp[3] = 0;
        lbp[4] = 0;
        lbp[5] = (center < img[r+1][num_cols - 1] ? 1 : 0);
        lbp[6] = (center < img[r+1][num_cols - 2] ? 1 : 0);
        lbp[7] = (center < img[r][num_cols - 2] ? 1 : 0);

        //manually convert binary string to decimal
        int index = 0;

        for(int i = 0; i <= 7; i++) {
            index += pow(2,(7 - i)) * lbp[i];
        }

        hist[index]++;
    }

    // process corners of the image

    // r = 0 and c = 0
    int center = img[0][0];
    lbp[0] = 0;
    lbp[1] = 0;
    lbp[2] = 0;
    lbp[3] = (center < img[0][1] ? 1 : 0);
    lbp[4] = (center < img[1][1] ? 1 : 0);
    lbp[5] = (center < img[1][0] ? 1 : 0);
    lbp[6] = 0;
    lbp[7] = 0;

    //manually convert binary string to decimal
    int index = 0;

    for(int i = 0; i <= 7; i++) {
        index += pow(2,(7 - i)) * lbp[i];
    }

    hist[index]++;

    // r = img_height - 1 and c = 0
    center = img[num_rows - 1][0];
    lbp[0] = 0;
    lbp[1] = (center < img[num_rows - 2][0] ? 1 : 0);
    lbp[2] = (center < img[num_rows - 2][1] ? 1 : 0);
    lbp[3] = (center < img[num_rows - 1][1] ? 1 : 0);
    lbp[4] = 0;
    lbp[5] = 0;
    lbp[6] = 0;
    lbp[7] = 0;

    //manually convert binary string to decimal
    index = 0;

    for(int i = 0; i <= 7; i++) {
        index += pow(2,(7 - i)) * lbp[i];
    }

    hist[index]++;

    // r = img_height - 1 and c = img_width - 1
    center = img[num_rows - 1][num_cols - 1];
    lbp[0] = (center < img[num_rows - 2][num_cols - 2] ? 1 : 0);
    lbp[1] = (center < img[num_rows - 2][num_cols - 1] ? 1 : 0);
    lbp[2] = 0;
    lbp[3] = 0;
    lbp[4] = 0;
    lbp[5] = 0;
    lbp[6] = 0;
    lbp[7] = (center < img[num_rows - 1][num_cols - 2] ? 1 : 0);

    //manually convert binary string to decimal
    index = 0;

    for(int i = 0; i <= 7; i++) {
        index += pow(2,(7 - i)) * lbp[i];
    }

    hist[index]++;

    // r = 0 and c = img_width - 1
    center = img[0][num_cols - 1];
    lbp[0] = 0;
    lbp[1] = 0;
    lbp[2] = 0;
    lbp[3] = 0;
    lbp[4] = 0;
    lbp[5] = (center < img[1][num_cols - 1] ? 1 : 0);
    lbp[6] = (center < img[1][num_cols - 2] ? 1 : 0);
    lbp[7] = (center < img[0][num_cols - 2] ? 1 : 0);

    //manually convert binary string to decimal
    index = 0;

    for(int i = 0; i <= 7; i++) {
        index += pow(2,(7 - i)) * lbp[i];
    }

    hist[index]++;
}

double distance(int *a, int *b, int size) {

    double distance = 0.0;
    double running = 0.0;

    for(int i = 0; i < size; i++) {
        running = pow(a[i] - b[i], 2) * 0.5;
        running = (a[i] + b[i] == 0) ? 0 : running / (a[i] + b[i]);
        distance += running;
    }

    return distance;
}

int find_closest(int ***training_set, int num_persons, int num_training, int size, int *test_image) {

    int min_dist =  INT_MAX;
    int min_pid = -1;
    int temp_dist = 0;

    for (int i = 0; i < num_persons; i++) {
        for (int j = 0; j < num_training; j++) {
            temp_dist = distance(training_set[i][j], test_image, size);
            if(temp_dist < min_dist) {
                min_dist = temp_dist;
                min_pid = i+1;
            }
        }
    }

    return min_pid;

}

int main(int * argc, char **argv) {

    clock_t begin = clock();

    
    int k = atoi(argv[1]); // k is the number of training set images, and 20 > k > 0

    // following variables are self-explanatory
    int num_person = 18;
    int img_per_person = 20; 
    int img_width = 200;
    int img_height = 180;
    int bin_count = 255;

    char * img_dir = "./images";
    char * img_name;

    // allocate a 3d array to hold histograms for training and test set images.
    int *** training_set_histograms = malloc(num_person * sizeof(int**));
    int *** test_set_histograms = malloc(num_person * sizeof(int**));
    for(int i = 0; i < num_person; i++) {
        training_set_histograms[i] = alloc_2d_matrix(k, bin_count);
        test_set_histograms[i] = alloc_2d_matrix(20 - k, bin_count);
    }

    //--------------------------- Training Step & Test Step Histogram Creation -----------------------------------------------

    // read image files in the "images/"" directory and generate corresponding histograms
    int cur_person_id;
    int cur_img_id;
    int ** cur_img;
    char * abs_file_path = malloc (19 * sizeof(char));

    for (cur_person_id = 1; cur_person_id <= num_person; cur_person_id++) {
        for (cur_img_id = 1; cur_img_id <= img_per_person; cur_img_id++ ) {

            sprintf(abs_file_path, "%s/%d.%d.txt", img_dir, cur_person_id, cur_img_id);
            cur_img = read_pgm_file(abs_file_path, img_height, img_width); 


            if(cur_img_id > k) {
                // the image is in test set i.e. the images with ids between k + 1 and 20
                create_histogram(test_set_histograms[cur_person_id - 1][cur_img_id - 1 - k], cur_img, img_height, img_width); 
            }
            else {
                // the image is in training set i.e. the images with ids between 1 and k
                create_histogram(training_set_histograms[cur_person_id - 1][cur_img_id - 1], cur_img, img_height, img_width); 
            }

            dealloc_2d_matrix(cur_img, img_height, img_width);
        }

    }

    // now histograms for the training and test set are computed.
    // generate test result s and compare them with correct values.
    int correct_count = 0;
    int pred = -1;
    for( int i = 0; i < num_person; i++) {
        for( int j = 0; j < 20 - k; j++) {
            // find closest training hist.
            pred = find_closest(training_set_histograms, num_person, k,  bin_count, test_set_histograms[i][j]);
            printf("%d.%d.txt %d %d\n", (i+1), (j + k + 1), pred, i+1);
            if(pred == (i+1)){ 
                correct_count++;
            }
        }
    }

    clock_t end = clock();
    double seq_time = (double)(end - begin) * 1000 / CLOCKS_PER_SEC;

    printf("Accuracy: %d correct answers for %d tests\n", correct_count, num_person*(20 - k));
    printf("Sequential time: %f ms\n", seq_time);
    printf("Parallel time: 00.00 ms\n");

    return 0;
}