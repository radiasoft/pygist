/*
 * pmin.c -- $Id: pmin.c,v 1.1 2009/11/19 23:44:50 dave Exp $
 * minimally intrusive play event handling (no stdio)
 */

#include "playw.h"
#include "pmin.h"

#ifdef __CYGWIN__
#include <stdio.h>
#include <sys/select.h>
#else
#include <conio.h>
#endif

static void (*w_abort_hook)(void) = 0;
static void (*w_exception)(int signal, char *errmsg) = 0;
static char *w_errmsg = 0;
static int w_checksig(void);

volatile int p_signalling = 0;

static int w_hook_recurse = 0;

void
p_abort(void)
{
  if (!p_signalling) p_signalling = PSIG_SOFT;
  w_hook_recurse = 0;
  w_abort_hook();   /* blow up if w_abort_hook == 0 */
}

#ifdef __CYGWIN__
/* Cygwin lacks the _kbhit function. Adding it here. */

static int _kbhit(void)
{
  fd_set fds;
  struct timeval tv;

  const int fd = fileno(stdin);

  FD_ZERO (&fds);
  FD_SET (fd, &fds);

  tv.tv_sec = 0;
  tv.tv_usec = 0;

  return select(fd+1, &fds, NULL, NULL, &tv) && FD_ISSET(fd, &fds);
}
#endif

int
p_wait_stdin(void)
{
  HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  const int redirected = (GetFileType(hStdIn)!=FILE_TYPE_CHAR);
  for (;;) {
    double wait_secs;
    MSG msg;
    while (PeekMessage(&msg, 0, 0,0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      p_on_idle(1);
    }
    wait_secs = p_timeout();
    if (wait_secs==0.0) p_on_idle(0);
    else {
      DWORD result;
      UINT timeout = (wait_secs < 0.0) ? INFINITE : (UINT) (1000.*wait_secs);
      result = MsgWaitForMultipleObjects(1,
                                         &hStdIn,
                                         FALSE,
                                         timeout,
                                         QS_ALLINPUT);
      if (result==WAIT_OBJECT_0) {
        if (redirected || _kbhit()) return 1;
        /* Read and discard the event */
        FlushConsoleInputBuffer(hStdIn);
      }
    }
  }
  return 0;
}

/* does not include stdin, but irrelevant for graphics events */
void
p_pending_events(void)
{
  MSG msg;
  w_checksig();
  while (PeekMessage(&msg, 0, 0,0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) break;
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    w_checksig();
  }
}

void
p_wait_while(int *flag)
{
  MSG msg;
  if (!w_checksig()) {
    while (*flag) {
      double wait_secs = p_timeout();
      if (wait_secs >= 0.0)
      { UINT timeout = (UINT)(1000*wait_secs); /* milliseconds */
        UINT timerid = SetTimer(NULL, 0, timeout, NULL);
        GetMessage(&msg, 0, 0,0);
        KillTimer(NULL, timerid);
        if (msg.message==WM_TIMER)
        { p_on_idle(0);
          continue;
        }
      }
      else if(!PeekMessage(&msg, 0, 0,0, PM_REMOVE))
      { p_on_idle(0);
        continue;
      }
      if (msg.message == WM_QUIT) break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      p_on_idle(1);
      if (w_checksig()) break;
    }
  }
}

static int
w_checksig(void)
{
  int sig = p_signalling;
  if (sig) {
    p_signalling = 0;
    if (w_exception) w_exception(sig, (char *)0);
  }
  return sig;
}

/* the WH_KEYBOARD hook sounds a little scary -- MSDN warns:
 *   Before terminating, an application must call the UnhookWindowsHookEx
 *   function to free system resources associated with the hook.
 * unclear whether this warning applies only to global hooks
 */
#if USE_KB_HOOK
static LRESULT CALLBACK w_hook(int n, WPARAM w, LPARAM l);
static HHOOK w_hhook = 0;
#endif

void
p_xhandler(void (*abort_hook)(void),
           void (*on_exception)(int signal, char *errmsg))
{
  w_abort_hook = abort_hook;   /* replaces p_abort */
  w_exception = on_exception;  /* when p_signalling detected */
#if USE_KB_HOOK
  if (!w_hhook)
    w_hhook = SetWindowsHookEx(WH_KEYBOARD, &w_hook, 0,
                               GetCurrentThreadId());
#endif
}

#if USE_KB_HOOK
static LRESULT CALLBACK
w_hook(int n, WPARAM w, LPARAM l)
{
  p_wait_stdin();
  return CallNextHookEx(w_hhook, n, w, l);
}
#endif
