/* lkrloc3.c */

/*
 *  Copyright (C) 1989-2014  Alan R. Baldwin
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
 *   With enhancements from:
 *
 *	John L. Hartman	(JLH)
 *	jhartman@compuserve.com
 *
 *	Bill McKinnon (BM)
 *	w_mckinnon@conknet.com
 */

#include "aslink.h"

/*)Module	lkrloc3.c
 *
 *	The module lkrloc3.c contains the functions which
 *	perform the version 3 relocation calculations.
 *
 *	lkrloc.c contains the following functions:
 *		a_uint	adb_lo()
 *		a_uint	adb_hi()
 *		VOID	erpdmp3()
 *		VOID	errdmp3()
 *		VOID	rele3()
 *		VOID	relerr3()
 *		VOID	relerp3()
 *		VOID	reloc3()
 *		VOID	relp3()
 *		VOID	relr3()
 *		VOID	relt3()
 *
 *	lkrloc3.c the local variable errmsg3[].
 *
 */

/*)Function	VOID	reloc3(c)
 *
 *		int c			process code
 *
 *	The function reloc3() calls a particular relocation
 *	function determined by the process code.
 *
 *	local variable:
 *		none
 *
 *	global variables:
 *		int	lkerr		error flag
 *
 *	called functions:
 *		int	fprintf()	c_library
 *		VOID	rele3()		lkrloc3.c
 *		VOID	relp3()		lkrloc3.c
 *		VOID	relr3()		lkrloc3.c
 *		VOId	relt3()		lkrloc3.c
 *
 *	side effects:
 *		Refer to the called relocation functions.
 *
 */

VOID reloc3(c)
int c;
{
  switch (c) {

  case 'T':
    relt3();
    break;

  case 'R':
    relr3();
    break;

  case 'P':
    relp3();
    break;

  case 'E':
    rele3();
    break;

  default:
    fprintf(stderr, "Undefined Relocation Operation\n");
    lkerr++;
    break;
  }
}

/*)Function	VOID	relt3()
 *
 *	The function relt3() evaluates a T line read by
 *	the linker. Each byte value read is saved in the
 *	rtval[] array, rtflg[] is set, and the number of
 *	evaluations is maintained in rtcnt.
 *
 *		T Line
 *
 *		T xx xx nn nn nn nn nn ...
 *
 *
 *		In:	"T n0 n1 n2 n3 ... nn"
 *
 *		Out:	  0    1    2    ..  rtcnt
 *			+----+----+----+----+----+
 *		  rtval | n0 | n1 | n2 | .. | nn |
 *			+----+----+----+----+----+
 *		  rtflag|  1 |  1 |  1 |  1 |  1 |
 *			+----+----+----+----+----+
 *
 * 	The  T  line contains the assembled code output by the assem-
 *	bler with xx xx being the offset address from the  current  area
 *	base address and nn being the assembled instructions and data in
 *	byte format.
 *
 *	local variable:
 *		none
 *
 *	global variables:
 *		int	rtcnt		number of values evaluated
 *		int	rtflg[]		array of evaluation flags
 *		int	rtval[]		array of evaluation values
 *
 *	called functions:
 *		int	eval()		lkeval.c
 *		int	more()		lklex.c
 *
 *	side effects:
 *		Linker input T line evaluated.
 *
 */

VOID relt3() {
  rtcnt = 0;
  while (more()) {
    if (rtcnt < NTXT) {
      rtval[rtcnt] = eval();
      rtflg[rtcnt] = 1;
      rterr[rtcnt] = 0;
      rtcnt++;
    }
  }
}

