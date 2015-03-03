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

  mruby_dir = '~/dev/mruby'
  vst_sdk_dir = '~/dev/vst-sdk'

  mruby_includes = %W(
    -L#{mruby_dir}/build/host/lib/
    #{mruby_dir}/build/host/lib/libmruby.a
  ).join(' ')

  vst_includes = %W(
    -I#{vst_sdk_dir}
    -I#{vst_sdk_dir}/public.sdk/source/vst2.x/
  ).join(' ')

  vst_sdk_sources = %W(
    #{vst_sdk_dir}/public.sdk/source/vst2.x/audioeffect.cpp
    #{vst_sdk_dir}/public.sdk/source/vst2.x/audioeffectx.cpp
    #{vst_sdk_dir}/public.sdk/source/vst2.x/vstplugmain.cpp
  ).join(' ')

  sh "g++ -arch i386 -bundle #{vst_includes} -o mrubyvst.vst/Contents/MacOS/mrubyvst #{mruby_includes} #{vst_sdk_sources} mrubyvst.cpp"
end
