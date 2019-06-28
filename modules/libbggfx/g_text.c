/*
 *  Copyright (C) 2006-2019 SplinterGU (Fenix/BennuGD)
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

/* --------------------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>

#include "bgdrtm.h"

#include "bgddl.h"
#include "dlvaracc.h"

#include "xstrings.h"

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */

typedef struct _text {
    int64_t id;
    int64_t on;                /* 1 - Text; 2 - VarSTR; 3 - VarINT; 4 - VarFLOAT; 5 - VarWORD; 6 - VarBYTE */
    int64_t fontid;
    int64_t x;
    int64_t y;
    int64_t z;
    int64_t alignment;

    Uint8 alpha;
    Uint8 color_r;
    Uint8 color_g;
    Uint8 color_b;

    int64_t objectid;

    char * text;
    const void * var;     /* CHANGED TO VOID to allow diff. data types */

    /* Internals, for speed up */
    int64_t _x;
    int64_t _y;

} TEXT;

/* --------------------------------------------------------------------------- */

TEXT texts[MAX_TEXTS];

int64_t text_nextid = 1;
int64_t text_count  = 0;

/* --------------------------------------------------------------------------- */

static uint32_t ansi_colors_4[][3] = {
    { 0x00, 0x00, 0x00 }, { 0x80, 0x00, 0x00 }, { 0x00, 0x80, 0x00 }, { 0x80, 0x80, 0x00 },
    { 0x00, 0x00, 0x80 }, { 0x80, 0x00, 0x80 }, { 0x00, 0x80, 0x80 }, { 0xc0, 0xc0, 0xc0 },
    { 0x80, 0x80, 0x80 }, { 0xff, 0x00, 0x00 }, { 0x00, 0xff, 0x00 }, { 0xff, 0xff, 0x00 },
    { 0x00, 0x00, 0xff }, { 0xff, 0x00, 0xff }, { 0x00, 0xff, 0xff }, { 0xff, 0xff, 0xff }
};

