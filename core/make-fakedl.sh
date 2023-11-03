#!/bin/bash

FAKE_DL_FNAME=include/fake_dl.h
MODULES_PATH=../modules

credits()
{
cat > $1 <<EOT
/*
 *  $1 generated by build.sh
 *
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
 *  Copyright (C) 2002-2006 Fenix Team (Fenix)
 *  Copyright (C) 1999-2002 Jos� Luis Cebri�n Pag�e (Fenix)
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

EOT
}

search_bgdc_includes()
{
    echo "#ifdef __BGDC__"
    for i in $(for ii in $SCOPE; do find $MODULES_PATH/$ii -maxdepth 1 -regex '.+\(_exports\.h\)'; done); do echo "#include \"$(basename $i)\""; done
    echo "#endif"
    echo " "
}

search_symbols()
{
    echo "/* ---------- $2 ---------- */"
    echo " "

    for i in $(grep __bgdexport $(for ii in $SCOPE; do find $MODULES_PATH/$ii -maxdepth 1 -regex '.+\(_exports\.h\|\.c\)'; done) /dev/null | cut -f2 -d "(" | cut -f1 -d ")" | sed -r 's/\s(\w+)[, ]+(\w+)/\1_\2/'| grep $2); do
        echo "extern $1 $i$3;"
    done

    echo " "
}

#make_fake_dl_item()
#{
#    for i in $(grep __bgdexport $(find $MODULES_PATH/$i -maxdepth 1 -regex '.+\(_exports\.h\|\.c\)') /dev/null | cut -f2 -d "(" | cut -f1 -d ")" | sed -r 's/\s(\w+)[, ]+(\w+)/\1_\2/' 2>/dev/null| grep $2); do
#        echo -n $i
#    done
#}


make_fake_dl_item()
{
    local symbol="$2"
    local line_to_print=""

    for i in $(grep -w __bgdexport $(find $MODULES_PATH/$i -maxdepth 1 -regex '.+\(_exports\.h\|\.c\)') /dev/null | cut -f2 -d "(" | cut -f1 -d ")" | sed -r 's/\s(\w+)[, ]+(\w+)/\1_\2/' 2>/dev/null|grep $symbol); do
        line_to_print="$i"
        echo $line_to_print
    done

    if [ -z "$line_to_print" ]; then
        for i in $(grep -w __bgdexport_ifdef $(find $MODULES_PATH/$i -maxdepth 1 -regex '.+\(_exports\.h\|\.c\)') /dev/null | cut -f2 -d "(" | cut -f1 -d ")" | sed -r "s/\s(\w+)[, ]+(\w+)[, ]+(\w+)/\1 \2_\3/" 2>/dev/null|grep $symbol); do
            line_to_print="$i"
            echo "$line_to_print"
        done
    fi
}

check_conditional() {
    if [ "$2" == "&" ]; then
        if [ "$3" == "" ]; then
            echo "$1 NULL;"
        else
            if [ "$4" == "" ]; then
                echo "$1 &$3;"
            else
                echo "#ifdef $3"
                echo "$1 &$4;"
                echo "#else"
                echo "$1 NULL;"
                echo "#endif"
            fi
        fi
    else
        if [ "$2" == "" ]; then
            echo "$1 NULL;"
        else
            if [ "$3" == "" ]; then
                echo "$1 $2;"
            else
                echo "#ifdef $2"
                echo "$1 $3;"
                echo "#else"
                echo "$1 NULL;"
                echo "#endif"
            fi
        fi
    fi
}


