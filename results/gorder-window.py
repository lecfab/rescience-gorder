# Take time measurement with different window sizes for gorder
# Plot duration wrt window size to help tuning parameter w

from gordermain import *

folder = relative("gorder-window/")

dataset = "flickr"
algo = "pr"

data = []
wmax = 21 # test w ranging from 1 to 2^wmax
for logw in range(wmax):
    w = 2**logw
    loop=0
    while loop < 100: # each experiment was repeated 100 times for stability
        file = "{}window-{}-{}-{}.{}.txt".format(folder, dataset, algo, w, loop)
        loop+=1
        for l in through_file(file):
            algo_name, time = l[:-1].split("\t")[:2]
            if algo_name == algo:
                data.append((w, int(time) / 1000)) # ms to s

quartile = lambda q : [np.percentile([t for w,t in data if w==2**x], q) for x in range(wmax)]
median, high, low = quartile(50), quartile(95), quartile(5)


fig, ax = plt.subplots(figsize=(4,3))
# ax.set_title('Duration of {} on ${}$ dataset for various window sizes'.format(algo.capitalize(), dataset))
ax.set_xlabel('Window size $w$ for Gorder')
ax.set_xlim(-.5, wmax-.5)
ax.set_xticks(np.arange(0,wmax,2))
ax.set_xticklabels([2**x if x<7 else "$2^{"+str(x)+"}$" for x in range(0,wmax,2)])#, rotation=45)

ax.set_ylabel('Duration of {} on ${}$ (sec)'.format(algo.upper(), dataset))
ax.set_ylim([min([t for w,t in data])-.1,max([t for w,t in data])])

plt.plot(median)
plt.fill_between(np.arange(len(median)), low, high, alpha=.4)

print([(2**i,median[i]) for i in range(len(median))])
save_img("{}tune-w-{}-{}.pdf".format(folder, dataset, algo))
