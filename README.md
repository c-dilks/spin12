SPIN ASYMMETRY ANALYSIS DOCUMENTATION 
-------------------------------------

 -  refer to `doc_diagram.pdf` for a diagram of code
 -  parallelograms = data (3d for multiple files)
 -  rectangles = executable
 -  trapezoid = env vars


Checking and Switching Cone Sizes
---------------------------------
0. make sure `Output` and `redset` are symlinks to the correct directories, by running
   `ls -l ../../ | grep '\->'`; for example, `redset -> redset_100mr` for the 100mr 
   isolation cone reduced data set 

1. make sure `diag.root` is for the correct cone size (I have it symlinked as 
   `diag.root -> diag_100mr` for the 100mr isolation cone)

2. make sure `mass_cuts` is for the correct cone size; if you're unsure, just run
   `root -b -q MassCutter.C` using the `diag.root` file mentioned in step 2




Installation
------------
 0. make sure you have the relative luminosity and polarization code directories first;
    (see the next section)
    you need the following root files:
    - counts.root -- scaler counts
    - rtree.root -- relative luminosity tree
    - pol.root -- polarization tree

 1. obtain the code: git clone `https://github.com/c-dilks/spin_release.git`

 2. my convention: relocate directory of spin code to ~/h5/root12fms/spin, i.e.,
      this README file is ~/h5/root12fms/spin/README.md
    - you can probably run it in any directory you choose, but mind the location 
      of output files, which are expected to be found in ../../Output

 3. execute `install` to create some directories and build source code
    - creates some new empty directories
    - builds source code in src/
    - symlink redset --> ../../redset 
    - symlink counts.root --> ../scalers/counts.root
    - symlink rtree.root --> ../scalers/rtree.root
    - symlink pol.root --> ../polar/pol.root
    - * my polar or scalers directory might be called polar13 and scalers13


Relative Luminosity & Polarization
----------------------------------

 - code and data are found in ../scalers for rellum and in ../polar for polarization
 - rellum root files are rtree.root and counts.root; polarization root file is pol.root
 - spin and `bXing kicked` status are read from ../scalers/counts.root


Data set reduction
------------------

 - This is the process of extracting the TwoTr from Output files to look at
   spin asymmetries for two photon events, which are suspected pion decays
   (see below for some TwoTr documentation)
 - The data set that will be reduced is ../../Output
 - `loop_ReduceData` creates a condor batch file to run `ReduceData.C` on
   all root files in ../../Output
   --> NOTE: FOR RUN 12, THIS SCRIPT IMPLEMENTS A SHIFT FORWARD OF 1 BXING
       YOU MUST LOOK AT OUTPUT OF BxingDistPi0.C TO VERIFY THIS SHIFT IS CORRECT
       FOR THE RUNS BEING CONSIDERED
 - after reduction, run `Diagnostics.C` to look at dependences of kinematics on
   geometry; produces `diag.pdf` with various 2d histograms
 - you can also check for corruption by using GetRedsetEntries.C; if the tree
   in the reduced data set has zero entries, then the Output file tree may have
   been corrupt; see the comments in the header of GetRedsetEntries.C to see
   how to loop this over all files in redset/



Asymmetry Analysis 
------------------
0. Make diagnostic plots `diag.root` by running `Diagnostics.C`; this may take a while
   - `diag.root` will have a number of plots including:
     - full mass distribution for 2 photon events
     - Z distribution for 2 photon events and naive pi0 mass cut ( |M-135|<=100 MeV) 
     - trigger distribution for 2 photon events
     - mass vs. energy & pt for 2 photon events
     - mass distributions for 10 energy bins { [0,10),..,[90,100) }
     - Z vs. eta and phi for 2 photon events
     - for three event classes pi0s (pi0), single photons (sph), and three or more photons (thr)
       - pt and energy vs. eta and phi
       - eta vs. phi
       - energy vs. pt


1. Create `env_bins.sh` by running `Bin_Splitter.C` and then source environment variables 
   with `source env_bins.sh`
   - (you should be in a bash shell)
   - you must source this file while in the directory spin
   - most scripts automatically source this file for you, but some of them might not
     - edit the script `Bin_Splitter.C` to control the binning


2. Build phi distributions using `loop_PhiDists` to run `PhiDists.C` on reduced files
   in redset/ to produce root files in phiset/
   - SEE "CUTS" SECTION FOR JET TYPE CUTS BEFORE RUNNING ANYTHING!
   - (alternatively, use `ploop_PhiDists` for a processor loop, i.e., submit jobs
     over all cores without using condor)
   - phi distributions for each run are written to the output file in object arrays, 
     which are named as `phi_s[spinbit]_g[eta bin]_p[pt bin]_e[en bin]`
     - the histograms in the object arrays have the run number appended to their names
   - `PhiDists3.C` implements an energy-dependent mass cut
     - first, using `diag.root`, run `MassCutter.C` and look at the mass cuts; the 
       cuts will be written out to `mass_cuts`, which is then read by `PhiDists3.C`
   - you need to supply `PhiDists3.C` with exclusion lists, which are lists of runs
     for each jet type (sph,pi0,thr); these runs were excluded AFTER `toa_add.C` was
     executed (see next step) to produce `wdist` pdfs, which are pt and energy distributions
     for each run; a QA was done using these pdfs to highlight pathological runs, and 
     thus if you're running things for the first time, you must run `loop_PhiDists` again
     after the QA


