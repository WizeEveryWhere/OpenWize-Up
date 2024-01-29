
This file explain how to locally build the documentation

# Prerequisite
## Installation
apt-get -y install python3-pip
apt-get -y install pipenv
pip install pipenv


# How to
## Create virtenv (if doesn't already exist)
mkdir sphinx_docu
virtualenv -p python3 sphinx_docu
pip install Sphinx
pip install sphinx_rtd_theme
pip install breathe

## Then activate it 
source sphinx_docu/bin/activate

## Build the documentation
cd docs
make html

## Optionally, deactivate and remove the virtual environment 
deactivate
rm -rf sphinx_docu

# Other helping
## Get local installation package
pip freeze --local > local_pck.txt