/*)Function	VOID	relr3()
 *
 *	The function relr3() evaluates a R line read by
 *	the linker.  The R line data is combined with the
 *	previous T line data to perform the relocation of
 *	code and data bytes.  The S19 / IHX output and
 *	translation of the LST files to RST files may be
 *	performed.
 *
 *		R Line
 *
 *		R 0 0 nn nn n1 n2 xx xx ...
 *
 * 	The R line provides the relocation information to the linker.
 *	The nn nn value is the current area index, i.e.  which area  the
 *	current  values  were  assembled.  Relocation information is en-
 *	coded in groups of 4 bytes:
 *
 *	1.  n1 is the relocation mode and object format
 *	 	1.  bit 0 word(0x00)/byte(0x01)
 *	 	2.  bit 1 relocatable area(0x00)/symbol(0x02)
 *	 	3.  bit 2 normal(0x00)/PC relative(0x04) relocation
 *	 	4.  bit 3 1-byte(0x00)/2-byte(0x08) byte data
 *	 	5.  bit 4 signed(0x00)/unsigned(0x10) byte data
 *	 	6.  bit 5 normal(0x00)/page '0'(0x20) reference
 *	 	7.  bit 6 normal(0x00)/page 'nnn'(0x40) reference
 *		8.  bit 7 LSB(0x00)/MSB(0x80) byte
 *
 *	2.  n2  is  a byte index into the corresponding (i.e.  pre-
 *	 	ceeding) T line data (i.e.  a pointer to the data to be
 *	 	updated  by  the  relocation).   The T line data may be
 *	 	1-byte or  2-byte  byte  data  format  or  2-byte  word
 *	 	format.
 *
 *	3.  xx xx  is the area/symbol index for the area/symbol be-
 *	 	ing referenced.  the corresponding area/symbol is found
 *		in the header area/symbol lists.
 *
 *	The groups of 4 bytes are repeated for each item requiring relo-
 *	cation in the preceeding T line.
 *
 *	local variable:
 *		areax	**a		pointer to array of area pointers
 *		int	aindex		area index
 *		char	*errmsg3[]	array of pointers to error strings
 *		int	error		error code
 *		int	mode		relocation mode
 *		adrr_t	paga		paging base area address
 *		a_uint	pags		paging symbol address
 *		a_uint	reli		relocation initial value
 *		a_uint	relv		relocation final value
 *		int	rindex		symbol / area index
 *		a_uint	rtbase		base code address
 *		a_uint	rtofst		rtval[] index offset
 *		a_uint	rtpofst		rtval[] index offset (initial)
 *		int	rtp		index into T data
 *		sym	**s		pointer to array of symbol pointers
 *		int	v		temporary
 *
 *	global variables:
 *		int	a_bytes		T Line Address Bytes
 *		area	*ap	        pointer to the area structure
 *		head	*hp		pointer to the head structure
 *		int	lkerr		error flag
 *		int	oflag		output type flag
 *		FILE	*ofp	        object output file handle
 *		a_uint	pc		relocated base address
 *		int	pcb	        bytes per instruction word
 *		rerr	rerr		linker error structure
 *		bank	*rtabnk	        current bank structure
 *		int	rtaflg		current bank structure flags
 *		struct	sdp		paging structure
 *		FILE	*stderr		standard error device
 *		int	uflag		relocation listing flag
 *
 *	called functions:
 *		a_uint	adb_1b()	lkrloc.c
 *		a_uint	adb_2b()	lkrloc.c
 *		a_uint	adb_xb()	lkrloc.c
 *		a_uint	adb_lo()	lkrloc3.c
 *		a_uint	adb_hi()	lkrloc3.c
 *		a_uint	evword()	lkrloc.c
 *		int	eval()		lkeval.c
 *		int	fprintf()	c_library
 *		VOID	lkout()		lkout.c
 *		VOID	lkulist		lklist.c
 *		int	more()		lklex.c
 *		VOID	relerr3()	lkrloc3.c
 *		int	symval()	lksym.c
 *
 *	side effects:
 *		The R and T lines are combined to produce
 *		relocated code and data.  Output Sxx / Ixx
 *		and relocated listing files may be produced.
 *
 */

