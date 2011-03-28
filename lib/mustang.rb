require 'v8'
require 'v8/object'
require 'v8/string'
require 'v8/integer'
require 'v8/number'
require 'v8/array'
require 'mustang/context'
require 'support/delegated'

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

  # Global context used by <tt>Mustang.evaluate</tt> and <tt>Mustang.load</tt>
  # singleton methods. 
  def self.global
    @global ||= Context.new
  end

  # We need enter into global context to avoid segfaults. Apart of my laziness
  # keeping one global context is much easier than handling errors in all 
  # V8 datatypes implementation... 
  enter

  # ... so we have to disable exit as well. 
  def self.exit
    raise RuntimeError, "Global V8 context can't be exited"
  end
end # Mustang
