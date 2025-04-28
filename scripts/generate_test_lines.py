def main():
    legacy_horizontal = [-0.125, -0.125, -0.75]
    legacy_vertical = [-0.375, -0.625]
    legacy_arbitrary = [-0.26516504294495535, -0.618718433538229, -0.8838834764831844]

    with open("legacy_horizontal_lines.dat", "w") as file:
        for d in legacy_horizontal:
            file.write("0.0 1.0 " + str(d) + "\n")

    with open("legacy_vertical_lines.dat", "w") as file:
        for d in legacy_vertical:
            file.write("1.0 0.0 " + str(d) + "\n")

    with open("legacy_diagonal_lines.dat", "w") as file:
        for d in legacy_arbitrary:
            file.write("0.70710678 0.70710678 " + str(d) + "\n")

    with open("arbitrary_lines.dat", "w") as file:
        file.write("1.0 0.0 -0.375\n")
        file.write("0.70710678 0.70710678 -0.8838834764831844\n")

    with open("horizontal_lines.dat", "w") as file:
        file.write("0.0 1.0 -0.625\n")

    with open("coincide_lines.dat", "w") as file:
        file.write("-0.70710678 0.70710678 0.0\n")

    with open("parallel_lines.dat", "w") as file:
        file.write("-0.70710678 0.70710678 -0.08838834765\n")
main()