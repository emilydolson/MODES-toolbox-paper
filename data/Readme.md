# Data column description

Note that these files contain some additional data that didn't make it into the paper because we weren't able to get a complete set of data for that experimental condition and/or didn't have space in the paper to discuss it.

## avida_data.csv

- Unlabeled first column: row label
- generation: the average generation of the population for the data in this row
- change: the value of the change metric at this time point
- novelty: the value of the novelty metric at this time point
- ecology: the value of the ecology metric at this time point
- complexity: the value of the complexity metric at this time point
- seed: the random seed used for this run (useful for perfectly reproducing results)
- environment: the name of the environment this run was performed in
- population_size: the population size used for this run
- filter: the filter length (t) used for this run (i.e. how many generations must a lineage survive to count as persistent)
- res: the resolution that MODES metrics were measured at (in generations)
- fixedlength: boolean indicating whether the genome was allowed to change in this run (the paper only includes data where fixedlength = false)
- update: the update (Avida's internal measurement of time) at this time point
- uniquephenotypetask: number of phenotypes in the population, evaluated by the set of tasks they perform
- shannondiv: Shannon diversity of phenotypes in the population, evaluated by the set of tasks they perform
- uniquephenotypecount: number of phenotypes in the population, evaluated by the number of times they do each task
- avgshannondiversity: Shannon diversity of phenotypes in the population, evaluated by the the number of times they do each task

## nk_data.csv

Lowercase variable names are things that were measured over the course of the run while capitals indicate parameters that are set at the beginning and not changed.

- generation: the generation of the population for the data in this row
- CHANGE_RATE: number of generations between environmental changes (by default, this is set to be longer than the experiment)
- CHANGE_TYPE: 0 = complete (i.e. a completely new landscape is generated), 1 = oscillating (i.e. we switch back and forth between two environments)
- FILTER_LENGTH: the filter length (t) used for this run (i.e. how many generations must a lineage survive to count as persistent)
- K: the value of K (epistasis) used for the NK landscape
- MAX_GENS: how many generations to run the experiment for
- MODES_RESOLUTION: the resolution that MODES metrics were measured at (in generations)
- MUT_RATE: per-site mutation probability
- N: number of bits in the bitstring (N term for the NK landscape)
- POP_SIZE: the population size used for this run
- SEED: the random seed used for this run (useful for perfectly reproducing results)
- SELECTION: selection scheme - 0 = Tournament selection, 1 = Fitness sharing
- SHARING_ALPHA: alpha value for determining the shape of the sharing function
- SHARING_THRESHOLD: sharing threshold for fitness sharing (how similar do two individuals need to be to compete?)
- TOURNAMENT_SIZE: Size of tournament for tournament selection
- change: the value of the change metric at this time point
- novelty: the value of the novelty metric at this time point
- ecology: the value of the ecology metric at this time point
- complexity: the value of the complexity metric at this time point
- treatment: The experimental treatment (this is just a conglomeration of all parameter values for convenience)


