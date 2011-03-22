require 'mustang/engine'

module Mustang
  class ScriptNotFoundError < Errno::ENOENT; end

  class Runtime < V8::Runtime
    alias_method :set, :"[]="
    alias_method :get, :"[]"
    
    def initialize(locals={})
      super()
      evaluate("", locals);
    end

    def evaluate(source, locals={}, filename="unknown")
      locals.each { |name, value| set(name, value) }
      super(source, filename) unless source.empty?
    end

    def load(filename, locals={})
      if File.exists?(filename)
        evaluate(File.read(filename), locals, filename)
      else
        raise ScriptNotFoundError, "Script `#{filename}' does not exts."
      end
    end
  end # Runtime
end # Mustang
