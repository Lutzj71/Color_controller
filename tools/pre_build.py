# PlatformIO pre-build script: regenerate LVGL 8.3 image sources from the PNGs.
import subprocess
import sys

Import("env")  # noqa: F821  (provided by SCons/PlatformIO)

try:
    import PIL  # noqa: F401
except ImportError:
    print("pre_build: installing Pillow into PlatformIO's Python")
    subprocess.check_call([sys.executable, "-m", "pip", "install", "--quiet", "pillow"])

subprocess.check_call([sys.executable, str(env.subst("$PROJECT_DIR")) + "/tools/img2c.py"])
