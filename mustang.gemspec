# -*- ruby -*-

begin
  Ore::Specification.new do |gemspec|
    gemspec.files += `git ls-files vendor/v8/`
    gemspec.files += `git ls-files ext/`
    gemspec.extensions << "ext/extconf.rb"
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
