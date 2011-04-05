# -*- ruby -*-

begin
  Ore::Specification.new do |gemspec|
    gemspec.files = `git ls-files`.split("\n")
    gemspec.test_files = `git ls-files spec`.split("\n")
    gemspec.extensions = ["ext/v8/extconf.rb"]
    gemspec.require_paths = ["lib", "ext", "vendor"]
  end
rescue NameError
  begin
    require 'ore/specification'
    retry
  rescue LoadError
    STDERR.puts "The '#{__FILE__}' file requires Ore."
    STDERR.puts "Run `gem install ore-core` to install Ore."
  end
end
