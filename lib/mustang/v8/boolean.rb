module Mustang
  module V8
    class Boolean
      def true?
        self.is_a?(TrueClass)
      end

      def false?
        self.is_a?(FalseClass)
      end

      def to_s
        to_bool.to_s
      end

      def &(other)
        to_bool & other
      end

      def |(other)
        to_bool | other
      end

      def ^(other)
        to_bool ^ other
      end

      def ==(other)
        to_bool == other
      end

      def <=>(other)
        to_bool <=> other
      end

      def kind_of?(klass)
        klass == (false? ? false.class : true.class) or super(klass)
      end

      def to_bool
        false? ? false : true
      end
    end # Boolean
  end # V8
end # Mustang
