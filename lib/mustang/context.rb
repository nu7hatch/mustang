module Mustang
  # Raised when file specified to load doesn't exist. 
  class ScriptNotFoundError < Errno::ENOENT 
  end

  # Extended and more user-friendly version of <tt>Mustang::V8::Context</tt>.
  class Context < V8::Context
    attr_accessor :raise_errors
    alias_method :raise_errors?, :raise_errors

    # Evaluates given javascript source. Before evaluation it's able to
    # set given local variables within current context, eg:
    #
    #   rt = Mustang::Context.new
    #   rt.evaluate("foo=1")                     # => 1
    #   rt.evaluate("bar=foo+spam", :spam => 10) # => 11
    #   rt.evaliate("bar")                       # => 11
    #
    def evaluate(source, locals={}, filename="<eval>")
      result = super(source, filename)
      raise(error, error.message) if error and raise_errors?
      return result
    end
    alias_method :eval, :evaluate

    # Loads and evaluates given list of javascript files, eg:
    #
    #   rt = Mustang::Runtime.new
    #   rt.load("foo.js", "bar.js")
    #
    def load(*files)
      files.map { |filename|
        if File.exists?(filename)
          evaluate(File.read(filename), {}, filename)
        else
          raise ScriptNotFoundError, "Script `#{filename}' does not exts."
        end
      }.last
    end

    # Returns <tt>true</tt> when it is global (immortal) context.
    def global?
      false
    end
  end # Context

  # This kind of context is used by <tt>Mustang</tt> module. 
  class GlobalContext < Context
    def initialize(*)
      super 
      enter
    end

    def exit
      # We have to disable exit, because global context is immortal. 
      raise RuntimeError, "Global context can't be exited"
    end
    
    def global?
      true
    end
  end # GlobalContext
end # Mustang
