# -*- ruby -*-

begin
  require 'ore/tasks'
  Ore::Tasks.new
rescue LoadError
  STDERR.puts "Run `gem install ore-tasks` to install 'ore/tasks'."
end

begin
  require 'rake/extensiontask'
  Rake::ExtensionTask.new("v8") do |ext|
    ext.lib_dir = 'lib/mustang'
    ext.source_pattern = "*.{cpp,h}"
    ext.config_options << '--with-v8-include'
    ext.config_options << '--with-v8-lib'
  end
rescue LoadError
  STDERR.puts "Run `gem install rake-compiler` to install 'rake-compiler'."
end

begin
  require 'rspec/core/rake_task'
  RSpec::Core::RakeTask.new(:spec) do |t|
    t.rspec_opts = ENV['RSPEC_OPTS']
  end
rescue LoadError
  task :spec do
    abort 'Run `gem install rspec` to install RSpec'
  end
end

task :test => [:clean, :compile, :spec]
task :default => :test

begin
  require 'metric_fu'
rescue LoadError
  STDERR.puts "Run `gem install metric_fu` to install Metric-Fu"
end

require 'rake/rdoctask'
Rake::RDocTask.new do |rdoc|
  rdoc.rdoc_dir = 'rdoc'
  rdoc.title = "Mustang - V8 for ruby"
  rdoc.rdoc_files.include('README*')
  rdoc.rdoc_files.include('lib/**/*.rb')
end

task :init_mustang do
  require 'mustang'
  $c = Mustang::Context.new
end 

Rake::Task[:console].prerequisites << :init_mustang
