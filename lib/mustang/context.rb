module Mustang
  class Context < V8::Context
    
    def evaluate(source, locals={}, filename="<eval>")
      super(source, filename)
    end
    alias_method :eval, :evaluate

  end # Context
end # Mustang
