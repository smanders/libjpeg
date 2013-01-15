/*
 * xjpeglib.h
 *
 * Copyright (C) 1991-1998, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file defines the application interface for the JPEG library.
 * Most applications using the library need only include this file,
 * and perhaps jerror.h if they want to know the exact error codes.
 */

#ifndef XJPEGLIB_H
#define XJPEGLIB_H

/*
 * First we include the configuration files that record how this
 * installation of the JPEG library is set up.  jconfig.h can be
 * generated automatically for many systems.  jmorecfg.h contains
 * manual configuration options that most people need not worry about.
 */

#ifndef JCONFIG_INCLUDED	/* in case jinclude.h already did */
#include "jconfig.h"		/* widely used configuration options */
#endif
#include "jmorecfg.h"		/* seldom changed options */
#include "jpeglib.h"


/* Extended record for a compression instance */

typedef struct {
  struct jpeg_destination_mgr_xp * dest_xp;  /* Destination for compressed data */
  /*
   * Links to compression subobjects (methods and private variables of modules)
   */
  struct jpeg_comp_master_xp * master_xp;
  struct jpeg_c_main_controller_xp * main_xp;
  struct jpeg_c_prep_controller_xp * prep_xp;
  struct jpeg_c_coef_controller_xp  * coef_xp;
  struct jpeg_marker_writer_xp * marker_xp;
  struct jpeg_color_converter_xp * cconvert_xp;
  struct jpeg_downsampler_xp * downsample_xp;
  struct jpeg_forward_dct_xp * fdct_xp;
  struct jpeg_entropy_encoder_xp * entropy_xp;
} jpeg_compress_struct_xp;

typedef jpeg_compress_struct_xp * j_compress_ptr_xp;


/* Extended record for a decompression instance */

typedef struct {
  struct jpeg_source_mgr_xp * src_xp;   /* Source of compressed data */
  JSAMPARRAYXP colormap_xp;		/* The color map as a 2-D pixel array */
  JSAMPLEXP * sample_range_limit_xp;    /* table for fast range-limiting */
  int bits_in_JSAMPLEXP;
  JSAMPLEXP max_JSAMPLEXP;
  JSAMPLEXP center_JSAMPLEXP;
  /*
   * Links to decompression subobjects (methods, private variables of modules)
   */
  struct jpeg_decomp_master_xp * master_xp;
  struct jpeg_d_main_controller_xp * main_xp;
  struct jpeg_d_coef_controller_xp * coef_xp;
  struct jpeg_d_post_controller_xp * post_xp;
  struct jpeg_input_controller_xp * inputctl_xp;
  struct jpeg_marker_reader_xp * marker_xp;
  struct jpeg_entropy_decoder_xp * entropy_xp;
  struct jpeg_inverse_dct_xp * idct_xp;
  struct jpeg_upsampler_xp * upsample_xp;
  struct jpeg_color_deconverter_xp * cconvert_xp;
  struct jpeg_color_quantizer_xp * cquantize_xp;
} jpeg_decompress_struct_xp;

typedef jpeg_decompress_struct_xp * j_decompress_ptr_xp;


/* Data destination object for compression */

struct jpeg_destination_mgr_xp {
  JOCTET * next_output_byte;	/* => next byte to write in buffer */
  size_t free_in_buffer;	/* # of byte spaces remaining in buffer */

  JMETHOD(void, init_destination_xp, (j_compress_ptr cinfo));
  JMETHOD(boolean, empty_output_buffer_xp, (j_compress_ptr cinfo));
  JMETHOD(void, term_destination_xp, (j_compress_ptr cinfo));
};


/* Data source object for decompression */

struct jpeg_source_mgr_xp {
  const JOCTET * next_input_byte; /* => next byte to read from buffer */
  size_t bytes_in_buffer;	/* # of bytes remaining in buffer */

