import random

def differential_encoder(data):
    LUT = (
        (0, 0), (0, 1), (1, 0), (1, 1),
        (1, 0), (0, 0), (1, 1), (0, 1),
        (0, 1), (1, 1), (0, 0), (1, 0),
        (1, 1), (1, 0), (0, 1), (0, 0)
    )
    delay_bits = (0, 0)
    out_bits = []

    for i in range(0, len(data), 2):
        bits = data[i:i+2]
        LUT_index = bits[0] * 8 + bits[1] * 4 + delay_bits[0] * 2 + delay_bits[1]
        delay_bits = LUT[LUT_index]
        
        out_bits.append(delay_bits[0]); out_bits.append(delay_bits[1])
    return out_bits

def differential_decoder(encoded_data):
    LUT = (
        (0, 0), (1, 0), (0, 1), (1, 1),
        (0, 1), (0, 0), (1, 1), (1, 0),
        (1, 0), (1, 1), (0, 0), (0, 1),
        (1, 1), (0, 1), (1, 0), (0, 0)
    )
    delay_bits = (0, 0)
    out_bits = []

    for i in range(0, len(encoded_data), 2):
        bits = encoded_data[i:i+2]
        LUT_index = delay_bits[0] * 8 + delay_bits[1] * 4 + bits[0] * 2 + bits[1]
        decoded_bits = LUT[LUT_index]

        out_bits.append(decoded_bits[0]); out_bits.append(decoded_bits[1])

        delay_bits = bits
    return out_bits


# Test the functions.
test_data = [0,0, 0,0, 0,1, 0,1, 1,0, 1,0, 1,1, 1,1,
             0,0, 1,0, 0,1, 0,0, 1,1, 0,1, 1,1, 1,0, 0,0]

# all possible transitions that should be tested
        #00 -> 00  01 -> 00  10 -> 00  11 -> 00
        #00 -> 01  01 -> 01  10 -> 01  11 -> 01
        #00 -> 10  01 -> 10  10 -> 10  11 -> 10
        #00 -> 11  01 -> 11  10 -> 11  11 -> 11

#test_data = [random.randint(0, 1) for _ in range(1000)] 
   
encoded_test_data = differential_encoder(test_data)
decoded_test_data = differential_decoder(encoded_test_data)

with open("differential_encoding.txt", "w") as f:
    f.write("Test data:    " + str(test_data) + "\n")
    f.write("\nEncoded data: " + str(encoded_test_data) + "\n")
    f.write("\nDecoded data: " + str(decoded_test_data) + "\n")
    f.write("\nTest data matches decoded data: " + str(test_data == decoded_test_data) + "\n")
