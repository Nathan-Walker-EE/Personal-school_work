import numpy as np
from scipy.special import erfc

def Q_function(x):
    return 0.5 * erfc(x/np.sqrt(2))

def compute_all_Q_terms(K1_list, K2_list):
    Q_values = np.array([Q_function(np.sqrt(K2)) for K2 in K2_list])
    terms = np.array(K1_list) * Q_values
    return terms, np.sum(terms)

r = 2
No = 2/5

error = 2 * Q_function((np.sqrt(6)*r)/(2*np.sqrt(No)))
print(error)