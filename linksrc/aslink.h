/* aslink.h */

/*
 *  Copyright (C) 1989-2019  Alan R. Baldwin
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 *
 *   With enhancements from
 *	John L. Hartman	(JLH)
 *	jhartman@compuserve.com
 */

/*
 * System Include Files
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Local Definitions
 */

#define VERSION "V05.31"
#define COPYRIGHT "2019"

/*
 * To include NoICE Debugging set non-zero
 */
#define NOICE 1

/*
 * To include SDCC Debugging set non-zero
 */
#define SDCDB 1

/*
 * The assembler requires certain variables to have
 * at least 32 bits to allow correct address processing.
 *
 * The type INT32 is defined so that compiler dependent
 * variable sizes may be specified in one place.
 *
 * LONGINT is defined when INT32 is 'long' to
 * select the 'l' forms for format strings.
 */

/* Turbo C++ 3.0 for DOS */
/* 'int' is 16-bits, 'long' is 32-bits */

#ifdef __TURBOC__
#define INT32 long
#define LONGINT
#endif

/* Symantec C++ V6.x/V7.x for DOS (not DOSX) */
/* 'int' is 16-bits, 'long' is 32-bits */

#ifdef __SC__
#define INT32 long
#define LONGINT
#endif

/* The DEFAULT is 'int' is 32 bits */
#ifndef INT32
#define INT32 int
#endif

/*)Module	aslink.h
 *
 *	The module aslink.h contains the definitions for constants,
 *	structures, global variables, and LKxxxx functions
 *	contained in the LKxxxx.c files.
 */

/*)BUILD
        $(PROGRAM) =	ASLINK
        $(INCLUDE) =	ASLINK.H
        $(FILES) = {
                LKMAIN.C
                LKLEX.C
                LKAREA.C
                LKBANK.C
                LKHEAD.C
                LKSYM.C
                LKEVAL.C
                LKDATA.C
                LKLIST.C
                LKNOICE.C
                LKSDCDB.C
                LKRLOC.C
                LKRLOC3.C
                LKRLOC4.C
                LKLIBR.C
                LKOUT.C
        }
        $(STACK) = 2000
*/

#undef VOID

/* DECUS C void definition */
/* File/extension seperator */

#ifdef DECUS
#define VOID char
#define FSEPX '.'
#endif

/* PDOS C void definition */
/* File/extension seperator */

#ifdef PDOS
#define VOID char
#define FSEPX ':'
#endif

/* Default void definition */
/* File/extension seperator */

#ifndef VOID
#define VOID void
#define FSEPX '.'
#define OTHERSYSTEM
#endif

/*
 *	Common Definitions
 */

/*
 *	.HLR Definitions from the Assemblers
 */
#define NLIST 0 /* No listing */
#define SLIST 1 /* Source only */
#define ALIST 2 /* Address only */
#define BLIST 3 /* Address only with allocation */
#define CLIST 4 /* Code */
#define ELIST 5 /* Equate only */

#define LIST_ERR 0x0001 /* Error Code(s) */
#define LIST_LOC 0x0002 /* Location */
#define LIST_BIN 0x0004 /* Generated Binary Value(s)*/
#define LIST_EQT 0x0008 /* Assembler Equate Value */
#define LIST_CYC 0x0010 /* Opcode Cycles */
#define LIST_LIN 0x0020 /* Line Numbers */
#define LIST_SRC 0x0040 /* Assembler Source Code */

#define HLR_NLST 0x0080 /* For HLR file only */

#define HLR_NONE 0x0000 /* NLIST Flags Mask */

/*
 * Global symbol types.
 */
#define S_REF 1 /* referenced */
#define S_DEF 2 /* defined */

/*
 * File types
 */
#define F_OUT 0 /* File.ixx / File.sxx */
#define F_STD 1 /* stdin */
#define F_LNK 2 /* File.lnk */
#define F_REL 3 /* File.rel */

/*
 * Error definitions
 */
#define ER_NONE 0    /* No error */
#define ER_WARNING 1 /* Warning */
#define ER_ERROR 2   /* Assembly error */
#define ER_FATAL 3   /* Fatal error */

/*
 * This file defines the format of the
 * relocatable binary file.
 */

#define NCPS 80           /* characters per symbol */
#define NINPUT 512        /* Input buffer size */
#define NHASH (1 << 6)    /* Buckets in hash table */
#define HMASK (NHASH - 1) /* Hash mask */
#define NLPP 60           /* Lines per page */
#define NMAX 78           /* IXX/SXX/DBX Buffer Length */
#define IXXMAXBYTES 32    /* NMAX > (2 * IXXMAXBYTES) */
#define SXXMAXBYTES 32    /* NMAX > (2 * SXXMAXBYTES) */
#define DBXMAXBYTES 64    /* NMAX > (  DBXMAXBYTES  ) */
#define FILSPC 80         /* File spec length */

/*
 * NTXT must be defined to have the same value in
 * the ASxxxx assemblers and ASLink.
 *
 * The R Line coding allows only 4-bits for coding
 * the T Line index.  The MAXIMUM value for NTXT
 * is 16.  It should not be changed.
 */
#define NTXT 16 /* T values */

/*
 * Opcode Cycle definitions (Must Be The Same In ASxxxx / ASLink)
 */
#define CYCNT_BGN '[' /* Cycle count begin delimiter */
#define CYCNT_END ']' /* Cycle count end   delimiter */

/*
 * Default Page Length Mask
 */
#define DEFAULT_PMASK 0xFF /* 256 Element Boundary / Length */

/*
 * Internal ASxxxx Version Variable
 */
extern int ASxxxx_VERSION;

/*
 *	ASLINK - Version 3 Definitions
 */

