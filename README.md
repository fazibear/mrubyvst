# mrubyvst
VST plugin with MRuby

# requirements
- VST SDK
- MRuby complied with 32bit flags
  - rake LDFLAGS="-m32" CFLAGS="-m32"

# install
- `https://github.com/fazibear/mrubyvst.git`
- `rake init`
- edit Rakefile
  - change MRUBY_DIR
  - change VST_SDK_DIR
- edit mrubyvst.h
  - change SCRIPTS_DIR
- `rake link`
- `rake compile`
- run your DAW (Tested in Ableton Live 9 Trial)
- enjoy ;)
