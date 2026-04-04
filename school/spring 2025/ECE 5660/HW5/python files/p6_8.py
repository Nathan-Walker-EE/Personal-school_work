import numpy as np
from scipy.special import erfc

def Q_function(x):
    return 0.5 * erfc(x/np.sqrt(2))

def compute_all_Q_terms(K1_list, K2_list):
    Q_values = np.array([Q_function(np.sqrt(K2)) for K2 in K2_list])
    terms = np.array(K1_list) * Q_values
    return terms, np.sum(terms)

K1_list = [3, 9/4, 2, 3, 1, 1, 3/2, 1, 1/4]
K2_list = [2, 4, 8, 10, 16, 18, 20, 26, 36]

terms, total_sum = compute_all_Q_terms(K1_list, K2_list)

for i, (K1, K2, term) in enumerate(zip(K1_list, K2_list, terms)):
    print(f"Term {i+1}: {K1} * Q(sqrt({K2})) = {term:.6e}")

print(f"Term {i+1}: {K1} * Q(sqrt({K2})) = {term:.6e}")