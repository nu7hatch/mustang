$LOAD_PATH.unshift File.dirname(__FILE__) + "/lib"
require 'rubygems'

Dir["spec/**/*_spec.rb"].each { |spec|
  require spec
}
