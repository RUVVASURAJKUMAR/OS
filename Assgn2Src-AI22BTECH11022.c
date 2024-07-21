#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

struct arg {
    int num1;
    int size;
    int *arr;
    int  thread;
};

int yes_count = 0;

int is_odd(int num) {
    if (num % 2 == 0) {
        return 0;
    } else {
        return 1;
    }
}

int is_prime(int num) {
    if(num <= 1) {
        return 0;
    }
    else if(num ==2) {
        return 1;
    }
    else if(num %2==0) {
        return 0;
    }
    for(int i =2;i<=sqrt(num);i++) {
        if(num % i == 0) {
            return 0;
        }
    }
    return 1;
}

bool vampier_num_check(int num) {
    int digit_count = 0;
    int temp = num;
    // printf("num: %d\n",num);
    int freq_array[10];
    for (int i = 0; i < 10; i++) {
        freq_array[i] = 0;
    }
    // for(int i=0;i<10;i++) {
    //                 printf("%d ",freq_array[i]);
    // }
    // printf("%d)\n",num);
    while (temp > 0) {
        freq_array[temp % 10]++;
        digit_count++;
        temp = temp - (temp%10);
        temp = temp/10;
    }
    // for(int i=0;i<10;i++) {
    //                 printf("%d ",freq_array[i]);
    // }
    // printf("\n");
    if(is_odd(digit_count) == 1) {
        return false;
    }
    if (is_prime(num) == 1) {
        return false;
    } 
    else if (digit_count <= 3) {
        return false;
    }

    // int half_digit = digit_count / 2;
    int st_divisors = pow(10, (digit_count / 2) - 1);
    int end_divisors = sqrt(num);

    for (int divisor = st_divisors; divisor <= end_divisors; divisor++) {
        if (num % divisor == 0) {
            // printf("d: %d 0f %d\n",divisor,num);
            int other_divisor = num / divisor;
            // printf("d: %d of %d\n",other_divisor,num);
            if (divisor % 10 == 0 && other_divisor % 10 == 0) {
                // continue;
                // printf("hi ");
            }
            // else if(other_divisor >= num/2) {
            //     // printf("h1 ");
            // }
            else {
                // printf("l1\n");
                int freq_arraycheck[10];
                for (int i = 0; i < 10; i++) {
                    freq_arraycheck[i] = 0;
                }

                int temp_div = divisor;
                while (temp_div > 0) {
                    freq_arraycheck[temp_div % 10]++;
                            temp = temp - (temp%10);

                    temp_div = temp_div / 10;
                }

                temp_div = other_divisor;
                while (temp_div > 0) {
                    freq_arraycheck[temp_div % 10]++;
                            temp = temp - (temp%10);

                    temp_div = temp_div / 10;
                }
                // for(int i=0;i<10;i++) {
                //     printf("%d ",freq_array[i]);
                // }
                // printf("%d\n",num);
                // for(int i=0;i<10;i++) {
                //     printf("%d ",freq_arraycheck[i]);
                // }
                int i;
                for ( i = 0; i < 10; i++) {
                    if (freq_array[i] != freq_arraycheck[i]) {
                        break;
                    }
                }
                if(i==10) {
                    return true;
                }
                
            }
        }
    }

    return false;
}

void *is_vampire(void *arg) {
    struct arg *argument = (struct arg *)arg;
    // yes_count = 0;

    for (int i = 0; i < argument->size; i++) {
        if(argument->arr[i] <= argument->num1 && argument->arr[i] >= 0) {
            // printf("%d\n", argument->arr[i]);
            if (vampier_num_check(argument->arr[i])) {
                    printf("%d: found by thread %d\n",argument->arr[i],argument->thread+1);
                    yes_count++;  
            } 
            else {
                // printf("no\n");
            }
        }
        
    }
    // if (vampier_num_check(688)) {
    //                 printf("%d) yes at thread %d\n",688,argument->thread);
    //         yes_count++;  
    //         } 
    // else {
    //     printf("%d) no\n",688);
    // }
    return NULL;
}

int main() {

    clock_t start_time = clock();
    int num1, num2;

    FILE *file = fopen("input.txt", "r");

    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    fscanf(file, "%d %d", &num1, &num2);

    fclose(file);

    printf("Numbers upto: %d\n", num1);
    printf("N0.of threads : %d\n", num2);

    int nums_per_thread;
    int nums_per_thread_plus;
    if(num1 % num2 == 0) {
        nums_per_thread = num1 / num2;
        nums_per_thread_plus = num1 / num2;
    }
    else {
        nums_per_thread = num1 / num2;
        nums_per_thread_plus = num1 / num2 + 1;
    }
    // printf("l10\n");
    int *input = (int *)malloc(num1 * sizeof(int));
    if (input == NULL) {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    // printf("l10\n");
    pthread_t threads[num2];
    // printf("l10\n");
    // int nums_split[num2][nums_per_thread];
    // printf("l10\n");
    for (int i = 0; i < num1; i++) {
        input[i] = i + 1;
    }
    // printf("l10\n");
    int i = 0;
    struct arg *arg_array[num2];
    for (int j = 0; j < num2; j++) {
        // printf("l11\n");
        arg_array[j] = (struct arg *)malloc(sizeof(struct arg));
        arg_array[j]->size = nums_per_thread_plus;
        arg_array[j]->arr = (int *)malloc(nums_per_thread_plus * sizeof(int));
        arg_array[j]->num1 = num1;
        // zarg_array[j]->thread = j;
        // printf("l12\n");
        for (int k = 0; k < nums_per_thread; k++) {
            arg_array[j]->thread = j;
            arg_array[j]->arr[k] = input[i];
            // printf("%d) %d ",j,arg_array[j]->arr[k]);
            // nums_split[j][k] = input[i];
            i++;
            if(i==num1) {
                break;
            }
        }
        if(i==num1) {
            break;
        }
        // printf("\n");
    }
    if(i != num1) {
        for (int j = 0; j < num2; j++) {
        // arg_array[j] = (struct arg *)malloc(sizeof(struct arg));
        // arg_array[j]->size = nums_per_thread;
        // arg_array[j]->arr = (int *)malloc(nums_per_thread * sizeof(int));
        for (int k = nums_per_thread; k < nums_per_thread+1; k++) {
            arg_array[j]->thread = k;
            arg_array[j]->arr[k] = input[i];
            // printf("%d) %d ",j,arg_array[j]->arr[k]);
            // nums_split[j][k] = input[i];
            i++;
            if(i==num1) {
                break;
            }
        }
        if(i==num1) {
            
            break;
        }
        // printf("\n");
    }
    }

    for (i = 0; i < num2; i++) {
        pthread_create(&threads[i], NULL, is_vampire, (void *)arg_array[i]);
    }

    for (i = 0; i < num2; i++) {
        pthread_join(threads[i], NULL);
    }
        printf("count : %d\n", yes_count);

    for(int i=0;i<num2;i++) {
        free(arg_array[i]);
    }
    free(input);

    clock_t end_time = clock();

        // Calculate the elapsed time in seconds
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Print the elapsed time
    printf("Execution Time: %.6f seconds\n", elapsed_time);
    
    return 0;
}

