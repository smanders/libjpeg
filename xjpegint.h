/*
 * xjpegint.h
 *
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file provides common declarations for the various JPEG modules.
 * These declarations are considered internal to the JPEG library; most
 * applications using the library shouldn't need to include this file.
 */


/* Declarations for both compression & decompression */

typedef enum {			/* Operating modes for buffer controllers */
	JBUF_PASS_THRU,		/* Plain stripwise operation */
	/* Remaining modes require a full-image buffer to have been created */
	JBUF_SAVE_SOURCE,	/* Run source subobject only, save output */
	JBUF_CRANK_DEST,	/* Run dest subobject only, using saved data */
	JBUF_SAVE_AND_PASS	/* Run both subobjects, save output */
} J_BUF_MODE;

/* Values of global_state field (jdapi.c has some dependencies on ordering!) */
#define CSTATE_START	100	/* after create_compress */
#define CSTATE_SCANNING	101	/* start_compress done, write_scanlines OK */
#define CSTATE_RAW_OK	102	/* start_compress done, write_raw_data OK */
#define CSTATE_WRCOEFS	103	/* jpeg_write_coefficients done */
#define DSTATE_START	200	/* after create_decompress */
#define DSTATE_INHEADER	201	/* reading header markers, no SOS yet */
#define DSTATE_READY	202	/* found SOS, ready for start_decompress */
#define DSTATE_PRELOAD	203	/* reading multiscan file in start_decompress*/
#define DSTATE_PRESCAN	204	/* performing dummy pass for 2-pass quant */
#define DSTATE_SCANNING	205	/* start_decompress done, read_scanlines OK */
#define DSTATE_RAW_OK	206	/* start_decompress done, read_raw_data OK */
#define DSTATE_BUFIMAGE	207	/* expecting jpeg_start_output */
#define DSTATE_BUFPOST	208	/* looking for SOS/EOI in jpeg_finish_output */
#define DSTATE_RDCOEFS	209	/* reading file in jpeg_read_coefficients */
#define DSTATE_STOPPING	210	/* looking for EOI in jpeg_finish_decompress */


/* Declarations for compression modules */

/* Master control module */
struct jpeg_comp_master_xp {
  JMETHOD(void, prepare_for_pass_xp, (j_compress_ptr cinfo));
  JMETHOD(void, pass_startup_xp, (j_compress_ptr cinfo));
  JMETHOD(void, finish_pass_xp, (j_compress_ptr cinfo));

  /* State variables made visible to other modules */
  boolean call_pass_startup;	/* True if pass_startup must be called */
  boolean is_last_pass;		/* True during last pass */
};

/* Main buffer control (downsampled-data buffer) */
struct jpeg_c_main_controller_xp {
  JMETHOD(void, start_pass_xp, (j_compress_ptr cinfo, J_BUF_MODE pass_mode));
  JMETHOD(void, process_data_xp, (j_compress_ptr cinfo,
			       JSAMPARRAYXP input_buf, JDIMENSION *in_row_ctr,
			       JDIMENSION in_rows_avail));
};

/* Compression preprocessing (downsampling input buffer control) */
struct jpeg_c_prep_controller_xp {
  JMETHOD(void, start_pass_xp, (j_compress_ptr cinfo, J_BUF_MODE pass_mode));
  JMETHOD(void, pre_process_data_xp, (j_compress_ptr cinfo,
				   JSAMPARRAYXP input_buf,
				   JDIMENSION *in_row_ctr,
				   JDIMENSION in_rows_avail,
				   JSAMPIMAGEXP output_buf,
				   JDIMENSION *out_row_group_ctr,
				   JDIMENSION out_row_groups_avail));
};

/* Coefficient buffer control */
struct jpeg_c_coef_controller_xp {
  JMETHOD(void, start_pass_xp, (j_compress_ptr cinfo, J_BUF_MODE pass_mode));
  JMETHOD(boolean, compress_data_xp, (j_compress_ptr cinfo,
				   JSAMPIMAGEXP input_buf));
};

