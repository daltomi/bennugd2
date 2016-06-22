/*
 *  Copyright (C) 2006-2016 SplinterGU (Fenix/BennuGD)
 *  Copyright (C) 2002-2006 Fenix Team (Fenix)
 *  Copyright (C) 1999-2002 José Luis Cebrián Pagüe (Fenix)
 *
 *  This file is part of Bennu Game Development
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bgddl.h"

#include "files.h"
#include "xstrings.h"

#include "libmod_misc.h"

/* STRINGS */

/** LEN (STRING SOURCE)
 *  Returns the size of a string
 */

int64_t libmod_misc_string_strlen( INSTANCE * my, int64_t * params ) {
    const char * str = string_get( params[0] ) ;
    int64_t r = str ? strlen( str ) : 0 ;
    string_discard( params[0] ) ;
    return r ;
}

/** STRING UCASE (STRING SOURCE)
 *  Converts a string to upper-case
 */

int64_t libmod_misc_string_strupper( INSTANCE * my, int64_t * params ) {
    int64_t r = string_ucase( params[0] ) ;
    string_discard( params[0] ) ;
    string_use( r ) ;
    return r ;
}

/** STRING LCASE (STRING SOURCE)
 *  Converts a string to lower-case
 */

int64_t libmod_misc_string_strlower( INSTANCE * my, int64_t * params ) {
    int64_t r = string_lcase( params[0] ) ;
    string_discard( params[0] ) ;
    string_use( r ) ;
    return r ;
}

/** STRCASECMP (STRING S1, STRING S2)
 *  Compares two strings, case-insensitive
 */

int64_t libmod_misc_string_strcasecmp( INSTANCE * my, int64_t * params ) {
    int64_t r = string_casecmp( params[0], params[1] ) ;
    string_discard( params[0] ) ;
    string_discard( params[1] ) ;
    return r ;
}

/** SUBSTR (STRING SOURCE, INT FIRST_CHARACTER, INT COUNT)
 *  Returns part of a given string, starting at the given character position
 *  and returning a string limited to COUNT characters
 */

int64_t libmod_misc_string_substr( INSTANCE * my, int64_t * params ) {
    int64_t r = string_substr( params[0], params[1], ( params[2] < 0 ) ? ( params[2] - 1 ) : params[2] ) ;
    string_discard( params[0] ) ;
    string_use( r ) ;
    return r ;
}

/** SUBSTR (STRING SOURCE, INT FIRST_CHARACTER)
 *  Returns a substring, from the character given to the end of the source string
 */

int64_t libmod_misc_string_substr2( INSTANCE * my, int64_t * params ) {
    int64_t r = string_substr( params[0], params[1], -1 ) ;
    string_discard( params[0] ) ;
    string_use( r ) ;
    return r ;
}

/** FIND (STRING SOURCE, STRING SUBSTRING)
 *  Searchs a substring in a string, and returns its position
 */

int64_t libmod_misc_string_strfind( INSTANCE * my, int64_t * params ) {
    int64_t r = string_find( params[0], params[1], 0 ) ;
    string_discard( params[0] ) ;
    string_discard( params[1] ) ;
    return r ;
}

/** FIND (STRING SOURCE, STRING SUBSTRING, INT FIRST)
 *  Searchs a substring in a string, starting from the given position, and returns its position
 */

int64_t libmod_misc_string_strfindSSI( INSTANCE * my, int64_t * params ) {
    int64_t r = string_find( params[0], params[1], params[2] ) ;
    string_discard( params[0] ) ;
    string_discard( params[1] ) ;
    return r ;
}

/** STRING LPAD (STRING SOURCE, LENGTH)
 *  Expands the string up to the given length, adding spaces at the left
 */

int64_t libmod_misc_string_lpad( INSTANCE * my, int64_t * params ) {
    int64_t r = string_pad( params[0], params[1], 0 );
    string_discard( params[0] );
    string_use( r );
    return r;
}

/** STRING RPAD (STRING SOURCE, LENGTH)
 *  Expands the string up to the given length, adding spaces at the right
 */

int64_t libmod_misc_string_rpad( INSTANCE * my, int64_t * params ) {
    int64_t r = string_pad( params[0], params[1], 1 );
    string_discard( params[0] );
    string_use( r );
    return r;
}

