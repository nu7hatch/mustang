$LOAD_PATH.unshift File.expand_path("../../lib", __FILE__)

require 'rspec'
require 'mocha'
require 'mustang'

RSpec.configure do |config|
  config.mock_with :mocha
end

def debug
  require 'ruby-debug'
  debugger
end

def setup_context
  let(:cxt) { Mustang::V8::Context.new }
  before { cxt.enter }
  after { cxt.exit }
end
