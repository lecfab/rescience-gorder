# Shared tools for Gorder replication

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import sys
import os
import re

# ----------------------------------------
# ---- define datasets, orders, algos ----
# ----------------------------------------
datasets_stat = {
    "epinion": {"n": 75879, "m": 508837},
    "pokec": {"n": 1632803, "m": 30622564},
    "flickr": {"n": 2302925, "m": 33140017},
    "livejournal": {"n": 4847571, "m": 68993773},
    "wiki": {"n": 10248152, "m": 229101833},
    "gplus": {"n": 28943739, "m": 462994069},
    "pldarc": {"n": 39497204, "m": 623056313},
    "twitter": {"n": 41652230, "m": 1468365182},
    "sdarc": {"n": 82924686, "m": 1937489265}
}
datasets = list(datasets_stat.keys())
D = len(datasets)

order_names = {  "original": "Original",
            "rand": "Random",
            "minla": "MinLA",
            "minloga": "MinLogA",
            "rcm": "RCM",
            "deg-": "InDegSort",
            "dfs": "ChDFS",
            "slashburn": "SlashBurn",
            "ldg": "LDG",
            "gorder": "Gorder" }
orders = list(order_names.keys())
O = len(orders)

algo_names = {  "nq": "NQ",
                "bfs": "BFS",
                "dfs": "DFS",
                "tarjan": "SCC",
                "bellman": "SP",
                "pr": "PR",
                "dominatingset": "DS",
                "kcore": "Kcore",
                "diameter": "Diam"
                 }
algos = list(algo_names.keys())
A = len(algos)

# ----------------------------------------
# ---- file opening, reading, saving -----
# ----------------------------------------
def relative(relFolder):
    return os.path.join(os.path.dirname(__file__), relFolder)

def r____():
    folder = relative("r____/")
    if len(sys.argv) > 1:
        folder = relative("r{}/".format(sys.argv[1]))
    while not os.path.isdir(folder):
        folder_n = input("ID of results folder (eg 1000 for folder r1000/)? ")
        folder = relative("r{}/".format(folder_n))
    return folder

def through_file(file, silent=False):
    try:
        with open(file) as f:
            for l in f:
                yield l
            f.close()

    except IOError:
        if not silent: print("Error with", file)
        raise IOError("Error with "+ file)
    except FileNotFoundError:
        if not silent: print("Unknown file", file)
        raise IOError("Unknown file "+ file)
    except GeneratorExit:
        pass
    except:
        raise Exception("error " + str(sys.exc_info()[0]))

def save_img(img_file):
    plt.savefig(img_file, dpi=300, bbox_inches='tight')
    print("Image has been saved in", img_file)

def extract_perf(file, perf):
    specs = []
    for spec in perf:
        try:
            for l in through_file(file):
                if spec not in l: continue
                l = l.split("\n")[0]
                l = re.sub(r'[^0-9,]+{}.*'.format(spec), '', l)
                l = re.sub(r',','', l)
                specs.append(int(l)) # print(spec, int(l))
                break
        except: pass
    return specs
