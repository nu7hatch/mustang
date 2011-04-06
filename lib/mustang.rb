require 'v8'
require 'v8/context'
require 'v8/object'
require 'v8/string'
require 'v8/integer'
require 'v8/number'
require 'v8/array'
require 'v8/date'
require 'v8/regexp'
require 'v8/function'
require 'v8/external'
require 'v8/error'

require 'support/delegated'

require 'core_ext/object'
require 'core_ext/class'
require 'core_ext/symbol'

require 'mustang/context'

module Mustang
  extend Delegated

  def self.delegate
    global
  end

  def self.global
    @global or reset!
  end

  # Resets global context state (just creates new global context and enters to it). 
  def self.reset!(*args, &block)
    @global = Context.new(*args, &block)
  end
end # Mustang
