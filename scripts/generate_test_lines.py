legacy_horizontal = [-0.125, -0.125, -0.5, -0.75]
legacy_vertical = [-0.375, -0.625, -0.75, -0.625]
legacy_arbitrary = [-0.26516504294495535, -0.4419417382415923, -0.618718433538229, -0.8838834764831844]

arbitrary_mixed = [-0.375, -0.4419417382415923, -0.5, -0.8838834764831844, -2, -2, -2, -2, -2, -2]
horizontal = [-2, -2, -2, -2, -2, -2, -2, -2, -0.625, -0.625, -0.625, -0.625, -2, -2, -2, -2]
arbitrary = [0.0, -0.08838834765]

def main():

    # Let's clear the file first.
    with open("test_clip_poly_legacy_horizontal.txt", "w") as file:
        pass

    with open("test_clip_poly_legacy_horizontal.txt", "a") as file:
        for i in legacy_horizontal:
            file.write("0,1 " + str(i) + "\n")

    with open("test_clip_poly_legacy_vertical.txt", "w") as file:
        pass

    with open("test_clip_poly_legacy_vertical.txt", "a") as file:
        for i in legacy_vertical:
            file.write("1,0 " + str(i) + "\n")

    with open("test_clip_poly_legacy_diagonal.txt", "w") as file:
        pass

    with open("test_clip_poly_legacy_diagonal.txt", "a") as file:
        for i in legacy_arbitrary:
            file.write("1,0 " + str(i) + "\n")

    with open("test_clip_poly_arbitrary.txt", "w") as file:
        pass

    with open("test_clip_poly_arbitrary.txt", "a") as file:
        for i in range(len(arbitrary_mixed)):
            if i == 0:
                file.write("1,0 " + str(arbitrary_mixed[i]) + "\n")
            elif i == 1 or i == 3:
                file.write("0.70710678,0.70710678 " + str(arbitrary_mixed[i]) + "\n")
            else:
                file.write("0,1 " + str(arbitrary_mixed[i]) + "\n")

    with open("test_clip_grid_horizontal.txt", "w") as file:
        pass

    with open("test_clip_grid_horizontal.txt", "a") as file:
        for i in horizontal:
            file.write("0,1" + str(i) + "\n")

    with open("test_clip_grid_diagonal_coincide.txt", "w") as file:
        pass

    with open("test_clip_grid_diagonal_coincide.txt", "a") as file:
        for i in range(2):
            file.write("-0.70710678,0.70710678" + str(arbitrary[0]) + "\n")

    with open("test_clip_grid_diagonal_parallel.txt", "w") as file:
        pass

    with open("test_clip_grid_diagonal_parallel.txt", "a") as file:
        for i in range(2):
            file.write("-0.70710678,0.70710678" + str(arbitrary[1]) + "\n")
main()