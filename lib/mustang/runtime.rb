require 'mustang/engine'

module Mustang
  class Runtime < V8::Runtime
    
    def evaluate(source, locals={}, filename="unknown")
      super(source, filename)
    end

  end # Runtime
end # Mustang