/** ITOA (INT VALUE)
 *  Converts an integer to string
 */

int64_t libmod_misc_string_itos( INSTANCE * my, int64_t * params ) {
    int64_t r = string_itoa( params[0] ) ;
    string_use( r ) ;
    return r ;
}

/** FTOA (FLOAT VALUE)
 *  Converts a floating-point number to string
 */

int64_t libmod_misc_string_ftos( INSTANCE * my, int64_t * params ) {
    int64_t r = string_ftoa( *( double * ) & params[0] ) ;
    string_use( r ) ;
    return r ;
}

/** ATOI (STRING VALUE)
 *  Converts a string to integer
 */

int64_t libmod_misc_string_stoi( INSTANCE * my, int64_t * params ) {
    const char * str = string_get( params[0] ) ;
    int64_t r = str ? atoll( str ) : 0 ;
    string_discard( params[0] ) ;
    return r ;
}

/** ATOF (STRING VALUE)
 *  Converts a string to floating-point number
 */

int64_t libmod_misc_string_stof( INSTANCE * my, int64_t * params ) {
    const char * str = string_get( params[0] ) ;
    double res = ( double )( str ? atof( str ) : 0 );
    string_discard( params[0] ) ;
    return *(( int64_t * )&res ) ;
}

/** ASC(STRING C)
 *  Return the ASCII code of the first character at the string
 */

int64_t libmod_misc_string_asc( INSTANCE * my, int64_t * params ) {
    const unsigned char * str = ( unsigned char * ) string_get( params[0] ) ;
    int64_t r = str ? *str : '\0' ;
    string_discard( params[0] ) ;
    return r ;
}

/** CHR(ASCII)
 *  Returns a string of length 1, with the character of the given ASCII code
 */

int64_t libmod_misc_string_chr( INSTANCE * my, int64_t * params ) {
    unsigned char buffer[2] = " " ;
    int64_t r ;
    buffer[0] = ( unsigned char ) params[0] ;
    r = string_new( ( char * ) buffer ) ;
    string_use( r ) ;
    return r ;
}

/** STRING TRIM(STRING SOURCE)
 *  Returns the given string, stripping any space characters at the beginning or the end
 */

int64_t libmod_misc_string_trim( INSTANCE * my, int64_t * params ) {
    int64_t r = string_strip( params[0] ) ;
    string_discard( params[0] ) ;
    string_use( r ) ;
    return r;
}

#ifndef WIN32
static char * strrev( char * str ) {
    char *start = str;
    char *left = str;
    char ch;

    while ( *str++ ); str -= 2;

    while ( left < str ) {
        ch = *left;
        *left++ = *str;
        *str-- = ch;
    }

    return( start );
}
#endif

/** STRING STRREV (STRING SOURCE)
 *  Returns the reverse of the source string
 */

int64_t libmod_misc_string_strrev( INSTANCE * my, int64_t * params ) {
    int64_t r = string_new( string_get( params[0] ) );
    string_discard( params[0] ) ;
    string_use( r ) ;
    strrev(( char * ) string_get( r ) );
    return r;
}

/** FORMAT (INT VALUE)
 *  Converts a given integer value to string form
 */

int64_t libmod_misc_string_formatI( INSTANCE * my, int64_t * params ) {
    int64_t r = string_format( params[0], 0, '.', ',' );
    string_use( r ) ;
    return r;
}

/** FORMAT (FLOAT VALUE)
 *  Converts a given value to string form
 */

int64_t libmod_misc_string_formatF( INSTANCE * my, int64_t * params ) {
    int64_t r = string_format( *( double * ) &params[0], -1, '.', ',' );
    string_use( r ) ;
    return r;
}

/** FORMAT (INT VALUE, INT DECIMALS)
 *  Converts a given integer value to string form. Uses a fixed number
 *  of decimals, as given with the second parameter.
 */

int64_t libmod_misc_string_formatFI( INSTANCE * my, int64_t * params ) {
    int64_t r = string_format( *( double * ) &params[0], params[1], '.', ',' );
    string_use( r ) ;
    return r;
}

/* ----------------------------------------------------------------- */