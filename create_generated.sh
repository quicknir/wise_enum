#!/bin/bash

N=${1:-42} #default

awk -v N="$N" '
BEGIN {
  # ARG_N
  printf("#define WISE_ENUM_IMPL_ARG_N(");
  for (i = 1; i <= N; ++i) printf("_%d, ", i);
  printf("N, ...) \\\n  N\n\n");

  # RSEQ_N
  printf("#define WISE_ENUM_IMPL_RSEQ_N() \\\n  ");
  for (i = N; i >= 1; --i) {
    printf("%d", i);
    if (i > 1) printf(", ");
    if ((N - i + 1) % 10 == 0 && i > 1) printf("\\\n  ");
  }
  printf(", 0\n\n");
}
'

awk -v N="$N" '
BEGIN {
  for (i = 1; i <= N; ++i) {
    printf("#define WISE_ENUM_IMPL_LOOP_%d(M, C, D, x", i);
    if (i > 1) printf(", ...");
    printf(") \\\n");
    printf("M(C, x)");
    if (i > 1) {
      printf(" D() WISE_ENUM_IMPL_EXPAND(WISE_ENUM_IMPL_LOOP_%d(M, C, D, __VA_ARGS__))", i - 1);
    }
    printf("\n\n");
  }
}
'
