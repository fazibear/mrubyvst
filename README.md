# mrubyvst
VST plugin with MRuby as a scripting language.

# Setting up

## VST SDK

Install VST SDK form (Steinberg Website)[https://www.steinberg.net/]

## MRuby

Clone MRuby repo and add your build to `build_config.rb`, and build it with `rake`.

```ruby
MRuby::Build.new('mrubyvst') do |conf|
  toolchain :gcc

  conf.gembox 'default'
  conf.gem :core => 'mruby-eval'
  conf.gem :github => 'iij/mruby-dir'
  conf.gem :github => 'iij/mruby-io'

  # conf.gem :github => 'iij/mruby-pack'
  # conf.gem :github => 'iij/mruby-socket'
  # conf.gem :github => 'mattn/mruby-http'
  # conf.gem :github => 'matsumoto-r/mruby-simplehttpserver'
end
```

## mrubyvst

- `git clone https://github.com/fazibear/mrubyvst.git`
- `rake init`
- edit Rakefile
  - change MRUBY_DIR - path to MRuby root directory
  - change VST_SDK_DIR - path to VST SDK root directory
- `rake link`
- `rake compile`
- run your DAW (Tested in Ableton Live 9 Trial)
- enjoy ;)

# screenshot
![](/screenshot.png?raw=true "screenshot")
