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

#ifndef __LIBMOD_INPUT_EXPORTS
#define __LIBMOD_INPUT_EXPORTS

/* ----------------------------------------------------------------- */

#include "bgddl.h"

/* ----------------------------------------------------------------- */

DLSYSFUNCS  __bgdexport( libmod_input, functions_exports )[] = {

    FUNC( "KEY"                 , "Q"       , TYPE_INT64    , libmod_input_key                          ),

    FUNC( "JOY_AXES"            , ""        , TYPE_INT64    , libmod_input_joy_axes                     ),
    FUNC( "JOY_AXES"            , "Q"       , TYPE_INT64    , libmod_input_joy_axes_specific            ),
    FUNC( "JOY_NUMAXES"         , ""        , TYPE_INT64    , libmod_input_joy_axes                     ),
    FUNC( "JOY_NUMAXES"         , "Q"       , TYPE_INT64    , libmod_input_joy_axes_specific            ),
    FUNC( "JOY_GETAXIS"         , "Q"       , TYPE_INT64    , libmod_input_joy_get_position             ),
    FUNC( "JOY_GETAXIS"         , "QQ"      , TYPE_INT64    , libmod_input_joy_get_position_specific    ),

    FUNC( "JOY_BUTTONS"         , ""        , TYPE_INT64    , libmod_input_joy_buttons                  ),
    FUNC( "JOY_BUTTONS"         , "Q"       , TYPE_INT64    , libmod_input_joy_buttons_specific         ),

    FUNC( "JOY_NAME"            , "Q"       , TYPE_STRING   , libmod_input_joy_name                     ),

    FUNC( "JOY_NUMBUTTONS"      , ""        , TYPE_INT64    , libmod_input_joy_buttons                  ),
    FUNC( "JOY_NUMBUTTONS"      , "Q"       , TYPE_INT64    , libmod_input_joy_buttons_specific         ),

    FUNC( "JOY_NUMBER"          , ""        , TYPE_INT64    , libmod_input_joy_num                      ),
    FUNC( "JOY_NUMJOYSTICKS"    , ""        , TYPE_INT64    , libmod_input_joy_num                      ),

    FUNC( "JOY_SELECT"          , "Q"       , TYPE_INT64    , libmod_input_joy_select                   ),

    FUNC( "JOY_GETBUTTON"       , "Q"       , TYPE_INT64    , libmod_input_joy_get_button               ),
    FUNC( "JOY_GETBUTTON"       , "QQ"      , TYPE_INT64    , libmod_input_joy_get_button_specific      ),
    FUNC( "JOY_GETPOSITION"     , "Q"       , TYPE_INT64    , libmod_input_joy_get_position             ),
    FUNC( "JOY_GETPOSITION"     , "QQ"      , TYPE_INT64    , libmod_input_joy_get_position_specific    ),

    FUNC( "JOY_NUMHATS"         , ""        , TYPE_INT64    , libmod_input_joy_hats                     ),
    FUNC( "JOY_NUMHATS"         , "Q"       , TYPE_INT64    , libmod_input_joy_hats_specific            ),   /* Added by Sandman */
    FUNC( "JOY_NUMBALLS"        , ""        , TYPE_INT64    , libmod_input_joy_balls                    ),
    FUNC( "JOY_NUMBALLS"        , "Q"       , TYPE_INT64    , libmod_input_joy_balls_specific           ),   /* Added by Sandman */
    FUNC( "JOY_GETHAT"          , "Q"       , TYPE_INT64    , libmod_input_joy_get_hat                  ),
    FUNC( "JOY_GETHAT"          , "QQ"      , TYPE_INT64    , libmod_input_joy_get_hat_specific         ),   /* Added by Sandman */
    FUNC( "JOY_GETBALL"         , "QPP"     , TYPE_INT64    , libmod_input_joy_get_ball                 ),
    FUNC( "JOY_GETBALL"         , "QQPP"    , TYPE_INT64    , libmod_input_joy_get_ball_specific        ),   /* Added by Sandman */

    FUNC( "JOY_GETACCEL"        , "PPP"     , TYPE_INT64    , libmod_input_joy_get_accel                ),
    FUNC( "JOY_GETACCEL"        , "QPPP"    , TYPE_INT64    , libmod_input_joy_get_accel_specific       ),

    /* Compatibility */

    FUNC( "NUMBER_JOY"          , ""        , TYPE_INT64    , libmod_input_joy_num                      ),
    FUNC( "SELECT_JOY"          , "Q"       , TYPE_INT64    , libmod_input_joy_select                   ),
    FUNC( "GET_JOY_BUTTON"      , "Q"       , TYPE_INT64    , libmod_input_joy_get_button               ),
    FUNC( "GET_JOY_BUTTON"      , "QQ"      , TYPE_INT64    , libmod_input_joy_get_button_specific      ),
    FUNC( "GET_JOY_POSITION"    , "Q"       , TYPE_INT64    , libmod_input_joy_get_position             ),
    FUNC( "GET_JOY_POSITION"    , "QQ"      , TYPE_INT64    , libmod_input_joy_get_position_specific    ),

    FUNC( 0                     , 0         , 0             , 0                                         )
};

/* ----------------------------------------------------------------- */

char * __bgdexport( mod_input, modules_dependency )[] = {
    "libbginput",
    NULL
};

/* ----------------------------------------------------------------- */

#endif