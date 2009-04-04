#include "auto_home.h"

void hier()
{
  h(auto_home,-1,-1,02755);

  d(auto_home,"bin",-1,-1,02755);

  c(auto_home,"bin","822addr",-1,-1,0755);
  c(auto_home,"bin","822body",-1,-1,0755);
  c(auto_home,"bin","822bodyfilter",-1,-1,0755);
  c(auto_home,"bin","822fields",-1,-1,0755);
  c(auto_home,"bin","822headerfilter",-1,-1,0755);
  c(auto_home,"bin","822headerok",-1,-1,0755);
  c(auto_home,"bin","822headers",-1,-1,0755);
  c(auto_home,"bin","checkaddr",-1,-1,0755);
  c(auto_home,"bin","checkdomain",-1,-1,0755);
  c(auto_home,"bin","condtomaildir",-1,-1,0755);
  c(auto_home,"bin","filterto",-1,-1,0755);
  c(auto_home,"bin","ifaddr",-1,-1,0755);
  c(auto_home,"bin","iftoccfrom",-1,-1,0755);
  c(auto_home,"bin","replier",-1,-1,0755);
  c(auto_home,"bin","replier-config",-1,-1,0755);
  c(auto_home,"bin","tomaildir",-1,-1,0755);
}
