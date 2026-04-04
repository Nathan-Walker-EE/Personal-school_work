import numpy as np
from scipy.special import erfc

def Q_function(x):
    return 0.5 * erfc(x/np.sqrt(2))

def compute_all_Q_terms(K1_list, K2_list):
    Q_values = np.array([Q_function(np.sqrt(K2)) for K2 in K2_list])
    terms = np.array(K1_list) * Q_values
    return terms, np.sum(terms)

d = 0.057
sigman2 = 0.0035
sigman = np.sqrt(sigman2)

corner = Q_function(d/(2*sigman)) + Q_function(d/(sigman)) 
- Q_function(d/(2*sigman))*Q_function(d/(sigman))
interior = Q_function(d/(2*sigman)) + 2*Q_function(d/(sigman)) 
- 2*Q_function(d/(2*sigman))*Q_function(d/(sigman))
overall = 4*corner + 4*interior
overall = overall/8
bit_error = overall/3

print("sigma = ", sigman, "corner error = ", corner, "interior error = ", interior)
print("overall error =", overall)
print("bit error = ", bit_error) 

