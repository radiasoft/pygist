/*
 * feep.c -- $Id: feep.c,v 1.1 2009/11/19 23:44:49 dave Exp $
 * p_feep for MS Windows
 *
 * Copyright (c) 1999.  See accompanying LEGAL file for details.
 */

#include "playw.h"

/* ARGSUSED */
void
p_feep(p_win *w)
{
  MessageBeep(MB_OK);
}
