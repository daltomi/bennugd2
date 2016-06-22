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

#ifndef __G_INSTANCE_H
#define __G_INSTANCE_H

#include <bgddl.h>
#include <g_bitmap.h>
#include <g_regions.h>

/* --------------------------------------------------------------------------- */

/* Flags para gr_blit                                                          */

#define B_HMIRROR       0x0001
#define B_VMIRROR       0x0002
#define B_TRANSLUCENT   0x0004
#define B_ABLEND        0x0010
#define B_SBLEND        0x0020
#define B_NOCOLORKEY    0x0080

/* --------------------------------------------------------------------------- */

extern void instance_get_bbox( INSTANCE * i, GRAPH * gr, REGION * dest );
extern void draw_instance_at( INSTANCE * i, REGION * r, int64_t x, int64_t y, GRAPH * dest ) ;
extern void draw_instance( void * what, REGION * clip ) ;
extern GRAPH * instance_graph( INSTANCE * i ) ;
extern int draw_instance_info( void * what, REGION * region, int64_t * z, int64_t * drawme );

/* --------------------------------------------------------------------------- */

#endif