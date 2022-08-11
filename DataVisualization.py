#%%

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

#%%

''' 2D simulation visualization '''

fig = plt.figure()
ax = fig.add_subplot(projection='3d')
for i in range (0,1000+1):
    plt.cla()
    ca = np.array(pd.read_csv('D:/google drive/cpp/2DMacroBalloonModelData/DiffusionAndReaction '+str(i)+'.txt'))
    ax.scatter(ca[:,0], ca[:,1], ca[:,2], marker='o')
    ax.set_zlim(0,2.5)
    plt.savefig('D:/google drive/cpp/2DMacroBalloonModelData/DiffusionAndReaction '+str(i)+'.png')

#%%

''' 1D simulation visualization '''

for i in range (0,1000+1):
    plt.cla()
    ca = np.array(pd.read_csv('D:/google drive/cpp/1DMacroBalloonModelData/DiffusionAndReaction '+str(i)+'.txt'))
    plt.scatter(np.arange(1,ca.shape[0]+1), ca, marker='o')
    plt.xlim(0,ca.shape[0]+1)
    plt.ylim(0,2.5)
    plt.savefig('D:/google drive/cpp/1DMacroBalloonModelData/DiffusionAndReaction '+str(i)+'.png')
