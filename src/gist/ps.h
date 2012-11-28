/*
 * $Id: ps.h,v 1.1 2009/11/19 23:44:48 dave Exp $
 * Declare the PostScript engine for GIST.
 */
/* Copyright (c) 2005, The Regents of the University of California.
 * All rights reserved.
 * This file is part of yorick (http://yorick.sourceforge.net).
 * Read the accompanying LICENSE file for details.
 */

#ifndef PS_H
#define PS_H

#include "gist.h"
#include "engine.h"

#include <stdio.h>

typedef struct GpsBBox GpsBBox;
struct GpsBBox {
  int xll, yll, xur, yur;
};

typedef struct PSEngine PSEngine;
struct PSEngine {
  Engine e;

  /* --------------- Specific to PSEngine ------------------- */

  char *filename;
  p_file *file;     /* 0 until file is actually written into */
  int closed;     /* if file==0 and closed!=0, there was a write error */

  /* Page orientation and color table can only be changed at the beginning
     of each page, so the entries in the Engine base class are repeated
     here as the "currently in effect" values.  When a new page begins,
     these values are brought into agreement with those in the base
     class.  The ChangePalette virtual function temporarily resets colorMode
     to 0, to avoid any references to the new palette until the
     next page begins.  */
  int landscape;
  int colorMode;
  int nColors;

  GpsBBox pageBB;   /* bounding box for current page */
  GpsBBox docBB;    /* bounding box for entire document */
  int currentPage;  /* current page number, incremented by EndPage */
  long fonts;       /* bits correspond to Gist fonts (set when used) */

  /* The ps.ps GistPrimitives assume knowledge of the several
     graphical state parameters, mostly from gistA.  These are
     reset at the beginning of each page.  */
  GpBox clipBox;
  int curClip;
  unsigned long curColor;
  int curType;
  GpReal curWidth;
  int curFont;
  GpReal curHeight;
  int curAlignH, curAlignV;
  int curOpaque;

  /* When clipping is turned off, the ps.ps GistPrimitives state
     partially reverts to its condition when clipping was turned on.  */
  unsigned long clipColor;
  int clipType;
  GpReal clipWidth;
  int clipFont;
  GpReal clipHeight;

  char line[80];   /* buffer in which to build current output line */
  int nchars;      /* current number of characters in line */
};

PLUG_API PSEngine *GisPSEngine(Engine *engine);