/*
 *	The "A3_" area constants define values used in
 *	generating the assembler area output data.
 *
 * Area flags
 *
 *	   7     6     5     4     3     2     1     0
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 *	|     |     |     | PAG | ABS | OVR |     |     |
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 */

#define A3_CON 000   /* concatenate */
#define A3_OVR 004   /* overlay */
#define A3_REL 000   /* relocatable */
#define A3_ABS 010   /* absolute */
#define A3_NOPAG 000 /* non-paged */
#define A3_PAG 020   /* paged */

/*
 *	The "R3_" relocation constants define values used in
 *	generating the assembler relocation output data for
 *	areas, symbols, and code.
 *
 *
 *	Relocation types.
 *
 *	       7     6     5     4     3     2     1     0
 *	    +-----+-----+-----+-----+-----+-----+-----+-----+
 *	    | MSB | PAGn| PAG0| USGN| BYT2| PCR | SYM | BYT |
 *	    +-----+-----+-----+-----+-----+-----+-----+-----+
 */

#define R3_WORD 0000 /* 16 bit */
#define R3_BYTE 0001 /*  8 bit */

#define R3_AREA 0000 /* Base type */
#define R3_SYM 0002

#define R3_NORM 0000 /* PC adjust */
#define R3_PCR 0004

#define R3_BYT1 0000 /* Byte count for R_BYTE = 1 */
#define R3_BYTX 0010 /* Byte count for R_BYTE = X */

#define R3_SGND 0000 /* Signed value */
#define R3_USGN 0020 /* Unsigned value */

#define R3_NOPAG 0000 /* Page Mode */
#define R3_PAG0 0040  /* Page '0' */
#define R3_PAG 0100   /* Page 'nnn' */

#define R3_LSB 0000 /* output low byte */
#define R3_MSB 0200 /* output high byte */

/*
 *	Additional "R3_" functionality is required to support
 *	some microprocesssor architectures.   The 'illegal'
 *	"R3_" mode of R3_WORD | R3_BYTX is used as a designator
 *	of the extended R3_ modes.  The extended modes replace
 *	the PAGING modes and are being added in an adhoc manner
 *	as follows:
 *
 * Extended Mode relocation flags
 *
 *	   7     6     5     4     3     2     1     0
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 *	| MSB |  x  |  x  | USGN|  1  | PCR | SYM |  0  |
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 */

#define R3_ECHEK 0011 /* Extended Mode Check Bits */
#define R3_EXTND 0010 /* Extended Mode Code */
#define R3_EMASK 0151 /* Extended Mode Mask */

/* #define R3_AREA	0000 */ /* Base type */
/* #define R3_SYM	0002 */

/* #define R3_NORM	0000 */ /* PC adjust */
/* #define R3_PCR	0004 */

/* #define R3_SGND	0000 */ /* Signed value */
/* #define R3_USGN	0020 */ /* Unsigned value */

/* #define R3_LSB	0000 */ /* output low byte */
/* #define R3_MSB	0200 */ /* output high byte */

#define R3_J11 0010   /* JLH: 11 bit JMP and CALL (8051) */
#define R3_J19 0050   /* BM:	19 bit JMP and CALL (DS80C390) */
#define R3_3BYTE 0110 /* 	24 bit */
#define R3_4BYTE 0150 /* 	32 bit */

/*
 *	ASLINK - Version 4 Definitions
 */

/*
 *	The "A4_" area constants define values used in
 *	generating the assembler area output data.
 *
 * Area flags
 *
 *	   7     6     5     4     3     2     1     0
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 *	| BNK | SEG |     | PAG | ABS | OVR | WL1 | WL0 |
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 */

#define A4_BYTE 0x0000 /*  8 bit */
#define A4_WORD 0x0001 /* 16 bit */

#define A4_1BYTE 0x0000 /* 1 Byte Word Length */
#define A4_2BYTE 0x0001 /* 2 Byte Word Length */
#define A4_3BYTE 0x0002 /* 3 Byte Word Length */
#define A4_4BYTE 0x0003 /* 4 Byte Word Length */
#define A4_WLMSK 0x0003 /* Word Length Mask */

#define A4_CON 0x0400   /* Concatenating */
#define A4_OVR 0x0404   /* Overlaying */
#define A4_REL 0x0800   /* Relocatable */
#define A4_ABS 0x0808   /* absolute */
#define A4_NOPAG 0x1000 /* Non-Paged */
#define A4_PAG 0x1010   /* Paged */

#define A4_CSEG 0x4000  /* CSEG */
#define A4_DSEG 0x4040  /* DSEG */
#define A4_NOBNK 0x8000 /* Non-Banked */
#define A4_BNK 0x8080   /* Banked */

#define A4_OUT 0x0100 /* Output Code Flag */

/*
 *	The "R4_" relocation constants define values used in
 *	generating the assembler relocation output data for
 *	areas, symbols, and code.
 *
 * Note:  The PAGE modes, PCR modes, Signed, Unsigned,
 *        and MSB codes are mutually exclusive !!!
 *
 *
 * Relocation flags
 *
 *	   7     6     5     4     3     2     1     0
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 *	| SYM | PCR | PAGn| PAG0| USGN| SGND| BYT1| BYT0|
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 */

#define R4_BYTE 0x0000 /*  8 bit */
#define R4_WORD 0x0001 /* 16 bit */

#define R4_1BYTE 0x0000 /* 1 Byte */
#define R4_2BYTE 0x0001 /* 2 Byte */
#define R4_3BYTE 0x0002 /* 3 Byte */
#define R4_4BYTE 0x0003 /* 4 Byte */
#define R4_BYTES 0x0003 /* Data Size */

