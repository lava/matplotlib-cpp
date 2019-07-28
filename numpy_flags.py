from os import path

try:
    from numpy import __file__ as numpyloc

    # Get numpy directory
    numpy_dir = path.dirname(numpyloc)

    # Print the result of joining this to core and include
    print("-I" + path.join(numpy_dir, "core", "include"))
except:
    print("-DWITHOUT_NUMPY")
