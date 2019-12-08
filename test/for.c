#include <stdio.h>

int main() {
    unsigned char a;
    long long cnt = 0;
    for (a = 255; a >= 0; a--) {
        printf ("%lld\n", ++cnt);
    }
    /*for (a = 0; a <= 255; a++) {
        printf ("%lld\n", ++cnt);
    }*/
    /*char b;
    for (b = 127; b >= -128; b--) {
        printf ("%lld\n", ++cnt);
    }*/
    return 0;
}
