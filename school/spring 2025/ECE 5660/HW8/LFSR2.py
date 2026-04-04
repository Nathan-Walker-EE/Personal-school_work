def lfsr_with_states(polynomial, seed, num_bits):
    """
    Simulate an LFSR and print the state at each cycle.

    Args:
        polynomial (list of int): Feedback polynomial coefficients (MSB to LSB).
        seed (list of int): Initial register state (MSB to LSB).
        num_bits (int): Number of bits to generate.

    Returns:
        list of int: Output sequence (can be MSB or LSB depending on config).
        list of list of int: History of LFSR states at each step.
    """
    n = len(polynomial) - 1
    state = seed
    output = []
    states = []

    print("Cycle | State       | Output")
    print("-----------------------------")

    for cycle in range(num_bits):
        out_bit = state[0]  # Output from MSB — change to state[-1] for LSB
        output.append(out_bit)
        states.append(state.copy())

        print(f"{cycle:5} | {''.join(map(str, state))} |   {out_bit}")

        # Compute feedback from taps (skip x^n which is always 1)
        feedback = 0
        for i in range(1, n+1):
            if polynomial[i] == 1:
                feedback ^= state[i]  # MSB is state[0], so state[i] aligns with poly[i]

        # Shift left and insert feedback at LSB
        state = state[1:] + [feedback]

    return output, states


# Feedback polynomial: x^4 + x + 1
# MSB is x^4, so polynomial = [1, 0, 0, 1, 1]
polynomial = [1, 0, 0, 1, 1]

# Initial state: MSB is leftmost
seed = [1, 0, 0, 0]  # This is your "1000"

# Number of output bits you want to generate
num_bits = 20

# Call the function
output_bits = lfsr_with_states(polynomial, seed, num_bits)

# Print output
print("Output bits:", output_bits)
