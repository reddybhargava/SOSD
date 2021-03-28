import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from math import log

# z = np.random.zipf(1.1, 100000)
# unique, counts = np.unique(z, return_counts=True)
# # print(dict(zip(unique,counts)))
# # plt.hist(z)
# plt.bar(unique, counts)
# plt.savefig("graphs/plot.png")

datasets = [
    # "books_200M_uint32",
    "books_200M_uint64",
    # "fb_200M_uint64",
    # "osm_cellids_200M_uint64",
    # "wiki_ts_200M_uint64"
]

def generateLookups(lookups, lookups_size):
    z = np.random.zipf(2, lookups)
    if max(z) > 200e6:
        # z_norm = [np.interp(key,min(z),max(z),1,200e6) for key in z]
        # z_norm = [translate(key,min(z),max(z),1,200e6) for key in z]
        # print(min(z_norm), max(z_norm))
        print(f"{lookups} can't")
        return
    print(min(z), max(z), len(np.unique(z)))
    
    for dataset in datasets:
        if 'uint64' in dataset:
            keys = np.fromfile(f"data/{dataset}", dtype=np.uint64)[1:]
        elif 'uint32' in dataset:
            keys = np.fromfile(f"data/{dataset}", dtype=np.uint32)[1:]
        
        print("keys: ", len(keys))
        lookup_keys = [keys[index] for index in z]
        # unique, counts = np.unique(lookup_keys, return_counts=True)
        # vals = sorted(list(zip(unique, counts)), key=lambda x : x[0])
        # unique, counts = [i[0] for i in vals], [log(i[1]+1) for i in vals]
        # print(dict(zip(unique, counts)))

        fig = plt.figure(figsize = (20, 10))
        l = list(filter(lambda x : x < 200, lookup_keys[:1000]))
        print(len(np.unique(l)))
        plt.xlim(0,len(l))
        plt.ylim(0,200)
        plt.xticks(fontsize=16)
        plt.yticks(fontsize=16)
        plt.xlabel("Index", fontsize=18)
        plt.ylabel("Lookup Key", fontsize=18)
        plt.title("books_200M_uint64_10K_lookup_keys", fontsize=22)
        # plt.bar(range(len(l)), l, width=0.2)
        # print(len(l))
        g = sns.barplot(x = list(range(len(l))), y = l)
        g.set_xticklabels(labels=list(range(len(l))), rotation=90)
        plt.savefig(f"graphs/{dataset}_{lookups_size}")

        # lookup_keys.insert(0, lookups)
        # if 'uint64' in dataset:
        #     lookup_keys = np.array(lookup_keys, dtype=np.uint64)
        # elif 'uint32' in dataset:
        #     lookup_keys = np.array(lookup_keys, dtype=np.uint32)
        
    

        # lookup_keys.tofile(f"data/{dataset}_equality_lookups_zipf_{lookups_size}")

        # # Plot 
        # g = sns.barplot(x = unique, y = counts)
        # g.set_xticklabels(labels=unique, rotation=70)
        # plt.xticks(fontsize=16)
        # plt.yticks(fontsize=16)
        # plt.xlabel("Lookup Key", fontsize=20)
        # plt.ylabel("Frequency", fontsize=20)
        # plt.title("books_200M_uint64_10K_zipf_distribution", fontsize=22)
        # plt.savefig(f"graphs/{dataset}_{lookups_size}")



generateLookups(10000, "10K")
# generateLookups(50000, "50K")
# generateLookups(100000, "100K")
# # generateLookups(1000000)
