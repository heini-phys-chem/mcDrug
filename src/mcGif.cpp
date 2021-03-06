#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream>

using namespace std;

int mcGif(string mol_out)
{
    int ret_val1;
    int ret_val2;
    
    ostringstream oss;
    oss << "jmol " << mol_out << " -s gif/cmd.script --nodisplay";
    string cmd1 = oss.str(); 
    string cmd2 = "convert -delay 120 -loop 0 gif/all*.jpg gif/all.gif";

    ret_val1 = system(cmd1.c_str());
    ret_val2 = system(cmd2.c_str());

    if (ret_val1 == 0 && ret_val2 == 0 && errno == 0) {
        printf("successfully created fancy GIF!\n");
    } else {
        printf("where the fuck is my GIF?!?\n");
    }

    return 0;
}
