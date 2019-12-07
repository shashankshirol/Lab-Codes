#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int arr[100];

struct arg_struct{
    int arg1;
    int arg2;
};

int isPrime(int n){
    for(int i = 2; i <= sqrt(n); i++){
        if(n%i == 0)
            return 0;
    }
    return 1;
}

void* get_prime(void* arguments){
    struct arg_struct *args = (struct arg_struct *)arguments;
    int m = args -> arg1;
    int n = args -> arg2;
    int k = 0;
    for(int i = m; i <= n; i++){
        if(isPrime(i)){
            arr[k] = i;
            k++;
        }
    }

    return (void*)k;
}

int main(){
    pthread_t thread;
    struct arg_struct args;
    printf("Enter the limits: \n");
    scanf("%d %d", &args.arg1, &args.arg2);
    pthread_create(&thread, 0, get_prime, (void*)&args);
    int n;
    pthread_join(thread, (void**)&n);
    printf("List of primes: \n");
    for(int i = 0; i<n; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
    exit(0);
}