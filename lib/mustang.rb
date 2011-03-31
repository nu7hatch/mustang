require 'v8'
require 'v8/object'
require 'v8/string'
require 'v8/integer'
require 'v8/number'
require 'v8/array'
require 'v8/date'

require 'support/delegated'
require 'mustang/context'

class Object
  # From now on each object can be easily converted to <tt>V8::*</tt> object
  # using <tt>#to_v8</tt> method. 
  include V8::Cast
end

module Mustang
  extend Delegated

  def self.delegate
    global
  end

  # Global context.
  def self.global
    @global or reset!
  end

  # Resets global context state. Appartently just creates new global
  # context and enters it. 
  def self.reset!(*args, &block)
    @global = GlobalContext.new(*args, &block)
  end

  # We need enter into global context to avoid segfaults. Apart of my laziness
  # keeping one global context is much easier than handling errors in all 
  # V8 datatypes implementation... 
  enter
end # Mustang