  JMETHOD(void, init_source, (j_decompress_ptr cinfo));
  JMETHOD(boolean, fill_input_buffer, (j_decompress_ptr cinfo));
  JMETHOD(void, skip_input_data, (j_decompress_ptr cinfo, long num_bytes));
  JMETHOD(boolean, resync_to_restart_xp, (j_decompress_ptr cinfo, int desired));
  JMETHOD(void, term_source, (j_decompress_ptr cinfo));
};
/* Routine signature for application-supplied marker processing methods.
 * Need not pass marker code since it is stored in cinfo->unread_marker.
 */
typedef JMETHOD(boolean, jpeg_marker_parser_method_xp, (j_decompress_ptr cinfo));


/* Declarations for routines called by application.
 * The JPP macro hides prototype parameters from compilers that can't cope.
 * Note JPP requires double parentheses.
 */

#ifdef HAVE_PROTOTYPES
#define JPP(arglist)	arglist
#else
#define JPP(arglist)	()
#endif


/* Short forms of external names for systems with brain-damaged linkers.
 * We shorten external names to be unique in the first six letters, which
 * is good enough for all known systems.
 * (If your compiler itself needs names to be unique in less than 15 
 * characters, you are out of luck.  Get a better compiler.)
 */

#ifdef NEED_SHORT_EXTERNAL_NAMES
#define jpeg_std_error_xp		jStdErrXp
#define jpeg_CreateCompress_xp	jCreaCmpXp
#define jpeg_CreateDecompress_xp	jCreaDecXp
#define jpeg_destroy_compress_xp	jDestCmpXp
#define jpeg_destroy_decompress_xp	jDestDecXp
#define jpeg_stdio_dest_xp		jStdDestXp
#define jpeg_stdio_src_xp	jStdSrcXp
#define jpeg_set_defaults_xp	jSetDefaultsXp
#define jpeg_set_colorspace_xp	jSetColorspXp
#define jpeg_default_colorspace_xp	jDefColorspXp
#define jpeg_set_quality_xp	jSetQualityXp
#define jpeg_set_linear_quality_xp	jSetLQualityXp
#define jpeg_add_quant_table_xp	jAddQuantTableXp
#define jpeg_quality_scaling	jQualityScaling
#define jpeg_simple_progression_xp	jSimProgressXp
#define jpeg_suppress_tables_xp	jSuppressTablesXp
//#define jpeg_alloc_quant_table_xp	jAlcQTableXp
//#define jpeg_alloc_huff_table_xp	jAlcHTableXp
#define jpeg_start_compress_xp	jStrtCompressXp
#define jpeg_write_scanlines_xp	jWrtScanlinesXp
#define jpeg_finish_compress_xp	jFinCompressXp
#define jpeg_write_raw_data_xp	jWrtRawDataXp
#define jpeg_write_marker_xp	jWrtMarkerXp
#define jpeg_write_m_header_xp	jWrtMHeaderXp
#define jpeg_write_m_byte_xp	jWrtMByteXp
#define jpeg_write_tables_xp	jWrtTablesXp
#define jpeg_read_header_xp	jReadHeaderXp
#define jpeg_start_decompress_xp	jStrtDecompressXp
#define jpeg_read_scanlines_xp	jReadScanlinesXp
#define jpeg_finish_decompress_xp	jFinDecompressXp
#define jpeg_read_raw_data_xp	jReadRawDataXp
#define jpeg_has_multiple_scans_xp	jHasMultScnXp
#define jpeg_start_output_xp	jStrtOutputXp
#define jpeg_finish_output_xp	jFinOutputXp
#define jpeg_input_complete_xp	jInCompleteXp
#define jpeg_new_colormap_xp	jNewCMapXp
#define jpeg_consume_input_xp	jConsumeInputXp
#define jpeg_calc_output_dimensions_xp	jCalcDimsXp
#define jpeg_save_markers_xp	jSaveMarkersXp
#define jpeg_set_marker_processor_xp	jSetMarkerXp
#define jpeg_read_coefficients_xp	jReadCoefsXp
#define jpeg_write_coefficients_xp	jWrtCoefsXp
#define jpeg_copy_critical_parameters_xp	jCopyCritXp
#define jpeg_abort_compress_xp	jAbrtCmpXp
#define jpeg_abort_decompress_xp	jAbrtDecXp
#define jpeg_abort_xp		jAbortXp
#define jpeg_destroy_xp		jDestroyXp
#define jpeg_resync_to_restart_xp	jResyncRestartXp
#endif /* NEED_SHORT_EXTERNAL_NAMES */


