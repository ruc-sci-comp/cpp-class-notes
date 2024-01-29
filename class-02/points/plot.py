import sys

import hvplot.pandas
import matplotlib.pyplot as plt
import pandas as pd


data = pd.read_csv(sys.stdin, names=['x', 'y'])
viz = data.plot('x', 'y', grid=True, legend=False, title='Cosine', style='-o')
plt.savefig('cosine.png')
