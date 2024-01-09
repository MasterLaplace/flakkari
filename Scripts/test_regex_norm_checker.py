#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
# File name: test_regex_norm_checker.py
# Author: MasterLaplace
# Created on: 2023-10-12

import re

# Define your regex pattern
keyword_regex = r"^([a-z]+\([\w_.-]+\)|^[a-z]+): "

# List of sample commit messages
commit_messages = [
    "fix(readme): fix link about the wiki",
    "fix(.github): move funding script",
    "feat(main): add new feature",
    "style: fix linting issues",
    "style(docs-folder): style the docs folder",
    "docs(2D): add new documentation",
]

# Test the regex pattern on each commit message
for message in commit_messages:
    match = re.search(keyword_regex, message)
    if match:
        print(f"Match found in '{message}': {match.group()}")
    else:
        print(f"No match found in '{message}'")
