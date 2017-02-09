#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream>

using namespace std;

int mcGif(string mol_out)
{
    ostringstream oss;
    oss << "jmol " << mol_out << " -s gif/cmd.script --nodisplay";
    string cmd1 = oss.str(); 
    string cmd2 = "convert -delay 120 -loop 0 gif/all*.jpg gif/all.gif";

    system(cmd1.c_str());
    system(cmd2.c_str());

    return 0;
}
