
void slow_multiply(int ny, int nm, int nx, const double* D1, const double* D2, double* result) {

    int ny1 = ny;
    int nx1 = nm;
    //int ny2 = nm;
    int nx2 = nx;

    for (int i = 0; i < ny1; i++) {
        for (int j = 0; j < nx2; j++) {

            double e = 0;
            for (int k = 0; k < nx1; k++) {
                e += D1[i*nx1+k] * D2[k*nx2+j];
            }
            result[i*nx2+j] = e;
        }
    }
}