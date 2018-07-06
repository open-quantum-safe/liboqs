Script to choose parameters for the lattice-based signature scheme qTESLA 
--------------------------------------------------------------------------------
Authors: Nina Bindel and Jefferson Ricardini
Owners: All submitters of qTESLA by name. 
--------------------------------------------------------------------------------

This sage script chooses parameters for the lattice-based signature scheme
qTESLA according to the tight security reduction in the quantum random oracle model.

The main function 
"calculate_params()" takes as input "xi, n, eta_e, eta_s, M, kappa, lamb, prof" and 
saves all computed parameters in a dictionary. The names of the parameters are 
according to the names given in the submission document except for "lamb" which
corresponds to the security parameters "lambda" in the document, and prof is
to choose security proof for parameter selection ("Heuristic","Provable"). 

To compute the hardness it calls the LWE-Estimator in the function "call_LWE_Estimator".

All functions to choose parameters are in the file "parameterchoice.sage". All
other files belong to the LWE-Estimator. More information about the use of 
the LWE-Estimator can be found in "README_LWE_Estimator.rst".

To print all the proposed parameter sets of qTESLA first load the file 
"parameterchoice.sage" and afterwards call "print_qtesla_parameters()". 
The order of the printed parameters is
'xi', 'la', 'n', 'k', 'eta_e', 'eta_s', 'M', 'kappa', 'sigma', 'h', 'Le', 'Ls',
'q', 'logq', 'B', 'd', 'pksize', 'sksize', 'sigsize', 'hardness'. 

The key and signature sizes are given in bits. The value 'hardness' is the estimated 
bit hardness against quantum attacks. 


