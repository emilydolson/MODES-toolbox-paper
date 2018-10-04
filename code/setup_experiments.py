import os

defaults = {"N":20, "K":3, "POP_SIZE":200, "MUT_RATE":.05, "TOURNAMENT_SIZE":15, "SELECTION":0, "CHANGE_RATE":100000, "MAX_GENS": 5000}
conditions = [{},{"K":10}, {"N":100, "MUT_RATE":.01}, {"MUT_RATE":.005}, {"MUT_RATE": .1}, {"POP_SIZE":20}, {"POP_SIZE":2000}, {"SELECTION":1}, {"CHANGE_RATE":500}]

seed = 0

for condition in conditions:
    print(condition)
    command = ["./nk_oee -SEED", seed]
    dir_name = []
    for var in defaults:
        command.append("-"+var)
        dir_name.append("".join(var.split("_"))) # Underscores in variable naems will screw up parsing later
        if var in condition:
            command.append(str(condition[var]))
            dir_name.append(str(condition[var]))
        else:
            command.append(str(defaults[var]))
            dir_name.append(str(defaults[var]))
    
    str_dir_name = "_".join(dir_name)
    os.mkdir(str_dir_name)
    for i in range(30):
        seed += 1
        command[1] = str(seed)
        print(command)
        os.mkdir(str_dir_name+"/"+str(i))
        with open(str_dir_name+"/"+str(i)+"/command.sh", "w") as infile:
            infile.write(" ".join(command))