static uint32_t ansi_colors_8[][3] = {
    { 0x00, 0x00, 0x00 }, { 0x80, 0x00, 0x00 }, { 0x00, 0x80, 0x00 }, { 0x80, 0x80, 0x00 },
    { 0x00, 0x00, 0x80 }, { 0x80, 0x00, 0x80 }, { 0x00, 0x80, 0x80 }, { 0xc0, 0xc0, 0xc0 },
    { 0x80, 0x80, 0x80 }, { 0xff, 0x00, 0x00 }, { 0x00, 0xff, 0x00 }, { 0xff, 0xff, 0x00 },
    { 0x00, 0x00, 0xff }, { 0xff, 0x00, 0xff }, { 0x00, 0xff, 0xff }, { 0xff, 0xff, 0xff },
    { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x5f }, { 0x00, 0x00, 0x87 }, { 0x00, 0x00, 0xaf },
    { 0x00, 0x00, 0xd7 }, { 0x00, 0x00, 0xff }, { 0x00, 0x5f, 0x00 }, { 0x00, 0x5f, 0x5f },
    { 0x00, 0x5f, 0x87 }, { 0x00, 0x5f, 0xaf }, { 0x00, 0x5f, 0xd7 }, { 0x00, 0x5f, 0xff },
    { 0x00, 0x87, 0x00 }, { 0x00, 0x87, 0x5f }, { 0x00, 0x87, 0x87 }, { 0x00, 0x87, 0xaf },
    { 0x00, 0x87, 0xd7 }, { 0x00, 0x87, 0xff }, { 0x00, 0xaf, 0x00 }, { 0x00, 0xaf, 0x5f },
    { 0x00, 0xaf, 0x87 }, { 0x00, 0xaf, 0xaf }, { 0x00, 0xaf, 0xd7 }, { 0x00, 0xaf, 0xff },
    { 0x00, 0xd7, 0x00 }, { 0x00, 0xd7, 0x5f }, { 0x00, 0xd7, 0x87 }, { 0x00, 0xd7, 0xaf },
    { 0x00, 0xd7, 0xd7 }, { 0x00, 0xd7, 0xff }, { 0x00, 0xff, 0x00 }, { 0x00, 0xff, 0x5f },
    { 0x00, 0xff, 0x87 }, { 0x00, 0xff, 0xaf }, { 0x00, 0xff, 0xd7 }, { 0x00, 0xff, 0xff },
    { 0x5f, 0x00, 0x00 }, { 0x5f, 0x00, 0x5f }, { 0x5f, 0x00, 0x87 }, { 0x5f, 0x00, 0xaf },
    { 0x5f, 0x00, 0xd7 }, { 0x5f, 0x00, 0xff }, { 0x5f, 0x5f, 0x00 }, { 0x5f, 0x5f, 0x5f },
    { 0x5f, 0x5f, 0x87 }, { 0x5f, 0x5f, 0xaf }, { 0x5f, 0x5f, 0xd7 }, { 0x5f, 0x5f, 0xff },
    { 0x5f, 0x87, 0x00 }, { 0x5f, 0x87, 0x5f }, { 0x5f, 0x87, 0x87 }, { 0x5f, 0x87, 0xaf },
    { 0x5f, 0x87, 0xd7 }, { 0x5f, 0x87, 0xff }, { 0x5f, 0xaf, 0x00 }, { 0x5f, 0xaf, 0x5f },
    { 0x5f, 0xaf, 0x87 }, { 0x5f, 0xaf, 0xaf }, { 0x5f, 0xaf, 0xd7 }, { 0x5f, 0xaf, 0xff },
    { 0x5f, 0xd7, 0x00 }, { 0x5f, 0xd7, 0x5f }, { 0x5f, 0xd7, 0x87 }, { 0x5f, 0xd7, 0xaf },
    { 0x5f, 0xd7, 0xd7 }, { 0x5f, 0xd7, 0xff }, { 0x5f, 0xff, 0x00 }, { 0x5f, 0xff, 0x5f },
    { 0x5f, 0xff, 0x87 }, { 0x5f, 0xff, 0xaf }, { 0x5f, 0xff, 0xd7 }, { 0x5f, 0xff, 0xff },
    { 0x87, 0x00, 0x00 }, { 0x87, 0x00, 0x5f }, { 0x87, 0x00, 0x87 }, { 0x87, 0x00, 0xaf },
    { 0x87, 0x00, 0xd7 }, { 0x87, 0x00, 0xff }, { 0x87, 0x5f, 0x00 }, { 0x87, 0x5f, 0x5f },
    { 0x87, 0x5f, 0x87 }, { 0x87, 0x5f, 0xaf }, { 0x87, 0x5f, 0xd7 }, { 0x87, 0x5f, 0xff },
    { 0x87, 0x87, 0x00 }, { 0x87, 0x87, 0x5f }, { 0x87, 0x87, 0x87 }, { 0x87, 0x87, 0xaf },
    { 0x87, 0x87, 0xd7 }, { 0x87, 0x87, 0xff }, { 0x87, 0xaf, 0x00 }, { 0x87, 0xaf, 0x5f },
    { 0x87, 0xaf, 0x87 }, { 0x87, 0xaf, 0xaf }, { 0x87, 0xaf, 0xd7 }, { 0x87, 0xaf, 0xff },
    { 0x87, 0xd7, 0x00 }, { 0x87, 0xd7, 0x5f }, { 0x87, 0xd7, 0x87 }, { 0x87, 0xd7, 0xaf },
    { 0x87, 0xd7, 0xd7 }, { 0x87, 0xd7, 0xff }, { 0x87, 0xff, 0x00 }, { 0x87, 0xff, 0x5f },
    { 0x87, 0xff, 0x87 }, { 0x87, 0xff, 0xaf }, { 0x87, 0xff, 0xd7 }, { 0x87, 0xff, 0xff },
    { 0xaf, 0x00, 0x00 }, { 0xaf, 0x00, 0x5f }, { 0xaf, 0x00, 0x87 }, { 0xaf, 0x00, 0xaf },
    { 0xaf, 0x00, 0xd7 }, { 0xaf, 0x00, 0xff }, { 0xaf, 0x5f, 0x00 }, { 0xaf, 0x5f, 0x5f },
    { 0xaf, 0x5f, 0x87 }, { 0xaf, 0x5f, 0xaf }, { 0xaf, 0x5f, 0xd7 }, { 0xaf, 0x5f, 0xff },
    { 0xaf, 0x87, 0x00 }, { 0xaf, 0x87, 0x5f }, { 0xaf, 0x87, 0x87 }, { 0xaf, 0x87, 0xaf },
    { 0xaf, 0x87, 0xd7 }, { 0xaf, 0x87, 0xff }, { 0xaf, 0xaf, 0x00 }, { 0xaf, 0xaf, 0x5f },
    { 0xaf, 0xaf, 0x87 }, { 0xaf, 0xaf, 0xaf }, { 0xaf, 0xaf, 0xd7 }, { 0xaf, 0xaf, 0xff },
    { 0xaf, 0xd7, 0x00 }, { 0xaf, 0xd7, 0x5f }, { 0xaf, 0xd7, 0x87 }, { 0xaf, 0xd7, 0xaf },
    { 0xaf, 0xd7, 0xd7 }, { 0xaf, 0xd7, 0xff }, { 0xaf, 0xff, 0x00 }, { 0xaf, 0xff, 0x5f },
    { 0xaf, 0xff, 0x87 }, { 0xaf, 0xff, 0xaf }, { 0xaf, 0xff, 0xd7 }, { 0xaf, 0xff, 0xff },
    { 0xd7, 0x00, 0x00 }, { 0xd7, 0x00, 0x5f }, { 0xd7, 0x00, 0x87 }, { 0xd7, 0x00, 0xaf },
    { 0xd7, 0x00, 0xd7 }, { 0xd7, 0x00, 0xff }, { 0xd7, 0x5f, 0x00 }, { 0xd7, 0x5f, 0x5f },
    { 0xd7, 0x5f, 0x87 }, { 0xd7, 0x5f, 0xaf }, { 0xd7, 0x5f, 0xd7 }, { 0xd7, 0x5f, 0xff },
    { 0xd7, 0x87, 0x00 }, { 0xd7, 0x87, 0x5f }, { 0xd7, 0x87, 0x87 }, { 0xd7, 0x87, 0xaf },
    { 0xd7, 0x87, 0xd7 }, { 0xd7, 0x87, 0xff }, { 0xd7, 0xaf, 0x00 }, { 0xd7, 0xaf, 0x5f },
    { 0xd7, 0xaf, 0x87 }, { 0xd7, 0xaf, 0xaf }, { 0xd7, 0xaf, 0xd7 }, { 0xd7, 0xaf, 0xff },
    { 0xd7, 0xd7, 0x00 }, { 0xd7, 0xd7, 0x5f }, { 0xd7, 0xd7, 0x87 }, { 0xd7, 0xd7, 0xaf },
    { 0xd7, 0xd7, 0xd7 }, { 0xd7, 0xd7, 0xff }, { 0xd7, 0xff, 0x00 }, { 0xd7, 0xff, 0x5f },
    { 0xd7, 0xff, 0x87 }, { 0xd7, 0xff, 0xaf }, { 0xd7, 0xff, 0xd7 }, { 0xd7, 0xff, 0xff },
    { 0xff, 0x00, 0x00 }, { 0xff, 0x00, 0x5f }, { 0xff, 0x00, 0x87 }, { 0xff, 0x00, 0xaf },
    { 0xff, 0x00, 0xd7 }, { 0xff, 0x00, 0xff }, { 0xff, 0x5f, 0x00 }, { 0xff, 0x5f, 0x5f },
    { 0xff, 0x5f, 0x87 }, { 0xff, 0x5f, 0xaf }, { 0xff, 0x5f, 0xd7 }, { 0xff, 0x5f, 0xff },
    { 0xff, 0x87, 0x00 }, { 0xff, 0x87, 0x5f }, { 0xff, 0x87, 0x87 }, { 0xff, 0x87, 0xaf },
    { 0xff, 0x87, 0xd7 }, { 0xff, 0x87, 0xff }, { 0xff, 0xaf, 0x00 }, { 0xff, 0xaf, 0x5f },
    { 0xff, 0xaf, 0x87 }, { 0xff, 0xaf, 0xaf }, { 0xff, 0xaf, 0xd7 }, { 0xff, 0xaf, 0xff },
    { 0xff, 0xd7, 0x00 }, { 0xff, 0xd7, 0x5f }, { 0xff, 0xd7, 0x87 }, { 0xff, 0xd7, 0xaf },
    { 0xff, 0xd7, 0xd7 }, { 0xff, 0xd7, 0xff }, { 0xff, 0xff, 0x00 }, { 0xff, 0xff, 0x5f },
    { 0xff, 0xff, 0x87 }, { 0xff, 0xff, 0xaf }, { 0xff, 0xff, 0xd7 }, { 0xff, 0xff, 0xff },
    { 0x08, 0x08, 0x08 }, { 0x12, 0x12, 0x12 }, { 0x1c, 0x1c, 0x1c }, { 0x26, 0x26, 0x26 },
    { 0x30, 0x30, 0x30 }, { 0x3a, 0x3a, 0x3a }, { 0x44, 0x44, 0x44 }, { 0x4e, 0x4e, 0x4e },
    { 0x58, 0x58, 0x58 }, { 0x62, 0x62, 0x62 }, { 0x6c, 0x6c, 0x6c }, { 0x76, 0x76, 0x76 },
    { 0x80, 0x80, 0x80 }, { 0x8a, 0x8a, 0x8a }, { 0x94, 0x94, 0x94 }, { 0x9e, 0x9e, 0x9e },
    { 0xa8, 0xa8, 0xa8 }, { 0xb2, 0xb2, 0xb2 }, { 0xbc, 0xbc, 0xbc }, { 0xc6, 0xc6, 0xc6 },
    { 0xd0, 0xd0, 0xd0 }, { 0xda, 0xda, 0xda }, { 0xe4, 0xe4, 0xe4 }, { 0xee, 0xee, 0xee }
};

