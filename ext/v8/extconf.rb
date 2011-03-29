require 'rbconfig'
require 'mkmf'

def darwin?
  RUBY_PLATFORM =~ /darwin/
end

def cpu_x64?
  unless defined?(RUBY_ENGINE) and RUBY_ENGINE == 'rbx'
    !!(RbConfig::MAKEFILE_CONFIG['arch'] =~ /x86_64/ ||
       RbConfig::MAKEFILE_CONFIG['target_cpu'] == 'x86_64' ||
       RbConfig::MAKEFILE_CONFIG['build_cpu'] == 'x86_64' ||
       RbConfig::MAKEFILE_CONFIG['ARCH_FLAG'] =~ /x86_64/)
  else
    ['x'].pack('y').size == 8
  end
end

def make_sure_scons_installed!
  unless `hash scons; echo $?`.to_i == 0
    raise RuntimeError, "ERROR: To compile V8 engine you need to install the Scons library!"
  end
end

def compile_v8!(v8_dir)
  Dir.chdir v8_dir do 
    if Dir["**/libv8.a"].empty?
      make_sure_scons_installed!
      defaults, ENV['CCFLAGS'] = ENV['CCFLAGS'], V8_FLAGS
      build_cmd = "scons mode=release snapshot=off library=static arch=#{V8_ARCH}"
      puts build_cmd
      system build_cmd
      ENV['CCFLAGS'] = defaults
    end
  end
end

V8_DIR = File.expand_path("../../../vendor/v8", __FILE__)
V8_ARCH = cpu_x64? ? 'x64' : 'ia32' 
V8_FLAGS = '-fPIC -fno-builtin-memcpy'
           
compile_v8!(V8_DIR)

dir_config('mustang/v8')
find_header('v8.h', File.join(V8_DIR, "include"))
have_library('pthread')
have_library('objc') if darwin?

CONFIG['LDSHARED'] = '$(CXX) -shared' unless darwin?
$LOCAL_LIBS << Dir[File.join(V8_DIR, "**/**/libv8.a")].first

%w[-Wall -g -rdynamic -fPIC].each { |flag|
  $CPPFLAGS += " #{flag}" unless $CPPFLAGS.include?(flag)
}

create_makefile('v8')
