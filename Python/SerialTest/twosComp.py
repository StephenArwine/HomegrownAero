


# two's Complement
def twosComplement( byteOne, byteTwo):

    if (byteTwo >> 7) == 1:
        twos = (byteOne << 0) + (byteTwo << 8)
        twos = twos  | ~((1 << 16) - 1)
        twos = twos >> 4

    else:
        twos = (byteOne << 0) + (byteTwo << 8)
        twos = twos >> 4

    return twos * 0.0078125