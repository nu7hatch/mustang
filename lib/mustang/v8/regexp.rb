module Mustang
  module V8
    class Regexp
      include Delegated
      
      def ==(other)
        to_regexp == other
      end
      
      def ===(other)
        to_regexp == other
      end
      
      def delegate
        to_regexp
      end
      
      def to_regexp
        ::Regexp.new(source.to_s, options.to_i)
      end

      def kind_of?(klass)
        klass == ::Regexp or super(klass)
      end

      def ==(other)
        super(other) or to_regexp == other
      end
    end # Regexp
  end # V8
end # Mustang
