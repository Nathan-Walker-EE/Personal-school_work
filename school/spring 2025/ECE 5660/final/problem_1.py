import numpy as np

def branch_metric(vk, f_array, I_array):
    vkhat = (f_array[0]*I_array[0] + f_array[1]*I_array[1] + f_array[2]*I_array[2])
    print('vkhat = ', vkhat)
    mu = vk - vkhat
    mu = mu**2
    print('mu = ', mu)
    return mu

metrics = [13.1, 12.2, 14.3, 14.1]
vk = -1.9
f = [0.7, -0.3, 0.2]

temp1 = branch_metric(vk, f, [1, 1, 1])
temp2 = branch_metric(vk, f, [1, 1, -1])
M1 = np.min([temp1 + metrics[0], temp2 + metrics[1]])
print(M1)

temp1 = branch_metric(vk, f, [1, -1, 1])
temp2 = branch_metric(vk, f, [1, -1, -1])
M2 = np.min([temp1 + metrics[2], temp2 + metrics[3]])
print(M2)

temp1 = branch_metric(vk, f, [-1, 1, 1])
temp2 = branch_metric(vk, f, [-1, 1, -1])
M3 = np.min([temp1 + metrics[0], temp2 + metrics[1]])
print(M3)

temp1 = branch_metric(vk, f, [-1, -1, 1])
temp2 = branch_metric(vk, f, [-1, -1, -1])
M4 = np.min([temp1 + metrics[2], temp2 + metrics[3]])
print(M4)