
/* $Id: h2_hull_max_coord_prod.h 4727 2009-11-16 20:53:54Z kobus $ */

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

#ifndef H2_HULL_MAX_COORD_PROD_INCLUDED
#define H2_HULL_MAX_COORD_PROD_INCLUDED


#include "h/h_gen.h"

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace kjb_c {
#endif
#endif


int set_hull_max_coord_prod_options
(
    const char* option,
    const char* value
);

int find_hull_max_coord_product
(
    const Hull* hp,
    Vector**    point_vpp,
    double*     product_ptr
);

int estimate_hull_max_coord_product
(
    const Hull* hp,
    Vector**    point_vpp,
    double*     product_ptr
);


#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
#endif

#endif

