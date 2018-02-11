


# two's Complement
def twos_complement( byteOne, byteTwo):

    if (byteTwo >> 7) == 1:
        twos = (byteOne << 0) + (byteTwo << 8)
        twos = twos | ~((1 << 16) - 1)
    else:
        twos = (byteOne << 0) + (byteTwo << 8)

    return twos
