#!/usr/bin/env python3

import pandas as pd
import sys
import matplotlib.pyplot as plt

df = pd.read_csv(sys.argv[1])

fig, axs = plt.subplots(4, figsize=(15,15))

axs[0].set_title('Message Latency (ms)')
axs[0].plot(df['latency_min'] / 1000000, label='latency_min')
axs[0].plot(df['latency_mean'] / 1000000, label='latency_mean')
axs[0].plot(df['latency_max'] / 1000000, label='latency_max')
axs[0].legend()

axs[1].set_title('CPU Usage (%)')
axs[1].plot(df['cpu_perc'] * 100)

axs[2].set_title('Resident Memory Usage (kb)')
axs[2].plot(df['max_rss'])

axs[3].set_title('Context Switches and Page Faults')
axs[3].plot(df['context_switches'], label='Context switches')
axs[3].plot(df['minor_page_faults'], label='Minor page faults')
axs[3].plot(df['major_page_faults'], label='Major page faults')
axs[3].legend()

plt.xlabel('time (sec)')
print('Saving plot to perf_test_result.png')
plt.savefig('perf_test_result.png')
