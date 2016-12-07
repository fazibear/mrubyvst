MRUBY_DIR = '/Users/fazibear/dev/mruby'
VST_SDK_DIR = '/Users/fazibear/dev/vst-sdk'

desc 'Init VST directory'
task :init do
  sh "mkdir mrubyvst.vst"
  sh "mkdir mrubyvst.vst/Contents"
  sh "mkdir mrubyvst.vst/Contents/MacOS"
end

desc 'Link VST'
task :link do
  sh 'ln -s mrubyvst.vst ~/Library/Audio/Plug-Ins/VST/mrubyvst.vst'
end

desc 'Unink VST'
task :unlink do
  sh 'rm ~/Library/Audio/Plug-Ins/VST/mrubyvst.vst'
end

desc 'Compile VST'
task :compile do

  mruby_includes = %W(
    -I#{MRUBY_DIR}/include/
    -L#{MRUBY_DIR}/build/mrubyvst/lib/
    #{MRUBY_DIR}/build/mrubyvst/lib/libmruby.a
  ).join(' ')

  vst_includes = %W(
    -I#{VST_SDK_DIR}
    -I#{VST_SDK_DIR}/public.sdk/source/vst2.x/
  ).join(' ')

  vst_sdk_sources = %W(
    #{VST_SDK_DIR}/public.sdk/source/vst2.x/audioeffect.cpp
    #{VST_SDK_DIR}/public.sdk/source/vst2.x/audioeffectx.cpp
    #{VST_SDK_DIR}/public.sdk/source/vst2.x/vstplugmain.cpp
  ).join(' ')

  sh "g++ -bundle #{vst_includes} -o mrubyvst.vst/Contents/MacOS/mrubyvst #{mruby_includes} #{vst_sdk_sources} mrubyvst.cpp"
end

task :default => :compile
