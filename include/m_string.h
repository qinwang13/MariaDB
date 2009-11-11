/* Copyright (C) 2000 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

/* There may be prolems include all of theese. Try to test in
   configure with ones are needed? */

/*  This is needed for the definitions of strchr... on solaris */

#ifndef _m_string_h
#define _m_string_h
#ifndef __USE_GNU
#define __USE_GNU				/* We want to use stpcpy */
#endif
#if defined(HAVE_STRINGS_H)
#include <strings.h>
#endif
#if defined(HAVE_STRING_H)
#include <string.h>
#endif

/* need by my_vsnprintf */
#include <stdarg.h> 

#ifdef _AIX
#undef HAVE_BCMP
#endif

/*  This is needed for the definitions of bzero... on solaris */
#if defined(HAVE_STRINGS_H)
#include <strings.h>
#endif

/*  This is needed for the definitions of memcpy... on solaris */
#if defined(HAVE_MEMORY_H) && !defined(__cplusplus)
#include <memory.h>
#endif

#if !defined(HAVE_MEMCPY) && !defined(HAVE_MEMMOVE)
# define memcpy(d, s, n)	bcopy ((s), (d), (n))
# define memset(A,C,B)		bfill((A),(B),(C))
# define memmove(d, s, n)	bmove ((d), (s), (n))
#elif defined(HAVE_MEMMOVE)
# define bmove(d, s, n)		memmove((d), (s), (n))
#else
# define memmove(d, s, n)	bmove((d), (s), (n)) /* our bmove */
#endif

/* Unixware 7 */
#if !defined(HAVE_BFILL)
# define bfill(A,B,C)           memset((A),(C),(B))
# define bmove_align(A,B,C)    memcpy((A),(B),(C))
#endif

#if !defined(HAVE_BCMP)
# define bcopy(s, d, n)		memcpy((d), (s), (n))
# define bcmp(A,B,C)		memcmp((A),(B),(C))
# define bzero(A,B)		memset((A),0,(B))
# define bmove_align(A,B,C)     memcpy((A),(B),(C))
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/*
  my_str_malloc() and my_str_free() are assigned to implementations in
  strings/alloc.c, but can be overridden in the calling program.
 */
extern void *(*my_str_malloc)(size_t);
extern void (*my_str_free)(void *);

#if defined(HAVE_STPCPY)
#define strmov(A,B) stpcpy((A),(B))
#ifndef stpcpy
extern char *stpcpy(char *, const char *);	/* For AIX with gcc 2.95.3 */
#endif
#endif

/* Declared in int2str() */
extern char NEAR _dig_vec_upper[];
extern char NEAR _dig_vec_lower[];

/* Defined in strtod.c */
extern const double log_10[309];

#ifdef BAD_STRING_COMPILER
#define strmov(A,B)  (memccpy(A,B,0,INT_MAX)-1)
#else
#define strmov_overlapp(A,B) strmov(A,B)
#define strmake_overlapp(A,B,C) strmake(A,B,C)
#endif

#ifdef BAD_MEMCPY			/* Problem with gcc on Alpha */
#define memcpy_fixed(A,B,C) bmove((A),(B),(C))
#else
#define memcpy_fixed(A,B,C) memcpy((A),(B),(C))
#endif

#if (!defined(USE_BMOVE512) || defined(HAVE_purify)) && !defined(bmove512)
#define bmove512(A,B,C) memcpy(A,B,C)
#endif

	/* Prototypes for string functions */

#if !defined(bfill) && !defined(HAVE_BFILL)
extern	void bfill(uchar *dst,size_t len,pchar fill);
#endif

#if !defined(bzero) && !defined(HAVE_BZERO)
extern	void bzero(uchar * dst,size_t len);
#endif

#if !defined(bcmp) && !defined(HAVE_BCMP)
extern	size_t bcmp(const uchar *s1,const uchar *s2,size_t len);
#endif
#ifdef HAVE_purify
extern	size_t my_bcmp(const uchar *s1,const uchar *s2,size_t len);
#undef bcmp
#define bcmp(A,B,C) my_bcmp((A),(B),(C))
#define bzero_if_purify(A,B) bzero(A,B)
#else
#define bzero_if_purify(A,B)
#endif /* HAVE_purify */

