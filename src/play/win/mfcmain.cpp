/*
 * mfcmain.cpp -- $Id: mfcmain.cpp,v 1.1 2009/11/19 23:44:50 dave Exp $
 * MFC main program stub
 *
 * Copyright (c) 2000.  See accompanying LEGAL file for details.
 */

extern "C" {
  extern int on_launch(int argc, char *argv[]);
}
#include "mfcapp.h"

mfc_boss the_boss(on_launch);
