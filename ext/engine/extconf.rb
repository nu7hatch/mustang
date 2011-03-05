require 'rubygems'
require 'mkmf-rice'

arch = 'x64' # TODO: find out how to discover current arch

v8_dir = File.expand_path("../../../vendor/v8", __FILE__)
v8_ccflags = '-fPIC -fno-builtin-memcpy'
            
Dir.chdir v8_dir do 
  ccflags, ENV['CCFLAGS'] = ENV['CCFLAGS'], v8_ccflags
  system "scons mode=release snapshot=off library=static arch=#{arch}" if Dir["**/libv8.a"].empty?
  ENV['CCFLAGS'] = ccflags;
end

libv8 = Dir[v8_dir + "/**/**/libv8.a"].first

$LOCAL_LIBS << libv8

dir_config 'engine'
find_header 'v8.h', v8_dir + "/include/"
have_header 'pthread'

create_makefile 'engine'
