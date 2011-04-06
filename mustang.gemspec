# -*- ruby -*-

Gem::Specification.new do |s|
  s.name = "mustang"
  s.summary = "Awesome V8 JavaScript engine embedded into the Ruby's shiny body"
  s.version = "0.2.0"
  s.authors = ["Chris Kowalik", "Cubox"]
  s.description = "Ruby proxy library for Google V8 Javascript engine."
  s.homepage = "http://github.com/nu7hatch/mustang"
  s.email = "chris@nu7hat.ch" 
  s.license = "MIT"

  s.files = `git ls-files`.split("\n")
  s.test_files = `git ls-files spec`.split("\n")
  s.extensions = ["ext/v8/extconf.rb"]
  s.require_paths = ["lib", "ext"]

  s.add_development_dependency "ore-tasks", "~> 0.4"
  s.add_development_dependency "rspec", "~> 2.0"
  s.add_development_dependency "mocha", "~> 0.9"
  s.add_development_dependency "rake-compiler", "~> 0.7"
end
