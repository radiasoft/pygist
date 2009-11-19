/*
 * EPS.H
 *
 * $Id: eps.h,v 1.1 2009/11/19 23:44:47 dave Exp $
 *
 * Declare the Encapsulated PostScript pseudo-engine for GIST.
 *
 */
/*    Copyright (c) 1994.  The Regents of the University of California.
                    All rights reserved.  */

#ifndef EPS_H
#define EPS_H

#include "gist.h"

extern Engine *EPSPreview(Engine *engine, char *file);

extern int epsFMbug;

#endif
