#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
# File name: increment_version.py
# Author: MasterLaplace
# Created on: 2023-10-12

import re
import os
import sys

def increment_version(current_version: str) -> str:
    """
    Increment the given version string based on the command line argument.

    The function expects the version string to be in the format 'v<major>.<minor>.<patch>'.
    It increments the version based on the first command line argument, which can be "major", "minor", or "patch".

    Args:
        current_version (str): The current version string in the format 'v<major>.<minor>.<patch>'.

    Returns:
        str: The incremented version string.

    Raises:
        ValueError: If the version format is invalid or if the increment type is invalid.
    """
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

def increment_doxyfile_version(new_version: str) -> None:
    """
    Updates the version number in the Doxyfile.cfg file for the Flakkari-Server project.

    Args:
        new_version (str): The new version number to set in the format 'major.minor.patch'.

    Returns:
        None
    """
    with open("Doxyfile.cfg", "r") as f:
        content = f.read()

    content = re.sub(r'PROJECT_NUMBER\s+=\s+\d+\.\d+\.\d+', f'PROJECT_NUMBER         = {new_version}', content)

    with open("Doxyfile.cfg", "w", newline='\n') as f:
        f.write(content)

def increment_cmakelists_version(new_version: str) -> None:
    """
    Updates the version number in the CMakeLists.txt file for the Flakkari-Server project.

    Args:
        new_version (str): The new version number to set in the format 'major.minor.patch'.

    Returns:
        None
    """
    with open("CMakeLists.txt", "r") as f:
        content = f.read()

    content = re.sub(r'project\(Flakkari-Server VERSION \d+\.\d+\.\d+', f'project(Flakkari-Server VERSION {new_version}', content)

    with open("CMakeLists.txt", "w", newline='\n') as f:
        f.write(content)

def increment_config_in_version(path: str, new_version: str) -> None:
    """
    Updates the version information in the specified configuration file.

    This function reads the content of the file at the given path, updates the version
    information to the new version provided, and writes the updated content back to the file.
    The version information is expected to be in the format 'major.minor.patch'.

    Args:
        path (str): The path to the configuration file to be updated.
        new_version (str): The new version string in the format 'major.minor.patch'.

    Returns:
        None
    """
    major, minor, patch = map(int, new_version.split('.'))

    with open(path, "r") as f:
        content = f.read()

    content = re.sub(r'(\d+\.\d+\.\d+)', f"{major}.{minor}.{patch}", content)
    content = re.sub(r'#define FLAKKARI_VERSION_MAJOR \d+', f'#define FLAKKARI_VERSION_MAJOR {major}', content)
    content = re.sub(r'#define FLAKKARI_VERSION_MINOR \d+', f'#define FLAKKARI_VERSION_MINOR {minor}', content)
    content = re.sub(r'#define FLAKKARI_VERSION_PATCH \d+', f'#define FLAKKARI_VERSION_PATCH {patch}', content)

    with open(path, "w", newline='\n') as f:
        f.write(content)

def loop_in_files(path: str, current_version: str, new_version: str) -> None:
    """
    Recursively updates the version number in specified file types within a given directory.

    Args:
        path (str): The root directory path to start the search.
        current_version (str): The current version number to be replaced.
        new_version (str): The new version number to replace the current version.

    Returns:
        None

    This function searches for files with extensions .cpp, .h, .md, .hpp, and .h.in within the given directory
    and its subdirectories. It updates the version number in these files from the current version to the new version.
    """
    for root, dirs, files in os.walk(path):
        for file in files:
            if file.endswith(".cpp") or file.endswith(".h") or file.endswith(".md") or file.endswith(".hpp"):
                print(f"Updating version {current_version} -> {new_version} in {file}")
                match = re.search(r'(\d+\.\d+\.\d+)', current_version)
                if match:
                    current_version_number = match.group(1)
                    with open(os.path.join(root, file), "r") as f:
                        content = f.read()
                    with open(os.path.join(root, file), "w", newline='\n') as f:
                        f.write(content.replace(current_version_number, new_version))

            if file.endswith(".h.in"):
                print(f"Updating version {current_version} -> {new_version} in {file}")
                increment_config_in_version(os.path.join(root, file), new_version)

def main():
    """
    Main function to increment the version of the project.

    This function performs the following steps:
    1. Reads the current version from the VERSION file.
    2. Increments the version using the `increment_version` function.
    3. Updates the VERSION file with the new version.
    4. Updates the version in the Doxyfile.cfg file.
    5. Updates the version in the CMakeLists.txt file.
    6. Recursively updates the version in all relevant files within the ./Flakkari/ directory.

    Raises:
        FileNotFoundError: If the VERSION file does not exist.
        IOError: If there is an error reading or writing to the VERSION file.
    """
    with open("VERSION", "r") as version_file:
        current_version = version_file.read().strip()

    new_version = increment_version(current_version)

    print(f"Updating version {current_version} -> {new_version} in VERSION")
    with open("VERSION", "w", newline='\n') as version_file:
        version_file.write(f"Flakkari v{new_version}\n")

    print(f"Updating version {current_version} -> {new_version} in Doxyfile.cfg")
    increment_doxyfile_version(new_version)

    print(f"Updating version {current_version} -> {new_version} in CMakeLists.txt")
    increment_cmakelists_version(new_version)

    loop_in_files("./Flakkari/", current_version, new_version)

if __name__ == "__main__" and len(sys.argv) == 2:
    main()
