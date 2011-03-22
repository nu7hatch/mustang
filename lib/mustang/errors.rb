require 'ostruct'

module Mustang
  # Javascript errors container. Its objects keeps list of errors encountered
  # during code evaluation.
  class Errors < Array
    # Single error info entry.
    class Error < OpenStruct; end

    def initialize(runtime)
      @runtime = runtime
      super()
    end

    def push(errinfo)
      errinfo = Error.new(errinfo)
      print_error(errinfo) if @runtime.debug?
      super(errinfo)
    end

    private
    
    def print_error(err)
      $stderr.write("(JS) %s " % err.message)
      if err.file and err.line
        $stderr.write("in file `%s' at line %d\n" % [err.file, err.line])
        if err.snippet and err.pos
          $stderr.write("  %s\n  " % err.snippet)
          err.pos.first.times { $stderr.write(" ") }
          [(err.pos.first)..(err.pos.last)].each { $stderr.write("^") }
          $stderr.write("\n")
        end
      end
    end
  end # Errors
end # Mustang
