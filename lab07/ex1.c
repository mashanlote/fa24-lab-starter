#include "ex1.h"
#include <emmintrin.h>
#include <immintrin.h>
#include <stdio.h>
#include <time.h>
#include <x86intrin.h>

long long int sum(int vals[NUM_ELEMS]) {
  clock_t start = clock();

  long long int sum = 0;
  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    for (unsigned int i = 0; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        sum += vals[i];
      }
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  long long int sum = 0;

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
      if (vals[i] >= 128)
        sum += vals[i];
      if (vals[i + 1] >= 128)
        sum += vals[i + 1];
      if (vals[i + 2] >= 128)
        sum += vals[i + 2];
      if (vals[i + 3] >= 128)
        sum += vals[i + 3];
    }

    // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
    // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
    // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
    for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        sum += vals[i];
      }
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m512i _512 = _mm512_set1_epi32(
      127); // This is a vector with 127s in it... Why might you need this?
  long long int result = 0; // This is where you should put your final result!
  /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    /* YOUR CODE GOES HERE */
    __m512i sum_vec = _mm512_setzero_si512();
    __m512i tmp;
    __mmask16 cmp_res;
    for (unsigned int i = 0; i < NUM_ELEMS / 16 * 16; i += 16) {
      tmp = _mm512_loadu_si512((__m512i *)(vals + i));
      cmp_res = _mm512_cmpgt_epi32_mask(tmp, _512);
      sum_vec = _mm512_mask_add_epi32(sum_vec, cmp_res, sum_vec, tmp);
    }
    int tmp_arr[16];
    _mm512_storeu_si512((__m512i *)tmp_arr, sum_vec);
    for (unsigned int i = 0; i < 16; i++) {
      result += tmp_arr[i];
    }
    for (unsigned int i = NUM_ELEMS / 16 * 16; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        result += vals[i];
      }
    }
  }

  /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m512i _512 = _mm512_set1_epi32(
      127); // This is a vector with 127s in it... Why might you need this?
  long long int result = 0;
  /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    /* YOUR CODE GOES HERE */
    /* Copy your sum_simd() implementation here, and unroll it */
    __m512i sum_vec = _mm512_setzero_si512();
    __m512i tmp1;
    __m512i tmp2;
    __m512i tmp3;
    __m512i tmp4;
    __mmask16 cmp1;
    __mmask16 cmp2;
    __mmask16 cmp3;
    __mmask16 cmp4;
    for (unsigned int i = 0; i < NUM_ELEMS / 64 * 64; i += 64) {
      tmp1 = _mm512_loadu_si512((__m512i *)(vals + i));
      cmp1 = _mm512_cmpgt_epi32_mask(tmp1, _512);
      sum_vec = _mm512_mask_add_epi32(sum_vec, cmp1, sum_vec, tmp1);
      tmp2 = _mm512_loadu_si512((__m512i *)(vals + i + 16));
      cmp2 = _mm512_cmpgt_epi32_mask(tmp2, _512);
      sum_vec = _mm512_mask_add_epi32(sum_vec, cmp2, sum_vec, tmp2);
      tmp3 = _mm512_loadu_si512((__m512i *)(vals + i + 32));
      cmp3 = _mm512_cmpgt_epi32_mask(tmp3, _512);
      sum_vec = _mm512_mask_add_epi32(sum_vec, cmp3, sum_vec, tmp3);
      tmp4 = _mm512_loadu_si512((__m512i *)(vals + i + 48));
      cmp4 = _mm512_cmpgt_epi32_mask(tmp4, _512);
      sum_vec = _mm512_mask_add_epi32(sum_vec, cmp4, sum_vec, tmp4);
    }
    int tmp_arr[64];
    _mm512_storeu_si512((__m512i *)tmp_arr, sum_vec);
    for (unsigned int i = 0; i < 16; i++) {
      result += tmp_arr[i];
    }
    for (unsigned int i = NUM_ELEMS / 64 * 64; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        result += vals[i];
      }
    }

    /* Hint: you'll need 1 or maybe 2 tail cases here. */
  }

  /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}