/* Colorspace conversion */
struct jpeg_color_converter_xp {
  JMETHOD(void, start_pass_xp, (j_compress_ptr cinfo));
  JMETHOD(void, color_convert_xp, (j_compress_ptr cinfo,
				JSAMPARRAYXP input_buf, JSAMPIMAGEXP output_buf,
				JDIMENSION output_row, int num_rows));
};

/* Downsampling */
struct jpeg_downsampler_xp {
  JMETHOD(void, start_pass_xp, (j_compress_ptr cinfo));
  JMETHOD(void, downsample_xp, (j_compress_ptr cinfo,
			     JSAMPIMAGEXP input_buf, JDIMENSION in_row_index,
			     JSAMPIMAGEXP output_buf,
			     JDIMENSION out_row_group_index));

  boolean need_context_rows;	/* TRUE if need rows above & below */
};

/* Forward DCT (also controls coefficient quantization) */
struct jpeg_forward_dct_xp {
  JMETHOD(void, start_pass_xp, (j_compress_ptr cinfo));
  /* perhaps this should be an array??? */
  JMETHOD(void, forward_DCT_xp, (j_compress_ptr cinfo,
			      jpeg_component_info * compptr,
			      JSAMPARRAYXP sample_data, JBLOCKROW coef_blocks,
			      JDIMENSION start_row, JDIMENSION start_col,
			      JDIMENSION num_blocks));
};

/* Entropy encoding */
struct jpeg_entropy_encoder_xp {
  JMETHOD(void, start_pass_xp, (j_compress_ptr cinfo, boolean gather_statistics));
  JMETHOD(boolean, encode_mcu_xp, (j_compress_ptr cinfo, JBLOCKROW *MCU_data));
  JMETHOD(void, finish_pass_xp, (j_compress_ptr cinfo));
};

/* Marker writing */
struct jpeg_marker_writer_xp {
  JMETHOD(void, write_file_header_xp, (j_compress_ptr cinfo));
  JMETHOD(void, write_frame_header_xp, (j_compress_ptr cinfo));
  JMETHOD(void, write_scan_header_xp, (j_compress_ptr cinfo));
  JMETHOD(void, write_file_trailer_xp, (j_compress_ptr cinfo));
  JMETHOD(void, write_tables_only_xp, (j_compress_ptr cinfo));
  /* These routines are exported to allow insertion of extra markers */
  /* Probably only COM and APPn markers should be written this way */
  JMETHOD(void, write_marker_header_xp, (j_compress_ptr cinfo, int marker,
				      unsigned int datalen));
  JMETHOD(void, write_marker_byte_xp, (j_compress_ptr cinfo, int val));
};


/* Declarations for decompression modules */

/* Master control module */
struct jpeg_decomp_master_xp {
  JMETHOD(void, prepare_for_output_pass_xp, (j_decompress_ptr cinfo));
  JMETHOD(void, finish_output_pass_xp, (j_decompress_ptr cinfo));

  /* State variables made visible to other modules */
  boolean is_dummy_pass;	/* True during 1st pass for 2-pass quant */
};

/* Input control module */
struct jpeg_input_controller_xp {
  JMETHOD(int, consume_input_xp, (j_decompress_ptr cinfo));
  JMETHOD(void, reset_input_controller_xp, (j_decompress_ptr cinfo));
  JMETHOD(void, start_input_pass_xp, (j_decompress_ptr cinfo));
  JMETHOD(void, finish_input_pass_xp, (j_decompress_ptr cinfo));

  /* State variables made visible to other modules */
  boolean has_multiple_scans;	/* True if file has multiple scans */
  boolean eoi_reached;		/* True when EOI has been consumed */
};