#define R4_SGND 0x0004 /* Signed */
#define R4_USGN 0x0008 /* Unsigned */
#define R4_OVRF 0x0008 /* Overflow */

#define R4_MBRS 0x0004 /* Merge Bit Range Signed */
                       /* An alias for Signed */
#define R4_MBRU 0x0008 /* Merge Bit Range Unsigned */
                       /* An alias for Unsigned */
#define R4_MBRO 0x0008 /* Merge Bit Range Overflow */
                       /* An alias for Unsigned */

#define R4_MSB 0x000C /* MSB */
                      /* Mutually exclusive with Signed / Unsigned */

#define R4_AREA 0x0000 /* Base type */
#define R4_SYM 0x0080

/*
 * Paging Modes:
 */

#define R4_NOPAG 0x0000 /* Page Mode */
#define R4_PBITS 0x003C /* Paging Bits */
#define R4_PAGE 0x0030  /* Paged Addressing */
#define R4_PAG0 0x0010  /* Page '0'    .setdp */
#define R4_PAGN 0x0020  /* Page 'nnn'  .setdp */
#define R4_PAGX 0x0030  /* Page 'x', Extended Relocation Mode */
#define R4_PAGX0 0x0030 /* Page 'x', pc + 0 */
#define R4_PAGX1 0x0034 /* Page 'x', pc + 1 */
#define R4_PAGX2 0x0038 /* Page 'x', pc + 2 */
#define R4_PAGX3 0x003C /* Page 'x', pc + 3 */

/*
 * PCR Modes:
 */

#define R4_PCR 0x0040  /* PC adjust (default)    */
#define R4_PCRN 0x0050 /* PC adjust (default) no range check */

#define R4_PCR0 0x0054 /* PC adjust (offset = 0) */
#define R4_PCR1 0x0060 /* PC adjust (offset = 1) */
#define R4_PCR2 0x0064 /* PC adjust (offset = 2) */
#define R4_PCR3 0x0068 /* PC adjust (offset = 3) */
#define R4_PCR4 0x006C /* PC adjust (offset = 4) */

#define R4_PCR0N 0x0058 /* PC adjust (offset = 0) no range check */
#define R4_PCR1N 0x0070 /* PC adjust (offset = 1) no range check */
#define R4_PCR2N 0x0074 /* PC adjust (offset = 2) no range check */
#define R4_PCR3N 0x0078 /* PC adjust (offset = 3) no range check */
#define R4_PCR4N 0x007C /* PC adjust (offset = 4) no range check */

/*
 * Basic Relocation Modes
 */

#define R4_NORM 0x0000 /* No Bit Positioning */

/*
 *	The defined type 'a_uint' is used for all address and
 *	unsigned variable value calculations.  Its size is
 *	required to be at least 32-bits to allow upto
 *	32-bit addressing or 32-bit value manipulation.
 */
typedef unsigned INT32 a_uint;

/*
 *	The defined type 'v_sint' is used for address and
 *	variable value calculations requiring a sign.
 *	Its size is required to be at least 32-bits to allow
 *	upto 32-bit addressing or 32-bit value manipulation.
 */
typedef signed INT32 v_sint;

/*
 *	The structures of head, mode, bank, area, areax, and sym
 *	are created as the REL files are read during the first
 *	pass of the linker.  The struct head is created upon
 *	encountering a H directive in the REL file.  The
 *	structure contains a link to a link file structure
 *	(struct lfile) which describes the file containing the H
 *	directive, a pointer to an array of merge mode
 *	definition pointers, the number of data/code areas
 *	contained in this header segment, the number of
 *	symbols referenced/defined in this header segment, a pointer
 *	to an array of pointers to areax structures (struct areax)
 *	created as each A directive is read, a pointer to an
 *	array of pointers to symbol structures (struct sym) for
 *	all referenced/defined symbols and a pointer to an array
 *	of pointers to bank structures (struct bank) referenced
 *	by this module.  As H directives are read
 *	from the REL files a linked list of head structures is
 *	created by placing a link to the new head structure
 *	in the previous head structure.
 */
struct head {
  struct head *h_hp;     /* Header link */
  struct lfile *h_lfile; /* Associated file */
  int h_narea;           /* # of areas */
  struct areax **a_list; /* Area list */
  int h_nsym;            /* # of symbols */
  struct sym **s_list;   /* Symbol list */
  int h_nbank;           /* # of banks */
  struct bank **b_list;  /* Bank list */
  int h_nmode;           /* # of modes */
  struct mode **m_list;  /* Mode list */
  char *m_id;            /* Module name */
};

/*
 *	The MODE structure contains the specification of one of the
 *	assemblers' relocation modes.  Each assembler must specify
 *	at least one relocation mode.  The relocation specification
 *	allows arbitrarily defined active bits and bit positions.
 *	The 32 element arrays are indexed from 0 to 31.
 *	Index 0 corresponds to bit 0, ..., and 31 corresponds to bit 31
 *	of a normal integer value.
 *
 *	The value an array element defines if the normal integer bit is
 *	active (bit <7> is set, 0x80) and what destination bit
 *	(bits <4:0>, 0 - 31) should be loaded with this normal
 *	integer bit.
 *
 *	The specification for a 32-bit integer:
 *
 *	char mode_[32] = {
 *		0x80,	0x81,	0x82,	0x83,	0x84,	0x85,	0x86,	0x87,
 *		0x88,	0x89,	0x8A,	0x8B,	0x8C,	0x8D,	0x8E,	0x8F,
 *		0x90,	0x91,	0x92,	0x93,	0x94,	0x95,	0x96,	0x97,
 *		0x98,	0x99,	0x9A,	0x9B,	0x9C,	0x9D,	0x9E,	0x9F
 *	};
 *
 *
 *	The specification for the 11-bit 8051 addressing mode:
 *
 *	char mode_[32] = {
 *		0x80,	0x81,	0x82,	0x83,	0x84,	0x85,	0x86,	0x87,
 *		0x8D,	0x8E,	0x8F,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
 *		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,
 *		0x18,	0x19,	0x1A,	0x1B,	0x1C,	0x1D,	0x1E,	0x1F
 *	};
 *
 *
 *	m_def is the bit relocation definition array.
 *	m_flag indicates that bit position swapping is required.
 *	m_dbits contains the active bit positions for the output.
 *	m_sbits contains the active bit positions for the input.
 */
