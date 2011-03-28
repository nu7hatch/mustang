$LOAD_PATH.unshift File.expand_path("../../lib", __FILE__)

require 'rspec'
require 'mocha'
require 'mustang'

RSpec.configure do |config|
  config.mock_with :mocha
end

def setup_context
  let(:cxt) { V8::Context.new }
  before { cxt.enter }
  after { cxt.exit }
end