/* Main buffer control (downsampled-data buffer) */
struct jpeg_d_main_controller_xp {
  JMETHOD(void, start_pass_xp, (j_decompress_ptr cinfo, J_BUF_MODE pass_mode));
  JMETHOD(void, process_data_xp, (j_decompress_ptr cinfo,
			       JSAMPARRAYXP output_buf, JDIMENSION *out_row_ctr,
			       JDIMENSION out_rows_avail));
};

/* Coefficient buffer control */
struct jpeg_d_coef_controller_xp {
  JMETHOD(void, start_input_pass_xp, (j_decompress_ptr cinfo));
  JMETHOD(int, consume_data_xp, (j_decompress_ptr cinfo));
  JMETHOD(void, start_output_pass_xp, (j_decompress_ptr cinfo));
  JMETHOD(int, decompress_data_xp, (j_decompress_ptr cinfo,
				 JSAMPIMAGEXP output_buf));
  /* Pointer to array of coefficient virtual arrays, or NULL if none */
  jvirt_barray_ptr *coef_arrays;
};

/* Decompression postprocessing (color quantization buffer control) */
struct jpeg_d_post_controller_xp {
  JMETHOD(void, start_pass_xp, (j_decompress_ptr cinfo, J_BUF_MODE pass_mode));
  JMETHOD(void, post_process_data_xp, (j_decompress_ptr cinfo,
				    JSAMPIMAGEXP input_buf,
				    JDIMENSION *in_row_group_ctr,
				    JDIMENSION in_row_groups_avail,
				    JSAMPARRAYXP output_buf,
				    JDIMENSION *out_row_ctr,
				    JDIMENSION out_rows_avail));
};

/* Marker reading & parsing */
struct jpeg_marker_reader_xp {
  JMETHOD(void, reset_marker_reader_xp, (j_decompress_ptr cinfo));
  /* Read markers until SOS or EOI.
   * Returns same codes as are defined for jpeg_consume_input:
   * JPEG_SUSPENDED, JPEG_REACHED_SOS, or JPEG_REACHED_EOI.
   */
  JMETHOD(int, read_markers_xp, (j_decompress_ptr cinfo));
  /* Read a restart marker --- exported for use by entropy decoder only */
  jpeg_marker_parser_method_xp read_restart_marker;

  /* State of marker reader --- nominally internal, but applications
   * supplying COM or APPn handlers might like to know the state.
   */
  boolean saw_SOI;		/* found SOI? */
  boolean saw_SOF;		/* found SOF? */
  int next_restart_num;		/* next restart number expected (0-7) */
  unsigned int discarded_bytes;	/* # of bytes skipped looking for a marker */
};

/* Entropy decoding */
struct jpeg_entropy_decoder_xp {
  JMETHOD(void, start_pass_xp, (j_decompress_ptr cinfo));
  JMETHOD(boolean, decode_mcu_xp, (j_decompress_ptr cinfo,
				JBLOCKROW *MCU_data));

  /* This is here to share code between baseline and progressive decoders; */
  /* other modules probably should not use it */
  boolean insufficient_data;	/* set TRUE after emitting warning */
};

/* Inverse DCT (also performs dequantization) */
typedef JMETHOD(void, inverse_DCT_method_ptr_xp,
		(j_decompress_ptr cinfo, jpeg_component_info * compptr,
		 JCOEFPTR coef_block,
		 JSAMPARRAYXP output_buf, JDIMENSION output_col));

struct jpeg_inverse_dct_xp {
  JMETHOD(void, start_pass_xp, (j_decompress_ptr cinfo));
  /* It is useful to allow each component to have a separate IDCT method. */
  inverse_DCT_method_ptr_xp inverse_DCT[MAX_COMPONENTS];
};

/* Upsampling (note that upsampler must also call color converter) */
struct jpeg_upsampler_xp {
  JMETHOD(void, start_pass_xp, (j_decompress_ptr cinfo));
  JMETHOD(void, upsample_xp, (j_decompress_ptr cinfo,
			   JSAMPIMAGEXP input_buf,
			   JDIMENSION *in_row_group_ctr,
			   JDIMENSION in_row_groups_avail,
			   JSAMPARRAYXP output_buf,
			   JDIMENSION *out_row_ctr,
			   JDIMENSION out_rows_avail));

