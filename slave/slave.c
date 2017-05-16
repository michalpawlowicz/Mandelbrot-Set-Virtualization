#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

#define RE_MAX 1
#define RE_MIN -2
#define IM_MAX 1
#define IM_MIN -1

int N, K;
char* path;

struct Imaginary_Number{
    double re;
    double im;
};

double drand ( double low, double high ) {
    return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}

void add(struct Imaginary_Number *A, struct Imaginary_Number *B){
    A->re += B->re;
    A->im += B->im;
}


double module_squared(struct Imaginary_Number* Z){ return (Z->im * Z->im + Z->re * Z->re); }

struct Imaginary_Number* z_sqr(struct Imaginary_Number *Z){
    double re = Z->re;
    double im = Z->im;
    Z->re = (re*re - im*im);
    Z->im = (2*re*im);
    return Z;
}

int iters(struct Imaginary_Number *c){
    int it = 0;
    struct Imaginary_Number z;
    z.im = 0;
    z.re = 0;
    while(it < K && module_squared(&z) < 4){
        add(z_sqr(&z), c);
        it++;
    }
    return it;
}

struct Imaginary_Number* random_c(){
    struct Imaginary_Number* C = malloc(sizeof(struct Imaginary_Number));
    C->re = drand(RE_MIN, RE_MAX);
    C->im = drand(IM_MIN, IM_MAX);
    return C;
}

int main(int argc, char** argv){
    if(argc != 4){
        printf("Specify path, N, K\n");
        return 0;
    }

    path = argv[1];
    N = atoi(argv[2]);
    K = atoi(argv[3]);

    // Open pipeline
    FILE* f = NULL;
    if((f = fopen(path, "w")) == NULL){
        perror(NULL);
    }

    //za wolno
    srand(time(NULL));

    for(int i = 0; i < N; i++){
        struct Imaginary_Number *C = random_c();
	double x, y;
	x = C->re;
	y = C->im;
        char buff[256];
        snprintf(buff, 256, "%f %f %d\n", x, y, iters(C));
        fwrite(&buff, sizeof(char), strlen(buff), f);
	fflush(f);
	free(C);
    }
    fclose(f);
    return 0;
}
