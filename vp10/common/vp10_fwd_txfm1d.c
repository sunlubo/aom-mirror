/*
 *  Copyright (c) 2015 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "vp10/common/vp10_fwd_txfm1d.h"
#if CONFIG_COEFFICIENT_RANGE_CHECKING
#define range_check(stage, input, buf, size, bit)                         \
  {                                                                       \
    int i, j;                                                             \
    for (i = 0; i < size; ++i) {                                          \
      int buf_bit = get_max_bit(abs(buf[i])) + 1;                         \
      if (buf_bit > bit) {                                                \
        printf("======== %s overflow ========\n", __func__);              \
        printf("stage: %d node: %d\n", stage, i);                         \
        printf("bit: %d buf_bit: %d buf[i]: %d\n", bit, buf_bit, buf[i]); \
        printf("input:\n");                                               \
        for (j = 0; j < size; j++) {                                      \
          printf("%d,", input[j]);                                        \
        }                                                                 \
        printf("\n");                                                     \
        assert(0, "vp10_fwd_txfm1d.c: range_check overflow");             \
      }                                                                   \
    }                                                                     \
  }
#else
#define range_check(stage, input, buf, size, bit) \
  {                                               \
    (void) stage;                                 \
    (void) input;                                 \
    (void) buf;                                   \
    (void) size;                                  \
    (void) bit;                                   \
  }
#endif

void vp10_fdct4_new(const int32_t *input, int32_t *output,
                    const int8_t *cos_bit, const int8_t *stage_range) {
  const int32_t size = 4;
  const int32_t *cospi;

  int32_t stage = 0;
  int32_t *bf0, *bf1;
  int32_t step[4];

  // stage 0;
  range_check(stage, input, input, size, stage_range[stage]);

  // stage 1;
  stage++;
  bf1 = output;
  bf1[0] = input[0] + input[3];
  bf1[1] = input[1] + input[2];
  bf1[2] = -input[2] + input[1];
  bf1[3] = -input[3] + input[0];
  range_check(stage, input, bf1, size, stage_range[stage]);

  // stage 2
  stage++;
  cospi = cospi_arr[cos_bit[stage] - cos_bit_min];
  bf0 = output;
  bf1 = step;
  bf1[0] = half_btf(cospi[32], bf0[0], cospi[32], bf0[1], cos_bit[stage]);
  bf1[1] = half_btf(-cospi[32], bf0[1], cospi[32], bf0[0], cos_bit[stage]);
  bf1[2] = half_btf(cospi[48], bf0[2], cospi[16], bf0[3], cos_bit[stage]);
  bf1[3] = half_btf(cospi[48], bf0[3], -cospi[16], bf0[2], cos_bit[stage]);
  range_check(stage, input, bf1, size, stage_range[stage]);

  // stage 3
  stage++;
  bf0 = step;
  bf1 = output;
  bf1[0] = bf0[0];
  bf1[1] = bf0[2];
  bf1[2] = bf0[1];
  bf1[3] = bf0[3];
  range_check(stage, input, bf1, size, stage_range[stage]);
}

void vp10_fdct8_new(const int32_t *input, int32_t *output,
                    const int8_t *cos_bit, const int8_t *stage_range) {
  const int32_t size = 8;
  const int32_t *cospi;

  int32_t stage = 0;
  int32_t *bf0, *bf1;
  int32_t step[8];

  // stage 0;
  range_check(stage, input, input, size, stage_range[stage]);

  // stage 1;
  stage++;
  bf1 = output;
  bf1[0] = input[0] + input[7];
  bf1[1] = input[1] + input[6];
  bf1[2] = input[2] + input[5];
  bf1[3] = input[3] + input[4];
  bf1[4] = -input[4] + input[3];
  bf1[5] = -input[5] + input[2];
  bf1[6] = -input[6] + input[1];
  bf1[7] = -input[7] + input[0];
  range_check(stage, input, bf1, size, stage_range[stage]);

  // stage 2
  stage++;
  cospi = cospi_arr[cos_bit[stage] - cos_bit_min];
  bf0 = output;
  bf1 = step;
  bf1[0] = bf0[0] + bf0[3];
  bf1[1] = bf0[1] + bf0[2];
  bf1[2] = -bf0[2] + bf0[1];
  bf1[3] = -bf0[3] + bf0[0];
  bf1[4] = bf0[4];
  bf1[5] = half_btf(-cospi[32], bf0[5], cospi[32], bf0[6], cos_bit[stage]);
  bf1[6] = half_btf(cospi[32], bf0[6], cospi[32], bf0[5], cos_bit[stage]);
  bf1[7] = bf0[7];
  range_check(stage, input, bf1, size, stage_range[stage]);

  // stage 3
  stage++;
  cospi = cospi_arr[cos_bit[stage] - cos_bit_min];
  bf0 = step;
  bf1 = output;
  bf1[0] = half_btf(cospi[32], bf0[0], cospi[32], bf0[1], cos_bit[stage]);
  bf1[1] = half_btf(-cospi[32], bf0[1], cospi[32], bf0[0], cos_bit[stage]);
  bf1[2] = half_btf(cospi[48], bf0[2], cospi[16], bf0[3], cos_bit[stage]);
  bf1[3] = half_btf(cospi[48], bf0[3], -cospi[16], bf0[2], cos_bit[stage]);
  bf1[4] = bf0[4] + bf0[5];
  bf1[5] = -bf0[5] + bf0[4];
  bf1[6] = -bf0[6] + bf0[7];
  bf1[7] = bf0[7] + bf0[6];
  range_check(stage, input, bf1, size, stage_range[stage]);

  // stage 4
  stage++;
  cospi = cospi_arr[cos_bit[stage] - cos_bit_min];
  bf0 = output;
  bf1 = step;
  bf1[0] = bf0[0];
  bf1[1] = bf0[1];
  bf1[2] = bf0[2];
  bf1[3] = bf0[3];
  bf1[4] = half_btf(cospi[56], bf0[4], cospi[8], bf0[7], cos_bit[stage]);
  bf1[5] = half_btf(cospi[24], bf0[5], cospi[40], bf0[6], cos_bit[stage]);
  bf1[6] = half_btf(cospi[24], bf0[6], -cospi[40], bf0[5], cos_bit[stage]);
  bf1[7] = half_btf(cospi[56], bf0[7], -cospi[8], bf0[4], cos_bit[stage]);
  range_check(stage, input, bf1, size, stage_range[stage]);

  // stage 5
  stage++;
  bf0 = step;
  bf1 = output;
  bf1[0] = bf0[0];
  bf1[1] = bf0[4];
  bf1[2] = bf0[2];
  bf1[3] = bf0[6];
  bf1[4] = bf0[1];
  bf1[5] = bf0[5];
  bf1[6] = bf0[3];
  bf1[7] = bf0[7];
  range_check(stage, input, bf1, size, stage_range[stage]);
}

void vp10_fdct16_new(const int32_t *input, int32_t *output,
                     const int8_t *cos_bit, const int8_t *stage_range) {
  const int32_t size = 16;
  const int32_t *cospi;

  int32_t stage = 0;
  int32_t *bf0, *bf1;
  int32_t step[16];

  // stage 0;
  range_check(stage, input, input, size, stage_range[stage]);

  // stage 1;
  stage++;
  bf1 = output;
  bf1[0] = input[0] + input[15];
  bf1[1] = input[1] + input[14];
  bf1[2] = input[2] + input[13];
  bf1[3] = input[3] + input[12];
  bf1[4] = input[4] + input[11];
  bf1[5] = input[5] + input[10];
  bf1[6] = input[6] + input[9];
  bf1[7] = input[7] + input[8];
  bf1[8] = -input[8] + input[7];
  bf1[9] = -input[9] + input[6];
  bf1[10] = -input[10] + input[5];
  bf1[11] = -input[11] + input[4];
  bf1[12] = -input[12] + input[3];
  bf1[13] = -input[13] + input[2];
  bf1[14] = -input[14] + input[1];
  bf1[15] = -input[15] + input[0];
  range_check(stage, input, bf1, size, stage_range[stage]);

  // stage 2
  stage++;
  cospi = cospi_arr[cos_bit[stage] - cos_bit_min];
  bf0 = output;
  bf1 = step;
  bf1[0] = bf0[0] + bf0[7];
  bf1[1] = bf0[1] + bf0[6];
  bf1[2] = bf0[2] + bf0[5];
  bf1[3] = bf0[3] + bf0[4];
  bf1[4] = -bf0[4] + bf0[3];
  bf1[5] = -bf0[5] + bf0[2];
  bf1[6] = -bf0[6] + bf0[1];
  bf1[7] = -bf0[7] + bf0[0];
  bf1[8] = bf0[8];
  bf1[9] = bf0[9];
  bf1[10] = half_btf(-cospi[32], bf0[10], cospi[32], bf0[13], cos_bit[stage]);
  bf1[11] = half_btf(-cospi[32], bf0[11], cospi[32], bf0[12], cos_bit[stage]);
  bf1[12] = half_btf(cospi[32], bf0[12], cospi[32], bf0[11], cos_bit[stage]);
  bf1[13] = half_btf(cospi[32], bf0[13], cospi[32], bf0[10], cos_bit[stage]);
  bf1[14] = bf0[14];
  bf1[15] = bf0[15];
  range_check(stage, input, bf1, size, stage_range[stage]);

  // stage 3
  stage++;
  cospi = cospi_arr[cos_bit[stage] - cos_bit_min];
  bf0 = step;
  bf1 = output;
  bf1[0] = bf0[0] + bf0[3];
  bf1[1] = bf0[1] + bf0[2];
  bf1[2] = -bf0[2] + bf0[1];
  bf1[3] = -bf0[3] + bf0[0];
  bf1[4] = bf0[4];
  bf1[5] = half_btf(-cospi[32], bf0[5], cospi[32], bf0[6], cos_bit[stage]);
  bf1[6] = half_btf(cospi[32], bf0[6], cospi[32], bf0[5], cos_bit[stage]);
  bf1[7] = bf0[7];
  bf1[8] = bf0[8] + bf0[11];
  bf1[9] = bf0[9] + bf0[10];
  bf1[10] = -bf0[10] + bf0[9];
  bf1[11] = -bf0[11] + bf0[8];
  bf1[12] = -bf0[12] + bf0[15];
  bf1[13] = -bf0[13] + bf0[14];
  bf1[14] = bf0[14] + bf0[13];
  bf1[15] = bf0[15] + bf0[12];
  range_check(stage, input, bf1, size, stage_range[stage]);

  // stage 4
  stage++;
  cospi = cospi_arr[cos_bit[stage] - cos_bit_min];
  bf0 = output;
  bf1 = step;
  bf1[0] = half_btf(cospi[32], bf0[0], cospi[32], bf0[1], cos_bit[stage]);
  bf1[1] = half_btf(-cospi[32], bf0[1], cospi[32], bf0[0], cos_bit[stage]);
  bf1[2] = half_btf(cospi[48], bf0[2], cospi[16], bf0[3], cos_bit[stage]);
  bf1[3] = half_btf(cospi[48], bf0[3], -cospi[16], bf0[2], cos_bit[stage]);
  bf1[4] = bf0[4] + bf0[5];
  bf1[5] = -bf0[5] + bf0[4];
  bf1[6] = -bf0[6] + bf0[7];
  bf1[7] = bf0[7] + bf0[6];
  bf1[8] = bf0[8];
  bf1[9] = half_btf(-cospi[16], bf0[9], cospi[48], bf0[14], cos_bit[stage]);
  bf1[10] = half_btf(-cospi[48], bf0[10], -cospi[16], bf0[13], cos_bit[stage]);
  bf1[11] = bf0[11];
  bf1[12] = bf0[12];
  bf1[13] = half_btf(cospi[48], bf0[13], -cospi[16], bf0[10], cos_bit[stage]);
  bf1[14] = half_btf(cospi[16], bf0[14], cospi[48], bf0[9], cos_bit[stage]);
  bf1[15] = bf0[15];
  range_check(stage, input, bf1, size, stage_range[stage]);

  // stage 5
  stage++;
  cospi = cospi_arr[cos_bit[stage] - cos_bit_min];
  bf0 = step;
  bf1 = output;
  bf1[0] = bf0[0];
  bf1[1] = bf0[1];
  bf1[2] = bf0[2];
  bf1[3] = bf0[3];
  bf1[4] = half_btf(cospi[56], bf0[4], cospi[8], bf0[7], cos_bit[stage]);
  bf1[5] = half_btf(cospi[24], bf0[5], cospi[40], bf0[6], cos_bit[stage]);
  bf1[6] = half_btf(cospi[24], bf0[6], -cospi[40], bf0[5], cos_bit[stage]);
  bf1[7] = half_btf(cospi[56], bf0[7], -cospi[8], bf0[4], cos_bit[stage]);
  bf1[8] = bf0[8] + bf0[9];
  bf1[9] = -bf0[9] + bf0[8];
  bf1[10] = -bf0[10] + bf0[11];
  bf1[11] = bf0[11] + bf0[10];
  bf1[12] = bf0[12] + bf0[13];
  bf1[13] = -bf0[13] + bf0[12];
  bf1[14] = -bf0[14] + bf0[15];
  bf1[15] = bf0[15] + bf0[14];
  range_check(stage, input, bf1, size, stage_range[stage]);

  // stage 6
  stage++;
  cospi = cospi_arr[cos_bit[stage] - cos_bit_min];
  bf0 = output;
  bf1 = step;
  bf1[0] = bf0[0];
  bf1[1] = bf0[1];
  bf1[2] = bf0[2];
  bf1[3] = bf0[3];
  bf1[4] = bf0[4];
  bf1[5] = bf0[5];
  bf1[6] = bf0[6];
  bf1[7] = bf0[7];
  bf1[8] = half_btf(cospi[60], bf0[8], cospi[4], bf0[15], cos_bit[stage]);
  bf1[9] = half_btf(cospi[28], bf0[9], cospi[36], bf0[14], cos_bit[stage]);
  bf1[10] = half_btf(cospi[44], bf0[10], cospi[20], bf0[13], cos_bit[stage]);
  bf1[11] = half_btf(cospi[12], bf0[11], cospi[52], bf0[12], cos_bit[stage]);
  bf1[12] = half_btf(cospi[12], bf0[12], -cospi[52], bf0[11], cos_bit[stage]);
  bf1[13] = half_btf(cospi[44], bf0[13], -cospi[20], bf0[10], cos_bit[stage]);
  bf1[14] = half_btf(cospi[28], bf0[14], -cospi[36], bf0[9], cos_bit[stage]);
  bf1[15] = half_btf(cospi[60], bf0[15], -cospi[4], bf0[8], cos_bit[stage]);
  range_check(stage, input, bf1, size, stage_range[stage]);

  // stage 7
  stage++;
  bf0 = step;
  bf1 = output;
  bf1[0] = bf0[0];
  bf1[1] = bf0[8];
  bf1[2] = bf0[4];
  bf1[3] = bf0[12];
  bf1[4] = bf0[2];
  bf1[5] = bf0[10];
  bf1[6] = bf0[6];
  bf1[7] = bf0[14];
  bf1[8] = bf0[1];
  bf1[9] = bf0[9];
  bf1[10] = bf0[5];
  bf1[11] = bf0[13];
  bf1[12] = bf0[3];
  bf1[13] = bf0[11];
  bf1[14] = bf0[7];
  bf1[15] = bf0[15];
  range_check(stage, input, bf1, size, stage_range[stage]);
}