struct mode {
  char m_def[32]; /* Bit Relocation Definition */
  int m_flag;     /* Bit Swapping Flag */
  a_uint m_dbits; /* Destination Bit Mask */
  a_uint m_sbits; /* Source Bit Mask */
};

/*
 *	The bank structure contains the parameter values for a
 *	specific program or data bank.  The bank structure
 *	is a linked list of banks.  The initial default bank
 *	is unnamed and is defined in lkdata.c, the next bank structure
 *	will be linked to this structure through the structure
 *	element 'struct bank *b_bp'.  The structure contains the
 *	bank name, the bank base address (default = 0), the bank size
 *	(default = 0, whole addressing space), the bank mapping,
 *	and the file name suffix. (default is none)  These optional
 *	parameters are from  the .bank assembler directive.
 *	The bank structure also contains the bank data output
 *	file specification, file handle pointer and the
 *	bank first output flag.
 */
struct bank {
  struct bank *b_bp; /* Bank link */
  char *b_id;        /* Bank Name */
  char *b_fsfx;      /* Bank File Suffix / File Specification */
  a_uint b_base;     /* Bank base address */
  a_uint b_size;     /* Bank size */
  a_uint b_map;      /* Bank mapping */
  int b_flag;        /* Bank flags */
  char *b_fspec;     /* Bank File Specification */
  FILE *b_ofp;       /* Bank File Handle */
  char *b_ofspec;    /* Bank Output File Specification */
  int b_oflag;       /* Bank has output flag */
  int b_rtaflg;      /* Bank First Output flag */
};

#define B_BASE 0001 /* 'base' address specified */
#define B_SIZE 0002 /* 'size' of bank specified */
#define B_FSFX 0004 /* File suffix specified */
#define B_MAP 0010  /* Mapped Bank Flag */

/*
 *	A structure area is created for each 'unique' data/code
 *	area definition found as the REL files are read.  The
 *	struct area contains the name of the area, a flag byte
 *	which contains the area attributes (REL/CON/OVR/ABS),
 *	the area base address set flag byte (-b option), and the
 *	area base address and total size which will be filled
 *	in at the end of the first pass through the REL files.
 *	The area structure also contains a link to the bank
 *	this area is a part of and a data output file handle
 *	pointer which is loaded from from the bank structure.
 *	As A directives are read from the REL files a linked
 *	list of unique area structures is created by placing a
 *	link to the new area structure in the previous area structure.
 */
struct area {
  struct area *a_ap;   /* Area link */
  struct areax *a_axp; /* Area extension link */
  struct bank *a_bp;   /* Bank link */
  FILE *a_ofp;         /* Area File Handle */
  a_uint a_addr;       /* Beginning address of area */
  a_uint a_size;       /* Total size of the area */
  int a_bset;          /* Area base address set */
  int a_flag;          /* Flags */
  char *a_id;          /* Name */
};

/*
 *	An areax structure is created for every A directive found
 *	while reading the REL files.  The struct areax contains a
 *	link to the 'unique' area structure referenced by the A
 *	directive and to the head structure this area segment is
 *	a part of.  The size of this area segment as read from the
 *	A directive is placed in the areax structure.  The beginning
 *	address of this segment will be filled in at the end of the
 *	first pass through the REL files.  As A directives are read
 *	from the REL files a linked list of areax structures is
 *	created for each unique area.  The final areax linked
 *	list has at its head the 'unique' area structure linked
 *	to the linked areax structures (one areax structure for
 *	each A directive for this area).
 */
struct areax {
  struct areax *a_axp; /* Area extension link */
  struct area *a_bap;  /* Base area link */
  struct head *a_bhp;  /* Base header link */
  a_uint a_addr;       /* Beginning address of section */
  a_uint a_size;       /* Size of the area in section */
  a_uint a_bndry;      /* Boundary for this A directive */
};

/*
 *	A sym structure is created for every unique symbol
 *	referenced/defined while reading the REL files.  The
 *	struct sym contains the symbol's name, a flag value
 *	set to inhibit map output, a symbol type denoting
 *	referenced/defined, and an address which is loaded
 *	with the relative address within the area in which
 *	the symbol was defined.  The sym structure also
 *	contains a link to the area where the symbol was defined.
 *	The sym structures are linked into linked lists using
 *	the symbol link element.
 */
struct sym {
  struct sym *s_sp;    /* Symbol link */
  struct areax *s_axp; /* Symbol area link */
  char s_type;         /* Symbol subtype */
  char s_flag;         /* Flag byte */
  a_uint s_addr;       /* Address */
  char *s_id;          /* Name (JLH) */
  char *m_id;          /* Module symbol define in */
};

/*
 *	The structure lfile contains a pointer to a
 *	file specification string, an index which points
 *	to the file name (past the 'path'), the file type,
 *	an object output flag, and a link to the next
 *	lfile structure.
 */
struct lfile {
  struct lfile *f_flp; /* lfile link */
  int f_type;          /* File type */
  char *f_idp;         /* Pointer to file spec */
  int f_idx;           /* Index to file name */
  int f_obj;           /* Object output flag */
};

