MRuby::Build.new do |conf|
    toolchain :gcc
    conf.gembox 'default'
end

MRuby::CrossBuild.new('wasm32-unknown-gnu') do |conf|
    toolchain :clang
  
    conf.gembox 'default'
    conf.gem :github => 'mattn/mruby-onig-regexp', :checksum_hash => '76087d150d12f167e95ae10d326099b352cf3d18'
    conf.gem :github => 'pulsejet/mruby-marshal'
    conf.gem :github => 'monochromegane/mruby-time-strftime'
    conf.gem :core => 'mruby-eval'
    conf.cc.command = 'emcc'
    conf.cc.flags = %W(-O2 -g0)
    conf.cxx.command = 'em++'
    conf.cxx.flags = %W(-O2 -g0 -std=c++14)
  
    conf.linker.command = 'emcc'
    conf.archiver.command = 'emar'
end