VOID relr3() {
  int mode;
  a_uint reli, relv;
  int aindex, rindex, rtp, error, i;
  a_uint rtbase, rtofst, rtpofst, paga, pags;
  a_uint m, v;
  struct areax **a;
  struct sym **s;

  /*
   * Get area and symbol lists
   */
  a = hp->a_list;
  s = hp->s_list;

  /*
   * Verify Area Mode
   */
  if (eval() != R3_AREA || eval()) {
    fprintf(stderr, "R input error\n");
    lkerr++;
    return;
  }

  /*
   * Get area pointer
   */
  aindex = (int)evword();
  if (aindex >= hp->h_narea) {
    fprintf(stderr, "R area error\n");
    lkerr++;
    return;
  }

  /*
   * Select Output File
   */
  if (oflag != 0) {
    ap = a[aindex]->a_bap;
    if (ofp != NULL) {
      rtabnk->b_rtaflg = rtaflg;
      if (ofp != ap->a_ofp) {
        lkflush();
      }
    }
    ofp = ap->a_ofp;
    rtabnk = ap->a_bp;
    rtaflg = rtabnk->b_rtaflg;
  }

  /*
   * Base values
   */
  rtbase = adb_xb(0, 0);
  rtofst = a_bytes;

  /*
   * Relocate address
   */
  pc = adb_xb(a[aindex]->a_addr, 0);

  /*
   * Number of 'bytes' per PC address
   */
  pcb = 1;

  /*
   * Do remaining relocations
   */
  while (more()) {
    error = 0;
    relv = 0;
    rtpofst = rtofst;
    mode = (int)eval();
    rtp = (int)eval();
    rindex = (int)evword();

    /*
     * R3_SYM or R3_AREA references
     */
    if (mode & R3_SYM) {
      if (rindex >= hp->h_nsym) {
        fprintf(stderr, "R symbol error\n");
        lkerr++;
        return;
      }
      reli = symval(s[rindex]);
    } else {
      if (rindex >= hp->h_narea) {
        fprintf(stderr, "R area error\n");
        lkerr++;
        return;
      }
      reli = a[rindex]->a_addr;
    }

    /*
     * R3_PCR addressing
     */
    if (mode & R3_PCR) {
      if (mode & R3_BYTE) {
        reli -= (pc + (rtp - rtofst) + 1);
      } else {
        reli -= (pc + (rtp - rtofst) + 2);
      }
    }

    /*
     * Standard Modes
     */
    if ((mode & R3_ECHEK) != R3_EXTND) {
      paga = 0;
      pags = 0;
      /*
       * R3_PAG0 or R3_PAG addressing
       */
      if (mode & (R3_PAG0 | R3_PAG)) {
        paga = sdp.s_area->a_addr;
        pags = sdp.s_addr;
        reli -= paga + pags;
      }

      /*
       * R3_BYTE or R3_WORD operation
       */
      if (mode & R3_BYTE) {
        if (mode & R3_BYTX) {
          if (mode & R3_MSB) {
            relv = adb_hi(reli, rtp);
          } else {
            relv = adb_lo(reli, rtp);
          }
          rtofst += (a_bytes - 1);
        } else {
          relv = adb_1b(reli, rtp);
        }
      } else {
        relv = adw_xb(2, reli, rtp);
        rtofst += (a_bytes - 2);
      }

      /*
       * Page Relocation Error Checking
       */
      if (mode & R3_PAG0 && (relv & ~((a_uint)0x000000FF) || paga || pags))
        error = 4;
      if (mode & R3_PAG && (relv & ~((a_uint)0x000000FF)))
        error = 5;
      /*
       * Extended Modes
       */
    } else {
      switch (mode & R3_EMASK) {
      case R3_J11:
        if ((hilo == 0) || (a_bytes < 2)) {
          error = 8;
        }
        /*
         * JLH: 11 bit jump destination for 8051.
         * Forms two byte instruction with
         * op-code bits in the MIDDLE!
         * rtp points at 3 byte locus:
         * first two will get the address,
         * third one has raw op-code
         */
        relv = adw_xb(2, reli, rtp);

        /*
         * Calculate absolute destination
         * relv must be on same 2K page as pc
         */
        if ((relv & ~((a_uint)0x000007FF)) !=
            ((pc + rtp - rtofst) & ~((a_uint)0x000007FF))) {
          error = 6;
        }

        rtofst += (a_bytes - 2);

        /*
         * Merge MSB with op-code,
         * ignoring top 5 bits of address.
         * Then hide the op-code.
         */
        rtval[rtp + (a_bytes - 2)] =
            rtval[rtp + a_bytes] |
            ((rtval[rtp + (a_bytes - 2)] & ((a_uint)0x00000007)) << 5);
        rtflg[rtp + a_bytes] = 0;
        rtofst += 1;
        break;

      case R3_J19:
        if ((hilo == 0) || (a_bytes < 3)) {
          error = 8;
        }
        /*
         * BK: 19 bit jump destination for DS80C390.
         * Forms four byte instruction with
         * op-code bits in the MIDDLE!
         * rtp points at 4 byte locus:
         * first three will get the address,
         * fourth one has raw op-code
         */
        relv = adw_xb(3, reli, rtp);

        /*
         * Calculate absolute destination
         * relv must be on same 512K page as pc
         */
#ifdef LONGINT
        if ((relv & ~((a_uint)0x0007FFFFl)) !=
            ((pc + rtp - rtofst) & ~((a_uint)0x0007FFFFl))) {
          error = 7;
        }
#else
        if ((relv & ~((a_uint)0x0007FFFF)) !=
            ((pc + rtp - rtofst) & ~((a_uint)0x0007FFFF))) {
          error = 7;
        }
#endif

        rtofst += (a_bytes - 3);

        /*
         * Merge MSB with op-code,
         * ignoring top 5 bits of address.
         * Then hide the op-code.
         */
        rtval[rtp + (a_bytes - 3)] =
            rtval[rtp + a_bytes] |
            ((rtval[rtp + (a_bytes - 3)] & ((a_uint)0x00000007)) << 5);
        rtflg[rtp + a_bytes] = 0;
        rtofst += 1;
        break;

      case R3_3BYTE:
        /*
         * 24 bit destination
         */
        relv = adb_3b(reli, rtp);
        break;

      case R3_4BYTE:
        /*
         * 32 bit destination
         */
        relv = adb_4b(reli, rtp);
        break;

      default:
        error = 8;
        break;
      }
    }

    /*
     * Unsigned Byte Checking
     */
    if (mode & R3_USGN && mode & R3_BYTE && relv & ~((a_uint)0x000000FF))
      error = 1;

    /*
     * PCR Relocation Error Checking
     */
    if (mode & R3_PCR) {
      v = relv - reli;
      if ((mode & R3_BYTE) && (mode & R3_BYTX)) {
        m = ~((a_uint)0x0000007F);
        if (((v & m) != m) && ((v & m) != 0)) {
          error = 2;
        }
      } else {
        m = ~((a_uint)0x00007FFF);
        if (((v & m) != m) && ((v & m) != 0)) {
          error = 3;
        }
      }
    }

    /*
     * Error Processing
     */
    if (error) {
      rerr.aindex = aindex;
      rerr.mode = mode;
      rerr.rtbase = rtbase + rtp - rtpofst;
      rerr.rindex = rindex;
      rerr.rval = relv - reli;
      relerr3(errmsg3[error]);

      for (i = rtp; i < rtp + a_bytes; i++) {
        if (rtflg[i]) {
          rterr[i] = error;
          break;
        }
      }
    }
    /*
     * Bank Has Output
     */
    if ((oflag != 0) && (obj_flag == 0)) {
      rtabnk->b_oflag = 1;
    }
  }
  if (uflag != 0) {
    lkulist(1);
  }
  if (oflag != 0) {
    lkout(1);
  }
}