/*
 *	The struct base contains a pointer to a
 *	base definition string and a link to the next
 *	base structure.
 */
struct base {
  struct base *b_base; /* Base link */
  char *b_strp;        /* String pointer */
};

/*
 *	The struct globl contains a pointer to a
 *	global definition string and a link to the next
 *	global structure.
 */
struct globl {
  struct globl *g_globl; /* Global link */
  char *g_strp;          /* String pointer */
};

/*
 *	A structure sdp is created for each 'unique' paged
 *	area definition found as the REL files are read.
 *	As P directives are read from the REL files a linked
 *	list of unique sdp structures is created by placing a
 *	link to the new sdp structure in the previous area structure.
 */
struct sdp {
  struct area *s_area;   /* Paged Area link */
  struct areax *s_areax; /* Paged Area Extension Link */
  a_uint s_addr;         /* Page address offset */
};

/*
 *	The structure rerr is loaded with the information
 *	required to report an error during the linking
 *	process.  The structure contains an index value
 *	which selects the areax structure from the header
 *	areax structure list, a mode value which selects
 *	symbol or area relocation, the base address in the
 *	area section, an area/symbol list index value, and
 *	an area/symbol offset value.
 */
struct rerr {
  int aindex;    /* Linking area */
  int mode;      /* Relocation mode */
  a_uint rtbase; /* Base address in section */
  int rindex;    /* Area/Symbol reloaction index */
  a_uint rval;   /* Area/Symbol offset value */
};

/*
 *	The structure lbpath is created for each library
 *	path specification input by the -k option.  The
 *	lbpath structures are linked into a list using
 *	the next link element.
 */
struct lbpath {
  struct lbpath *next;
  char *path;
};

/*
 *	The structure lbname is created for all combinations of the
 *	library path specifications (input by the -k option) and the
 *	library file specifications (input by the -l option) that
 *	lead to an existing file.  The element path points to
 *	the path string, element libfil points to the library
 *	file string, and the element libspc is the concatenation
 *	of the valid path and libfil strings.
 *
 *	The lbpath structures are linked into a list
 *	using the next link element.
 *
 *	Each library file contains a list of object files
 *	that are contained in the particular library. e.g.:
 *
 *		\iolib\termio
 *		\inilib\termio
 *
 *	Only one specification per line is allowed.
 */
struct lbname {
  struct lbname *next;
  char *path;
  char *libfil;
  char *libspc;
  int f_obj;
};

/*
 *	The function fndsym() searches through all combinations of the
 *	library path specifications (input by the -k option) and the
 *	library file specifications (input by the -l option) that
 *	lead to an existing file for a symbol definition.
 *
 *	The structure lbfile is created for the first library
 *	object file which contains the definition for the
 *	specified undefined symbol.
 *
 *	The element libspc points to the library file path specification
 *	and element relfil points to the object file specification string.
 *	The element filspc is the complete path/file specification for
 *	the library file to be imported into the linker.  The f_obj
 *	flag specifies if the object code from this file is
 *	to be output by the linker.  The file specification
 *	may be formed in one of two ways:
 *
 *	(1)	If the library file contained an absolute
 *		path/file specification then this becomes filspc.
 *		(i.e. C:\...)
 *
 *	(2)	If the library file contains a relative path/file
 *		specification then the concatenation of the path
 *		and this file specification becomes filspc.
 *		(i.e. \...)
 *
 *	The lbpath structures are linked into a list
 *	using the next link element.
 */
struct lbfile {
  struct lbfile *next;
  char *libspc;
  char *relfil;
  char *filspc;
  int f_obj;
};

/*
 *	External Definitions for all Global Variables
 */

extern char *_abs_;     /*	= { ".  .ABS." };
                         */
extern int lkerr;       /*	ASLink error flag
                         */
extern char *ip;        /*	pointer into the REL file
                         *	text line in ib[]
                         */
extern char ib[NINPUT]; /*	REL file text line
                         */
extern char *rp;        /*	pointer into the LST file
                         *	text line in rb[]
                         */
extern char rb[NINPUT]; /*	LST file text line being
                         *	address relocated
                         */
extern char ctype[];    /*	array of character types, one per
                         *	ASCII character
                         */

/*
 *	Character Type Definitions
 */
#define SPACE '\000'
#define ETC '\000'
#define LETTER '\001'
#define DIGIT '\002'
#define BINOP '\004'
#define RAD2 '\010'
#define RAD8 '\020'
#define RAD10 '\040'
#define RAD16 '\100'
#define ILL '\200'

#define DGT2 DIGIT | RAD16 | RAD10 | RAD8 | RAD2
#define DGT8 DIGIT | RAD16 | RAD10 | RAD8
#define DGT10 DIGIT | RAD16 | RAD10
#define LTR16 LETTER | RAD16

extern char afspec[]; /*	The filespec created by afile()
                       */
extern char ccase[];  /*	an array of characters which
                       *	perform the case translation function
                       */

extern struct lfile *filep;        /*	The pointers (lfile *) filep,
                                    *	(lfile *) cfp, and (FILE *) sfp
                                    *	are used in conjunction with
                                    *	the routine nxtline() to read
                                    *	aslink commands from
                                    *	(1) the standard input or
                                    *	(2) or a command file
                                    *	and to read the REL files
                                    *	sequentially as defined by the
                                    *	aslink input commands.
                                    *
                                    *	The pointer *filep points to the
                                    *	beginning of a linked list of
                                    *	lfile structures.
                                    */
extern struct lfile *cfp;          /*	The pointer *cfp points to the
                                    *	current lfile structure
                                    */
extern struct lfile *startp;       /*	aslink startup file structure
                                    */
