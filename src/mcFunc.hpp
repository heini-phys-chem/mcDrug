#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream>

using namespace std;

void mcGrow(OpenBabel::OBMol &mol, string fragment, int bondorder);

OpenBabel::OBMol mcDock(OpenBabel::OBMol mol_glucose, OpenBabel::OBMol mol);

int mcGif(string mol_out);
