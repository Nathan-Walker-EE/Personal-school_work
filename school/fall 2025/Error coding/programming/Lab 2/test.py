import numpy as np

LUT = [0, 1]
p = .75
probs = [p, 1-p]

for i in range(10):
    print(int(np.random.choice(LUT, size=1, p=probs)))