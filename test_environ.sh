#!/bin/bash
set -e

# Clean up any previous test environment
rm -rf testdir

# Create main test directory
mkdir testdir

# --- Create files in testdir ---
echo "Contents of file1" > testdir/file1.txt
echo "Contents of file2" > testdir/file2.log
echo "Hidden file contents" > testdir/.hiddenfile

# Set specific timestamps for ordering tests
touch -t 202401010000 testdir/file1.txt
touch -t 202412312359 testdir/file2.log

# --- Create subdirectories ---
mkdir testdir/subdir
mkdir testdir/subdir_empty

# Create files in subdir
echo "Contents of fileA" > testdir/subdir/fileA
echo "Hidden file in subdir" > testdir/subdir/.hiddenA

# Set timestamps for subdir files
touch -t 202402010101 testdir/subdir/fileA
touch -t 202402020202 testdir/subdir/.hiddenA

# --- Create symbolic links ---
# In testdir: link_to_file1 pointing to file1.txt
ln -s file1.txt testdir/link_to_file1

# In subdir: link to file2.log located in parent directory
ln -s ../file2.log testdir/subdir/link_to_parent_file2

# --- Create a restricted directory for permission tests ---
mkdir testdir/restricted_dir
chmod 000 testdir/restricted_dir

echo "Test environment prepared in 'testdir'."