/* --------------------------------------------------------------------------- */

#define get_number(v) for ( v = 0; *text && *text >= '0' && *text <= '9'; text++ ) v = v * 10 + *text - '0'; if ( *text == ';' ) text++;

#define PARSE_ANSI() \
    if ( *text == '\e' && *( text + 1 ) == '[' ) { /* Ansi secuence */ \
        text += 2, stop = 0; \
        while ( *text && !stop ) { \
            switch ( *text ) { \
                case 'm': text++; stop = 1; break; \
                case ';': text++; break; \
                case '0': r = original_r, g = original_g, b = original_b; text++; break; \
                case '1': \
                    if ( *( text + 1 ) == ';' && *( text + 2 ) == '3' && *( text + 3 ) >= '0' && *( text + 3 ) <= '7' ) { /* Light colors \e[1;30 to \e[1;37 */ \
                        idx = 8 + *( text + 3 ) - '0';  r = ansi_colors_4[ idx ][ 0 ], g = ansi_colors_4[ idx ][ 1 ], b = ansi_colors_4[ idx ][ 2 ]; text += 4; \
                    } else { stop = 1; } \
                    break; \
                case '3': \
                    if ( *( text + 1 ) >= '0' && *( text + 1 ) <= '7' ) { /* Normal colors \e[31 to \e[37 */ \
                        idx = *( text + 1 ) - '0'; r = ansi_colors_4[ idx ][ 0 ], g = ansi_colors_4[ idx ][ 1 ], b = ansi_colors_4[ idx ][ 2 ]; text += 2; break; \
                    } else if ( *( text + 1 ) == '8' && *( text + 2 ) == ';' ) { /* Ansi colors \e[38; */ \
                        text += 3; \
                        switch ( *text ) { \
                            case '2': /* 24 bits mode \e[38;2;r;g;b */ \
                                text++; if ( *text != ';' ) { stop = 1; break;  } text++; get_number(r); get_number(g); get_number(b); break; \
                            case '5': /* 8 bits mode \e[38;5;colorindex */ \
                                text++; if ( *text != ';' ) { stop = 1; break;  } text++; get_number(idx); idx &= 0xff; r = ansi_colors_8[ idx ][ 0 ], g = ansi_colors_8[ idx ][ 1 ], b = ansi_colors_8[ idx ][ 2 ]; break; \
                            default: stop = 1; break; \
                        } \
                    } else { stop = 1; } \
                    break; \
                default: stop = 1; break; \
            } \
        } \
    }

