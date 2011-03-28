require 'mustang/v8'
require 'mustang/v8/object'
require 'mustang/v8/string'
require 'mustang/v8/integer'
require 'mustang/v8/array'
require 'mustang/context'

module Mustang
  # Global context used by <tt>Mustang.evaluate</tt> and <tt>Mustang.load</tt>
  # singleton methods. 
  def self.global
    @global ||= Context.new
  end

  # Delegates all missing methods to global <tt>Mustang::Context</tt> instance. 

  def self.method_missing(meth, *args, &block) # :nodoc:
    if (global.respond_to?(meth))
      global.send(meth, *args, &block)
    else
      super
    end
  end

  def self.respond_to?(meth) # :nodoc:
    global.respond_to?(meth) or super
  end
end # Mustang
