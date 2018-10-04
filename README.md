# The MODES toolbox: Measurements of Open-ended Dynamics in Evolving Systems
[![DOI](https://zenodo.org/badge/151119818.svg)](https://zenodo.org/badge/latestdoi/151119818)

This repository contains code, analysis, and data for the paper 
"The MODES toolbox: Measurements of Open-ended Dynamics in Evolving Systems"
by [Emily Dolson](emilyldolson.com), [Anya Vostinar](https://vostinar.sites.grinnell.edu/), 
[Michael Wiser](https://msu.edu/~mwiser/) and [Charles Ofria](ofria.com). In this paper, we present 
a toolbox of measurements for quantifying hallmarks of open-ended evolution and test them in two systems:
NK Landscapes and Avida.

## Links

- Preliminary versions of the ideas in this paper are discussed in [this blog post](https://thewinnower.com/papers/2309-what-s-holding-artificial-life-back-from-open-ended-evolution) 
and [this paper presented at the second workshop on Open-ended evolution](http://www.tim-taylor.com/oee2/abstracts/vostinar-oee2-submission.pdf).

- Supplemental graphs are available here: [https://emilydolson.github.io/MODES-toolbox-paper/analysis/OEE.html](https://emilydolson.github.io/MODES-toolbox-paper/analysis/OEE.html)

- The version of Avida used for these experiments is here: [https://github.com/emilydolson/avida-empirical](https://github.com/emilydolson/avida-empirical)

- This NK Landscape code and the MODES toolbox itself are inside the [Empirical library](https://github.com/devosoft/Empirical).
The precise version of Empirical used in this paper is here: [https://github.com/emilydolson/Empirical/tree/OEE_metrics_paper_submission](https://github.com/emilydolson/Empirical/tree/OEE_metrics_paper_submission).

## Contents of this repository

- **Paper**: This where all of the latex code for the paper itself lives
- **Data**: This directory contains to csv files, one containing the nk landscape data (`nk_data.csv`) and one containing the Avida data (`avida_data.csv`)
- **Analysis**: This directory contains an R-markdown file with all of the analysis code (and a little commentary), a version of the R-markdown file rendered to html (linked to above as supplmentary material), and a script for making flat violin plots (required for the rain cloud plots).
- **Figs**: This directory contains the figures generated by the code in `analysis` so that the code in `paper` can access them.