#define SKIP_ANSI() \
    if ( *text == '\e' && *( text + 1 ) == '[' ) { /* Ansi secuence */ \
        text += 2, stop = 0; \
        while ( *text && !stop ) { \
            switch ( *text ) { \
                case 'm': text++; stop = 1; break; \
                case ';': text++; break; \
                case '0': text++; break; \
                case '1': \
                    if ( *( text + 1 ) == ';' && *( text + 2 ) == '3' && *( text + 3 ) >= '0' && *( text + 3 ) <= '7' ) { /* Light colors \e[1;30 to \e[1;37 */ text += 4; \
                    } else { stop = 1; } \
                    break; \
                case '3': \
                    if ( *( text + 1 ) >= '0' && *( text + 1 ) <= '7' ) { /* Normal colors \e[31 to \e[37 */  text += 2; break; \
                    } else if ( *( text + 1 ) == '8' && *( text + 2 ) == ';' ) { /* Ansi colors \e[38; */ text += 3; \
                        switch ( *text ) { \
                            case '2': /* 24 bits mode \e[38;2;r;g;b */ \
                                text++; if ( *text != ';' ) { stop = 1; break;  } text++; get_number(dummy); get_number(dummy); get_number(dummy); break; \
                            case '5': /* 8 bits mode \e[38;5;colorindex */ \
                                text++; if ( *text != ';' ) { stop = 1; break;  } text++; get_number(dummy); break; \
                            default: stop = 1; break; \
                        } \
                    } else { stop = 1; } \
                    break; \
                default: stop = 1; break; \
            } \
        } \
    }

#define WRITE_TEXT_FNT_CHARMAP(enc) \
                while ( *text ) { \
                    PARSE_ANSI() \
                    current_char = enc; \
                    fntclip = &f->glyph[current_char].fontsource; \
                    gr_blit( dest, clip, x + f->glyph[current_char].xoffset, y + f->glyph[current_char].yoffset, flags, 0, 100, 100, f->fontmap, fntclip, alpha, r, g, b ); \
                    x += f->glyph[current_char].xadvance; \
                    text++; \
                }

