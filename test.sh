#!/bin/bash

# Good tests
echo "Running good tests..."
./ft_ls -a
./ft_ls -l
./ft_ls -r
./ft_ls -R
./ft_ls -t

# Bad tests
echo "Running bad tests..."
./ft_ls -z  # Invalid flag
./ft_ls --invalid  # Invalid flag
./ft_ls -a -z  # Combination of valid and invalid flags
./ft_ls -l --invalid  # Combination of valid and invalid flags

# Mixed tests
echo "Running mixed tests..."
./ft_ls -a -l
./ft_ls -r -R
./ft_ls -t -a
./ft_ls -l -t -r
./ft_ls -a -R -l