/* Initialization of JPEG compression objects.
 * jpeg_create_compress() and jpeg_create_decompress() are the exported
 * names that applications should call.  These expand to calls on
 * jpeg_CreateCompress and jpeg_CreateDecompress with additional information
 * passed for version mismatch checking.
 * NB: you must set up the error-manager BEFORE calling jpeg_create_xxx.
 */
#define jpeg_create_compress_xp(cinfo) \
    jpeg_CreateCompress_xp((cinfo), JPEG_LIB_VERSION, \
			(size_t) sizeof(struct jpeg_compress_struct))
#define jpeg_create_decompress_xp(cinfo) \
    jpeg_CreateDecompress_xp((cinfo), JPEG_LIB_VERSION, \
			  (size_t) sizeof(struct jpeg_decompress_struct))
EXTERN(void) jpeg_CreateCompress_xp JPP((j_compress_ptr cinfo,
				      int version, size_t structsize));
EXTERN(void) jpeg_CreateDecompress_xp JPP((j_decompress_ptr cinfo,
					int version, size_t structsize));
/* Destruction of JPEG compression objects */
EXTERN(void) jpeg_destroy_compress_xp JPP((j_compress_ptr cinfo));
EXTERN(void) jpeg_destroy_decompress_xp JPP((j_decompress_ptr cinfo));

/* Standard data source and destination managers: stdio streams. */
/* Caller is responsible for opening the file before and closing after. */
EXTERN(void) jpeg_stdio_dest_xp JPP((j_compress_ptr cinfo, FILE * outfile));
EXTERN(void) jpeg_stdio_src_xp JPP((j_decompress_ptr cinfo, FILE * infile));

/* Default parameter setup for compression */
EXTERN(void) jpeg_set_defaults_xp JPP((j_compress_ptr cinfo));
/* Compression parameter setup aids */
EXTERN(void) jpeg_set_colorspace_xp JPP((j_compress_ptr cinfo,
				      J_COLOR_SPACE colorspace));
EXTERN(void) jpeg_default_colorspace_xp JPP((j_compress_ptr cinfo));
EXTERN(void) jpeg_set_quality_xp JPP((j_compress_ptr cinfo, int quality,
				   boolean force_baseline));
EXTERN(void) jpeg_set_linear_quality_xp JPP((j_compress_ptr cinfo,
					  int scale_factor,
					  boolean force_baseline));
EXTERN(void) jpeg_add_quant_table_xp JPP((j_compress_ptr cinfo, int which_tbl,
				       const unsigned int *basic_table,
				       int scale_factor,
				       boolean force_baseline));
EXTERN(int) jpeg_quality_scaling JPP((int quality));
EXTERN(void) jpeg_simple_progression_xp JPP((j_compress_ptr cinfo));
EXTERN(void) jpeg_suppress_tables_xp JPP((j_compress_ptr cinfo,
				       boolean suppress));
//EXTERN(JQUANT_TBL *) jpeg_alloc_quant_table_xp JPP((j_common_ptr cinfo));
//EXTERN(JHUFF_TBL *) jpeg_alloc_huff_table_xp JPP((j_common_ptr cinfo));

/* Main entry points for compression */
EXTERN(void) jpeg_start_compress_xp JPP((j_compress_ptr cinfo,
				      boolean write_all_tables));
EXTERN(JDIMENSION) jpeg_write_scanlines_xp JPP((j_compress_ptr cinfo,
					     JSAMPARRAYXP scanlines,
					     JDIMENSION num_lines));
EXTERN(void) jpeg_finish_compress_xp JPP((j_compress_ptr cinfo));

