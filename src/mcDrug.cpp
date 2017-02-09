#include <stdio.h>

#include <iostream>
#include <vector>
#include <math.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <random>

#include <openbabel/atom.h>
#include <openbabel/babelconfig.h>
#include <openbabel/base.h>
#include <openbabel/builder.h>
#include <openbabel/math/vector3.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/forcefield.h>
#include <openbabel/obutil.h>

#include "mcFunc.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // check command line
    if (argc < 3) {
        printf("cmd line must take 2 input files (molecule to grow and the target molecule)\n");
        return 1;
    }

    // read in file names
    string grow = argv[1];
    string glucose = argv[2];

    // define fragment fils
    string fragment[] = {"coords/ch3.xyz","coords/nh2.xyz","coords/cf3.xyz","coords/och3.xyz","coords/oh.xyz","coords/furan.xyz","coords/ph.xyz","coords/5mem.xyz"};
    
    // bondorder (still hardcoded)
    int bondorder=1;

    // initialize the molecules
    OpenBabel::OBMol mol, mol2, mol_glucose, mol_tmp;
    
    // initialize output writing/format
    OpenBabel::OBConversion conv;
    conv.SetInAndOutFormats("xyz","xyz");
    OpenBabel::OBConversion conv2;
    conv2.SetInAndOutFormats("xyz", "xyz");

    ofstream ofs("gif/test.xyz");
    ofstream ofs_min("gif/min.xyz", ofstream::app);
    ifstream ifs;

    // read in growing molecule
    ifs.open(grow.c_str());
    conv.Read(&mol, &ifs);
    ifs.close();

    // read in target molecule
    ifs.open(glucose.c_str());
    conv.Read(&mol_glucose, &ifs);
    ifs.close();

    // loop to grow the chain
    for (int i=1; i<6; i++){
        int decide = rand() % 8;
        mcGrow(mol, fragment[decide], bondorder);
        mol_tmp = mcDock(mol, mol_glucose);
        conv2.Write(&mol_tmp, &ofs_min);
    }

    ofs.close();
    ofs_min.close();
 

    //mcGif(mol_out);
    // make a GIF
    mcGif("gif/min.xyz");

return 0;
}