char *errmsg3[] = {
    /* 0 */ "LKRLOC3 Error List",
    /* 1 */ "Unsigned Byte error",
    /* 2 */ "Byte PCR relocation error",
    /* 3 */ "Word PCR relocation error",
    /* 4 */ "Page0 relocation error",
    /* 5 */ "Page Mode relocation error",
    /* 6 */ "2K Page relocation error",
    /* 7 */ "512K Page relocation error",
    /* 8 */ "Undefined Extended Mode error"};

/*)Function	VOID	relp3()
 *
 *	The function relp3() evaluates a P line read by
 *	the linker.  The P line data is combined with the
 *	previous T line data to set the base page address
 *	and test the paging boundary and length.
 *
 *		P Line
 *
 *		P 0 0 nn nn n1 n2 xx xx
 *
 * 	The  P  line provides the paging information to the linker as
 *	specified by a .setdp directive.  The format of  the  relocation
 *	information is identical to that of the R line.  The correspond-
 *	ing T line has the following information:
 *		T xx xx aa aa bb bb
 *
 * 	Where  aa aa is the area reference number which specifies the
 *	selected page area and bb bb is the base address  of  the  page.
 *	bb bb will require relocation processing if the 'n1 n2 xx xx' is
 *	specified in the P line.  The linker will verify that  the  base
 *	address is on a 256 byte boundary and that the page length of an
 *	area defined with the PAG type is not larger than 256 bytes.
 *
 *	local variable:
 *		areax	**a		pointer to array of area pointers
 *		int	aindex		area index
 *		int	mode		relocation mode
 *		a_uint	relv		relocation value
 *		int	rindex		symbol / area index
 *		int	rtp		index into T data
 *		sym	**s		pointer to array of symbol pointers
 *
 *	global variables:
 *		head	*hp		pointer to the head structure
 *		int	lkerr		error flag
 *		sdp	sdp		base page structure
 *		FILE	*stderr		standard error device
 *
 *	called functions:
 *		a_uint	adb_2b()	lkrloc.c
 *		a_uint	evword()	lkrloc.c
 *		int	eval()		lkeval.c
 *		int	fprintf()	c_library
 *		int	more()		lklex.c
 *		int	symval()	lksym.c
 *
 *	side effects:
 *		The P and T lines are combined to set
 *		the base page address and report any
 *		paging errors.
 *
 */

