import os
import zipfile
import sys


def make_zipfile(output_filename, entry):
    with zipfile.ZipFile(output_filename, "w", zipfile.ZIP_DEFLATED) as zip:
        if os.path.isdir(entry):
            relroot = os.path.abspath(os.path.join(entry, os.pardir))
            for root, dirs, files in os.walk(entry):
                zip.write(root, os.path.relpath(root, relroot))
                for file in files:
                    filename = os.path.join(root, file)
                    if os.path.isfile(filename): # regular files only
                        arcname = os.path.join(os.path.relpath(root, relroot), file)
                        zip.write(filename, arcname)
        else:
            zip.write(entry, os.path.basename(entry))


if __name__ == '__main__':
    # python zip.py file dir ... file name.zip
    arch = sys.argv[len(sys.argv) - 1]

    make_zipfile(arch, sys.argv[1])
