__kernel void flame(const int N, __global float *mat1, __global float *mat2, __global float *out) {
  int j,k;
  int i = get_global_id(0);
  out[i] = N;
  float tmp;
  for (j = 0; j < N; j++) {
    tmp = 0.0f;
    for (k = 0; k < N; k++) {
      tmp += mat1[i*N+k]*mat2[k*N+j];
    }
    out[i*N+j] = tmp;
  }
}