// This is from the file ps.ps. After each line, add \n\. Replace " with \",
// \( with \\(, \) with \\), and \0 with \\0.
static char* pspstext = "%%Creator: Gist\n\
%%DocumentData: Clean7Bit\n\
%%DocumentSuppliedResources: procset Gist-Primitives 1.0 0\n\
%%Pages: (atend)\n\
%%BoundingBox: (atend)\n\
%%DocumentFonts: (atend)\n\
%%EndComments\n\
%%BeginProlog\n\
%\n\
% Gist PostScript Prolog\n\
% $Id: ps.ps,v 1.1 2009/11/19 23:44:47 dave Exp $\n\
% Copyright (c) 1994.  The Regents of the University of California.\n\
%               All rights reserved.\n\
%\n\
%%BeginResource: procset Gist-Primitives 1.0 0\n\
/GistPrimitives 128 dict def\n\
GistPrimitives begin\n\
/PG 0 def\n\
/LAND { 90 rotate 0 -12240 translate } bind def\n\
/CLON {\n\
  gsave /TxYxs TxYx def /TxYns TxYn def\n\
    newpath\n\
    moveto dup 0 exch rlineto exch 0 rlineto neg 0 exch rlineto\n\
    closepath clip newpath\n\
} bind def\n\
/CLOF {\n\
  /TxYx TxYxs def /TxYn TxYns def grestore\n\
} bind def\n\
/BG { 1 setgray } bind def\n\
/FG { 0 setgray } bind def\n\
/BLK { 0 setgray } bind def\n\
/WHT { 1 setgray } bind def\n\
/RED { 1 0 0 setrgbcolor } bind def\n\
/GRN { 0 1 0 setrgbcolor } bind def\n\
/BLU { 0 0 1 setrgbcolor } bind def\n\
/CYA { 0 1 1 setrgbcolor } bind def\n\
/MAG { 1 0 1 setrgbcolor } bind def\n\
/YEL { 1 1 0 setrgbcolor } bind def\n\
/GYD { 0.392 setgray } bind def\n\
/GYC { 0.588 setgray } bind def\n\
/GYB { 0.745 setgray } bind def\n\
/GYA { 0.839 setgray } bind def\n\
/DSH {  % index DSH\n\
  [ [ ] [ 82.5 ] [ 4.5 61.5 ] [ 82.5 39.0 4.5 39.0 ]\n\
  [ 82.5 39.0 4.5 39.0 4.5 39.0 ] ] exch get\n\
  dup length 0 ne {\n\
    currentlinewidth dup 16.5 lt {\n\
      pop\n\
    } {\n\
      16.5 div 1 index { 2 copy mul 4 1 roll pop } forall pop astore\n\
    } ifelse\n\
  } if\n\
  0 setdash\n\
} bind def\n\
/LW /setlinewidth load def\n\
/GPL { 1 setlinecap 1 setlinejoin } bind def\n\
/GDJ { 2 setlinecap } bind def\n\
/GPT {\n\
  currentfile 4 string readhexstring pop { } forall\n\
  exch 8 bitshift or 3 1 roll exch 8 bitshift or exch\n\
} bind def\n\
/L {\n\
  GPL\n\
  newpath 1 sub GPT\n\
  { 3 2 roll dup 255 gt { 255 sub 255 } { 0 exch } ifelse\n\
    4 2 roll moveto 0 exch 0 exch\n\
    { GPT 4 2 roll pop pop 2 copy lineto } repeat stroke\n\
    2 index 0 le { pop pop pop exit } if\n\
  } loop\n\
} bind def\n\
/LS {\n\
  GPL\n\
  newpath 1 sub 3 idiv GPT\n\
  { 3 2 roll dup 85 gt { 85 sub 85 } { 0 exch } ifelse\n\
    4 2 roll moveto 0 exch 0 exch\n\
    { GPT GPT GPT 8 6 roll pop pop 2 copy 8 2 roll curveto } repeat stroke\n\
    2 index 0 le { pop pop pop exit } if\n\
  } loop\n\
} bind def\n\
/D {\n\
  GDJ\n\
  newpath { GPT moveto GPT lineto stroke } repeat\n\
} bind def\n\
/Cour {\n\
  [ /L-Courier /L-Courier-Bold /L-Courier-Oblique /L-Courier-BoldOblique ]\n\
  exch get FindLatin\n\
} bind def\n\
/Tims {\n\
  [ /L-Times-Roman /L-Times-Bold /L-Times-Italic /L-Times-BoldItalic ]\n\
  exch get FindLatin\n\
} bind def\n\
/Helv {\n\
  [ /L-Helvetica /L-Helvetica-Bold\n\
    /L-Helvetica-Oblique /L-Helvetica-BoldOblique ]\n\
  exch get FindLatin\n\
} bind def\n\
/Symb {\n\
  pop /Symbol findfont\n\
} bind def\n\
/NCen {\n\
  [ /L-NewCenturySchlbk-Roman /L-NewCenturySchlbk-Bold\n\
    /L-NewCenturySchlbk-Italic /L-NewCenturySchlbk-BoldItalic ]\n\
  exch get FindLatin\n\
} bind def\n\
/StdNames 16 dict begin % dictionary of fonts not yet re-encoded\n\
  /L-Courier /Courier def /L-Courier-Bold /Courier-Bold def\n\
  /L-Courier-Oblique /Courier-Oblique def\n\
  /L-Courier-BoldOblique /Courier-BoldOblique def\n\
  /L-Times-Roman /Times-Roman def /L-Times-Bold /Times-Bold def\n\
  /L-Times-Italic /Times-Italic def /L-Times-BoldItalic /Times-BoldItalic def\n\
  /L-Helvetica /Helvetica def /L-Helvetica-Bold /Helvetica-Bold def\n\
  /L-Helvetica-Oblique /Helvetica-Oblique def\n\
  /L-Helvetica-BoldOblique /Helvetica-BoldOblique def\n\
  /L-NewCenturySchlbk-Roman /NewCenturySchlbk-Roman def\n\
  /L-NewCenturySchlbk-Bold /NewCenturySchlbk-Bold def\n\
  /L-NewCenturySchlbk-Italic /NewCenturySchlbk-Italic def\n\
  /L-NewCenturySchlbk-BoldItalic /NewCenturySchlbk-BoldItalic def\n\
currentdict end def\n\
/FindLatin {\n\
  dup StdNames exch known\n\
  { dup StdNames exch get findfont dup length dict begin\n\
      { 1 index /FID ne { def } { pop pop } ifelse } forall\n\
      /Encoding ISOLatin1Encoding def\n\
    currentdict end\n\
    exch dup StdNames exch undef exch definefont }\n\
  { findfont } ifelse\n\
} bind def\n\
/FNT {\n\
  /LnSp exch FontRescale mul def\n\
  /PtSz exch FontRescale mul def\n\
  PtSz scalefont setfont\n\
  currentfont /FontBBox get aload pop\n\
  currentfont /FontMatrix get transform /TxYx exch def pop\n\
  currentfont /FontMatrix get transform /TxYn exch def pop\n\
} bind def\n\
/SS1cpy { dup length string copy } bind def\n\
/SS3cpy { 3 { 3 1 roll SS1cpy } repeat } bind def\n\
/SScleave {\n\
  dup 0 get exch dup length 1 sub 1 exch getinterval exch\n\
} bind def\n\
/SSstring {\n\
  1 string dup 3 2 roll 0 exch put\n\
} bind def\n\
/SFwidth {\n\
  (\\024) search {\n\
    /wfn 0 def\n\
    { stringwidth pop wfn add /wfn exch def pop SS1cpy SScleave SSstring\n\
      currentfont exch /Symbol findfont PtSz scalefont setfont\n\
      stringwidth pop wfn add /wfn exch def setfont\n\
      SS1cpy (\\024) search not { exit } if } loop\n\
    stringwidth pop wfn add /wfn exch def wfn 0\n\
  } { stringwidth } ifelse\n\
} bind def\n\
/SFshow {\n\
  (\\024) search {\n\
    { show pop SS1cpy SScleave SSstring\n\
      currentfont exch /Symbol findfont PtSz scalefont setfont show setfont\n\
      SS1cpy (\\024) search not { exit } if } loop\n\
  } if\n\
  show\n\
} bind def\n\
/SSscale 0.75000 def\n\
/SSdown -0.11111 def\n\
/SSup 0.36111 def\n\
/SSwidth { % string SSwidth --> dx dy\n\
  (\\021) search {\n\
    /wn 0 def\n\
    /ws 0 def\n\
    { SS3cpy SFwidth pop wn add /wn exch def\n\
      search pop SS3cpy SScleave pop SFwidth pop ws add /ws exch def\n\
      search not { exit } if } loop\n\
    SFwidth pop wn add /wn exch def\n\
    ws SSscale mul wn add 0\n\
  } { SFwidth } ifelse\n\
} bind def\n\
/SSshow {\n\
  (\\021) search {\n\
    { SS3cpy SFshow\n\
      search pop SS3cpy SScleave 8#022 eq { SSup } { SSdown } ifelse\n\
      TxYx mul dup 0 exch rmoveto exch\n\
      matrix currentmatrix exch SSscale SSscale scale SFshow setmatrix\n\
      neg 0 exch rmoveto\n\
      search not { exit } if } loop\n\
  } if\n\
  SFshow\n\
} bind def\n\
/OShw /pop load def\n\
/OPQ {\n\
  0 eq {\n\
    /OShw /SSshow load def\n\
  } {\n\
    /OShw {\n\
      gsave\n\
        dup SSwidth\n\
        0 TxYn\n\
        rmoveto 2 copy rlineto 0 LnSp rlineto\n\
        neg exch neg exch rlineto closepath\n\
        1 setgray fill\n\
      grestore\n\
      SSshow\n\
    } def\n\
  } ifelse\n\
} bind def\n\
/LF { } def\n\
/CN { dup SSwidth -0.5 mul exch -0.5 mul exch rmoveto } bind def\n\
/RT { dup SSwidth neg exch neg exch rmoveto } bind def\n\
/TP { 0 LnSp neg rmoveto } bind def\n\
/CP { 0 TxYx TxYn add neg rmoveto } bind def\n\
/HF { 0 TxYx TxYn add -0.5 mul rmoveto} bind def\n\
/BA { } def\n\
/BT { 0 TxYn neg rmoveto } bind def\n\
/JUS {\n\
  load /YAdj exch def\n\
  load /XAdj exch def\n\
} bind def\n\
/XAD { /XAdj load exec } bind def\n\
/YAD { /YAdj load exec } bind def\n\
/OSH { /OShw load exec } bind def\n\
/T {\n\
  newpath moveto YAD XAD OSH\n\
} bind def\n\
/TX { XAD OSH currentpoint exch pop 0 exch LnSp sub moveto } bind def\n\
/TA {\n\
  gsave\n\
    newpath translate 0 0 moveto YAD\n\
    { TX } forall\n\
  grestore\n\
} bind def\n\
/TR {\n\
  gsave\n\
    newpath translate rotate 0 0 moveto YAD\n\
    { TX } forall\n\
  grestore\n\
} bind def\n\
/M {\n\
  newpath { GPT moveto HF CN dup show } repeat pop\n\
} bind def\n\
/MX { } def\n\
/M1 { 1 0 rlineto stroke } bind def\n\
/M2 { PtSz 0.5 mul\n\
      dup -0.5 mul dup 0 rmoveto 1 index 0 rlineto\n\
      dup rmoveto 0 exch rlineto stroke\n\
} bind def\n\
/M3 { PtSz 0.5 mul\n\
      dup -0.5 mul dup 0 exch rmoveto 0 2 index rlineto\n\
      exch 0.866025 mul 2 copy -0.5 mul exch 0.5 mul rmoveto\n\
      exch 2 copy rlineto\n\
      dup 0 exch neg rmoveto\n\
      exch neg exch rlineto stroke\n\
} bind def\n\
/M4 { currentpoint PtSz 0.25 mul dup 0 rmoveto\n\
      0 360 arc stroke\n\
} bind def\n\
/M5 { PtSz 0.5 mul\n\
      dup -0.5 mul dup rmoveto dup dup rlineto\n\
      dup neg 0 rmoveto dup neg rlineto stroke\n\
} bind def\n\
/MS {\n\
  gsave\n\
    exch dup 0 eq {\n\
      PtSz 0.1 mul setlinewidth 1 setlinecap\n\
    } {\n\
      PtSz 0.05 mul setlinewidth 0 setlinecap\n\
    } ifelse\n\
    /MX [ /M1 /M2 /M3 /M4 /M5 ] 3 -1 roll get load def\n\
    [ ] 0 setdash\n\
    newpath { GPT moveto /MX load exec } repeat\n\
  grestore\n\
} bind def\n\
\n\
/CTrgb 0 array def\n\
/CTn 0 def\n\
/CThi 0 def\n\
/CTsn 0 def\n\
/CT {\n\
  dup dup /CTn exch def 1 sub /CThi exch def\n\
  /CTrgb exch 3 mul string def\n\
  currentfile CTrgb readhexstring pop pop\n\
  /CTX load exec\n\
} bind def\n\
/CT1 {\n\
  CTrgb\n\
    /CTrgb CTn array def\n\
    gsave\n\
      0 1 CThi {\n\
        2 copy 3 mul 3 getinterval { 255.0 div } forall\n\
        setrgbcolor currentgray\n\
        CTrgb 3 1 roll put\n\
      } for\n\
    grestore\n\
  pop\n\
  /I /I1 load def\n\
  /C /C1 load def\n\
  /CI { } def\n\
} bind def\n\
/CT2 {\n\
  CTrgb\n\
    /CTrgb CTn array def\n\
    0 1 CThi {\n\
      2 copy 3 mul 3 getinterval { 255.0 div } forall\n\
      3 array astore CTrgb 3 1 roll put\n\
    } for\n\
  pop\n\
  /I /I2 load def\n\
  /C /C2 load def\n\
  /CI { } def\n\
} bind def\n\
/CT3 {\n\
  CTrgb\n\
    /CTrgx CTn array def\n\
    0 1 CThi {\n\
      2 copy 3 mul 3 getinterval { 255.0 div } forall\n\
      3 array astore CTrgx 3 1 roll put\n\
    } for\n\
  pop\n\
  /I /I3 load def\n\
  /C /C3 load def\n\
  /CI { } def\n\
} bind def\n\
/F {\n\
  newpath GPT moveto\n\
  1 sub { GPT lineto } repeat closepath fill\n\
} bind def\n\
/E {\n\
  dup 0 eq {\n\
    pop stroke\n\
  } {\n\
    newpath GPT moveto\n\
    1 sub { GPT lineto } repeat closepath gsave fill grestore\n\
  } ifelse\n\
} bind def\n\
/GRGB {\n\
  exch dup 255 le { exch exec } { dup 255 and 255.0 div exch dup\n\
    -8 bitshift 255 and 255.0 div exch -16 bitshift 255 and 255.0 div\n\
    setrgbcolor pop } ifelse\n\
} bind def\n\
/CI { } def\n\
/C { } def\n\
/C0 {\n\
  { 255.0 div setgray } GRGB\n\
} bind def\n\
/C1 {\n\
  { CTrgb exch get setgray } GRGB\n\
} bind def\n\
/C2 {\n\
  { CTrgb exch get aload pop setrgbcolor } GRGB\n\
} bind def\n\
/C3 {\n\
  { CTrgx exch get aload pop setrgbcolor } GRGB\n\
} bind def\n\
/I { } def\n\
/I0 {\n\
  /ROW 7 index 6 index mul 7 add 8 idiv string def\n\
  gsave\n\
    translate scale\n\
    dup 1 exch bitshift 1 sub /CTsn exch def\n\
    [ 3 index 0 0 5 index 0 0 ]\n\
    { currentfile ROW readhexstring pop } image\n\
  grestore\n\
} bind def\n\
/I1 {\n\
  /ROW 7 index 6 index mul 7 add 8 idiv string def\n\
  gsave\n\
    translate scale\n\
    dup 1 exch bitshift 1 sub /CTsn exch def\n\
    [ 3 index 0 0 5 index 0 0 ]\n\
    [ { CTsn mul round cvi\n\
      dup CThi gt { pop CThi } if CTrgb exch get } /exec load\n\
      currenttransfer /exec load ] cvx settransfer\n\
    { currentfile ROW readhexstring pop } image\n\
  grestore                          % restore saved CTM and transfer\n\
} bind def\n\
/I2 {\n\
  /ROW 7 index 6 index mul 7 add 8 idiv string def\n\
  gsave\n\
    translate scale\n\
    dup 1 exch bitshift 1 sub /CTsn exch def\n\
    [ 3 index 0 0 5 index 0 0 ]\n\
    currentcolortransfer\n\
    [ { CTsn mul round cvi\n\
      dup CThi gt { pop CThi } if CTrgb exch get 0 get } /exec load\n\
      7 -1 roll /exec load ] cvx\n\
    [ { CTsn mul round cvi\n\
      dup CThi gt { pop CThi } if CTrgb exch get 1 get } /exec load\n\
      7 -1 roll /exec load ] cvx\n\
    [ { CTsn mul round cvi\n\
      dup CThi gt { pop CThi } if CTrgb exch get 2 get } /exec load\n\
      7 -1 roll /exec load ] cvx\n\
    [ { CTsn mul round cvi\n\
      dup CThi gt { pop CThi } if CTrgb exch get 0 get } /exec load\n\
      7 -1 roll /exec load ] cvx\n\
    setcolortransfer\n\
    { currentfile ROW readhexstring pop } { ROW } { ROW }\n\
    true 3 colorimage\n\
  grestore\n\
} bind def\n\
/I3 {\n\
  /ROW 7 index 6 index mul 7 add 8 idiv string def\n\
  gsave\n\
    translate scale\n\
    [ /Indexed /DeviceRGB CThi CTrgb ] setcolorspace\n\
    7 dict begin\n\
      /ImageType 1 def\n\
      /BitsPerComponent exch def\n\
      /Height exch def\n\
      /Width exch def\n\
      /ImageMatrix [ Width 0 0 Height 0 0 ] def\n\
      /Decode [ 0 1 BitsPerComponent bitshift 1 sub ] def\n\
      /DataSource { currentfile ROW readhexstring pop } def\n\
    currentdict end image\n\
  grestore\n\
} bind def\n\
/J { } def\n\
/J1 {\n\
  /ROW 6 index 3 mul string def\n\
  4 2 roll 2 index add exch 3 index add exch\n\
  gsave\n\
    0 setgray 20 setlinewidth 0 setlinejoin 2 setlinecap\n\
    3 index 3 index moveto 3 index 1 index lineto 1 index 1 index lineto\n\
    1 index 3 index lineto closepath stroke 0 setlinecap\n\
    3 index 3 index moveto 1 index 1 index lineto stroke\n\
    1 index 3 index moveto 3 index 1 index lineto stroke pop pop pop pop\n\
    exch pop { currentfile ROW readhexstring pop pop } repeat\n\
  grestore\n\
} bind def\n\
/J2 {\n\
  /ROW 6 index 3 mul string def\n\
  gsave\n\
    translate scale\n\
    8 [ 3 index 0 0 5 index 0 0 ]\n\
    { currentfile ROW readhexstring pop }\n\
    false 3 colorimage\n\
  grestore\n\
} bind def\n\
/GI {\n\
  0.05 0.05 scale\n\
  10 setlinewidth\n\
  0 Cour 240 240 FNT\n\
  0 OPQ\n\
  /LF /BA JUS\n\
  /I /I0 load def\n\
  /C /C0 load def\n\
  /CI { } def\n\
} bind def\n\
end\n\
%%EndResource\n\
%%EndProlog\n\
%%BeginSetup\n\
GistPrimitives begin\n\
/FontRescale where { pop } { /FontRescale 1 def } ifelse\n\
/languagelevel where { pop languagelevel } { 1 } ifelse\n\
2 lt { % this is level 1 PostScript\n\
  /colorimage where { % color extension is present\n\
    % assume that if colorimage is available,\n\
    % setcolortransfer and currentcolortransfer are too\n\
    % (These are all listed as CMYK extensions to PostScript level 1)\n\
    pop\n\
    /CTX /CT2 load def\n\
    /J /J2 load def\n\
  } {                 % color extension not present\n\
    /CTX /CT1 load def\n\
    /J /J1 load def\n\
  } ifelse\n\
} {    % this is level 2 PostScript\n\
  /CTX /CT3 load def\n\
  /J /J2 load def\n\
} ifelse\n\
end\n\
%%EndSetup\n\
";

#endif
