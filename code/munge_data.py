import glob
import pandas as pd

all_data = []

for path in glob.glob("TOURNAMENTSIZE*/*"):
   df = pd.read_csv(path+"/oee.csv", index_col="generation")
   local_data = {}

   with open(path + "/run.log") as run_log_file:
       for line in run_log_file:
           if line.startswith("0"):
               break
           elif not line.startswith("set"):
                continue
           line = line.split()
           local_data[line[1]] = line[2]

   for val in local_data:
        df[val] = local_data[val]

   all_data.append(df)

res = pd.concat(all_data)

res.to_csv("../data/nk_data.csv")