#ifndef bmove512
extern	void bmove512(uchar *dst,const uchar *src,size_t len);
#endif

#if !defined(HAVE_BMOVE) && !defined(bmove)
extern	void bmove(uuchar *dst, const uchar *src,size_t len);
#endif

extern	void bmove_upp(uchar *dst,const uchar *src,size_t len);
extern	void bchange(uchar *dst,size_t old_len,const uchar *src,
		     size_t new_len,size_t tot_len);
extern	void strappend(char *s,size_t len,pchar fill);
extern	char *strend(const char *s);
extern  char *strcend(const char *, pchar);
extern	char *strfield(char *src,int fields,int chars,int blanks,
			   int tabch);
extern	char *strfill(char * s,size_t len,pchar fill);
extern	size_t strinstr(const char *str,const char *search);
extern  size_t r_strinstr(const char *str, size_t from, const char *search);
extern	char *strkey(char *dst,char *head,char *tail,char *flags);
extern	char *strmake(char *dst,const char *src,size_t length);
#ifndef strmake_overlapp
extern	char *strmake_overlapp(char *dst,const char *src, size_t length);
#endif

#ifndef strmov
extern	char *strmov(char *dst,const char *src);
#endif
extern	char *strnmov(char *dst,const char *src,size_t n);
extern	char *strsuff(const char *src,const char *suffix);
extern	char *strcont(const char *src,const char *set);
extern	char *strxcat _VARARGS((char *dst,const char *src, ...));
extern	char *strxmov _VARARGS((char *dst,const char *src, ...));
extern	char *strxcpy _VARARGS((char *dst,const char *src, ...));
extern	char *strxncat _VARARGS((char *dst,size_t len, const char *src, ...));
extern	char *strxnmov _VARARGS((char *dst,size_t len, const char *src, ...));
extern	char *strxncpy _VARARGS((char *dst,size_t len, const char *src, ...));

/* Prototypes of normal stringfunctions (with may ours) */

#ifdef WANT_STRING_PROTOTYPES
extern char *strcat(char *, const char *);
extern char *strchr(const char *, pchar);
extern char *strrchr(const char *, pchar);
extern char *strcpy(char *, const char *);
extern int strcmp(const char *, const char *);
#ifndef __GNUC__
extern size_t strlen(const char *);
#endif
#endif
#ifndef HAVE_STRNLEN
extern size_t strnlen(const char *s, size_t n);
#endif

#if !defined(__cplusplus)
#ifndef HAVE_STRPBRK
extern char *strpbrk(const char *, const char *);
#endif
#ifndef HAVE_STRSTR
extern char *strstr(const char *, const char *);
#endif
#endif
extern int is_prefix(const char *, const char *);

/* Conversion routines */
double my_strtod(const char *str, char **end, int *error);
double my_atof(const char *nptr);

extern char *llstr(longlong value,char *buff);
extern char *ullstr(longlong value,char *buff);
#ifndef HAVE_STRTOUL
extern long strtol(const char *str, char **ptr, int base);
extern ulong strtoul(const char *str, char **ptr, int base);
#endif

extern char *int2str(long val, char *dst, int radix, int upcase);
extern char *int10_to_str(long val,char *dst,int radix);
extern char *str2int(const char *src,int radix,long lower,long upper,
			 long *val);
longlong my_strtoll10(const char *nptr, char **endptr, int *error);
#if SIZEOF_LONG == SIZEOF_LONG_LONG
#define longlong2str(A,B,C) int2str((A),(B),(C),1)
#define longlong10_to_str(A,B,C) int10_to_str((A),(B),(C))
#undef strtoll
#define strtoll(A,B,C) strtol((A),(B),(C))
#define strtoull(A,B,C) strtoul((A),(B),(C))
#ifndef HAVE_STRTOULL
#define HAVE_STRTOULL
#endif
#ifndef HAVE_STRTOLL
#define HAVE_STRTOLL
#endif
#else
#ifdef HAVE_LONG_LONG
extern char *longlong2str(longlong val,char *dst,int radix);
extern char *longlong10_to_str(longlong val,char *dst,int radix);
#if (!defined(HAVE_STRTOULL) || defined(NO_STRTOLL_PROTO))
extern longlong strtoll(const char *str, char **ptr, int base);
extern ulonglong strtoull(const char *str, char **ptr, int base);
#endif
#endif
#endif

