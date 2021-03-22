# take downloaded datasets and cleans them into standard edgelist
# save them under a standard name edgelist-NAME-NODES-EDGES.txt

import sys
import os

# ----------------------------------------
# --------- formatting functions ---------
# ----------------------------------------
def outputFormat(u, v):
    return "{} {}\n".format(u, v)

def kmg(x): # kilo, mega, giga, tera suffixe
    if x < 10**4: return str(x)
    elif x < 10**6: return str(x//10**3) + "k"
    elif x < 10**9: return str(x//10**6) + "M"
    elif x < 10**12: return str(x//10**9) + "G"
    else: return str(x//10**12) +"T"

def fileName(n=0, m=0): return "edgelist-{}-{}-{}.txt".format(sys.argv[1], kmg(n), kmg(m))
def createFile(output, n=0, m=0):
    file = fileName(n, m)
    if n == 0:
        ff = open(file, "a")
    else:
        print("File {} created with {} nodes and {} edges.".format(file, n, m))
        ff = open(file, "w")
    ff.write(output)
    ff.close()

def renumber(dict, key):
    if key not in dict.keys():
        dict[key] = len(dict)
    return dict[key]

# ----------------------------------------
# --- specific procedure for datasets ----
# ----------------------------------------

# epinion: http://snap.stanford.edu/data/soc-Epinions1.html
if sys.argv[1] == "epinion":
    file = "dl/soc-Epinions1.txt"
    f = open(file)
    l = "#"
    output = ""

    n, m = -1, 0
    while len(l):
        if l[0] != "#":
            nums = l.split("\t")
            u, v = int(nums[0]), int(nums[1])
            output += outputFormat(u, v)
            m += 1
            n = max([n, u, v])
            if m % 100000 == 0: print(m)
        l = f.readline()
    f.close()
    createFile(output, n, m)

# pokec: http://snap.stanford.edu/data/soc-Pokec.html
elif sys.argv[1] == "pokec":
    file = "dl/soc-pokec-relationships.txt"
    f = open(file)
    l = f.readline()
    output = ""

    n, m = -1, 0
    while len(l):
        nums = l.split("\t")
        u, v = int(nums[0])-1, int(nums[1])-1
        output += outputFormat(u, v)
        m += 1
        n = max([n, u, v])
        if m % 1000000 == 0: print(m)
        l = f.readline()
    f.close()
    createFile(output, n, m)

# flickr: http://socialnetworks.mpi-sws.org/data-wosn2008.html
elif sys.argv[1] == "flickr":
    file = "dl/out.flickr-growth"
    f = open(file)
    l = "%"
    output = ""

    n, m = -1, 0
    while len(l):
        if l[0] != "%":
            nums = l.split(" ")
            u, v = int(nums[0])-1, int(nums[1])-1
            output += outputFormat(u, v)
            m += 1
            n = max([n, u, v])
            if m % 1000000 == 0: print(m)
        l = f.readline()
    f.close()

    createFile(output, n, m)

# livejournal: http://snap.stanford.edu/data/soc-LiveJournal1.html
elif sys.argv[1] == "livejournal":
    file = "dl/soc-LiveJournal1.txt"
    f = open(file)
    l = "#"
    output = ""

    n, m = -1, 0
    while len(l):
        if l[0] != "#":
            nums = l.split("\t")
            u, v = int(nums[0]), int(nums[1])
            output += outputFormat(u, v)
            m += 1
            n = max([n, u, v])
            if m % 1000000 == 0: print(m)
        l = f.readline()
    f.close()
    createFile(output, n, m)

# wiki: http://konect.cc/networks/wikipedia_link_en/
elif sys.argv[1] == "wiki":
    file = "dl/out.wikipedia_link_en"
    f = open(file)
    l = "%"
    output = ""

    n, m = -1, 0
    while len(l):
        if l[0] != "%":
            nums = l.split("\t")
            u, v = int(nums[0])-1, int(nums[1])-1
            output += outputFormat(u, v)
            m += 1
            n = max([n, u, v])
            if m % 1000000 == 0:
                print(m)
                createFile(output)
                output = ""
        l = f.readline()
    f.close()
    createFile(output)
    os.rename(fileName(), fileName(n, m))

# gplus: http://gonglab.pratt.duke.edu/google-dataset
elif sys.argv[1] == "gplus":
    file = "dl/gplus/gplus/imc12/direct_social_structure.txt"
    f = open(file)
    l = f.readline()
    output = ""
    n, m = -1, 0
    while len(l):
        nums = l.split(" ") # format: u v t, with t first timestamp of edge
        u, v = int(nums[0]), int(nums[1])
        output += outputFormat(u, v)
        m += 1
        n = max([n, u, v])
        if m % 1000000 == 0:
            print(m)
            createFile(output)
            output = ""
        l = f.readline()
    f.close()
    createFile(output)
    os.rename(fileName(), fileName(n, m))

# pldarc: http://webdatacommons.org/hyperlinkgraph/2012-08/download.html
elif sys.argv[1] == "pldarc":
    file = "dl/pld-arc"
    f = open(file)
    l = f.readline()
    output = ""
    n, m = -1, 0
    while len(l):
        nums = l.split("\t")
        u, v = int(nums[0]), int(nums[1])
        output += outputFormat(u, v)
        m += 1
        n = max([n, u, v])
        if m % 1000000 == 0:
            print(m)
            createFile(output)
            output = ""
        l = f.readline()
    f.close()
    createFile(output)
    os.rename(fileName(), fileName(n, m))

# twitter: http://an.kaist.ac.kr/traces/WWW2010.html
elif sys.argv[1] == "twitter":
    file = "dl/twitter_rv.net"
    f = open(file)
    l = f.readline()
    output = ""
    n, m = -1, 0
    while len(l):
        nums = l.split("\t")
        u, v = int(nums[0])-12, int(nums[1])-12
        output += outputFormat(u, v)
        m += 1
        n = max([n, u, v])
        if m % 1000000 == 0:
            print(m)
            createFile(output)
            output = ""
        l = f.readline()
    f.close()
    createFile(output)
    os.rename(fileName(), fileName(n, m))

# sdarc: http://webdatacommons.org/hyperlinkgraph/2012-08/download.html
elif sys.argv[1] == "sdarc":
    file = "dl/sd1-arc"
    f = open(file)
    l = f.readline()
    output = ""
    n, m = -1, 0
    while len(l):
        nums = l.split("\t")
        u, v = int(nums[0])-1, int(nums[1])-1
        output += outputFormat(u, v)
        m += 1
        n = max([n, u, v])
        if m % 1000000 == 0:
            print(m)
            createFile(output)
            output = ""
        l = f.readline()
    f.close()
    createFile(output)
    os.rename(fileName(), fileName(n, m))
