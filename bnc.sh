# Build and compare

wine $SHC_BIN/shc.exe src\\incomplete\\_000572_8c01023c_sound.c \
 -object=build\\_000572_8c01023c_sound.src \
 -code=asmcode \
 -sub=shc.sub && \
 delta -s \
 <(cat src/_000572_8c01023c_sound.src) \
 <(cat build/_000572_8c01023c_sound.src | sed '/.LINE/d' | ./tools/rename_labels.py | ./tools/ljust_labels.py )

#  <(cat src/_004384_8c011120.src | sed '0,/^_request_dat_8c011182/d') \
#  <(cat build/_004384_8c011120.src | sed '/.LINE/d' | ./tools/rename_labels.py | sed '0,/^_request_dat_8c011182/d')

# wine $SHC_BIN/shc.exe src\\_004384_8c011120_FUN_8c0113d8.c \
#  -object=build\\_004384_8c011120_FUN_8c0113d8.src \
#  -code=asmcode \
#  -sub=shc.sub && \
#  delta -s \
#  <(cat src/_004384_8c011120_FUN_8c0113d8.src) \
#  <(cat build/_004384_8c011120_FUN_8c0113d8.src | sed '/.LINE/d' | ./tools/rename_labels.py)