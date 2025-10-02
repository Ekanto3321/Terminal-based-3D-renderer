#include <stdio.h>
#include <string.h>

int main() {
    char luminosity[] = " .c`',^:-~\";!i*+xoO0%#@&8MW$B%RQHNK9DX";
    int len = strlen(luminosity);

    int R = 255;
    int G = 0;
    int B = 0;

    for (int i = 0; i < len; i++) {
        int G = (255 * i) / (len - 1); // from 0 → 255
        printf("\033[38;2;%d;%d;%dm%c\033[0m", R, G, B, luminosity[i]);
    }
    return 0;
}