3. Merge the phiset files with `toa_add.C` (this is basically a sorted hadd for obj arrays)
   - this merges all the TObjArrays in the phiset file into `phiset/all.root`, where
     the entries of the TObjArrays are sorted by run number

4. Run an `asym_call` script (see Asymmetry Calculation Scripts section for further details)
   - `asym_call`
     - computes asymmetries for all runs considered; check three.png which 
       shows a comparison between single and double spin asymmetries
     - spin.root --> double and single spin asymmetries
       -  `en_dep*` = energy dependent asymmetry
       -  `pt_dep*` = pt dependent asymmetry
   - `asym_call_out`
     - computes asymmetry, filtering out "problem region" (14136002-14139001)
   - `asym_call_div`
     - computes asymmetry for various run regions, depending on how you've divided it;
       each run region is selected such that the number of pi0s is approximately the same
       - First, run `Count_pi0s.C` or `Count_pi0s_high_pt.C`, which counts the number
         of pi0s for whole pt range and pt>8, respectively; the output is a 
         root file called `pi0cnt.root` or `pi0cnt_high.root`
       - Then run `Read_pi0cnt.C`; MAKE SURE TO SPECIFY PROPER ARGUMENTS!
         - div is the number of run regions to consider
         - filename chooses which root file created by previous script
         - the output is `div.dat` which is read by `asym_call_div`
       - the root files and images are saved to the directory `study_runs` with
         the run numbers listed in the filename
   - `asym_call_run`
     - computes asymmetry for run regions; the argument you specify is the number
       of runs per region; files output to `study_runs/`
   - `asym_call_fill`
     - computes asymmetry for fill regions; the argument you specify is the number
       of fills per region; files output to `study_fill/`


Cuts For Event Classes
----------------------
- typical event in FMS is a pi0, but the code defines two other event classes
  - single photon events (denoted `sph`)
  - three or more photon events (denoted `thr`)
- the cuts are implemented in two scripts specifically:
  - `PhiDists3.C`
  - `Diagnostics.C`
- `Bin_Splitter.C` is only used to set kinematic bins for the analysis; low and high 
  boundaries of the full binning are overrided by cuts set in `PhiDist3.C` and `Diagnostics.C`
- the current main set of cuts are for pi0s (implemented in `PhiDists3.C` and `Diagnostics.C` 
  unless otherwise stated)
  - not in exclusion list (a run is in exclusion list if there was a hot tower)
  - TrigBits & 0x200 == 1
  - 2 Photon Event
  - Energy Sharing Z<0.8
  - Energy-dependent mass cut (determined from `diag.root` and thus this cut is NOT implemented
    in `Diagnostics.C`, but rather a naive cut |M - 135 MeV| <= 100 MeV is used instead for 
    producing various distributions in `diag.root`)
  - bXing not "kicked" (deviant in rellum), polarization was nonzero for this fill, and the 
    run was considered to have a "consistent" rellum measurement; this is a cut demanded 
    for all event classes (sph and thr)
  - hard kinematic cutoffs
    - 30 < E < 100 GeV for both Runs 12 & 13
    - 2.5 < pT < 10 GeV for run 12
    - 2.0 < pT < 10 GeV for run 13
    - geometry cuts 2.5 < eta < 4 and full azimuth defined in `Bin_Splitter.C` 
- for single photons and three or more photons classes, the cuts are:
  - not in exclusion list
  - 1 Photon event for sph, >2 photons for thr
  - bXing not kicked, polarization nonzero, consistent rellum for run 
  - no hard kinematic cutoffs (therefore kinematic bounds are set by `Bin_Splitter.C`)


Asymmetry Calculation Scripts
-----------------------------
 - Compute the double helicity asymmetry, using one of the following methods:

   1. Sum Method (also produces single spin asymmetries A_L blue & yellow)
      `Asym.C` produces `spin.root`, which sums all the phi distributions together
      (with their weights) and then computes asymmetry
      - TGraphErrors are kinematic dependence plots
      - numerator is weighted by 1/P, where P = product of beam polarizations

   2. Run Average Method 
      `Asym_run_by_run.C` produces `spin.root`, which contains asymmetry calculation results for
      each run; you can then run `DrawAverages.C` to draw average asymmetry over all runs
      for each kinematic bin; a kinematic-dependence plot called `kin_dep` will be drawn:
      - pT dependence drawn if `en_bins==1` and `eta_bins==1` and `pt_bins>1`
      - en dependence drawn if `pt_bins==1` and `eta_bins==1` and `en_bins>1`
      - `print_all` will draw the four phi distributions + asymmetry for each run and 
        kinematic bin; output pdfs are written to phiset (one pdf per bin, one page per run)


 - Draw asymmetries using `DrawThree.C`




TwoTr Tree Branches
-------------------

 - M12 -- diphoton mass
 - N12 -- number of photons (cf. Ntracks ??)
 - E12 -- diphoton energy
 - Eta -- pseudorapidity
 - Phi -- azimuth
 - Pt -- transverse momentum
 - Z -- energy sharing (Ehigh-Elow)/(Ehigh+Elow)
        - Z=1 if N12!=2
 - spin -- spinbit variable:
   - 0 = B-,Y-
   - 1 = B-,Y+
   - 2 = B+,Y-
   - 3 = B+,Y+
