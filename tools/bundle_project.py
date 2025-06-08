#!/usr/bin/env python3
"""
bundle_project.py

Recursively walks a directory and writes every file's path and contents
into a single text file, with clear separators.

Usage:
  python bundle_project.py --root . --output project_dump.txt
"""

import os
import argparse

def bundle(root, output_path):
    with open(output_path, 'w', encoding='utf-8', errors='replace') as out:
        for dirpath, dirnames, filenames in os.walk(root):
            # skip hidden directories (e.g. .git)
            dirnames[:] = [d for d in dirnames if not d.startswith('.')]
            for fname in sorted(filenames):
                if fname.startswith('.'): 
                    continue
                file_path = os.path.join(dirpath, fname)
                rel_path = os.path.relpath(file_path, root)
                out.write(f"\n=== {rel_path} ===\n")
                try:
                    with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
                        out.write(f.read())
                except Exception as e:
                    out.write(f"[Error reading file: {e}]\n")
        print(f"Bundled project into {output_path}")

if __name__ == "__main__":
    ap = argparse.ArgumentParser(description="Bundle project files into one text document")
    ap.add_argument("--root",   default=".", help="Root folder to start bundling")
    ap.add_argument("--output", default="project_dump.txt", help="Output bundle file")
    args = ap.parse_args()
    bundle(args.root, args.output)
