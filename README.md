# kamcuk libraries

My libraries

## How to use

Just `add_subdirectory(libs)` from your cmake project.
Then `target_include_libraries(.... kamcuk_all)` for all projects.

## Structure

- `./cmake` has cmake libraries.
- `./libs` has one library per folder. Each library should have some unit tests.
- `./scripts` - shell script that I wrote.
- `CMakeLists.txt` - this project cmake list. Don't use. See `Hot to use`.
- `Makefile` - scripted to run tests.

# Written

Written by Kamil Cukrowski (C) 2019.
Licensed jointly under MIT License and Beerware License.

