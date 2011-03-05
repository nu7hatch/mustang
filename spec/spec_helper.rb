$LOAD_PATH.unshift File.expand_path("../../lib", __FILE__)

require 'rspec'
require 'mocha'
require 'mustang'

RSpec.configure do |config|
  config.mock_with :mocha
end