make_fake_dl_lib()
{
    count=$2
    a01=$(make_fake_dl_item $1 constants_def)
    a02=$(make_fake_dl_item $1 types_def)
    a03=$(make_fake_dl_item $1 globals_def)
    a04=$(make_fake_dl_item $1 locals_def)
    a05=$(make_fake_dl_item $1 globals_fixup)
    a06=$(make_fake_dl_item $1 locals_fixup)
    a07=$(make_fake_dl_item $1 functions_exports)
    a08=$(make_fake_dl_item $1 module_initialize)
    a09=$(make_fake_dl_item $1 module_finalize)
    a10=$(make_fake_dl_item $1 instance_create_hook)
    a11=$(make_fake_dl_item $1 instance_destroy_hook)
    a12=$(make_fake_dl_item $1 instance_pre_execute_hook)
    a13=$(make_fake_dl_item $1 instance_pos_execute_hook)
    a14=$(make_fake_dl_item $1 process_exec_hook)
    a15=$(make_fake_dl_item $1 handler_hooks)
    a16=$(make_fake_dl_item $1 modules_dependency)

    if [ "$a01" != "" ] || [ "$a02" != "" ] || [ "$a03" != "" ] || [ "$a04" != "" ] || [ "$a05" != "" ] || [ "$a06" != "" ] || [ "$a07" != "" ] || [ "$a08" != "" ] || [ "$a09" != "" ] || [ "$a10" != "" ] || [ "$a11" != "" ] || [ "$a12" != "" ] || [ "$a13 != "" ] || [ "$a14 != "" ] || [ "$a15 != "" ] || [ "$a16 != "" ]; then
        echo " "
        echo "    /* -------------------- $1 -------------------- */"
        echo " "
        echo "    __fake_dl[$count].dlname                       = \"$1\";"
        echo "#ifdef __BGDC__"
        check_conditional "    __fake_dl[$count].constants_def                =" $a01
        check_conditional "    __fake_dl[$count].types_def                    =" "&" $a02
        check_conditional "    __fake_dl[$count].globals_def                  =" "&" $a03
        check_conditional "    __fake_dl[$count].locals_def                   =" "&" $a04
#        echo "    __fake_dl[$count].constants_def                = $a01;" | sed -r 's/= ;/= NULL;/g'
#        echo "    __fake_dl[$count].types_def                    = &$a02;" | sed -r 's/= &;/= NULL;/g'
#        echo "    __fake_dl[$count].globals_def                  = &$a03;" | sed -r 's/= &;/= NULL;/g'
#        echo "    __fake_dl[$count].locals_def                   = &$a04;" | sed -r 's/= &;/= NULL;/g'
        echo "    __fake_dl[$count].globals_fixup                = NULL;"
        echo "    __fake_dl[$count].locals_fixup                 = NULL;"
        echo "#else"
        echo "    __fake_dl[$count].constants_def                = NULL;"
        echo "    __fake_dl[$count].types_def                    = NULL;"
        echo "    __fake_dl[$count].globals_def                  = NULL;"
        echo "    __fake_dl[$count].locals_def                   = NULL;"
        check_conditional "    __fake_dl[$count].globals_fixup                =" $a05
        check_conditional "    __fake_dl[$count].locals_fixup                 =" $a06
#        echo "    __fake_dl[$count].globals_fixup                = $a05;" | sed -r 's/= ;/= NULL;/g'
#        echo "    __fake_dl[$count].locals_fixup                 = $a06;" | sed -r 's/= ;/= NULL;/g'
        echo "#endif"
        check_conditional "    __fake_dl[$count].functions_exports            =" $a07
#        echo "    __fake_dl[$count].functions_exports            = $a07;" | sed -r 's/= ;/= NULL;/g'
        echo "#ifdef __BGDC__"
        echo "    __fake_dl[$count].module_initialize            = NULL;"
        echo "    __fake_dl[$count].module_finalize              = NULL;"
        echo "    __fake_dl[$count].instance_create_hook         = NULL;"
        echo "    __fake_dl[$count].instance_destroy_hook        = NULL;"
        echo "    __fake_dl[$count].instance_pre_execute_hook    = NULL;"
        echo "    __fake_dl[$count].instance_pos_execute_hook    = NULL;"
        echo "    __fake_dl[$count].process_exec_hook            = NULL;"
        echo "    __fake_dl[$count].handler_hooks                = NULL;"
        echo "#else"
        check_conditional "    __fake_dl[$count].module_initialize            =" $a08
        check_conditional "    __fake_dl[$count].module_finalize              =" $a09
        check_conditional "    __fake_dl[$count].instance_create_hook         =" $a10
        check_conditional "    __fake_dl[$count].instance_destroy_hook        =" $a11
        check_conditional "    __fake_dl[$count].instance_pre_execute_hook    =" $a12
        check_conditional "    __fake_dl[$count].instance_pos_execute_hook    =" $a13
        check_conditional "    __fake_dl[$count].process_exec_hook            =" $a14
        check_conditional "    __fake_dl[$count].handler_hooks                =" $a15
#        echo "    __fake_dl[$count].module_initialize            = $a08;" | sed -r 's/= ;/= NULL;/g'
#        echo "    __fake_dl[$count].module_finalize              = $a09;" | sed -r 's/= ;/= NULL;/g'
#        echo "    __fake_dl[$count].instance_create_hook         = $a10;" | sed -r 's/= ;/= NULL;/g'
#        echo "    __fake_dl[$count].instance_destroy_hook        = $a11;" | sed -r 's/= ;/= NULL;/g'
#        echo "    __fake_dl[$count].instance_pre_execute_hook    = $a12;" | sed -r 's/= ;/= NULL;/g'
#        echo "    __fake_dl[$count].instance_pos_execute_hook    = $a13;" | sed -r 's/= ;/= NULL;/g'
#        echo "    __fake_dl[$count].process_exec_hook            = $a14;" | sed -r 's/= ;/= NULL;/g'
#        echo "    __fake_dl[$count].handler_hooks                = $a15;" | sed -r 's/= ;/= NULL;/g'
        echo "#endif"
#        echo "    __fake_dl[$count].modules_dependency           = $a16;" | sed -r 's/= ;/= NULL;/g'
        check_conditional "    __fake_dl[$count].modules_dependency           =" $a16
        echo " "
        count=$(($count + 1))
    fi

    return $count
}

