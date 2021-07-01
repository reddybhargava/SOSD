import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import copy

plt.rc('xtick', labelsize=15)

def autolabel(rects):
    """Attach a text label above each bar in *rects*, displaying its height."""
    for rect in rects:
        width = rect.get_width()
        ax.annotate('{}'.format(int(width)),
                    xy=(rect.get_y() + rect.get_height() / 2, width),
                    xytext=(3, 0),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='right', va='center')

combs = [
    "_100K_1.txt",
    "_100K_2.txt",
    "_100K_3.txt",
    "_500K_1.txt",
    "_500K_2.txt",
    "_500K_3.txt"
    ]

folders = ["results"]

files_prefix = [
    "books_200M_uint32",
    "books_200M_uint64",
    "fb_200M_uint64",
    "osm_cellids_200M_uint64"
    ]
files_suffix = [
                "_cold_zipf_results",
#                 "_normal_zipf_results"
                ]

files = [prefix + suffix for prefix in files_prefix for suffix in files_suffix]
print(files)


index_list = [
              "PGM",
              "PGMCacheL",
              "PGMCacheLO",
              "PGMCacheR",
              "PGMCacheRO"
             ]

data = {index:list() for index in index_list}
data["Dataset"] = []
temp_data = copy.deepcopy(data)

for comb in combs:
    for folder in folders:
        for file in files:
            f_obj = open(folder + "/" + file + comb, 'r')
            data["Dataset"].append(file.replace("_200M", "").replace("_cold_zipf_results", "").replace(comb, ""))

            inf = float('inf')
            best_times = {index:inf for index in index_list}


            while True:
                line = f_obj.readline()
                if not line:
                    break

                stats = []
                line = line.split()

                if line[0] == "RESULT:":
                    if line[1] in index_list[1: len(index_list)]:
                        stats = line[4].split(',')
                        stats[0] = line[1]
                    else:
                        stats = line[1].split(',')

                    if stats[0] in best_times:
                        if float(stats[2]) < best_times[stats[0]]:
                            best_times[stats[0]] = float(stats[2])

            for index in index_list:
                data[index].append(best_times[index])
        print(data)

        x = np.arange(len(data["Dataset"]))
        width = 0.075

        pos = []
        pos_gen = float(len(index_list) / 2) - 0.5
        for i in range(len(index_list)):
            pos.append(pos_gen)
            pos_gen -= 0.5
        
        colors1 = ["#104E8B", "#a0d2eb", "#3d7c47", "#a28089", "#8458B3", "#d0bdf4"]
        colors2 = ["#104E8B", "#c2dde6", "#431c5d", "#e05915", "#cdd422", "#a28089"]
        all_rects = []
        fig, ax = plt.subplots(figsize=(15, 10))
        for i in range(len(index_list)):
            all_rects.append(ax.barh(x + width * pos[i], data[index_list[i]], width, label=index_list[i], color=colors1[i]))

        ax.spines['right'].set_visible(False)
        ax.spines['top'].set_visible(False)
        ax.spines['bottom'].set_visible(False)
        ax.set_axisbelow(True)
        ax.xaxis.grid(color='gray', linestyle='dashed')

        ax.set_title('Lookup Latency - Zipf ' + comb[1:][:-4], fontsize=30)
        ax.set_xlabel('Time', fontsize = 20)
        ax.set_yticks(x)
        ax.set_yticklabels(data["Dataset"], fontsize=20)
        # ax.set_yticklabels(ax.xaxis.get_majorticklabels(), rotation=0, fontsize = 15)
        ax.legend(loc="upper left", prop={'size': 16}, bbox_to_anchor=(1.05, 1))

        plt.savefig("saved_graphs/" + comb[:-4][1:] + ".png", bbox_inches='tight')
        data = copy.deepcopy(temp_data)
