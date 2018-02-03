import matplotlib.pyplot as plt
import numpy as np

with open("output.txt","r") as f:
    inpt = f.read().split()
for i in range(len(inpt)):
    inpt[i] = float(inpt[i])

x = np.linspace(-15, 15, 100)

plt.plot([inpt[2*i+1] for i in range( (len(inpt) - 6) / 2)], [inpt[2*i+2] for i in range( (len(inpt) - 6) / 2 )],'ro')
plt.plot(x, inpt[-5]*x**4 + inpt[-4]*x**3 + inpt[-3]*x**2 + inpt[-2]*x + inpt[-1], label = 'graph')
plt.xlabel('x label')
plt.ylabel('y label')

plt.title("Simple Plot")

plt.legend()

plt.show()
