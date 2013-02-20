/*
 * jcodec.c
 *
 * Copyright (C) 1998, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains utility functions for the JPEG codec(s).
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "xjpeglib.h"
#include "jlossls.h"


/*
 * Initialize the decompression codec.
 * This is called only once, during master selection.
 */

GLOBAL(void)
jinit_d_codec_xp (j_decompress_ptr cinfo)
{
  j_decompress_ptr_xp xinfo = (j_decompress_ptr_xp) cinfo->client_data;
  if (xinfo->lossless_xp) {
#ifdef D_LOSSLESS_SUPPORTED
    jinit_lossless_d_codec_xp(cinfo);
#else
    ERREXIT(cinfo, JERR_NOT_COMPILED);
#endif
  } else
    ERREXIT(cinfo, JERR_NOT_COMPILED);
}
