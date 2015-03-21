# mrubyvst
VST plugin with MRuby

# requirements
- VST SDK
- MRuby complied with 32bit flags
  - add gems
    - conf.gem :core => 'mruby-math'
    - conf.gem :core => 'mruby-eval'
    - conf.gem :github => 'iij/mruby-pack'
    - conf.gem :github => 'iij/mruby-io'
    - conf.gem :github => 'iij/mruby-dir'
    - conf.gem :github => 'iij/mruby-socket'
    - conf.gem :github => 'mattn/mruby-http'
    - conf.gem :github => 'matsumoto-r/mruby-simplehttpserver'
    - conf.gem :github => 'mattn/mruby-thread'
  - rake LDFLAGS="-m32" CFLAGS="-m32"

# install
- `git clone https://github.com/fazibear/mrubyvst.git`
- `rake init`
- edit Rakefile
  - change MRUBY_DIR - path to MRuby root directory
  - change VST_SDK_DIR - path to VST SDK root directory
- edit mrubyvst.h
  - change SCRIPT_PATH - path to mrubyvst.rb file
- `rake link`
- `rake compile`
- run your DAW (Tested in Ableton Live 9 Trial)
- enjoy ;)

# notes
- takes a lot of cpu - mruby is not a speed demon
- it leaks :/

# screenshot
![](/screenshot.png?raw=true "screenshot")
