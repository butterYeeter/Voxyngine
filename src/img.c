#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv) {
    if(argc == 4) {
        int w = atoi(argv[1]), h = atoi(argv[2]);
        if(w < 40 || h < 40) exit(2);

        char *buffer = (char*) malloc(w*3*h+1);
        buffer[w*3*h] = '\0';
        for(int i = 0; i < h; i++) {
            for(int j = 0; j < w; j+=3) {
                    buffer[i*w+j] = 0xF;
                    buffer[i*w+j+1] = 0xF;
                    buffer[i*w+j+1] = 0xF;
            }
        }
        
        FILE *file = fopen(argv[3], "w");
        fwrite(buffer, 1, w*h*3, file);
        fclose(file);

        free(buffer); 

    }
    return 0;
}