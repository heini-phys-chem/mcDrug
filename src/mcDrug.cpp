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

	ifstream ifs;

	OpenBabel::OBMol frag1, frag2, mol_tmp;
	OpenBabel::OBConversion conv1, conv2;
	conv1.SetInAndOutFormats("xyz", "xyz");
	conv2.SetInAndOutFormats("xyz", "xyz");

	string fragment1 = "coords/fragment1.xyz";//argv[1];
	string fragment2 = "coords/fragment2.xyz";//argv[2];

	ifs.open(fragment1.c_str());
	conv1.Read(&frag1, &ifs);
	ifs.close();

	ifs.open(fragment2.c_str());
	conv2.Read(&frag2, &ifs);
	ifs.close();

	mol_tmp = mcDock(frag1, frag2);

	return 0;
}
