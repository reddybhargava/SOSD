import matplotlib.pyplot as plt 
import numpy as np 
import scipy.stats as st
import math 
import seaborn as sns
from math import log

filename = "data/books_200M_uint64_equality_lookups_repetitions_40K_temp"
# filename = "data/books_200M_uint64_equality_lookups_zipf_100K_1_temp"

# with open("data/books_200M_uint64", "rb") as f:
#     data = [int(i) for i in f.readlines()]
# data_log = [log(i) for i in data]
dataset = np.fromfile("data/books_200M_uint64", dtype=np.uint64)

# print(dataset[:100], len(dataset))


with open(filename) as f:
    data = [int(i) for i in f.readlines()]
data_log = [log(i) for i in data]

d1 = data[::2]
# d2 = data[1::2]

# c1 = len(list(filter(lambda x : x in dataset, d1)))
# c2 = len(list(filter(lambda x : x in dataset, d2)))


# # print(data[:10], len(data))
# # print(d1[:100], len(d1))
# # print('%2E' % min(d1[:100]), '%2E' % max(d1[:100]))
# # print(d2[:10], len(d2), c2)
# fig = plt.figure(figsize = (20, 10))
# l = list(filter(lambda x : x > 1e18, d1[:10000]))[:500]
# print(len(np.unique(l)))
# plt.xlim(0,len(l))
# # plt.ylim(0,200)
# plt.xticks(fontsize=16)
# plt.yticks(fontsize=16)
# plt.xlabel("Index", fontsize=18)
# plt.ylabel("Lookup Key", fontsize=18)
# plt.title("books_200M_uint64_40K_lookup_keys_random_repetitions", fontsize=22)
# plt.bar(list(range(len(l))), l, width=0.2)
# # g = sns.barplot(x = list(range(len(l))), y = l)
# # g.set_xticklabels(labels=list(range(len(l))), rotation=90)
# plt.savefig(f"graphs/final1.png")

# Plot 
fig = plt.figure(figsize = (20, 10))
unique, counts = np.unique(d1, return_counts=True)
keys_sorted = sorted(list(zip(unique,counts)), key=lambda x:x[1], reverse=True)[:100]
unique, counts = [i[0] for i in keys_sorted], [i[1] for i in keys_sorted]
print(dict(zip(unique, counts)))
g = sns.barplot(x = unique, y = counts)
# g.set_xticklabels(labels=unique, rotation=70)
plt.xticks(fontsize=0)
plt.yticks(fontsize=14)
plt.xlabel("Lookup Key", fontsize=16)
plt.ylabel("Frequency", fontsize=16)
plt.title("books_200M_uint64_40K_random_repetitions", fontsize=22)
plt.savefig(f"graphs/final2.png")
