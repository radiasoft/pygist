# $Id: addPyGistTracker.py,v 1.1 2009/11/19 23:44:45 dave Exp $
#  -----------------------------------------------------------------
#  LLNL-specific file
#  -----------------------------------------------------------------

from posix import system
try:
   system ( "/usr/apps/tracker/bin/tracker -s -n PYGIST -v %s" % __version__ )
except:
   pass