#define WRITE_TEXT_FNT_MAP(enc) \
                while ( *text ) { \
                    PARSE_ANSI() \
                    current_char = enc; \
                    ch = f->glyph[current_char].glymap; \
                    if ( ch ) gr_blit( dest, clip, x + f->glyph[current_char].xoffset, y + f->glyph[current_char].yoffset, flags, 0, 100, 100, ch, NULL, alpha, r, g, b ); \
                    x += f->glyph[current_char].xadvance; \
                    text++; \
                }

/* --------------------------------------------------------------------------- */

int64_t gr_text_height_no_margin( int64_t fontid, const unsigned char * text );

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_text
 *
 *  Returns the character string of a given text
 *  (may be the representation of a integer or float value)
 *
 *  PARAMS :
 *  text   Pointer to the text object
 *
 *  RETURN VALUE :
 *      String contained within the text
 *
 */

static const char * get_text( TEXT * text ) {
    static char buffer[384];

    switch ( text->on ) {
        case TEXT_TEXT:
            return text->text;

        case TEXT_STRING:
            return string_get( *( int64_t * )text->var );

        case TEXT_INT:
            _string_ntoa( buffer, *( int64_t * )text->var );
            return buffer;

        case TEXT_QWORD:
            _string_utoa( buffer, *( int64_t * )text->var );
            return buffer;

        case TEXT_INT32:
            _string_ntoa( buffer, *( int32_t * )text->var );
            return buffer;

        case TEXT_DWORD:
            _string_utoa( buffer, *( int32_t * )text->var );
            return buffer;

        case TEXT_DOUBLE: {
                char * aux = buffer + ( sprintf( buffer, "%lf", *( double * )text->var ) - 1 );
                while ( *aux == '0' && *( aux - 1 ) != '.' ) *aux-- = '\0';
                return buffer;
            }

        case TEXT_FLOAT: {
                char * aux = buffer + ( sprintf( buffer, "%f", *( float * )text->var ) - 1 );
                while ( *aux == '0' && *( aux - 1 ) != '.' ) *aux-- = '\0';
                return buffer;
            }

        case TEXT_BYTE:
            _string_utoa( buffer, *( uint8_t * )text->var );
            return buffer;

        case TEXT_SBYTE:
            _string_ntoa( buffer, *( int8_t * )text->var );
            return buffer;

        case TEXT_CHAR:
            *buffer = *( uint8_t * )text->var;
            *( buffer + 1 ) = '\0';
            return buffer;

        case TEXT_WORD:
            _string_utoa( buffer, *( uint16_t * )text->var );
            return buffer;

        case TEXT_SHORT:
            _string_ntoa( buffer, *( int16_t * )text->var );
            return buffer;

        case TEXT_CHARARRAY:
            return ( const char * )( text->var );

        case TEXT_POINTER:
            _string_ptoa( buffer, *( void ** ) text->var );
            return buffer;
    }

    return NULL;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : info_text
 *
 *  Returns information about a text object
 *
 *  PARAMS :
 *  text   Pointer to the text object
 *  bbox   Region to update with the text bounding box
 *
 *  RETURN VALUE :
 *      1 if the text has changed since last frame
 *
 */

static int info_text( void * what, REGION * bbox, int64_t * z, int64_t * drawme ) {
    TEXT * text = ( TEXT * ) what;
    const char * str = get_text( text );
//    REGION prev = *bbox;
    FONT * font;
//    int64_t changed = 0;

    * drawme = 0;

    // Splinter
    if ( !str || !*str ) return 0;

    if ( !( font = gr_font_get( text->fontid ) ) ) return 0 ;

    * drawme = 1;

    * z = text->z;

    /* Calculate the text dimensions */

    text->_x = text->x;
    text->_y = text->y;
//    text->_height = gr_text_height_no_margin( text->fontid, ( const unsigned char * ) str );

    /* Update the font's maxheight (if needed) */

    if ( !font->maxheight ) {
        int64_t c;
        if ( font->fontmap ) {
            for ( c = 0; c < 256; c++ ) {
                if ( font->maxheight < ( int64_t )font->glyph[c].fontsource.h + font->glyph[c].yoffset )
                    font->maxheight = ( int64_t )font->glyph[c].fontsource.h + font->glyph[c].yoffset;
            }
        } else {
            for ( c = 0; c < 256; c++ ) {
                if ( !font->glyph[c].glymap ) continue;
                if ( font->maxheight < ( int64_t )font->glyph[c].glymap->height + font->glyph[c].yoffset )
                    font->maxheight = ( int64_t )font->glyph[c].glymap->height + font->glyph[c].yoffset;
            }
        }
    }

    /* Adjust top-left coordinates for text alignment */

    int64_t width;

    switch ( text->alignment ) {
        case ALIGN_TOP:             // 1
        case ALIGN_CENTER:          // 4
        case ALIGN_BOTTOM:          // 7
            width = gr_text_width( text->fontid, ( const unsigned char * ) str );
            text->_x -= width / 2;
            break;

        case ALIGN_TOP_RIGHT:       // 2
        case ALIGN_CENTER_RIGHT:    // 5
        case ALIGN_BOTTOM_RIGHT:    // 8
            width = gr_text_width( text->fontid, ( const unsigned char * ) str );
            text->_x -= width - 1;
            break;
    }

    switch ( text->alignment ) {
        case ALIGN_CENTER_LEFT:     // 3
        case ALIGN_CENTER:          // 4
        case ALIGN_CENTER_RIGHT:    // 5
            text->_y -= font->maxheight / 2;
            break;

        case ALIGN_BOTTOM_LEFT:     // 6
        case ALIGN_BOTTOM:          // 7
        case ALIGN_BOTTOM_RIGHT:    // 8
            text->_y -= font->maxheight - 1;
            break;
    }

    return 1;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_text
 *
 *  Draws a text object
 *
 *  PARAMS :
 *  text   Pointer to the text object
 *  clip   Clipping region
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_text( void * what, REGION * clip ) {
    TEXT * text = ( TEXT * ) what;
    const char * str = get_text( text );
    FONT * font;

    // Splinter
    if ( !str ) return;

    if ( !( font = gr_font_get( text->fontid ) ) ) {
        gr_text_destroy( text->id );
        return;
    }

    /* Draw the text */
    if ( !gr_text_put( 0, what, clip, text->fontid, text->_x, text->_y, ( const unsigned char * ) str ) ) gr_text_destroy( text->id );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_text_new2
 *
 *  Create a new text, using a fixed text string
 *
 *  PARAMS :
 *  fontid      Font number
 *  x, y, z     Screen coordinates
 *  alignment   Alignment
 *  text        Pointer to text
 *
 *  RETURN VALUE :
 *      None
 *
 */

int64_t gr_text_new2( int64_t fontid, int64_t x, int64_t y, int64_t z, int64_t alignment, const char * text ) {
    int64_t textid = text_nextid;

    if ( text_nextid == MAX_TEXTS ) {
        for ( textid = 1; textid < MAX_TEXTS; textid++ ) if ( !texts[textid].on ) break;
        if ( textid == MAX_TEXTS ) return 0; // error ("Too many texts!");
    } else
        text_nextid++;

    text_count++;

    texts[textid].id = textid;
    texts[textid].on = TEXT_TEXT;
    texts[textid].fontid = fontid;
    texts[textid].x = x;
    texts[textid].y = y;
    texts[textid].z = z;
    texts[textid].alignment = alignment;
    texts[textid].text = text ? strdup( text ) : 0;

    texts[textid].alpha = GLOBYTE( libbggfx, TEXT_ALPHA );
    texts[textid].color_r = GLOBYTE( libbggfx, TEXT_COLORR );
    texts[textid].color_g = GLOBYTE( libbggfx, TEXT_COLORG );
    texts[textid].color_b = GLOBYTE( libbggfx, TEXT_COLORB );

    texts[textid].objectid = gr_new_object( texts[textid].z, info_text, draw_text, &texts[textid] );

    return textid;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_text_new
 *
 *  Create a new text, using a fixed text string
 *
 *  PARAMS :
 *  fontid     Font number
 *  x, y       Screen coordinates
 *  alignment  Alignment
 *  text       Pointer to text
 *
 *  RETURN VALUE :
 *      None
 *
 */

int64_t gr_text_new( int64_t fontid, int64_t x, int64_t y, int64_t alignment, const char * text ) {
    return gr_text_new2( fontid, x, y, GLOINT64( libbggfx, TEXTZ ), alignment, text );
}

/* --------------------------------------------------------------------------- */

int64_t gr_text_new_var( int64_t fontid, int64_t x, int64_t y, int64_t alignment, const void * var, int64_t type ) {
    int64_t textid = gr_text_new( fontid, x, y, alignment, 0 );
    if ( !textid ) return 0;
    texts[textid].on = type;
    if ( type > TEXT_TEXT ) texts[textid].var = var;
    return textid;
}

/* --------------------------------------------------------------------------- */

int64_t gr_text_new_var2( int64_t fontid, int64_t x, int64_t y, int64_t z, int64_t alignment, const void * var, int64_t type ) {
    int64_t textid = gr_text_new2( fontid, x, y, z, alignment, 0 );
    if ( !textid ) return 0;
    texts[textid].on = type;
    if ( type > TEXT_TEXT ) texts[textid].var = var;
    return textid;
}

/* --------------------------------------------------------------------------- */

void gr_text_move( int64_t textid, int64_t x, int64_t y ) {
    if ( textid > 0 && textid < text_nextid ) {
        texts[textid].x = x;
        texts[textid].y = y;
    }
}

/* --------------------------------------------------------------------------- */

void gr_text_move2( int64_t textid, int64_t x, int64_t y, int64_t z ) {
    if ( textid > 0 && textid < text_nextid ) {
        texts[textid].x = x;
        texts[textid].y = y;
        texts[textid].z = z;
    }
}

/* --------------------------------------------------------------------------- */

void gr_text_destroy( int64_t textid ) {
    if ( !textid ) {
        for ( textid = 1; textid < text_nextid; textid++ ) {
            if ( texts[textid].on ) {
                gr_destroy_object( texts[textid].objectid );
                if ( texts[textid].text ) free( texts[textid].text );
                texts[textid].on = 0;
            }
        }
        text_count = 0;
        text_nextid = 1;
        return;
    }
    if ( textid > 0 && textid < text_nextid ) {
        if ( !texts[textid].on ) return;

        gr_destroy_object( texts[textid].objectid );
        if ( texts[textid].text ) free( texts[textid].text );
        texts[textid].on = 0;
        if ( textid == text_nextid - 1 ) {
            while ( text_nextid > 1 && !texts[text_nextid-1].on ) text_nextid--;
        }
        text_count--;
    }
}

/* --------------------------------------------------------------------------- */

int64_t gr_text_width( int64_t fontid, const unsigned char * text ) {
    int stop = 0, idx, dummy;
    int64_t l = 0;
    FONT * f;

    if ( !text || !*text ) return 0;
    if ( !( f = gr_font_get( fontid ) ) ) return 0; // Incorrect font type

    switch ( f->charset ) {
        case CHARSET_ISO8859:
            while ( *text ) {
                SKIP_ANSI();
                l += f->glyph[dos_to_win[*text++]].xadvance;
            }
            break;

        case CHARSET_CP850:
            while ( *text ) {
                SKIP_ANSI();
                l += f->glyph[*text++].xadvance;
            }
            break;
    }

    return l;
}

/* --------------------------------------------------------------------------- */

int64_t gr_text_margintop( int64_t fontid, const unsigned char * text ) {
    int64_t minyoffset = 0x7FFFFFFF, t;
    FONT * f;

    if ( !text || !*text ) return 0;
    if ( !( f = gr_font_get( fontid ) ) ) return 0; // Incorrect font type

    switch ( f->charset ) {
        case CHARSET_ISO8859:
            while ( *text ) {
                if ( minyoffset > ( t = f->glyph[dos_to_win[*text]].yoffset ) ) minyoffset = t;
                text++;
            }
            break;

        case CHARSET_CP850:
            while ( *text ) {
                if ( minyoffset > ( t = f->glyph[*text].yoffset ) ) minyoffset = t;
                text++;
            }
            break;
    }

    return minyoffset;
}

/* --------------------------------------------------------------------------- */

int64_t gr_text_height_no_margin( int64_t fontid, const unsigned char * text ) {
    int64_t l = 0, t;
    FONT * f;

    if ( !text || !*text ) return 0;
    if ( !( f = gr_font_get( fontid ) ) ) return 0; // Incorrect font type

    if ( f->fontmap ) {
        switch ( f->charset ) {
            case CHARSET_ISO8859:
                while ( *text ) {
                    if ( l < ( t = f->glyph[dos_to_win[*text]].yoffset + ( int64_t )f->glyph[dos_to_win[*text]].fontsource.h ) ) l = t;
                    text++;
                }
                break;

            case CHARSET_CP850:
                while ( *text ) {
                    if ( l < ( t = f->glyph[*text].yoffset + ( int64_t )f->glyph[*text].fontsource.h ) ) l = t;
                    text++;
                }
                break;
        }
    }
    else {
        switch ( f->charset ) {
            case CHARSET_ISO8859:
                while ( *text ) {
                    if ( f->glyph[*text].glymap && l < ( t = f->glyph[dos_to_win[*text]].yoffset + ( int64_t )f->glyph[dos_to_win[*text]].glymap->height ) ) l = t;
                    text++;
                }
                break;

            case CHARSET_CP850:
                while ( *text ) {
                    if ( f->glyph[*text].glymap && l < ( t = f->glyph[*text].yoffset + ( int64_t )f->glyph[*text].glymap->height ) ) l = t;
                    text++;
                }
                break;
        }
    }
    return l;
}

/* --------------------------------------------------------------------------- */

int64_t gr_text_height( int64_t fontid, const unsigned char * text ) {
    int64_t l = gr_text_height_no_margin( fontid, text );
    if ( l ) l -= gr_text_margintop( fontid, text );
    return l;
}

/* --------------------------------------------------------------------------- */

int64_t gr_text_put( GRAPH * dest, void * ptext, REGION * clip, int64_t fontid, int64_t x, int64_t y, const unsigned char * text ) {
    GRAPH * ch;
    FONT * f;
    uint8_t current_char, alpha, r, g, b, original_r, original_g, original_b;
    int64_t flags;
    SDL_Rect * fntclip = NULL;
    int stop = 0, idx;

    if ( !text || !*text ) return -1;
    if ( !( f = gr_font_get( fontid ) ) ) return 0; // Incorrect font type

    if ( ptext ) {
        if ( !( alpha = ( ( TEXT * ) ptext )->alpha ) ) return 0;
        r = ( ( TEXT * ) ptext )->color_r;
        g = ( ( TEXT * ) ptext )->color_g;
        b = ( ( TEXT * ) ptext )->color_b;
    } else {
        if ( !( alpha = GLOBYTE( libbggfx, TEXT_ALPHA ) ) ) return 0;
        r = GLOBYTE( libbggfx, TEXT_COLORR );
        g = GLOBYTE( libbggfx, TEXT_COLORG );
        b = GLOBYTE( libbggfx, TEXT_COLORB );
    }

    original_r = r;
    original_g = g;
    original_b = b;

    flags = GLOQWORD( libbggfx, TEXT_FLAGS );

    if ( f->fontmap ) {
        switch ( f->charset ) {
            case CHARSET_ISO8859:
                WRITE_TEXT_FNT_CHARMAP(dos_to_win[*text]);
                break;

            case CHARSET_CP850:
                WRITE_TEXT_FNT_CHARMAP(*text);
                break;
        }
    } else {
        switch ( f->charset ) {
            case CHARSET_ISO8859:
                WRITE_TEXT_FNT_MAP(dos_to_win[*text]);
                break;

            case CHARSET_CP850:
                WRITE_TEXT_FNT_MAP(*text);
                break;
        }
    }

    return 1;
}

/* --------------------------------------------------------------------------- */

GRAPH * gr_text_bitmap( int64_t fontid, const char * text, int64_t alignment ) {
    GRAPH * gr;
    int64_t x, y;

    // Splinter
    if ( !text || !*text ) return NULL;
    if ( !gr_font_get( fontid ) ) return NULL; // Incorrect font type

    gr = bitmap_new_syslib( gr_text_width( fontid, ( const unsigned char * ) text ), gr_text_height( fontid, ( const unsigned char * ) text ) );
    if ( !gr ) return NULL;

    if ( !gr_text_put( gr, NULL, 0, fontid, 0, -gr_text_margintop( fontid, ( const unsigned char * ) text ), ( const unsigned char * ) text ) ) {
        bitmap_destroy( gr );
        return NULL;
    }

    switch ( alignment ) {
        case ALIGN_TOP_LEFT:    // 0
        case ALIGN_TOP:         // 1
        case ALIGN_TOP_RIGHT:   // 2
            y = 0;
            break;

        case ALIGN_CENTER_LEFT: // 3
        case ALIGN_CENTER:      // 4
        case ALIGN_CENTER_RIGHT:// 5
            y = gr->height / 2;
            break;

        default:
            y = gr->height - 1;
            break;

    }

    switch ( alignment ) {
        case ALIGN_TOP_LEFT:    // 0
        case ALIGN_CENTER_LEFT: // 3
        case ALIGN_BOTTOM_LEFT: // 6
            x = 0;
            break;

        case ALIGN_TOP:         // 1
        case ALIGN_CENTER:      // 4
        case ALIGN_BOTTOM:      // 7
            x = gr->width / 2;
            break;

        default:
            x = gr->width - 1;
            break;

    }

    bitmap_add_cpoint( gr, x, y );
    return gr;
}

/* --------------------------------------------------------------------------- */

void gr_text_setrgba( int64_t textid, uint8_t r, uint8_t g, uint8_t b, uint8_t a ) {
    if ( textid > 0 && textid < text_nextid ) {
        texts[textid].color_r = r;
        texts[textid].color_g = g;
        texts[textid].color_b = b;
        texts[textid].alpha = a;
    }
}

/* --------------------------------------------------------------------------- */

int64_t gr_text_getrgba( int64_t textid, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a ) {
    if ( textid > 0 && textid < text_nextid ) {
        if ( r ) * r = texts[textid].color_r;
        if ( g ) * g = texts[textid].color_g;
        if ( b ) * b = texts[textid].color_b;
        if ( a ) * a = texts[textid].alpha;
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
