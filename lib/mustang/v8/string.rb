module Mustang
  module V8
    class String
      # On ruby 1.9+ we have to force UTF-8 encoding here...
      if RUBY_VERSION > "1.9"
        alias_method :native_to_utf8, :to_utf8

        def to_utf8
          native_to_utf8.force_encoding("UTF-8")
        end
        alias_method :to_s, :to_utf8
      end

      include Delegated

      def <=>(other)
        to_s <=> other
      end

      def ==(other)

        super(other) or to_s == other
      end

      def delegate
        to_s
      end

      def kind_of?(klass)
        klass == ::String or super(klass)
      end
    end # String
  end # V8
end # Mustang
