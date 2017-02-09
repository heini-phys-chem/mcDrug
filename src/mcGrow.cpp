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

using namespace std;

void  mcGrow(OpenBabel::OBMol &mol, string fragment, int bondorder)
{

    OpenBabel::OBMol mol2;

    // initialize output format
    OpenBabel::OBConversion conv;
    conv.SetInAndOutFormats("xyz","xyz");
    ifstream ifs;

    // read in fragment
    ifs.open(fragment.c_str());
    conv.Read(&mol2, &ifs);
    ifs.close();
    
    // initialize OBBuilder
    OpenBabel::OBBuilder builder;

    // delete all hydrogens
    mol.DeleteHydrogens();
    mol2.DeleteHydrogens();

    // get last atom in growing molecule
    int counter = mol.NumAtoms();
    int pos1 = counter;
    int pos2 = counter +1;

    // add fragment to growing molecule (same coordinate system)
    mol += mol2;

    // add the molecules + the missing hydrogens
    builder.Connect(mol,pos1,pos2,bondorder);
    mol.AddHydrogens();

}
