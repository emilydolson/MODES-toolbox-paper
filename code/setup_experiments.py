import os


#defaults = {"N":20, "K":3, "POP_SIZE":200, "MUT_RATE":.05, "TOURNAMENT_SIZE":2, "SELECTION":0, "CHANGE_RATE":100000, "MAX_GENS": 5000, "FILTER_LENGTH":50}
defaults = {"N":20, "K":3, "POP_SIZE":200, "MUT_RATE":.05, "TOURNAMENT_SIZE":2, "SELECTION":0, "CHANGE_RATE":100000, "MAX_GENS": 5000, "FILTER_LENGTH":"POP_SIZE"}
conditions = [{},{"K":10}, {"N":100, "MUT_RATE":.01}, {"MUT_RATE":.005}, {"MUT_RATE": .1}, {"POP_SIZE":20}, {"POP_SIZE":2000}, {"SELECTION":1}, {"SELECTION":1, "FILTER_LENGTH":1000}, {"CHANGE_RATE":500}, {"CHANGE_RATE":500, "CHANGE_TYPE":1}]

seed = 0

for condition in conditions:
    print(condition)
    command = ["./nk_oee -MODES_RESOLUTION 10 -SEED", seed]
    dir_name = []
    for var in defaults:
        if var not in condition:
            condition[var] = defaults[var]

    for var in condition:
        while condition[var] in condition:
            condition[var] = condition[condition[var]]

        command.append("-"+var)
        dir_name.append("".join(var.split("_"))) # Underscores in variable names will screw up parsing later
        val = str(condition[var])
        command.append(val)
        dir_name.append(val)

    
    str_dir_name = "_".join(dir_name)
    if not os.path.exists(str_dir_name):
        os.mkdir(str_dir_name)
        
    for i in range(30):
        if os.path.exists(str_dir_name+"/"+str(i)+"/command.sh"):
            continue
        seed += 1
        command[1] = str(seed)
        print(command)
        os.mkdir(str_dir_name+"/"+str(i))
        with open(str_dir_name+"/"+str(i)+"/command.sh", "w") as infile:
            infile.write(" ".join(command))