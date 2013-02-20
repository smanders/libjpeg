/*
 * xjerror.h
 *
 * Copyright (C) 1994-1997, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file defines extended error and message codes for the JPEG library.
 * It wasn't working out to add these to jerror.h or to have the jpegxp
 * library have it's own error header file. The wxjpeg library was causing
 * too many conflicts.
 * Edit this file to add new codes, or to translate the message strings to
 * some other language.
 */

/*
 * To define the enum list of message codes, include this file without
 * defining macro JMESSAGE.  To create a message string table, include it
 * again with a suitable JMESSAGE definition (see jerror.c for an example).
 */
#ifndef JMESSAGE
#ifndef XJERROR_H
#define XJERROR_H
/* First time through, define the enum list */
#define JMAKE_ENUM_LIST
#else
/* Repeated inclusions of this file are no-ops unless JMESSAGE is defined */
#define JMESSAGE(code,string)
#endif /* XJERROR_H */
#endif /* JMESSAGE */

#ifdef JMAKE_ENUM_LIST

typedef enum {

#define JMESSAGE(code,string)	code ,

#endif /* JMAKE_ENUM_LIST */

JMESSAGE(JMSG_FIRSTADDONCODE=1000, NULL) /* Must be first entry! */

JMESSAGE(JERR_BAD_DIFF, "spatial difference out of range")
JMESSAGE(JERR_BAD_LOSSLESS,
	 "Invalid lossless parameters Ss=%d Se=%d Ah=%d Al=%d")
JMESSAGE(JERR_BAD_LOSSLESS_SCRIPT,
	 "Invalid lossless parameters at scan script entry %d")
JMESSAGE(JERR_BAD_RESTART, "Invalid restart interval: %d, must be an integer multiple of the number of MCUs in an MCU_row (%d)")
JMESSAGE(JERR_CANT_TRANSCODE,
	 "Cannot transcode to/from lossless JPEG datastreams")
JMESSAGE(JERR_NO_LOSSLESS_SCRIPT, "Lossless encoding was requested but no scan script was supplied")
JMESSAGE(JTRC_UNKNOWN_LOSSLESS_IDS,
	 "Unrecognized component IDs %d %d %d, assuming RGB")
JMESSAGE(JWRN_MUST_DOWNSCALE,
	 "Must downscale data from %d bits to %d")
JMESSAGE(JERR_BAD_LOSSLESS_PRECISION, "Unsupported lossless JPEG data precision %d")

#ifdef JMAKE_ENUM_LIST

  JMSG_LASTADDONCODE
} ADDON_MESSAGE_CODE;

#undef JMAKE_ENUM_LIST
#endif /* JMAKE_ENUM_LIST */

/* Zap JMESSAGE macro so that future re-inclusions do nothing by default */
#undef JMESSAGE
