module Mustang
  # Raised when file specified to load doesn't exist. 
  class ScriptNotFoundError < Errno::ENOENT 
  end

  # Extended and more user-friendly version of <tt>Mustang::V8::Context</tt>.
  class Context < V8::Context
    # Evaluates given javascript source. Before evaluation it's able to
    # set given local variables within current context, eg:
    #
    #   rt = Mustang::Context.new
    #   rt.evaluate("foo=1")                     # => 1
    #   rt.evaluate("bar=foo+spam", :spam => 10) # => 11
    #   rt.evaliate("bar")                       # => 11
    #
    def evaluate(source, locals={}, filename="<eval>")
      res = super(source, filename)
      errors << res if res.v8? and res.error?
      return res
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

    # Returns list of errors encountered within this context. 
    def errors
      @errors ||= []
    end
  end # Context
end # Mustang
