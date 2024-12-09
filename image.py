import matplotlib as mp
import matplotlib.pyplot as plt
import numpy as np

from matplotlib import cm
from matplotlib.ticker import LinearLocator

fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
fig, ax2 = plt.subplots(subplot_kw={"projection": "3d"})
# Import data.
arr = np.loadtxt("weather_simulation_parallel.csv", delimiter=",")
x_list = []
z_list = []
y_list = []
for row in arr:
    x_list.append(row[::3])
    y_list.append(row[2::3])
    z_list.append(row[1::3])
x_list = np.array(x_list)
y_list = np.array(y_list)
z_list = np.array(z_list)



# Make data.
X = np.arange(0, 100, 1)
Y = np.arange(0, 100, 1)
R = np.sqrt(X**2 + Y**2)
X, Y = np.meshgrid(X, Y)
X1 = x_list# + R
Y1 = y_list# + R
Z = z_list# + R

# Plot the surface.
norm1 = mp.colors.Normalize(vmin=np.min(216.15),vmax=np.max(330.15))
norm2 = mp.colors.Normalize(vmin=np.min(Y1),vmax=np.max(Y1))
scamap1 = plt.cm.ScalarMappable(cmap='coolwarm',norm=norm1)
scamap2 = plt.cm.ScalarMappable(cmap='gray',norm=norm2)
scamap1.set_array([])
scamap2.set_array([])
fcolours1 = scamap1.to_rgba(X1)
fcolours2 = scamap2.to_rgba(Y1)
surf = ax.plot_surface(X, Y, Z, facecolors=fcolours1, linewidth=0, antialiased=False)
surf2 = ax2.plot_surface(X, Y, Z, facecolors=fcolours2, linewidth=0, antialiased=False)

# Customize the x axis.
ax.set_xlim(0, 100)

ax.set_xlabel("Temperature")
# Customize the y axis.
ax.set_ylim(0, 100)

ax.set_ylabel("Humidity")
# Customize the z axis.
ax.set_zlim(Z.min(), Z.max())
ax.zaxis.set_major_locator(LinearLocator(3))
ax.set_zlabel("Pressure")
# A StrMethodFormatter is used automatically
ax.zaxis.set_major_formatter('{x:.02f}')
fig.colorbar(scamap1, shrink=0.5, aspect=5, ax=plt.gca())
ax2.set_xlim(0, 100)

ax2.set_xlabel("Temperature")
# Customize the y axis.
ax2.set_ylim(0, 100)

ax2.set_ylabel("Humidity")
# Customize the z axis.
ax2.set_zlim(Z.min(), Z.max())
ax2.zaxis.set_major_locator(LinearLocator(3))
ax2.set_zlabel("Pressure")
# A StrMethodFormatter is used automatically
ax2.zaxis.set_major_formatter('{x:.02f}')

# Add a color bar which maps values to colors.

fig.colorbar(scamap2, shrink=0.5, aspect=5, ax=plt.gca())
plt.show()