/*
 *  Copyright (c) 2012 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef VP9_ENCODER_DENOISER_H_
#define VP9_ENCODER_DENOISER_H_

#include "vp10/encoder/block.h"
#include "vpx_scale/yv12config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MOTION_MAGNITUDE_THRESHOLD (8 * 3)

typedef enum vp10_denoiser_decision {
  COPY_BLOCK,
  FILTER_BLOCK
} VP9_DENOISER_DECISION;

typedef struct vp10_denoiser {
  YV12_BUFFER_CONFIG running_avg_y[MAX_REF_FRAMES];
  YV12_BUFFER_CONFIG mc_running_avg_y;
  int increase_denoising;
  int frame_buffer_initialized;
} VP9_DENOISER;

void vp10_denoiser_update_frame_info(VP9_DENOISER *denoiser,
                                    YV12_BUFFER_CONFIG src,
                                    FRAME_TYPE frame_type,
                                    int refresh_last_frame,
#if !CONFIG_EXT_REFS && CONFIG_BIDIR_PRED
                                    int refresh_bwd_ref_frame,
#endif  // !CONFIG_EXT_REFS && CONFIG_BIDIR_PRED
                                    int refresh_alt_ref_frame,
                                    int refresh_golden_frame);

void vp10_denoiser_denoise(VP9_DENOISER *denoiser, MACROBLOCK *mb,
                          int mi_row, int mi_col, BLOCK_SIZE bs,
                          PICK_MODE_CONTEXT *ctx);

void vp10_denoiser_reset_frame_stats(PICK_MODE_CONTEXT *ctx);

void vp10_denoiser_update_frame_stats(MB_MODE_INFO *mbmi,
                                     unsigned int sse, PREDICTION_MODE mode,
                                     PICK_MODE_CONTEXT *ctx);

int vp10_denoiser_alloc(VP9_DENOISER *denoiser, int width, int height,
                       int ssx, int ssy,
#if CONFIG_VP9_HIGHBITDEPTH
                       int use_highbitdepth,
#endif
                       int border);

#if CONFIG_VP9_TEMPORAL_DENOISING
int total_adj_strong_thresh(BLOCK_SIZE bs, int increase_denoising);
#endif

void vp10_denoiser_free(VP9_DENOISER *denoiser);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // VP9_ENCODER_DENOISER_H_