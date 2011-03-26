require 'rbconfig'
require 'mkmf'

def darwin?
  RUBY_PLATFORM =~ /darwin/
end

def cpu_x64?
  if defined?(RUBY_ENGINE) and RUBY_ENGINE == 'rbx'
    RbConfig::MAKEFILE_CONFIG['build_cpu'] == 'x86_64' ||
      RbConfig::MAKEFILE_CONFIG['ARCH_FLAG'] =~ /x86_64/
  else
    ['x'].pack('y').size == 8
  end
end

V8_DIR = File.expand_path("../../../vendor/v8", __FILE__)
V8_ARCH = cpu_x64? ? 'x64' : 'ia32' 
V8_FLAGS = '-fPIC -fno-builtin-memcpy'
           
# compile V8 engine... 

Dir.chdir V8_DIR do 
  if Dir["**/libv8.a"].empty?
    defaults, ENV['CCFLAGS'] = ENV['CCFLAGS'], V8_FLAGS
    system "scons mode=release snapshot=off library=static arch=#{V8_ARCH}" 
    ENV['CCFLAGS'] = defaults;
  end
end

# generate makefile for the gem...

dir_config 'mustang/v8'
find_header 'v8.h', File.join(V8_DIR, "include")
have_library 'pthread'
have_library 'objc' if darwin?

$LOCAL_LIBS << Dir[File.join(V8_DIR, "**/**/libv8.a")].first

%w[-Wall -g -rdynamic -fPIC].each { |flag|
  $CPPFLAGS += " #{flag}" unless $CPPFLAGS.include?(flag)
}

CONFIG['LDSHARED'] = '$(CXX) -shared' unless darwin?

create_makefile 'v8'