VOID relp3() {
  int aindex, rindex;
  int mode, rtp;
  a_uint relv;
  struct areax **a;
  struct sym **s;

  /*
   * Get area and symbol lists
   */
  a = hp->a_list;
  s = hp->s_list;

  /*
   * Verify Area Mode
   */
  if (eval() != R3_AREA || eval()) {
    fprintf(stderr, "P input error\n");
    lkerr++;
  }

  /*
   * Get area pointer
   */
  aindex = (int)evword();
  if (aindex >= hp->h_narea) {
    fprintf(stderr, "P area error\n");
    lkerr++;
    return;
  }

  /*
   * Do remaining relocations
   */
  while (more()) {
    mode = (int)eval();
    rtp = (int)eval();
    rindex = (int)evword();

    /*
     * R3_SYM or R3_AREA references
     */
    if (mode & R3_SYM) {
      if (rindex >= hp->h_nsym) {
        fprintf(stderr, "P symbol error\n");
        lkerr++;
        return;
      }
      relv = symval(s[rindex]);
    } else {
      if (rindex >= hp->h_narea) {
        fprintf(stderr, "P area error\n");
        lkerr++;
        return;
      }
      relv = a[rindex]->a_addr;
    }
    adb_xb(relv, rtp);
  }

  /*
   * Paged values
   */
  aindex = (int)adb_xb(0, a_bytes);
  if (aindex >= hp->h_narea) {
    fprintf(stderr, "P area error\n");
    lkerr++;
    return;
  }
  sdp.s_areax = a[aindex];
  sdp.s_area = sdp.s_areax->a_bap;
  sdp.s_addr = adb_xb(0, a_bytes * 2);
  if (sdp.s_area->a_addr & 0xFF || sdp.s_addr & 0xFF)
    relerp3("Page Definition Boundary Error");
}

/*)Function	VOID	rele3()
 *
 *	The function rele3() closes all open output files
 *	at the end of the linking process.
 *
 *	local variable:
 *		none
 *
 *	global variables:
 *		int	oflag		output type flag
 *		int	uflag		relocation listing flag
 *
 *	called functions:
 *		VOID	lkfclose()	lkbank.c
 *		VOID	lkflush()	lkout.c
 *		VOID	lkulist()	lklist.c
 *
 *	side effects:
 *		All open output files are closed.
 *
 */

