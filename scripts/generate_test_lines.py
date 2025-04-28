legacy_horizontal = [-0.125, -0.125, -0.5, -0.75]
legacy_vertical = [-0.375, -0.625, -0.75, -0.625]
legacy_arbitrary = [-0.26516504294495535, -0.4419417382415923, -0.618718433538229, -0.8838834764831844]

arbitrary_mixed = [-0.375, -0.4419417382415923, -0.5, -0.8838834764831844, -2, -2, -2, -2, -2, -2]
horizontal = [-0.625]
arbitrary = [0.0, -0.08838834765]

def main():

    # Let's clear the file first.
    with open("legacy_horizontal_lines", "w") as file:
        pass

    with open("legacy_horizontal_lines", "a") as file:
        for i in range(len(legacy_horizontal)):
            file.write(str(i) + " 0 1 " + str(legacy_horizontal[i]) + "\n")

    with open("legacy_vertical_lines", "w") as file:
        pass

    with open("legacy_vertical_lines", "a") as file:
        for i in range(len(legacy_vertical)):
            file.write(str(i) + " 1 0 " + str(i) + "\n")

    with open("legacy_diagonal_lines", "w") as file:
        pass

    with open("legacy_diagonal_lines", "a") as file:
        for i in range(len(legacy_arbitrary)):
            file.write(str(i) + " 0.70710678 0.70710678 " + str(i) + "\n")




    with open("arbitrary_lines", "w") as file:
        pass

    with open("arbitrary_lines", "a") as file:
        for i in range(len(arbitrary_mixed)):
            if i == 0:
                file.write(str(i) + " 1 0 " + str(arbitrary_mixed[i]) + "\n")
            elif i == 3:
                file.write(str(i) + " 0.70710678 0.70710678 " + str(arbitrary_mixed[i]) + "\n")
            else:
                file.write(str(i) + " 0 1 " + str(arbitrary_mixed[i]) + "\n")

    with open("horizontal_lines", "w") as file:
        pass

    with open("horizontal_lines", "a") as file:
        for i in range(len(horizontal)):
            file.write("0 1 " + str(horizontal[i]) + "\n")

    with open("coincide_lines", "w") as file:
        pass

    with open("coincide_lines", "a") as file:
        file.write("-0.70710678 0.70710678 " + str(arbitrary[0]) + "\n")

    with open("diagonal_lines", "w") as file:
        pass

    with open("diagonal_lines", "a") as file:
        file.write("-0.70710678 0.70710678 " + str(arbitrary[1]) + "\n")
main()