extern struct lfile *linkp;        /*	pointer to first lfile structure
                                    *	containing an input REL file
                                    *	specification
                                    */
extern struct lfile *lfp;          /*	pointer to current lfile structure
                                    *	being processed by parse()
                                    */
extern struct head *headp;         /*	The pointer to the first
                                    *	head structure of a linked list
                                    */
extern struct head *hp;            /*	Pointer to the current
                                    *	head structure
                                    */
extern struct bank *bankp;         /*	The pointer to the first
                                    *	bank structure of a linked list
                                    */
extern struct bank *bp;            /*	Pointer to the current
                                    *	bank structure
                                    */
extern struct area *areap;         /*	The pointer to the first
                                    *	area structure of a linked list
                                    */
extern struct area *ap;            /*	Pointer to the current
                                    *	area structure
                                    */
extern struct areax *axp;          /*	Pointer to the current
                                    *	areax structure
                                    */
extern struct sym *symhash[NHASH]; /*	array of pointers to NHASH
                                    *	linked symbol lists
                                    */
extern struct base *basep;         /*	The pointer to the first
                                    *	base structure
                                    */
extern struct base *bsp;           /*	Pointer to the current
                                    *	base structure
                                    */
extern struct globl *globlp;       /*	The pointer to the first
                                    *	globl structure
                                    */
extern struct globl *gsp;          /*	Pointer to the current
                                    *	globl structure
                                    */
extern struct sdp sdp;             /*	Base Paged structure
                                    */
extern struct rerr rerr;           /*	Structure containing the
                                    *	linker error information
                                    */
extern FILE *nbofp;                /*	Non Banked Linker Output
                                    *	File Handle
                                    */
extern FILE *ofp;                  /*	Linker Output file handle
                                    */

#if NOICE
extern FILE *jfp; /*	NoICE output file handle
                   */
#endif

extern FILE *mfp; /*	Map output file handle
                   */
extern FILE *rfp; /*	File handle for output
                   *	address relocated ASxxxx
                   *	listing file
                   */
extern FILE *sfp; /*	The file handle sfp points to the
                   *	currently open file
                   */
extern FILE *tfp; /*	File handle for input
                   *	ASxxxx listing file
                   */
extern FILE *hfp; /*	File handle for input ASxxxx
                   *	.lst to .rst hint file
                   */

#if SDCDB
extern FILE *yfp; /*	SDCDB output file handle
                   */
#endif

extern int oflag;  /*	Output file type flag
                    */
extern int o1flag; /*	Output legacy Intel Hex flag
                    *	Start address record type set to 1
                    */
extern int objflg; /*	Linked file/library object output flag
                    */

#if NOICE
extern int jflag; /*	-j, enable NoICE Debug output
                   */
#endif

extern int mflag;  /*	Map output flag
                    */
extern int m1flag; /*	Include linker generated
                    *	symbols in map file
                    */
extern int xflag;  /*	Map file radix type flag
                    */

#if SDCDB
extern int yflag; /*	-y, enable SDCC Debug output
                   */
#endif

extern int pflag;              /*	print linker command file flag
                                */
extern int uflag;              /*	Listing relocation flag
                                */
extern int wflag;              /*	Enable wide format listing
                                */
extern int zflag;              /*	Enable symbol case sensitivity
                                */
extern int radix;              /*	current number conversion radix:
                                *	2 (binary), 8 (octal), 10 (decimal),
                                *	16 (hexadecimal)
                                */
extern int line;               /*	current line number
                                */
extern int page;               /*	current page number
                                */
extern int lop;                /*	current line number on page
                                */
extern time_t curtim;          /*	pointer to the current time string
                                */
extern int pass;               /*	linker pass number
                                */
extern a_uint pc;              /*	current relocation address
                                */
extern int pcb;                /*	current bytes per pc word
                                */
extern int rtcnt;              /*	count of elements in the
                                *	rtval[] and rtflg[] arrays
                                */
extern a_uint rtval[];         /*	data associated with relocation
                                */
extern int rtflg[];            /*	indicates if rtval[] value is
                                *	to be sent to the output file.
                                */
extern int rterr[];            /*	indicates if rtval[] value should
                                *	be flagged as a relocation error.
                                */
extern char rtbuf[];           /*	S19/IHX output buffer
                                */
extern struct bank *rtabnk;    /*	rtbuf[] processing
                                */
extern int rtaflg;             /*	rtbuf[] processing
                                */
extern a_uint rtadr0;          /*
                                */
extern a_uint rtadr1;          /*
                                */
extern a_uint rtadr2;          /*
                                */
extern int obj_flag;           /*	Linked file/library object output flag
                                */
extern int a_bytes;            /*	REL file T Line address length
                                */
extern int hilo;               /*	REL file byte ordering
                                */
extern a_uint a_mask;          /*	Address Mask
                                */
extern a_uint s_mask;          /*	Sign Mask
                                */
extern a_uint v_mask;          /*	Value Mask
                                */
extern a_uint p_mask;          /*	Page Mask
                                */
extern int gline;              /*	Read a LST line flag
                                */
extern int gcntr;              /*	Bytes processed in LST line
                                */
extern int hline;              /*	Read a HLR line flag
                                */
extern int listing;            /*	Assembled line listing bits
                                */
extern int lmode;              /*	Assembled line listing mode
                                */
extern int bytcnt;             /*	Assembled bytes for this line
                                */
extern int bgncnt;             /*	Assembled bytes for this line
                                */
extern char eqt_id[128];       /*	Area name for this ELIST line
                                */
extern struct lbpath *lbphead; /*	pointer to the first
                                *	library path structure
                                */
extern struct lbname *lbnhead; /*	pointer to the first
                                *	library name structure
                                */
