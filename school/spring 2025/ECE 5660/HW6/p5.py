import numpy as np

def encode_differential_qpsk2(bitstream):
    encoding = (

        (0,0), (0,1), (1,0), (1,1),
        (1,0), (0,0), (1,1), (0,1),
        (0,1), (1,1), (0,0), (1,0),
        (1,1), (1,0), (0,1), (0,0)
    )
    prevd0 = 0
    prevd1 = 0
    output = []

    for i in range(0, len(bitstream), 2):
        b0 = bitstream[i]
        b1 = bitstream[i+1]
        index = b0 * 8 + b1 * 4 + prevd0 * 2 + prevd1
        (d0, d1) = encoding[index]
        print(b0, b1, "  ", prevd0, prevd1, "  ", d0, d1)
        output.append(d0)
        output.append(d1)
        prevd0 = d0
        prevd1 = d1
    return output

def decode_differential_qpsk2(bitstream):
    decoding = (
        (0,0), (1,0), (0,1), (1,1),
        (0,1), (0,0), (1,1), (1,0),
        (1,0), (1,1), (0,0), (0,1),
        (1,1), (0,1), (1,0), (0,0)
    )
    prevd0 = 0
    prevd1 = 0
    output = []
    for i in range(0, len(bitstream), 2):
        d0 = bitstream[i]
        d1 = bitstream[i+1]
        index = prevd0 * 8 + prevd1 * 4 + d0 * 2 + d1
        (b0, b1) = decoding[index]
        output.append(b0)
        output.append(b1)
        prevd0 = d0
        prevd1 = d1
    return output

def map_to_QPSK(bits):
    constellation_map = {
        (0, 0) : (1, 1),
        (0, 1) : (1, -1),
        (1, 0) : (-1, -1),
        (1, 1) : (-1, 1)
    }
    bk = 0
    bk1 = 1
    I = []
    Q = []
    print("\nmappin")
    for i in range(0, len(bits), 2):
        bk = bits[i]
        bk1 = bits[i+1]
        [Idata, Qdata] = constellation_map[(bk, bk1)]
        I.append(Idata)
        Q.append(Qdata)
        print(bk, bk1, "   ", Idata, Qdata)
    print("\n\n")

    return [np.array(I), np.array(Q)]

def QPSK_decision(I, Q):
    out_bits = []
    for i in range(0,len(I), 1):
        if I[i] > 0:
            if Q[i] > 0:
                a0 = 0
                a1 = 0
            else:
                a0 = 0
                a1 = 1
        else:
            if Q[i] > 0:
                a0 = 1
                a1 = 1
            else:
                a0 = 1
                a1 = 0
        out_bits.append(a0)
        out_bits.append(a1)
    
    return out_bits

def test_dqpsk():
    """Tests differential QPSK encoding and decoding under various phase offsets."""
    bitstream = np.random.randint(0, 2, 20)  # Generate random binary sequence
    bitstream = [0,0, 0,0, 0,1, 0,1, 1,0, 1,0, 1,1, 1,1,
             0,0, 1,0, 0,1, 0,0, 1,1, 0,1, 1,1, 1,0, 0,0]
    encoded_symbols = encode_differential_qpsk2(bitstream)
    #encoded_symbols = differential_encoder(bitstream)
    [I, Q] = map_to_QPSK(encoded_symbols)
    print(I[0:2])

    for offset_deg in [0, 90, 180, 270]:
        phase_offset = np.deg2rad(offset_deg)
        rotated_symbols = (I + Q*1j) * np.exp(1j * phase_offset)  # Apply phase offset
        I = np.real(rotated_symbols)
        Q = np.imag(rotated_symbols)
        receiver_bits = QPSK_decision(I,Q)
        decoded_bits = decode_differential_qpsk2(receiver_bits)
        #decoded_bits = differential_decoder(receiver_bits)

        print(f"Phase Offset: {offset_deg}°")
        print("Original Bits: ", np.array(bitstream))
        print("Decoded Bits:  ", np.array(decoded_bits))
        print("-" * 50)

test_dqpsk()
