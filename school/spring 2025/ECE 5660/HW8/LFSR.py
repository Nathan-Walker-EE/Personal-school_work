def parta_LFSR():
    output = 0
    x0 = 1
    x1 = 0
    x2 = 0
    x3 = 0
    for i in range(16):
        print(i, x0, x1, x2, x3)
        output = x3
        x3 = x2
        x2 = x1
        x1 = (x0 + output)%2
        x0 = output
    return

def partb_LFSR():
    output = 0
    x0 = 1
    x1 = 0
    x2 = 1
    x3 = 1
    for i in range(16):
        print(i, x0, x1, x2, x3)
        output = x3
        x3 = x2
        x2 = (x1 + output)%2
        x1 = (x0 + output)%2
        x0 = output
    return

def partc_LFSR():
    output = 0
    x0 = 1
    x1 = 0
    x2 = 0
    x3 = 0
    for i in range(16):
        print("count = ", i,"  ", x0, x1, x2, x3, "  output = ", output)
        output = x3
        x3 = (x2 + output)%2
        x2 = x1 
        x1 = x0
        x0 = output
    return

# parta_LFSR()
# partb_LFSR()
partc_LFSR()