extern struct lbfile *lbfhead; /*	pointer to the first
                                *	library file structure
                                */

/* C Library function definitions */
/* for reference only
extern	VOID		exit();
extern	int		fclose();
extern	char *		fgets();
extern	FILE *		fopen();
extern	int		fprintf();
extern	VOID		free();
extern	VOID *		malloc();
extern	char		putc();
extern	char *		sprintf();
extern	char *		strcpy();
extern	int		strlen();
extern	char *		strncpy();
extern	char *		strrchr();
*/

/* Program function definitions */

#ifdef OTHERSYSTEM

/* C Library */
extern int delete(char *str);
extern VOID exit(int n);

/* lkmain.c */
extern FILE *afile(char *fn, char *ft, int wf);
extern VOID bassav(void);
extern int fndidx(char *str);
extern int fndext(char *str);
extern VOID gblsav(void);
extern int intsiz(void);
extern VOID link(void);
extern VOID lkexit(int i);
extern int main(int argc, char *argv[]);
extern VOID map(void);
extern int parse(void);
extern VOID doparse(void);
extern VOID setgbl(void);
extern VOID usage(void);

/* lklex.c */
extern VOID chopcrlf(char *str);
extern char endline(void);
extern int get(void);
extern VOID getfid(char *str, int c);
extern VOID getid(char *id, int c);
extern int getmap(int d);
extern int getnb(void);
extern int more(void);
extern int nxtline(void);
extern VOID skip(int c);
extern VOID unget(int c);

/* lkarea.c */
extern VOID lkparea(char *id);
extern VOID lnkarea(void);
extern VOID lnksect(struct area *tap);
extern VOID lnkserr(char *frmt, char *str);
extern VOID newarea(void);
extern VOID setarea(void);

/* lkbank.c */
extern VOID chkbank(FILE *fp);
extern VOID lkfclose(void);
extern VOID lkfopen(void);
extern VOID lkpbank(char *id);
extern VOID newbank(void);
extern VOID setbank(void);

/* lkhead.c */
extern VOID module(void);
extern VOID newhead(void);
extern VOID newmode(void);

/* lksym.c */
extern int hash(char *p, int cflag);
extern struct sym *lkpsym(char *id, int f);
extern char *new(unsigned int n);
extern struct sym *newsym(void);
extern char *strsto(char *str);
extern VOID symdef(FILE *fp);
extern int symeq(char *p1, char *p2, int cflag);
extern VOID syminit(void);
extern VOID symmod(FILE *fp, struct sym *tsp);
extern a_uint symval(struct sym *tsp);

/* lkeval.c */
extern int digit(int c, int r);
extern a_uint eval(void);
extern a_uint expr(int n);
extern int oprio(int c);
extern a_uint term(void);

/* lklist.c */
extern int dgt(int rdx, char *str, int n);
extern int gethlr(int nhline);
extern int getlst(int ngline);
extern VOID newpag(FILE *fp);
extern VOID slew(struct area *xp, struct bank *yp);
extern VOID lstarea(struct area *xp, struct bank *yp);
extern VOID lkulist(int i);
extern VOID lklist(a_uint cpc, int v, int err);
extern VOID lkalist(a_uint cpc);
extern VOID hlrlist(a_uint cpc, int v, int err);
extern VOID hlralist(a_uint cpc);
extern VOID hlrelist(void);
extern VOID hlrclist(a_uint cpc, int v);
extern VOID setgh(void);
extern VOID lsterr(int err);

/* lknoice.c */
extern VOID NoICEfopen(void);
extern VOID NoICEmagic(void);
extern VOID DefineNoICE(char *name, a_uint value, struct bank *yp);
extern VOID DefineGlobal(char *name, a_uint value, struct bank *yp);
extern VOID DefineScoped(char *name, a_uint value, struct bank *yp);
extern VOID DefineFile(char *name, a_uint value, struct bank *yp);
extern VOID DefineFunction(char *name, a_uint value, struct bank *yp);
extern VOID DefineStaticFunction(char *name, a_uint value, struct bank *yp);
extern VOID DefineEndFunction(a_uint value, struct bank *yp);
extern VOID DefineLine(char *lineString, a_uint value, struct bank *yp);
extern VOID PagedAddress(a_uint value, struct bank *yp);

/* lksccdb.c */
extern VOID SDCDBfopen(void);
extern VOID SDCDBcopy(char *str);
extern VOID DefineSDCDB(char *name, a_uint value);

/* lkrloc.c */
extern a_uint adb_1b(a_uint v, int i);
extern a_uint adb_2b(a_uint v, int i);
extern a_uint adb_3b(a_uint v, int i);
extern a_uint adb_4b(a_uint v, int i);
extern a_uint adb_xb(a_uint v, int i);
extern a_uint adw_xb(int x, a_uint v, int i);
extern a_uint evword(void);
extern VOID prntval(FILE *fptr, a_uint v);
extern VOID reloc(int c);

/* lkrloc3.c */
extern a_uint adb_hi(a_uint v, int i);
extern a_uint adb_lo(a_uint v, int i);
extern char *errmsg3[];
extern VOID errdmp3(FILE *fptr, char *str);
extern VOID erpdmp3(FILE *fptr, char *str);
extern VOID rele3(void);
extern VOID reloc3(int c);
extern VOID relt3(void);
extern VOID relr3(void);
extern VOID relp3(void);
extern VOID relerr3(char *str);
extern VOID relerp3(char *str);

