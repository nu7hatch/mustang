require 'v8'
require 'v8/object'
require 'v8/string'
require 'v8/integer'
require 'v8/number'
require 'v8/array'
require 'mustang/context'
require 'support/delegated'

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
end # Mustang