/* Replaces jpeg_write_scanlines when writing raw downsampled data. */
EXTERN(JDIMENSION) jpeg_write_raw_data_xp JPP((j_compress_ptr cinfo,
					    JSAMPIMAGEXP data,
					    JDIMENSION num_lines));

/* Write a special marker.  See libjpeg.doc concerning safe usage. */
EXTERN(void) jpeg_write_marker_xp
	JPP((j_compress_ptr cinfo, int marker,
	     const JOCTET * dataptr, unsigned int datalen));
/* Same, but piecemeal. */
EXTERN(void) jpeg_write_m_header_xp
	JPP((j_compress_ptr cinfo, int marker, unsigned int datalen));
EXTERN(void) jpeg_write_m_byte_xp
	JPP((j_compress_ptr cinfo, int val));

/* Alternate compression function: just write an abbreviated table file */
EXTERN(void) jpeg_write_tables_xp JPP((j_compress_ptr cinfo));

/* Decompression startup: read start of JPEG datastream to see what's there */
EXTERN(int) jpeg_read_header_xp JPP((j_decompress_ptr cinfo,
				  boolean require_image));
/* Return value is one of: */
#define JPEG_SUSPENDED		0 /* Suspended due to lack of input data */
#define JPEG_HEADER_OK		1 /* Found valid image datastream */
#define JPEG_HEADER_TABLES_ONLY	2 /* Found valid table-specs-only datastream */
/* If you pass require_image = TRUE (normal case), you need not check for
 * a TABLES_ONLY return code; an abbreviated file will cause an error exit.
 * JPEG_SUSPENDED is only possible if you use a data source module that can
 * give a suspension return (the stdio source module doesn't).
 */

/* Main entry points for decompression */
EXTERN(boolean) jpeg_start_decompress_xp JPP((j_decompress_ptr cinfo));
EXTERN(JDIMENSION) jpeg_read_scanlines_xp JPP((j_decompress_ptr cinfo,
					    JSAMPARRAYXP scanlines,
					    JDIMENSION max_lines));
EXTERN(boolean) jpeg_finish_decompress_xp JPP((j_decompress_ptr cinfo));

/* Replaces jpeg_read_scanlines when reading raw downsampled data. */
EXTERN(JDIMENSION) jpeg_read_raw_data_xp JPP((j_decompress_ptr cinfo,
					   JSAMPIMAGEXP data,
					   JDIMENSION max_lines));

/* Additional entry points for buffered-image mode. */
EXTERN(boolean) jpeg_has_multiple_scans_xp JPP((j_decompress_ptr cinfo));
EXTERN(boolean) jpeg_start_output_xp JPP((j_decompress_ptr cinfo,
				       int scan_number));
EXTERN(boolean) jpeg_finish_output_xp JPP((j_decompress_ptr cinfo));
EXTERN(boolean) jpeg_input_complete_xp JPP((j_decompress_ptr cinfo));
EXTERN(void) jpeg_new_colormap_xp JPP((j_decompress_ptr cinfo));
EXTERN(int) jpeg_consume_input_xp JPP((j_decompress_ptr cinfo));
/* Return value is one of: */
/* #define JPEG_SUSPENDED	0    Suspended due to lack of input data */
#define JPEG_REACHED_SOS	1 /* Reached start of new scan */
#define JPEG_REACHED_EOI	2 /* Reached end of image */
#define JPEG_ROW_COMPLETED	3 /* Completed one iMCU row */
#define JPEG_SCAN_COMPLETED	4 /* Completed last iMCU row of a scan */

/* Precalculate output dimensions for current decompression parameters. */
EXTERN(void) jpeg_calc_output_dimensions_xp JPP((j_decompress_ptr cinfo));

/* Control saving of COM and APPn markers into marker_list. */
EXTERN(void) jpeg_save_markers_xp
	JPP((j_decompress_ptr cinfo, int marker_code,
	     unsigned int length_limit));

/* Install a special processing method for COM or APPn markers. */
EXTERN(void) jpeg_set_marker_processor_xp
	JPP((j_decompress_ptr cinfo, int marker_code,
	     jpeg_marker_parser_method_xp routine));

