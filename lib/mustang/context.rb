module Mustang
  # Raised when file specified to load doesn't exist. 
  class ScriptNotFoundError < Errno::ENOENT 
  end

  # Extended and more user-friendly version of <tt>Mustang::V8::Context</tt>.
  class Context < V8::Context
    class << self
      alias_method :native_new, :new

      def new(locals={})
        cxt = native_new()
        cxt.set_all(locals)
        cxt
      end
    end

    # Evaluates given javascript source. Before evaluation it's able to
    # set given local variables within current context, eg:
    #
    #   rt = Mustang::Context.new
    #   rt.evaluate("foo=1")                     # => 1
    #   rt.evaluate("bar=foo+spam", :spam => 10) # => 11
    #   rt.evaliate("bar")                       # => 11
    #
    def evaluate(source, locals={}, filename="<eval>")
      set_all(locals)
      super(source, filename)
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
          raise ScriptNotFoundError, "script file `#{filename}' does not exist."
        end
      }.last
    end

    # Stores all given values to global prototype.
    def set_all(values={})
      values.each { |key, val| set(key, val) }
    end
  end # Context
end # Mustang