  boolean need_context_rows;	/* TRUE if need rows above & below */
};

/* Colorspace conversion */
struct jpeg_color_deconverter_xp {
  JMETHOD(void, start_pass_xp, (j_decompress_ptr cinfo));
  JMETHOD(void, color_convert_xp, (j_decompress_ptr cinfo,
				JSAMPIMAGEXP input_buf, JDIMENSION input_row,
				JSAMPARRAYXP output_buf, int num_rows));
};

/* Color quantization or color precision reduction */
struct jpeg_color_quantizer_xp {
  JMETHOD(void, start_pass_xp, (j_decompress_ptr cinfo, boolean is_pre_scan));
  JMETHOD(void, color_quantize_xp, (j_decompress_ptr cinfo,
				 JSAMPARRAYXP input_buf, JSAMPARRAYXP output_buf,
				 int num_rows));
  JMETHOD(void, finish_pass_xp, (j_decompress_ptr cinfo));
  JMETHOD(void, new_color_map_xp, (j_decompress_ptr cinfo));
};


/* Miscellaneous useful macros */

#undef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#undef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))


/* We assume that right shift corresponds to signed division by 2 with
 * rounding towards minus infinity.  This is correct for typical "arithmetic
 * shift" instructions that shift in copies of the sign bit.  But some
 * C compilers implement >> with an unsigned shift.  For these machines you
 * must define RIGHT_SHIFT_IS_UNSIGNED.
 * RIGHT_SHIFT provides a proper signed right shift of an INT32 quantity.
 * It is only applied with constant shift counts.  SHIFT_TEMPS must be
 * included in the variables of any routine using RIGHT_SHIFT.
 */

#ifdef RIGHT_SHIFT_IS_UNSIGNED
#define SHIFT_TEMPS	INT32 shift_temp;
#define RIGHT_SHIFT(x,shft)  \
	((shift_temp = (x)) < 0 ? \
	 (shift_temp >> (shft)) | ((~((INT32) 0)) << (32-(shft))) : \
	 (shift_temp >> (shft)))
#else
#define SHIFT_TEMPS
#define RIGHT_SHIFT(x,shft)	((x) >> (shft))
#endif


/* Short forms of external names for systems with brain-damaged linkers. */

#ifdef NEED_SHORT_EXTERNAL_NAMES
#define jinit_compress_master	jICompress
#define jinit_c_master_control_xp	jICMasterXp
#define jinit_c_main_controller	jICMainC
#define jinit_c_prep_controller	jICPrepC
#define jinit_c_coef_controller	jICCoefC
#define jinit_color_converter_xp	jICColorXp
#define jinit_downsampler	jIDownsampler
#define jinit_forward_dct	jIFDCT
#define jinit_huff_encoder	jIHEncoder
#define jinit_phuff_encoder	jIPHEncoder
#define jinit_marker_writer	jIMWriter
#define jinit_master_decompress_xp	jIDMasterXp
#define jinit_d_main_controller	jIDMainC
#define jinit_d_coef_controller	jIDCoefC
#define jinit_d_post_controller	jIDPostC
#define jinit_input_controller_xp	jIInCtlrXp
#define jinit_marker_reader_xp	jIMReaderXp
#define jinit_huff_decoder_xp	jIHDecoderXp
#define jinit_phuff_decoder_xp	jIPHDecoderXp
#define jinit_inverse_dct_xp	jIIDCTXp
#define jinit_upsampler_xp		jIUpsamplerXp
#define jinit_color_deconverter_xp	jIDColorXp
#define jinit_1pass_quantizer_xp	jI1QuantXp
#define jinit_2pass_quantizer_xp	jI2QuantXp
#define jinit_merged_upsampler_xp	jIMUpsamplerXp
#define jinit_memory_mgr_xp	jIMemMgrXp
#define jdiv_round_up		jDivRound
#define jround_up		jRound
#define jcopy_sample_rows_xp	jCopySamplesXp
#define jcopy_block_row		jCopyBlocks
#define jzero_far		jZeroFar
#define jpeg_zigzag_order	jZIGTable
#define jpeg_natural_order	jZAGTable
#endif /* NEED_SHORT_EXTERNAL_NAMES */


