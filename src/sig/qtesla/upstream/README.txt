Submission to NIST’s post-quantum project:
lattice-based digital signature scheme qTESLA
--------------------------------------------------------------------------------
Version 2.0: June 14, 2018
Version 2.1: June 30, 2018

Name of the cryptosystem: qTESLA
Principal and auxiliary submitters:
Sedat Akleylek, Erdem Alkim, Paulo Barreto, Nina Bindel, Johannes Buchmann, 
Edward Eaton, Gus Gutoski, Juliane Krämer, Patrick Longa, Harun Polat, 
Jefferson Ricardini, and Gustavo Zanon
--------------------------------------------------------------------------------

The submission of the lattice-based digital signature scheme qTESLA is organized
as follows. The submission folder "qTESLA_update_14.06.2018" includes four folders:

- "KAT":                      Contains the Known Answer Tests
- "Reference_Implementation": Contains the reference implementation
- "Optimized_Implementation": in this version, the optimized and reference
                              implementations are the same
- "Supporting_Documentation": Contains electronic version of all written 
                              materials, our sage script to choose parameters, 
                              and the submitters statements  

We list all files in the subfolders and give a brief description of each.
--------------------------------------------------------------------------------
Subfolder "KAT":
This folder contains knowns answer tests results for the proposed parameter sets.

- "PQCsignKAT_qTesla-I.rsp" : Known answer test results for qTesla-I
- "PQCsignKAT_qTesla-III-size.rsp" : Known answer test results for qTesla-III-size
- "PQCsignKAT_qTesla-III-speed.rsp" : Known answer test results for qTesla-III-speed
- "PQCsignKAT_qTesla-p-I.rsp" : Known answer test results for qTesla-p-I
- "PQCsignKAT_qTesla-p-III.rsp" : Known answer test results for qTesla-p-III

--------------------------------------------------------------------------------
Subfolder "Reference_Implementation":
This folder contains four subfolders which contain the reference implementations
for the proposed parameter sets.

- "qTesla-I" : Reference implementation of qTesla-I with parameters for
               NIST’s security category 1
- "qTesla-III-size" : Reference implementation of qTesla-III-size with parameters for
               NIST’s security category 3
- "qTesla-III-speed" : Reference implementation of qTesla-III-speed with parameters for
               NIST’s security category 3
- "qTesla-p-I" : Reference implementation of qTesla-p-I with parameters for
               NIST’s security category 1
- "qTesla-p-III" : Reference implementation of qTesla-p-III with parameters for
               NIST’s security category 3

--------------------------------------------------------------------------------
Subfolder "Supporting_Documentation":
This folder contains the following three subfolders and one pdf-file. 

- "Implementation_owners_statements_qTESLA": This folder contains the statements
according to Section 2.D.3 in NIST's call for proposals. Each file is named
"qTESLA-implementation-owners-statement-NAME.pdf" where NAME is the family
name of one of the implementations owners, i.e., Akleylek, Alkim, Barreto, Longa,
Polat, Ricardini, or Zanon. 

- "Submitters_statements_qTESLA": This folder contains the statements
according to Section 2.D.1 in NIST's call for proposals. Each file is named
"qTESLA-NIST-submitters-statement-NAME.pdf" where NAME is the family name of one
of the submitters, i.e., Akleylek, Alkim, Barreto, Bindel, Buchmann, Eaton, 
Gutoski, Krämer, Longa, Polat, Ricardini, or Zanon. 

- "Script_to_choose_parameters": This folder contains all files needed to run
the sage script that we used to choose our proposed parameter sets. Those files are: 

   - "README_Script_Parameterchoice.rst": This is the README file of our sage
                     script to compute parameters.
   - "parameterchoice.sage": This file is the main file to compute all system
                     parameters and estimate the hardness of the corresponding 
                     R-LWE instance. To this end it calls a function defined in
                     "estimator.py"
   - "README_LWE_Estimator.rst": This is the README file of the LWE-Estimator 
                     written by Albrecht, Player, and Scott. We use the 
                     LWE-Estimator to estimate the hardness of our chosen LWE 
                     instances.
   - "estimator.py": This python script is the implementation of the 
                     LWE-Estimator by Albrecht, Play, and Scott that we call in
                     our sage script "parameterchoice.sage".
   - The rest of the contained files are subroutines of the LWE-Estimator. We 
                     do not describe them in detail here but refer to the 
                     README file of the LWE-Estimator. 

- "Scripts_for_Gaussian_sampler": This folder contains Magma scripts to generate
the precomputed tables required by the Gaussian sampler. Those files are: 

   - "gaussBernoulliSample_table.magma": script to generate the Bernoulli table.
   - "gaussSigma2Sample_table.magma": script to generate the CDT table.

- "qTESLA_v2.1_06.30.2018.pdf": This pdf file is the description of our scheme qTESLA. We give 
all supporting information about our submitted signature scheme in the pdf.