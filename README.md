# mrubyvst
VST plugin with MRuby

# requirements
- VST SDK
- MRuby complied with 32bit flags
  - rake LDFLAGS="-m32" CFLAGS="-m32"

# install
- `git clone https://github.com/fazibear/mrubyvst.git`
- `rake init`
- edit Rakefile
  - change MRUBY_DIR - path to MRuby root directory
  - change VST_SDK_DIR - path to VST SDK root directory
- edit mrubyvst.h
  - change SCRIPTS_DIR - path to examples directory
- `rake link`
- `rake compile`
- run your DAW (Tested in Ableton Live 9 Trial)
- enjoy ;)