/* Compression module initialization routines */
EXTERN(void) jinit_compress_master_xp JPP((j_compress_ptr cinfo));
EXTERN(void) jinit_c_master_control_xp JPP((j_compress_ptr cinfo,
					 boolean transcode_only));
EXTERN(void) jinit_c_main_controller_xp JPP((j_compress_ptr cinfo,
					  boolean need_full_buffer));
EXTERN(void) jinit_c_prep_controller_xp JPP((j_compress_ptr cinfo,
					  boolean need_full_buffer));
EXTERN(void) jinit_c_coef_controller_xp JPP((j_compress_ptr cinfo,
					  boolean need_full_buffer));
EXTERN(void) jinit_color_converter_xp JPP((j_compress_ptr cinfo));
EXTERN(void) jinit_downsampler_xp JPP((j_compress_ptr cinfo));
EXTERN(void) jinit_forward_dct_xp JPP((j_compress_ptr cinfo));
EXTERN(void) jinit_huff_encoder_xp JPP((j_compress_ptr cinfo));
EXTERN(void) jinit_phuff_encoder_xp JPP((j_compress_ptr cinfo));
EXTERN(void) jinit_marker_writer_xp JPP((j_compress_ptr cinfo));
/* Decompression module initialization routines */
EXTERN(void) jinit_master_decompress_xp JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_d_main_controller_xp JPP((j_decompress_ptr cinfo,
					  boolean need_full_buffer));
EXTERN(void) jinit_d_coef_controller_xp JPP((j_decompress_ptr cinfo,
					  boolean need_full_buffer));
EXTERN(void) jinit_d_post_controller_xp JPP((j_decompress_ptr cinfo,
					  boolean need_full_buffer));
EXTERN(void) jinit_input_controller_xp JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_marker_reader_xp JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_huff_decoder_xp JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_phuff_decoder_xp JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_inverse_dct_xp JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_upsampler_xp JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_color_deconverter_xp JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_1pass_quantizer_xp JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_2pass_quantizer_xp JPP((j_decompress_ptr cinfo));
EXTERN(void) jinit_merged_upsampler_xp JPP((j_decompress_ptr cinfo));
/* Memory manager initialization */
EXTERN(void) jinit_memory_mgr_xp JPP((j_common_ptr cinfo));

/* Utility routines in jutils.c */
EXTERN(long) jdiv_round_up JPP((long a, long b));
EXTERN(long) jround_up JPP((long a, long b));
EXTERN(void) jcopy_sample_rows_xp JPP((JSAMPARRAYXP input_array, int source_row,
				    JSAMPARRAYXP output_array, int dest_row,
				    int num_rows, JDIMENSION num_cols));
EXTERN(void) jcopy_block_row JPP((JBLOCKROW input_row, JBLOCKROW output_row,
				  JDIMENSION num_blocks));
EXTERN(void) jzero_far JPP((void FAR * target, size_t bytestozero));
/* Constant tables in jutils.c */
#if 0				/* This table is not actually needed in v6a */
extern const int jpeg_zigzag_order[]; /* natural coef order to zigzag order */
#endif
extern const int jpeg_natural_order[]; /* zigzag coef order to natural order */

/* Suppress undefined-structure complaints if necessary. */

#ifdef INCOMPLETE_TYPES_BROKEN
#ifndef AM_MEMORY_MANAGER	/* only jmemmgr.c defines these */
struct jvirt_sarray_control_xp { long dummy; };
struct jvirt_barray_control { long dummy; };
#endif
#endif /* INCOMPLETE_TYPES_BROKEN */
