require 'mustang/support/delegated'

require 'mustang/v8'
require 'mustang/v8/context'
require 'mustang/v8/object'
require 'mustang/v8/string'
require 'mustang/v8/integer'
require 'mustang/v8/number'
require 'mustang/v8/array'
require 'mustang/v8/date'
require 'mustang/v8/regexp'
require 'mustang/v8/function'
require 'mustang/v8/external'
require 'mustang/v8/boolean'
require 'mustang/v8/error'

require 'mustang/core_ext/object'
require 'mustang/core_ext/class'
require 'mustang/core_ext/symbol'

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
