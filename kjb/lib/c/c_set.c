
/* $Id: c_set.c 4727 2009-11-16 20:53:54Z kobus $ */

/* =========================================================================== *
|
|  Copyright (c) 1994-2008 by Kobus Barnard (author).
|
|  Personal and educational use of this code is granted, provided that this
|  header is kept intact, and that the authorship is not misrepresented, that
|  its use is acknowledged in publications, and relevant papers are cited.
|
|  For other use contact the author (kobus AT cs DOT arizona DOT edu).
|
|  Please note that the code in this file has not necessarily been adequately
|  tested. Naturally, there is no guarantee of performance, support, or fitness
|  for any particular task. Nonetheless, I am interested in hearing about
|  problems that you encounter.
|
* =========================================================================== */

#include "c/c_gen.h"     /* Only safe as first include in a ".c" file. */

#include "c/c_colour_space.h"
#include "c/c_sensor.h"
#include "c/c_set.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */

/*
 * =============================================================================
 *                                  kjb_c_set
 *
 *
 *
 * Index:
 *     set option handling 
 *
 * -----------------------------------------------------------------------------
 */

int kjb_c_set(const char *option, const char *value)
{
    int         temp_result;
    int         result = NOT_FOUND;
    int         (**set_fn_ptr)(const char*, const char*);
    static int  (*set_fn[]) (const char*, const char*) =
                                    {
                                        set_sensor_options,
                                        set_colour_space_options,
                                        NULL
                                    };

    set_fn_ptr = set_fn;

    while (*set_fn_ptr != NULL)
    {
        ERE(temp_result = (**set_fn_ptr)(option, value));

        if (temp_result == NO_ERROR) result = NO_ERROR;

        set_fn_ptr++;
    }

    return result;
}

/*  /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\   */

#ifdef __cplusplus
}
#endif

