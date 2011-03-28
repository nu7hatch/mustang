module Mustang
  module V8
    class String
      include Comparable

      def <=>(other)
        to_s <=> other
      end

      if RUBY_VERSION > "1.9"
        alias_method :native_to_utf8, :to_utf8

        def to_utf8
          # On ruby 1.9+ we have to force UTF-8 encoding here...
          native_to_utf8.force_encoding("UTF-8")
        end
      end

      alias_method :to_s, :to_utf8
    end # String
  end # V8
end # Mustang
