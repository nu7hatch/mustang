require 'mustang/engine'

module Mustang
  class Runtime < Engine::Runtime
    
    def initialize(variables={})
      super()
      variables.each { |var, value| set(var, value) }
    end

    def get(var)
      evaluate("#{var.to_s};");
    end
    alias_method :"[]", :get

    def set(var, value)
      evaluate("var #{var.to_s}=#{value.inspect};");
    end
    alias_method :"[]=", :set

    def evaluate(source, variables={}, filename="unknown")
      variables.each { |var, value| set(var, value) }
      super(source, filename)
    end

  end # Runtime
end # Mustang