/* lkrloc4.c */
extern a_uint adb_byte(int p, a_uint v, int i);
extern char *errmsg4[];
extern VOID errdmp4(FILE *fptr, char *str);
extern VOID erpdmp4(FILE *fptr, char *str);
extern a_uint gtb_1b(int i);
extern a_uint gtb_2b(int i);
extern a_uint gtb_3b(int i);
extern a_uint gtb_4b(int i);
extern a_uint gtb_xb(int i);
extern a_uint lkmerge(a_uint val, int r, a_uint v);
extern a_uint ptb_1b(a_uint v, int i);
extern a_uint ptb_2b(a_uint v, int i);
extern a_uint ptb_3b(a_uint v, int i);
extern a_uint ptb_4b(a_uint v, int i);
extern a_uint ptb_xb(a_uint v, int i);
extern VOID rele4(void);
extern VOID reloc4(int c);
extern VOID relt4(void);
extern VOID relr4(void);
extern VOID relp4(void);
extern VOID relerr4(char *str);
extern VOID relerp4(char *str);

/* lklibr.c */
extern VOID addfile(char *path, char *libfil);
extern VOID addlib(void);
extern VOID addpath(void);
extern int fndsym(char *name);
extern VOID library(void);
extern VOID loadfile(char *filspc);
extern VOID search(void);

/* lkout.c */
extern VOID lkout(int i);
extern VOID lkflush(void);
extern VOID ixx(int i);
extern VOID iflush(void);
extern VOID sxx(int i);
extern VOID sflush(void);
extern VOID dbx(int i);
extern VOID dflush(void);

#else

/* C Library */
extern int delete();
extern VOID exit();

/* lkmain.c */
extern FILE *afile();
extern VOID bassav();
extern int fndext();
extern int fndidx();
extern VOID gblsav();
extern int intsiz();
extern VOID link();
extern VOID lkexit();
extern int main();
extern VOID map();
extern int parse();
extern VOID doparse();
extern VOID setgbl();
extern VOID usage();

/* lklex.c */
extern VOID chopcrlf();
extern char endline();
extern int get();
extern VOID getfid();
extern VOID getid();
extern int getmap();
extern int getnb();
extern int more();
extern int nxtline();
extern VOID skip();
extern VOID unget();

/* lkarea.c */
extern VOID lkparea();
extern VOID lnkarea();
extern VOID lnksect();
extern VOID lnkserr();
extern VOID newarea();
extern VOID setarea();

/* lkbank.c */
extern VOID chkbank();
extern VOID lkfclose();
extern VOID lkfopen();
extern VOID lkpbank();
extern VOID newbank();
extern VOID setbank();

/* lkhead.c */
extern VOID module();
extern VOID newhead();
extern VOID newbank();
extern VOID newmode();
extern VOID lkpbank();

/* lksym.c */
extern int hash();
extern struct sym *lkpsym();
extern VOID *new();
extern struct sym *newsym();
extern char *strsto();
extern VOID symdef();
extern int symeq();
extern VOID syminit();
extern VOID symmod();
extern a_uint symval();

/* lkeval.c */
extern int digit();
extern a_uint eval();
extern a_uint expr();
extern int oprio();
extern a_uint term();

/* lklist.c */
extern int dgt();
extern int gethlr();
extern int getlst();
extern VOID newpag();
extern VOID slew();
extern VOID lstarea();
extern VOID lkulist();
extern VOID lkalist();
extern VOID lkglist();
extern VOID hlrelist();
extern VOID hlrclist();
extern VOID setgh();
extern VOID lsterr();

/* lknoice.c */
extern VOID NoICEfopen();
extern VOID NoICEmagic();
extern VOID DefineNoICE();
extern VOID DefineGlobal();
extern VOID DefineScoped();
extern VOID DefineFile();
extern VOID DefineFunction();
extern VOID DefineStaticFunction();
extern VOID DefineEndFunction();
extern VOID DefineLine();
extern VOID PagedAddress();

/* lksdcdb.c */
extern VOID SDCDBfopen();
extern VOID SDCDBcopy();
extern VOID DefineSDCDB();

/* lkrloc.c */
extern a_uint adb_1b();
extern a_uint adb_2b();
extern a_uint adb_3b();
extern a_uint adb_4b();
extern a_uint adb_xb();
extern a_uint adw_xb();
extern a_uint evword();
extern VOID prntval();
extern VOID reloc();

/* lkrloc3.c */
extern a_uint adb_hi();
extern a_uint adb_lo();
extern char *errmsg3[];
extern VOID errdmp3();
extern VOID erpdmp3();
extern VOID rele3();
extern VOID reloc3();
extern VOID relt3();
extern VOID relr3();
extern VOID relp3();
extern VOID relerr3();
extern VOID relerp3(c);

/* lkrloc4.c */
extern a_uint adb_byte();
extern char *errmsg4[];
extern VOID errdmp4();
extern VOID erpdmp4();
extern a_uint gtb_1b();
extern a_uint gtb_2b();
extern a_uint gtb_3b();
extern a_uint gtb_4b();
extern a_uint gtb_xb();
extern int lkmerge();
extern a_uint ptb_1b();
extern a_uint ptb_2b();
extern a_uint ptb_3b();
extern a_uint ptb_4b();
extern a_uint ptb_xb();
extern VOID rele4();
extern VOID reloc4();
extern VOID relt4(v);
extern VOID relr4();
extern VOID relp4();
extern VOID relerr4();
extern VOID relerp4();

/* lklibr.c */
extern VOID addfile();
extern VOID addlib();
extern VOID addpath();
extern int fndsym();
extern VOID library();
extern VOID loadfile();
extern VOID search();

/* lkout.c */
extern VOID lkout();
extern VOID lkflush();
extern VOID ixx();
extern VOID iflush();
extern VOID sxx();
extern VOID sflush();
extern VOID dbx();
extern VOID dflush();

#endif
