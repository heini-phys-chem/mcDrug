#define HAVE_EIGEN

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
// #include <cmath>
#include <string.h>
#include <fstream>
#include <sstream>
#include <random>

#include <openbabel/babelconfig.h>
#include <openbabel/base.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/forcefield.h>
#include <openbabel/obutil.h>

#include "utils.hpp"

OpenBabel::OBMol mcDock(OpenBabel::OBMol mol, OpenBabel::OBMol mol_glucose) {


    OpenBabel::OBMol mol_tmp;

    OpenBabel::vector3 com;
    com = get_com(mol);
    move_molecule(mol, -com);
   
    std::string ff = "MMFF94"; 
    double ea;

    ea = minimize_molecule(mol_glucose, ff);

    com = get_com(mol_glucose);
    move_molecule(mol_glucose, -com);

    printf("Performing rotor search for ligand molecule    file: " );
    set_conformations(mol);
    printf("Found %3i rotatable bonds\n", mol.NumRotors());

    // Initialize random number generators
    std::default_random_engine generator;
    std::uniform_real_distribution<double> random_length(0.0, 0.25);
    std::uniform_real_distribution<double> random_angle(0.0, 90.0/180.0*M_PI);
    std::uniform_real_distribution<double> uniform1(0.0, 1.0);

    // Initialize variables
    double e_low = std::numeric_limits<double>::infinity();
    double eb_min = std::numeric_limits<double>::infinity();

    // Initialize force field
    OpenBabel::OBForceField* pFF = OpenBabel::OBForceField::FindForceField(ff);

    // Initialize variables
    double theta;
    OpenBabel::vector3 temp;
    OpenBabel::vector3 dir;
    OpenBabel::vector3 rot;
    OpenBabel::vector3 move;

    double eb;

    // Initialize file output
    OpenBabel::OBConversion conv;
    conv.SetInAndOutFormats("xyz", "xyz");
    std::remove("gif/conformers.xyz");
    std::ofstream ofs_conf("gif/conformers.xyz");

    // Minimize all conformers from rotamer search and dump to file
    for (int c = 0; c < mol.NumConformers(); ++c) {
        mol.SetConformer(c);

       eb = minimize_molecule(mol, ff);

        if (eb < eb_min) eb_min = eb;

        printf("Rotamer %4i     E = %10.4f kcal/mol\n", c, eb);
        conv.Write(&mol, &ofs_conf);
    }

    ofs_conf.close();

    printf("Lowest energy conformation  E = %10.4f kcal/mol\n", eb_min);
    printf("Running %3i trajectories for %10i steps.\n", 1, 1000);
    printf("MC temperature (tau) = %10.4f\n", 1.0);

    // Initialize output for trajectory
    std::remove("gif/out.xyz");
    std::ofstream ofs("gif/out.xyz");
    
    // Print header
    printf("\nConformation:       Trajectory:         Acceptance rate:    Final Ebind:\n");
    printf("---------------------------------------------------------------------------\n");

    for (int c = 0; c < mol.NumConformers(); ++c) {
        mol.SetConformer(c);

        // Minimize conformer
       eb = minimize_molecule(mol, ff);


        for (unsigned int n = 0; n < 1; n++) {
            
            // Translate molecule randomly
            dir.randomUnitVector();
            com = get_com(mol);
            temp = dir * 3.0 - com;
            move_molecule(mol, temp);
            
            // Rotate molecule randomly
            rot.randomUnitVector();
            theta = random_angle(generator);
            rotate_molecule(mol, rot, theta);
            
            // Make object to roll-back rejected MC moves
            OpenBabel::OBMol mol_ligand = mol_glucose;
            mol_ligand += mol;
            OpenBabel::OBMol mol_old = mol_ligand;
            mol_old.SetCoordinates(mol_ligand.GetCoordinates());

            // Initialize MC energy
            pFF->Setup(mol_ligand);
            double e;

            // Calculate energy of complex
            e = pFF->Energy();

            // Initialize variables
            double energy_old = e;
            double delta_e = 0.0;
            int accept = 0;
            double acceptance_ratio;

            // Start and end ID of ligand in complex
            int startid = mol_ligand.NumAtoms() - mol.NumAtoms() + 1;
            int endid = mol_ligand.NumAtoms() + 1;

            // Begin MC simulation
            for (int step = 0; step < 10; step++) {

                // Translation move
                move.randomUnitVector();
                move *= random_length(generator);
                move_molecule(mol_ligand, move, startid=startid, endid=endid);

                // Rotation move
                rot.randomUnitVector();
                theta = random_angle(generator);
                rotate_molecule(mol_ligand, rot, theta, startid=startid, endid=endid);

                // Evaluate total energy
                pFF->SetCoordinates(mol_ligand);
                e = pFF->Energy();


                delta_e =  e - energy_old;

                // Metropolis-Hastings MC criterion, accept ...
                 if (std::exp( - delta_e / 0.3) >= uniform1(generator)) {
                        mol_old.SetCoordinates(mol_ligand.GetCoordinates());
                        energy_old = e;
                        accept += 1;

                 } else {
                        mol_ligand.SetCoordinates(mol_old.GetCoordinates());
                        e = energy_old;
                 }
            }


            double ec;
            ec = minimize_molecule(mol_ligand, ff);

             double e_bind = ec - (ea + eb_min);
             acceptance_ratio = accept * 100.0 / (1000 + 1);

             printf(" %3i / %3i          %3i / %3i           %6.2f %%        %10.4f kcal/mol", c + 1, mol.NumConformers(), n + 1, 1, acceptance_ratio, e_bind);

            conv.Write(&mol_ligand, &ofs);

            // Check if we found the lowest energy
            if (e_bind < e_low) {
                mol_tmp = mol_ligand;
                 printf(" <---- New lowest\n");
                 //OpenBabel::OBConversion conv2;
                 //conv2.SetInAndOutFormats("xyz", "xyz");
                 e_low = e_bind;
                 //std::remove("min.xyz");
                 //std::ofstream ofs_min("min.xyz");
                 //std::string title = std::to_string(e_bind);
                 //mol_ligand.SetTitle(title);
                 //conv2.Write(&mol_ligand, &ofs_min);
                 //ofs_min.close();
            } else {
                printf("\n");
            }
        }
    }

    ofs.close();
    //double time_elapsed = timer.Elapsed();
    //printf("\nOptimized E_bind = %10.4f kcal/mol    Elapsed time = %6.2f seconds\n",
    //                e_low, time_elapsed);

    return mol_tmp;

}