VOID rele3() {
  if (uflag != 0) {
    lkulist(0);
  }
  if (oflag != 0) {
    lkflush();
    lkfclose();
  }
}

/*)Function	VOID	relerr3(str)
 *
 *		char	*str		error string
 *
 *	The function relerr3() outputs the error string to
 *	stderr and to the map file (if it is open).
 *
 *	local variable:
 *		none
 *
 *	global variables:
 *		FILE	*mfp		handle for the map file
 *
 *	called functions:
 *		VOID	errdmp3()	lkrloc3.c
 *
 *	side effects:
 *		Error message inserted into map file.
 *
 */

VOID relerr3(str)
char *str;
{
  errdmp3(stderr, str);
  if (mfp)
    errdmp3(mfp, str);
}

/*)Function	VOID	errdmp3(fptr, str)
 *
 *		FILE	*fptr		output file handle
 *		char	*str		error string
 *
 *	The function errdmp3() outputs the error string str
 *	to the device specified by fptr.  Additional information
 *	is output about the definition and referencing of
 *	the symbol / area error.
 *
 *	local variable:
 *		int	mode		error mode
 *		int	aindex		area index
 *		int	lkerr		error flag
 *		int	rindex		error index
 *		sym	**s		pointer to array of symbol pointers
 *		areax	**a		pointer to array of area pointers
 *		areax	*raxp		error area extension pointer
 *
 *	global variables:
 *		sdp	sdp		base page structure
 *
 *	called functions:
 *		int	fprintf()	c_library
 *		VOID	prntval()	lkrloc.c
 *
 *	side effects:
 *		Error reported.
 *
 */

VOID errdmp3(fptr, str)
FILE *fptr;
char *str;
{
  int mode, aindex, rindex;
  struct sym **s;
  struct areax **a;
  struct areax *raxp;

  a = hp->a_list;
  s = hp->s_list;

  mode = rerr.mode;
  aindex = rerr.aindex;
  rindex = rerr.rindex;

  /*
   * Print Error
   */
  fprintf(fptr, "\n?ASlink-Warning-%s", str);
  lkerr++;

  /*
   * Print symbol if symbol based
   */
  if (mode & R3_SYM) {
    fprintf(fptr, " for symbol  %s\n", &s[rindex]->s_id[0]);
  } else {
    fprintf(fptr, "\n");
  }

  /*
   * Print Ref Info
   */
  /*         11111111112222222222333333333344444444445555555555666666666677777*/
  /*12345678901234567890123456789012345678901234567890123456789012345678901234*/
  /*        |                 |                 |                 |           */
  fprintf(fptr, "         file              module            area             "
                "      offset\n");
  fprintf(fptr, "  Refby  %-14.14s    %-14.14s    %-14.14s    ",
          hp->h_lfile->f_idp, &hp->m_id[0], &a[aindex]->a_bap->a_id[0]);
  prntval(fptr, rerr.rtbase);

  /*
   * Print Def Info
   */
  if (mode & R3_SYM) {
    raxp = s[rindex]->s_axp;
  } else {
    raxp = a[rindex];
  }
  /*         11111111112222222222333333333344444444445555555555666666666677777*/
  /*12345678901234567890123456789012345678901234567890123456789012345678901234*/
  /*        |                 |                 |                 |           */
  fprintf(fptr, "  Defin  %-14.14s    %-14.14s    %-14.14s    ",
          raxp->a_bhp->h_lfile->f_idp, &raxp->a_bhp->m_id[0],
          &raxp->a_bap->a_id[0]);
  if (mode & R3_SYM) {
    prntval(fptr, s[rindex]->s_addr);
  } else {
    prntval(fptr, rerr.rval);
  }
}

/*)Function	VOID	relerp3(str)
 *
 *		char	*str		error string
 *
 *	The function relerp3() outputs the paging error string to
 *	stderr and to the map file (if it is open).
 *
 *	local variable:
 *		none
 *
 *	global variables:
 *		FILE	*mfp		handle for the map file
 *
 *	called functions:
 *		VOID	erpdmp3()	lkrloc3.c
 *
 *	side effects:
 *		Error message inserted into map file.
 *
 */

