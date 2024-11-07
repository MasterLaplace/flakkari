#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
# File name: increment_version.py
# Author: MasterLaplace
# Created on: 2023-10-12

import re
import os
import sys

major = 0
minor = 0
patch = 0

def increment_version(current_version: str) -> str:
    match = re.search(r'v(\d+\.\d+\.\d+)', current_version)
    if match:
        current_version_number = match.group(1)
        major, minor, patch = map(int, current_version_number.split('.'))
        if sys.argv[1] == "major":
            return f"{major + 1}.0.0"
        elif sys.argv[1] == "minor":
            return f"{major}.{minor + 1}.0"
        elif sys.argv[1] == "patch":
            return f"{major}.{minor}.{patch + 1}"
        else:
            raise ValueError(f"Invalid version increment: {sys.argv[1]}")

    raise ValueError(f"Invalid version format: {current_version}")

def increment_doxyfile_version(new_version: str):
    with open("Doxyfile.cfg", "r") as f:
        content = f.read()

    content = re.sub(r'PROJECT_NUMBER\s+=\s+\d+\.\d+\.\d+', f'PROJECT_NUMBER         = {new_version}', content)

    with open("Doxyfile.cfg", "w") as f:
        f.write(content)

def increment_config_in_version(path: str, new_version: str):
    major, minor, patch = map(int, new_version.split('.'))

    with open(path, "r") as f:
        content = f.read()

    content = re.sub(r'(\d+\.\d+\.\d+)', f"{major}.{minor}.{patch}", content)
    content = re.sub(r'#define FLAKKARI_VERSION_MAJOR \d+', f'#define FLAKKARI_VERSION_MAJOR {major}', content)
    content = re.sub(r'#define FLAKKARI_VERSION_MINOR \d+', f'#define FLAKKARI_VERSION_MINOR {minor}', content)
    content = re.sub(r'#define FLAKKARI_VERSION_PATCH \d+', f'#define FLAKKARI_VERSION_PATCH {patch}', content)

    with open(path, "w") as f:
        f.write(content)

if __name__ == "__main__" and len(sys.argv) == 2:
    # Read the current version from the VERSION file
    with open("VERSION", "r") as version_file:
        current_version = version_file.read().strip()

    new_version = increment_version(current_version)

    # Update the VERSION file with the new version
    with open("VERSION", "w") as version_file:
        version_file.write( f"Flakkari v{new_version}\n")

    print(f"Updating version {current_version} -> {new_version} in VERSION")

    increment_doxyfile_version(new_version)

    print(f"Updating version {current_version} -> {new_version} in Doxyfile.cfg")

    # loop in all the files recursively and update the version
    for root, dirs, files in os.walk("./Flakkari/"):
        for file in files:
            if file.endswith(".cpp") or file.endswith(".h") or file.endswith(".md") or file.endswith(".hpp"):
                print(f"Updating version {current_version} -> {new_version} in {file}")
                match = re.search(r'(\d+\.\d+\.\d+)', current_version)
                if match:
                    current_version_number = match.group(1)
                    with open(os.path.join(root, file), "r") as f:
                        content = f.read()
                    with open(os.path.join(root, file), "w") as f:
                        f.write(content.replace(current_version_number, new_version))
            if file.endswith(".h.in"):
                increment_config_in_version(os.path.join(root, file), new_version)
