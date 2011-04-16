$LOAD_PATH.unshift File.dirname(__FILE__) + "/lib"
require 'rubygems'
require 'mustang'

Mustang[:t] = Time.now

#Dir["spec/**/*_spec.rb"].each { |spec|
#  require spec
#}