/* my_vsnprintf.c */

extern size_t my_vsnprintf(char *str, size_t n,
                           const char *format, va_list ap);
extern size_t my_snprintf(char *to, size_t n, const char *fmt, ...)
  ATTRIBUTE_FORMAT(printf, 3, 4);

#if defined(__cplusplus)
}
#endif

/*
  LEX_STRING -- a pair of a C-string and its length.
*/

#ifndef _my_plugin_h
/* This definition must match the one given in mysql/plugin.h */
struct st_mysql_lex_string
{
  char *str;
  size_t length;
};
#endif
typedef struct st_mysql_lex_string LEX_STRING;

#define STRING_WITH_LEN(X) (X), ((size_t) (sizeof(X) - 1))
#define USTRING_WITH_LEN(X) ((uchar*) X), ((size_t) (sizeof(X) - 1))
#define C_STRING_WITH_LEN(X) ((char *) (X)), ((size_t) (sizeof(X) - 1))

/* SPACE_INT is a word that contains only spaces */
#if SIZEOF_INT == 4
#define SPACE_INT 0x20202020
#elif SIZEOF_INT == 8
#define SPACE_INT 0x2020202020202020
#else
#error define the appropriate constant for a word full of spaces
#endif

/**
  Skip trailing space.

  On most systems reading memory in larger chunks (ideally equal to the size of
  the chinks that the machine physically reads from memory) causes fewer memory
  access loops and hence increased performance.
  This is why the 'int' type is used : it's closest to that (according to how
  it's defined in C).
  So when we determine the amount of whitespace at the end of a string we do
  the following :
    1. We divide the string into 3 zones :
      a) from the start of the string (__start) to the first multiple
        of sizeof(int)  (__start_words)
      b) from the end of the string (__end) to the last multiple of sizeof(int)
        (__end_words)
      c) a zone that is aligned to sizeof(int) and can be safely accessed
        through an int *
    2. We start comparing backwards from (c) char-by-char. If all we find is
       space then we continue
    3. If there are elements in zone (b) we compare them as unsigned ints to a
       int mask (SPACE_INT) consisting of all spaces
    4. Finally we compare the remaining part (a) of the string char by char.
       This covers for the last non-space unsigned int from 3. (if any)

   This algorithm works well for relatively larger strings, but it will slow
   the things down for smaller strings (because of the additional calculations
   and checks compared to the naive method). Thus the barrier of length 20
   is added.

   @param     ptr   pointer to the input string
   @param     len   the length of the string
   @return          the last non-space character
*/

static inline const uchar *skip_trailing_space(const uchar *ptr,size_t len)
{
  const uchar *end= ptr + len;

  if (len > 20)
  {
    const uchar *end_words= (const uchar *)(intptr)
      (((ulonglong)(intptr)end) / SIZEOF_INT * SIZEOF_INT);
    const uchar *start_words= (const uchar *)(intptr)
       ((((ulonglong)(intptr)ptr) + SIZEOF_INT - 1) / SIZEOF_INT * SIZEOF_INT);

    DBUG_ASSERT(((ulonglong)(intptr)ptr) >= SIZEOF_INT);
    if (end_words > ptr)
    {
      while (end > end_words && end[-1] == 0x20)
        end--;
      if (end[-1] == 0x20 && start_words < end_words)
        while (end > start_words && ((unsigned *)end)[-1] == SPACE_INT)
          end -= SIZEOF_INT;
    }
  }
  while (end > ptr && end[-1] == 0x20)
    end--;
  return (end);
}

#endif