/* Read or write raw DCT coefficients --- useful for lossless transcoding. */
EXTERN(jvirt_barray_ptr *) jpeg_read_coefficients_xp JPP((j_decompress_ptr cinfo));
EXTERN(void) jpeg_write_coefficients_xp JPP((j_compress_ptr cinfo,
					  jvirt_barray_ptr * coef_arrays));
EXTERN(void) jpeg_copy_critical_parameters_xp JPP((j_decompress_ptr srcinfo,
						j_compress_ptr dstinfo));

/* If you choose to abort compression or decompression before completing
 * jpeg_finish_(de)compress, then you need to clean up to release memory,
 * temporary files, etc.  You can just call jpeg_destroy_(de)compress
 * if you're done with the JPEG object, but if you want to clean it up and
 * reuse it, call this:
 */
EXTERN(void) jpeg_abort_compress_xp JPP((j_compress_ptr cinfo));
EXTERN(void) jpeg_abort_decompress_xp JPP((j_decompress_ptr cinfo));

/* Generic versions of jpeg_abort and jpeg_destroy that work on either
 * flavor of JPEG object.  These may be more convenient in some places.
 */
EXTERN(void) jpeg_abort_xp JPP((j_common_ptr cinfo));
EXTERN(void) jpeg_destroy_xp JPP((j_common_ptr cinfo));

/* Default restart-marker-resync procedure for use by data source modules */
EXTERN(boolean) jpeg_resync_to_restart_xp JPP((j_decompress_ptr cinfo,
					    int desired));


/* These marker codes are exported since applications and data source modules
 * are likely to want to use them.
 */

#define JPEG_RST0	0xD0	/* RST0 marker code */
#define JPEG_EOI	0xD9	/* EOI marker code */
#define JPEG_APP0	0xE0	/* APP0 marker code */
#define JPEG_COM	0xFE	/* COM marker code */


/* If we have a brain-damaged compiler that emits warnings (or worse, errors)
 * for structure definitions that are never filled in, keep it quiet by
 * supplying dummy definitions for the various substructures.
 */

#ifdef INCOMPLETE_TYPES_BROKEN
#ifndef JPEG_INTERNALS		/* will be defined in jpegint.h */
struct jvirt_sarray_control_xp { long dummy; };
struct jvirt_barray_control { long dummy; };
struct jpeg_comp_master_xp { long dummy; };
struct jpeg_c_main_controller_xp { long dummy; };
struct jpeg_c_prep_controller_xp { long dummy; };
struct jpeg_c_coef_controller_xp { long dummy; };
struct jpeg_marker_writer_xp { long dummy; };
struct jpeg_color_converter_xp { long dummy; };
struct jpeg_downsampler_xp { long dummy; };
struct jpeg_forward_dct_xp { long dummy; };
struct jpeg_entropy_encoder_xp { long dummy; };
struct jpeg_decomp_master_xp { long dummy; };
struct jpeg_d_main_controller_xp { long dummy; };
struct jpeg_d_coef_controller_xp { long dummy; };
struct jpeg_d_post_controller_xp { long dummy; };
struct jpeg_input_controller_xp { long dummy; };
struct jpeg_marker_reader_xp { long dummy; };
struct jpeg_entropy_decoder_xp { long dummy; };
struct jpeg_inverse_dct_xp { long dummy; };
struct jpeg_upsampler_xp { long dummy; };
struct jpeg_color_deconverter_xp { long dummy; };
struct jpeg_color_quantizer_xp { long dummy; };
#endif /* JPEG_INTERNALS */
#endif /* INCOMPLETE_TYPES_BROKEN */


/*
 * The JPEG library modules define JPEG_INTERNALS before including this file.
 * The internal structure declarations are read only when that is true.
 * Applications using the library should not include jpegint.h, but may wish
 * to include jerror.h.
 */

#ifdef JPEG_INTERNALS
#include "xjpegint.h"		/* fetch private declarations */
#include "jerror.h"		  /* fetch error codes too */
#endif

#endif /* XJPEGLIB_H */
