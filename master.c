#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <memory.h>
#include <math.h>

int** tab;
int R;

void array_init(size_t R){
    tab = (int**)malloc(sizeof(int*) * R);
    for(int i = 0; i < R; i++){
        tab[i] = calloc(R, sizeof(int));
    }
}

void write_array(){
    FILE* f = fopen("data", "w+");
    if(f == NULL){
        perror(NULL);
        exit(0);
    }

    char buff[4096];
    for(int i=0; i<R; i++){
        for(int j=0; j<R; j++) {
            snprintf(buff, 4096, "%d %d %d\n", i, j, tab[i][j]);
            if(fwrite(buff, sizeof(char), strlen(buff), f) != strlen(buff)){
                perror(NULL);
                exit(0);
            }
        }
    }
    fclose(f);
}

double * parse_integer_sequence(char *sequence, char delimiter){
    int integer_number = 0;
    char *w = sequence;
    while(*w != NULL){
        if(*w != delimiter){
            integer_number++;
            while(*w != NULL && *w != delimiter) w++;
        } else w++;
    }

    double *res = malloc(sizeof(double) * integer_number);

    w = sequence;
    int i = 0;
    while(*w != NULL){
        if(*w != delimiter){
            res[i] = atof(w);
            while(*w != NULL && *w != delimiter) w++;
            i++;
        } else w++;
    }
    return res;
}
void draw_plot(char** argv){
    FILE *gnuplotPipe = popen ("gnuplot", "w");
    if(gnuplotPipe == NULL){
        perror(NULL);
    }

    char xrange[32];
    sprintf(xrange, "set xrange [0:%d]\n", (atoi(argv[2])-1));
    char yrange[32];
    sprintf(yrange, "set yrange [0:%d]\n", (atoi(argv[2])-1));

    printf("%s\n", xrange);
    printf("%s\n", yrange);

    fprintf(gnuplotPipe, "set view map\n");
    fprintf(gnuplotPipe, xrange);
    fprintf(gnuplotPipe, yrange);
    fprintf(gnuplotPipe, "plot 'data' with image\n");

    fflush(gnuplotPipe);
    getc(stdin);
    fclose(gnuplotPipe);
}

int main(int argc, char** argv){
    if(argc != 3){
        printf("Specify path and R");
        return 0;
    }

    char* path = argv[1];
    R = atoi(argv[2]);

    int status;
    status = mkfifo(path, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
    if(status == -1){
        perror(NULL);
        return 0;
    }

    array_init(R);

    FILE* f = fopen(path, "r");
    if(f == NULL){
        perror(NULL);
    }

    char buff[256];
    while(fgets(buff, 256, f) != NULL){
        double * r = parse_integer_sequence(buff, ' ');
        size_t x = (size_t)lround((r[0] + 2) / 3.0 * (R-1)) % R;
        size_t y = (size_t)lround((1.0+r[1]) / 2.0 * (R-1)) % R;
        tab[x][y] = (int)r[2];
    }

    write_array();
    fclose(f);
    draw_plot(argv);

    return 0;
}