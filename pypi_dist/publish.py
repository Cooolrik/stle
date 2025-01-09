# ctle Copyright (c) 2023 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

import os, sys, subprocess, shutil

# copy license and readme files from parent folder
shutil.copy('../README.md','./README.md')
shutil.copy('../LICENSE','./LICENSE')

# copy the ctlepy code to folder src
os.makedirs('src', exist_ok=True)
shutil.copytree('../code_gen/ctlepy', 'src/ctlepy', dirs_exist_ok=True )

# read version string from version file in parent dir
with open('../version','r') as f:
	version = f.readline()
	f.close()

# generate the setup.cfg file
with open('setup.cfg','w') as f:
	f.write(f'''[metadata]
name = ctlepy
version = {version}
author = Ulrik Lindahl
description = Python package for ctle
long_description = file: README.md, LICENSE
long_description_content_type = text/markdown
url = https://github.com/Cooolrik/ctle
project_urls =
    Bug Tracker = https://github.com/Cooolrik/ctle
    repository = https://github.com/Cooolrik/ctle
classifiers =
    Programming Language :: Python :: 3
    License :: OSI Approved :: MIT License
    Operating System :: OS Independent

[options]
package_dir =
    = src
packages = find:
python_requires = >=3.10

[options.packages.find]
where = src
''')
	f.close()

# generate the pyproject.toml file
with open('pyproject.toml','w') as f:
	f.write('''
[build-system]
requires = ['setuptools>=42']
build-backend = 'setuptools.build_meta'
''')
	f.close()

# package the module
subprocess.check_call([sys.executable, "-m", "pip", "install", "build"]) 
subprocess.check_call([sys.executable, '-m', 'build']) 

# upload to pypi using twine
subprocess.check_call([sys.executable, "-m", "pip", "install", "--upgrade", "twine"]) 
subprocess.check_call([sys.executable, "-m", "twine", "upload", "--repository", "pypi", "dist/*"]) 
