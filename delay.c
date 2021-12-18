void tempo(int x) {
    volatile unsigned char j, k;
    int i;
    for (i = 0; i < x; i++) {
        for (j = 0; j < 41; j++) {
            for (k = 0; k < 3; k++);
        }
    }
}