make_fake_dl()
{
    count=0
    data=""
    for i in $SCOPE;
    do
        data+="$(make_fake_dl_lib $i $count)"
        count=$?
    done

    echo "/* ---------- FAKE DYNAMIC LIBRARY ---------- */"
    echo " "
    echo "__FAKE_DL __fake_dl[$(($count+1))];"
    echo " "

    echo "/* ------------------------------------------ */"
    echo " "
    echo "void fake_dl_init()"
    echo "{"
    echo "$data"
    echo "    /* -------------------- LAST -------------------- */"
    echo " "
    echo "    __fake_dl[$count].dlname                       = NULL;"
    echo "    __fake_dl[$count].constants_def                = NULL;"
    echo "    __fake_dl[$count].types_def                    = NULL;"
    echo "    __fake_dl[$count].globals_def                  = NULL;"
    echo "    __fake_dl[$count].locals_def                   = NULL;"
    echo "    __fake_dl[$count].globals_fixup                = NULL;"
    echo "    __fake_dl[$count].locals_fixup                 = NULL;"
    echo "    __fake_dl[$count].functions_exports            = NULL;"
    echo "    __fake_dl[$count].module_initialize            = NULL;"
    echo "    __fake_dl[$count].module_finalize              = NULL;"
    echo "    __fake_dl[$count].instance_create_hook         = NULL;"
    echo "    __fake_dl[$count].instance_destroy_hook        = NULL;"
    echo "    __fake_dl[$count].instance_pre_execute_hook    = NULL;"
    echo "    __fake_dl[$count].instance_pos_execute_hook    = NULL;"
    echo "    __fake_dl[$count].process_exec_hook            = NULL;"
    echo "    __fake_dl[$count].handler_hooks                = NULL;"
    echo "    __fake_dl[$count].modules_dependency           = NULL;"
    echo " "
    echo "}"
    echo " "
    echo "/* ------------------------------------------ */"
    echo " "
}

# without mathi
SCOPE=$(for i in $(ls ../modules/); do basename $i; done)
export SCOPE

credits                                                            $FAKE_DL_FNAME

echo "#ifndef __FAKE_DL_H"                                      >> $FAKE_DL_FNAME
echo "#define __FAKE_DL_H"                                      >> $FAKE_DL_FNAME
echo " "                                                        >> $FAKE_DL_FNAME

echo "#include <bgddl.h>"                                       >> $FAKE_DL_FNAME
echo " "                                                        >> $FAKE_DL_FNAME

search_bgdc_includes                                            >> $FAKE_DL_FNAME

#echo "#ifdef __BGDC__"                                          >> $FAKE_DL_FNAME
#search_symbols DLCONSTANT constants_def '[]'                    >> $FAKE_DL_FNAME
#search_symbols "char *" types_def                               >> $FAKE_DL_FNAME
#search_symbols "char *" globals_def                             >> $FAKE_DL_FNAME
#search_symbols "char *" locals_def                              >> $FAKE_DL_FNAME
#echo "#endif"                                                   >> $FAKE_DL_FNAME

echo "#ifndef __BGDC__"                                         >> $FAKE_DL_FNAME
search_symbols DLVARFIXUP globals_fixup '[]'                    >> $FAKE_DL_FNAME
search_symbols DLVARFIXUP locals_fixup '[]'                     >> $FAKE_DL_FNAME
search_symbols DLSYSFUNCS functions_exports '[]'                >> $FAKE_DL_FNAME

search_symbols void module_initialize '()'                      >> $FAKE_DL_FNAME
search_symbols void module_finalize '()'                        >> $FAKE_DL_FNAME

search_symbols void instance_create_hook '( INSTANCE * )'       >> $FAKE_DL_FNAME
search_symbols void instance_destroy_hook '( INSTANCE * )'      >> $FAKE_DL_FNAME
search_symbols void instance_pre_execute_hook '( INSTANCE * )'  >> $FAKE_DL_FNAME
search_symbols void instance_pos_execute_hook '( INSTANCE * )'  >> $FAKE_DL_FNAME
search_symbols void process_exec_hook '( INSTANCE * )'          >> $FAKE_DL_FNAME

search_symbols HOOK handler_hooks '[]'                          >> $FAKE_DL_FNAME

search_symbols "char *" modules_dependency '[]'                 >> $FAKE_DL_FNAME
echo "#endif"                                                   >> $FAKE_DL_FNAME

cat >> $FAKE_DL_FNAME <<EOT

/* ---------- structs ---------- */

typedef struct __FAKE_DL
{
    char            * dlname;
    DLCONSTANT      * constants_def;
    char            ** types_def;
    char            ** globals_def;
    char            ** locals_def;
    DLVARFIXUP      * globals_fixup;
    DLVARFIXUP      * locals_fixup;
    DLSYSFUNCS      * functions_exports;
    void            (* module_initialize)();
    void            (* module_finalize)();
    void            (* instance_create_hook)(INSTANCE *);
    void            (* instance_destroy_hook)(INSTANCE *);
    void            (* instance_pre_execute_hook)(INSTANCE *);
    void            (* instance_pos_execute_hook)(INSTANCE *);
    void            (* process_exec_hook)(INSTANCE *);
    HOOK            * handler_hooks;
    char           ** modules_dependency;
} __FAKE_DL;

EOT

make_fake_dl >> $FAKE_DL_FNAME

echo    "#endif"                                            >> $FAKE_DL_FNAME