VOID relerp3(str)
char *str;
{
  erpdmp3(stderr, str);
  if (mfp)
    erpdmp3(mfp, str);
}

/*)Function	VOID	erpdmp3(fptr, str)
 *
 *		FILE	*fptr		output file handle
 *		char	*str		error string
 *
 *	The function erpdmp3() outputs the error string str
 *	to the device specified by fptr.
 *
 *	local variable:
 *		head	*thp		pointer to head structure
 *
 *	global variables:
 *		int	lkerr		error flag
 *		sdp	sdp		base page structure
 *
 *	called functions:
 *		int	fprintf()	c_library
 *		VOID	prntval()	lkrloc.c
 *
 *	side effects:
 *		Error reported.
 *
 */

VOID erpdmp3(fptr, str)
FILE *fptr;
char *str;
{
  struct head *thp;

  thp = sdp.s_areax->a_bhp;

  /*
   * Print Error
   */
  fprintf(fptr, "\n?ASlink-Warning-%s\n", str);
  lkerr++;

  /*
   * Print PgDef Info
   */
  /*         111111111122222222223333333333444444444455555555556666666666777*/
  /*123456789012345678901234567890123456789012345678901234567890123456789012*/
  fprintf(fptr, "         file              module            pgarea           "
                " pgoffset\n");
  fprintf(fptr, "  PgDef  %-14.14s    %-14.14s    %-14.14s    ",
          thp->h_lfile->f_idp, &thp->m_id[0], &sdp.s_area->a_id[0]);
  prntval(fptr, sdp.s_area->a_addr + sdp.s_addr);
}

/*)Function	a_uint 	adb_lo(v, i)
 *
 *		int	v		value to add to byte
 *		int	i		rtval[] index
 *
 *	The function adb_lo() adds the value of v to the
 *	value contained in rtval[i] through rtval[i + a_bytes - 1].
 *	The new value of rtval[i] ... is returned.
 *	The rtflg[] flags are cleared for all rtval[i] ... except
 *	the LSB.
 *
 *	local variable:
 *		a_uint	j		temporary evaluation variable
 *
 *	global variables:
 *		hilo			byte ordering parameter
 *
 *	called functions:
 *		none
 *
 *	side effects:
 *		The value of rtval[] is changed.
 *		The rtflg[] values corresponding to all bytes
 *		except the LSB of the value are cleared to reflect
 *		the fact that the LSB is the selected byte.
 *
 */

a_uint adb_lo(v, i)
a_uint v;
int i;
{
  a_uint j;
  int m, n;

  j = adb_xb(v, i);
  /*
   * LSB is lowest order byte of data
   */
  m = (hilo ? a_bytes - 1 : 0);
  for (n = 0; n < a_bytes; n++) {
    if (n != m)
      rtflg[i + n] = 0;
  }
  return (j);
}

/*)Function	a_uint 	adb_hi(v, i)
 *
 *		int	v		value to add to byte
 *		int	i		rtval[] index
 *
 *	The function adb_hi() adds the value of v to the
 *	value contained in rtval[i] through rtval[i + a_bytes - 1].
 *	The new value of rtval[i] .... is returned.
 *	The LSB rtflg[] is cleared.
 *
 *	local variable:
 *		a_uint	j		temporary evaluation variable
 *
 *	global variables:
 *		hilo			byte ordering parameter
 *
 *	called functions:
 *		none
 *
 *	side effects:
 *		The value of rtval[] is changed.
 *		The rtflg[] values corresponding to all bytes
 *		except the 2nd byte (MSB) are cleared to reflect
 *		the fact that the MSB is the selected byte.
 *
 */

a_uint adb_hi(v, i)
a_uint v;
int i;
{
  a_uint j;
  int m, n;

  j = adb_xb(v, i);
  /*
   * MSB is next lowest order byte of data
   */
  m = (hilo ? a_bytes - 2 : 1);
  for (n = 0; n < a_bytes; n++) {
    if (n != m)
      rtflg[i + n] = 0;
  }
  return (j);
}
