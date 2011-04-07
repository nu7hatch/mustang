require 'rbconfig'
require 'mkmf'

def darwin?
  RUBY_PLATFORM =~ /darwin/
end

def rubinius?
  defined?(RUBY_ENGINE) && RUBY_ENGINE == 'rbx'  
end

def cpu_x64?
  if rubinius?
    !!(RbConfig::MAKEFILE_CONFIG['build_cpu'] == 'x86_64' ||
       RbConfig::MAKEFILE_CONFIG['ARCH_FLAG'] =~ /x86_64/)
  else
    ['foo'].pack('p').size == 8
  end
end

def make_sure_scons_installed!
  unless `hash scons; echo $?`.to_i == 0
    raise RuntimeError, "ERROR: To compile V8 engine you need to install the Scons library!"
  end
end

def compile_vendor_v8!(dir)
  arch = cpu_x64? ? 'x64' : 'ia32' 
  flags = '-fPIC -fno-builtin-memcpy -shared'

  Dir.chdir dir do 
    begin
      make_sure_scons_installed!
      defaults, ENV['CCFLAGS'] = ENV['CCFLAGS'], flags
      build_cmd = "scons mode=release snapshot=off library=static arch=#{arch}"
      puts build_cmd
      system build_cmd
    ensure
      ENV['CCFLAGS'] = defaults
    end
  end
end

V8_DIR = File.expand_path("../../../vendor/v8", __FILE__)
inc, lib = dir_config('v8', File.join(V8_DIR, 'include'), V8_DIR)

if V8_DIR == lib
  compile_vendor_v8!(V8_DIR)
  $LOCAL_LIBS << Dir[File.join(V8_DIR, "**/**/libv8.a")].first
end

find_library('v8', nil, lib)
have_library('pthread')
have_header('v8.h')

CONFIG['LDSHARED'] = '$(CXX) -shared' unless darwin?

%w[-Wall -g -rdynamic -fPIC].each { |flag| 
  $CPPFLAGS += " #{flag}" unless $CPPFLAGS.include?(flag)
}

create_makefile('mustang/v8')
