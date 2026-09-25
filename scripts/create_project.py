#!/usr/bin/env python3

from pathlib import Path
import shutil
import zipfile

ROOT = Path(__file__).resolve().parents[1]
ZIP = ROOT.parent / "MCPE_Client_Project.zip"

# This script is intentionally small: it creates the directories expected by the
# repository. Existing generated build output is not touched.
DIRECTORIES = [
    "include/client/Modules/Performance",
    "include/client/Modules/PvP",
    "include/client/ui",
    "include/client/integration",
    "src/core",
    "src/ui",
    "src/Modules/Performance",
    "src/Modules/PvP",
    "src/integration",
    "docs",
    ".github/workflows",
]

for relative in DIRECTORIES:
    (ROOT / relative).mkdir(parents=True, exist_ok=True)

print(f"Project root: {ROOT}")
print("Directory structure verified.")
print("Use the root CMakeLists.txt to configure/build with the Android NDK.")
