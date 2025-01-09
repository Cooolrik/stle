# ctle Copyright (c) 2024 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

import os
from ctlepy.file_checker import line_define
from ctlepy.file_checker import check_file_lines
from ctlepy.file_checker import fix_file
import pathlib

# Set to True to fix files, False to only check and report errors
# Note that this will not fix all errors, and the files will still need to be checked manually
fix_fails = False

# Function to check both copyright and #ifdef in files
def check_file(file_path: str) -> bool:
	print(f"Checking file: {file_path}")
	defs = []

	if file_path.endswith(".py"):
		# python files	
		defs.extend( [
			line_define(1,r"# ctle Copyright \(c\) \d{4} Ulrik Lindahl","# ctle Copyright (c) 2024 Ulrik Lindahl"),
			line_define(2,"# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE")
		] )
	else:
		# c++ files
		defs.extend( [
			line_define(1,r"// ctle Copyright \(c\) \d{4} Ulrik Lindahl","// ctle Copyright (c) 2024 Ulrik Lindahl"),
			line_define(2,"// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE")
		] )

		if file_path.endswith(".h"):
			# header files
			file_title, extension = os.path.splitext(os.path.basename(file_path))
			file_title = file_title.upper().replace("-", "_").replace(" ", "_")
			extension = extension[1:].upper()  # Remove the leading dot and convert to uppercase
			file_def = f"_CTLE_{file_title}_{extension}_"		

			defs.extend( [
				line_define(3,"#pragma once"),
				line_define(4,f"#ifndef {file_def}"),
				line_define(5,f"#define {file_def}"),
				line_define(-1,f"#endif//{file_def}"),
			] )

	if len(check_file_lines(file_path, defs)) > 0:
		if not fix_fails:
			return False
		else:
			print(f"Fixing file: {file_path}")
			return fix_file(file_path, defs)

	return True

# Function to traverse directories and check files
def check_files_in_directory(ctle_root:pathlib.Path, subdir:str) -> list:
	directory = ctle_root / subdir
	file_types = [".cpp", ".h", ".inl", ".py"]
	invalid_headers = set()

	# check all files in directory
	for root, _, files in os.walk(directory):
		for file in files:
			for ext in file_types:
				if file.endswith(ext):
					file_path = os.path.join(root, file)
					if not check_file(file_path):
						invalid_headers.add(f"File: {file_path} fails header check")

	# if the subdir is ctle, also make sure there are .md files in docs
	if subdir == "ctle":
		docs_dir = ctle_root / "docs"
		for root, _, files in os.walk(directory):
			for file in files:
				file = pathlib.Path(file)
				if file.stem[0] == "_":
					continue
				md_path = pathlib.Path(ctle_root) / "docs" / (str(file.stem) + ".md")
				if not md_path.exists():
					invalid_headers.add(f"Missing file: {md_path}")

	return invalid_headers

if __name__ == "__main__":
	ctle_root = pathlib.Path(__file__).parent.parent.resolve()

	errors = []
	source_directories = ["ctle","unit_tests","code_gen"]
	for subdir in source_directories:
		errors.extend(check_files_in_directory(ctle_root,subdir))

	if len(errors) > 0:
		print(f"\033[31mFound {len(errors)} Errors:")
		for f in errors:
			print(f)
		print("\033[0m")
		exit(-1)
	else:
		print("All files checks out ok")
		exit(0)
