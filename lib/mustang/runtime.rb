require 'mustang/v8'
require 'mustang/errors'

module Mustang
  # Raised when given script file doesn't exist.
  class ScriptNotFoundError < Errno::ENOENT; end

  class Runtime < V8::Runtime
    attr_accessor :debug
    alias_method :debug?, :debug

    alias_method :set, :"[]="
    alias_method :get, :"[]"
    
    # On initialize you can specify additional local variables to set
    # within current context, eg:
    #
    #   rt = Mustang::Runtime.new(:foo => "bar")
    #   rt[:foo] # => "bar"
    #
    def initialize(locals={})
      @debug = false
      @errors = Errors.new(self)

      super()
      set_all(locals)
      #set("Ruby", Object)
    end

    # Evaluates given javascript source. Before evaluation it's able to
    # set given local variables within current context, eg:
    #
    #   rt = Mustang::Runtime.new
    #   rt.evaluate("foo=1") # => 1
    #   rt.evaluate("bar=foo+spam", :spam => 10) # => 11
    #   rt.evaliate("bar") # => 11
    #
    def evaluate(source, locals={}, filename="unknown")
      set_all(locals)
      super(source, filename) unless source.empty?
    end

    # Loads and evaluates given list of javascript files, eg:
    #
    #   rt = Mustang::Runtime.new
    #   rt.load("foo.js", "bar.js")
    #
    def load(*files)
      files.each { |filename|
        raise ScriptNotFoundError, "Script `#{filename}' does not exts." unless File.exists?(filename)
        evaluate(File.read(filename), {}, filename)
      }
    end

    private

    # Sets all given variables within current context. 
    def set_all(variables)
      variables.each { |name, value| set(name, value) }
    end
  end # Runtime
end # Mustang
