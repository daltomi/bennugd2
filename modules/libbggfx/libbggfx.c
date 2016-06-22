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

/* --------------------------------------------------------------------------- */

#include <stdlib.h>

/* --------------------------------------------------------------------------- */

#include "bgddl.h"
#include "dlvaracc.h"

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */
/* Son las variables que se desea acceder.                           */
/* El interprete completa esta estructura, si la variable existe.    */
/* (usada en tiempo de ejecucion)                                    */

DLVARFIXUP __bgdexport( libbggfx, globals_fixup )[] = {
    /* Nombre de variable global, puntero al dato, tama�o del elemento, cantidad de elementos */
    { "graph_mode"                      , NULL, -1, -1 },
    { "full_screen"                     , NULL, -1, -1 },
    { "scale_resolution"                , NULL, -1, -1 },
    { "scale_resolution_aspectratio"    , NULL, -1, -1 },

    { "fps"                             , NULL, -1, -1 },
    { "speed_gauge"                     , NULL, -1, -1 },
    { "frame_time"                      , NULL, -1, -1 },

    { "fading"                          , NULL, -1, -1 },

    { "exit_status"                     , NULL, -1, -1 },
    { "window_status"                   , NULL, -1, -1 },
    { "focus_status"                    , NULL, -1, -1 },
    { "mouse_status"                    , NULL, -1, -1 },

    { "text_z"                          , NULL, -1, -1 },
    { "text_flags"                      , NULL, -1, -1 },
    { "text_alpha"                      , NULL, -1, -1 },
    { "text_color_r"                    , NULL, -1, -1 },
    { "text_color_g"                    , NULL, -1, -1 },
    { "text_color_b"                    , NULL, -1, -1 },

    /* backgound */
    { "background_file"                 , NULL, -1, -1 },
    { "background_graph"                , NULL, -1, -1 },
    { "background_color_r"              , NULL, -1, -1 },
    { "background_color_g"              , NULL, -1, -1 },
    { "background_color_b"              , NULL, -1, -1 },
    { "background_flags"                , NULL, -1, -1 },
    { "background_angle"                , NULL, -1, -1 },
    { "background_size"                 , NULL, -1, -1 },
    { "background_size_x"               , NULL, -1, -1 },
    { "background_size_y"               , NULL, -1, -1 },

    { "scroll"                          , NULL, -1, -1 },

    { NULL                              , NULL, -1, -1 }
};

DLVARFIXUP __bgdexport( libbggfx, locals_fixup )[] = {
    { "ctype"                           , NULL, -1, -1 },       // 0            CTYPE
    { "cnumber"                         , NULL, -1, -1 },       // 1            CNUMBER
    { "x"                               , NULL, -1, -1 },       // 2            COORDX
    { "y"                               , NULL, -1, -1 },       // 3            COORDY
    { "z"                               , NULL, -1, -1 },       // 4            COORDZ
    { "file"                            , NULL, -1, -1 },       // 5            FILEID
    { "graph"                           , NULL, -1, -1 },       // 6            GRAPHID
    { "size"                            , NULL, -1, -1 },       // 7            GRAPHSIZE
    { "angle"                           , NULL, -1, -1 },       // 8            ANGLE
    { "flags"                           , NULL, -1, -1 },       // 9            FLAGS
    { "region"                          , NULL, -1, -1 },       // 10           REGIONID
    { "resolution"                      , NULL, -1, -1 },       // 11           RESOLUTION
    { "size_x"                          , NULL, -1, -1 },       // 12           GRAPHSIZEX
    { "size_y"                          , NULL, -1, -1 },       // 13           GRAPHSIZEY
    { "xgraph"                          , NULL, -1, -1 },       // 14           XGRAPH
    { "_render_reserved_.object_id"     , NULL, -1, -1 },       // 15           OBJECTID
    { "_render_reserved_.graph_ptr"     , NULL, -1, -1 },       // 16           GRAPHPTR
    { "_render_reserved_.xgraph_flags"  , NULL, -1, -1 },       // 17           XGRAPH_FLAGS
    { "reserved.status"                 , NULL, -1, -1 },       // 18           STATUS
    { "id"                              , NULL, -1, -1 },       // 19           PROCESS_ID
    { "render_file"                     , NULL, -1, -1 },       // 20           RENDER_FILEID,
    { "render_graph"                    , NULL, -1, -1 },       // 21           RENDER_GRAPHID
    { "clip_x"                          , NULL, -1, -1 },       // 22           CLIP_X
    { "clip_y"                          , NULL, -1, -1 },       // 23           CLIP_Y
    { "clip_w"                          , NULL, -1, -1 },       // 24           CLIP_W
    { "clip_h"                          , NULL, -1, -1 },       // 25           CLIP_H
    { "alpha"                           , NULL, -1, -1 },       // 26           ALPHA
    { "color_r"                         , NULL, -1, -1 },       // 27           COLOR_R
    { "color_g"                         , NULL, -1, -1 },       // 28           COLOR_G
    { "color_b"                         , NULL, -1, -1 },       // 29           COLOR_B

    { NULL                              , NULL, -1, -1 }
};

/* --------------------------------------------------------------------------- */

/* Bigest priority first execute
   Lowest priority last execute */

HOOK __bgdexport( libbggfx, handler_hooks )[] = {
    { 9500, gr_wait_frame },
    { 9000, gr_draw_frame },
    { 4700, wm_events     },
    {    0, NULL          }
} ;

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : instance_create_hook
 *
 *  PARAMS :
 *      r           Pointer to the instance
 *
 *  RETURN VALUE :
 *      None
 */

void __bgdexport( libbggfx, instance_create_hook )( INSTANCE * r ) {
    /* COORZ is 0 when a new instance is created */
    LOCQWORD( libbggfx, r, OBJECTID ) = gr_new_object( /* LOCINT32( libbggfx, r, COORDZ ) */ 0, draw_instance_info, draw_instance, r );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : instance_destroy_hook
 *
 *  PARAMS :
 *      r           Pointer to the instance
 *
 *  RETURN VALUE :
 *      None
 */

void __bgdexport( libbggfx, instance_destroy_hook )( INSTANCE * r ) {
    if ( LOCQWORD( libbggfx, r, OBJECTID ) ) gr_destroy_object( LOCQWORD( libbggfx, r, OBJECTID ) );
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libbggfx, module_initialize )() {
    grlib_init() ;
    gr_video_init();
    gr_font_init();
    frame_init();
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libbggfx, module_finalize )() {
    frame_exit();
    gr_video_exit();
}

/* --------------------------------------------------------------------------- */
/* exports                                                                     */
/* --------------------------------------------------------------------------- */

#include "libbggfx_exports.h"

/* --------------------------------------------------------------------------- */