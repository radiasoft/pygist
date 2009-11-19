
func color_bar(levs, colors, vert=, labs=, adjust=, ecolor=)
/* DOCUMENT color_bar
         or color_bar, levs, colors
     Draw a color bar below the current coordinate system.  If LEVS is
     not specified uses plfc_levs (set by previous call to plfc).  If
     COLORS is specified, it should have one more value than LEVS,
     otherwise equally spaced colors are chosen, or plfc_colors if
     plfc_levs was used.  With the vert=1 keyword the color bar appears
     to the left of the current coordinate system (vert=0 is default).
     By default, color_bar will attempt to label some of the color
     interfaces.  With the labs= keyword, you can force the labelling
     algorithm as follows: labs=0 supresses all labels, labs=n forces
     a label at every nth interface, labs=[i,n] forces a label at every
     nth interface starting from interface i (0<=i<=numberof(LEVS)).
     You can use the adjust= keyword to move the bar closer to (adjust<0)
     or further from (adjust>0) the viewport, and the height= keyword to
     set the height of any labels (default 14 points).
   SEE ALSO: plfc
 */
{
  if (is_void(levs)) {
    if (is_void(plfc_levs)) error, "no levels specified";
    levs= plfc_levs;
    n= numberof(levs)+1;
    if (is_void(colors)) colors= plfc_colors;
  } else {
    n= numberof(levs)+1;
    if (is_void(colors)) colors= bytscl(span(1,n,n),cmin=0.5,cmax=n+0.5);
  }
  if (n != numberof(colors))
    error, "numberof(colors) must be one more than numberof(levs)";

  port= viewport();
  if (is_void(adjust)) adjust= 0.;
  dx= dy= 0.;
  if (vert) {
    x= (port(2)+adjust+[0.022,0.042])(-:1:n+1,);
    dx= 0.005;
    y= span(port(3),port(4),n+1)(,-:1:2);
  } else {
    y= (port(3)-adjust-[0.045,0.065])(-:1:n+1,);
    dy= -0.005;
    x= span(port(1),port(2),n+1)(,-:1:2);
  }
  sys= plsys(0);
  plf,[colors],y,x,edges=1,ecolor=ecolor, legend="";
  plsys, sys;

  if (is_void(labs) || labs(0)>0) {
    if (numberof(levs)>1) {
      dz= levs(dif);
      if (numberof(dz)!=numberof(levs)-1 ||
          anyof((dz>0.)!=(dz(1)>0.)) || !dz(1))
        error, "levs must be monotone 1D";
      levs= levs(1:0);
      levs= grow([2*levs(1)-levs(2)],levs,[2*levs(0)-levs(-1)]);
    } else {
      levs= double(levs(1));
      if (!levs) levs= [-1.,levs,1.];
      else levs= [0.,levs,2*levs];
    }
    if (numberof(labs)<2) {
      if (is_void(labs)) labs= (n-1)/4 + 1;
      orig= where(levs<1.e-9*max(levs(dif)));
      if (numberof(orig)==1) labs= [orig(1)%labs,labs];
      else labs= [(n%labs)/2,labs];
    }
    list= where(indgen(0:n)%labs(2)==labs(1));
    x= x(list,);
    y= y(list,);
    labs= swrite(format="%g",levs(list));
    plsys, 0;
    pldj, x(,2),y(,2),x(,2)+dx,y(,2)+dy, legend="";
    plsys, sys;
    plt1, labs,x(,2)+dx,y(,2)+dy, justify=(vert?"LH":"CT"), height=height,
      font="helvetica";
  }
}

