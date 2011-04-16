module Mustang
  module V8
    class Date
      include Delegated

      def to_time
        Time.at(value)
      end
      
      def <=>(other)
        to_time <=> other
      end

      def delegate
        to_time
      end

      def ==(other)
        super(other) or to_time == other
      end

      def kind_of?(klass)
        klass == Time or super(klass)
      end
    end # Date
  end # V8
end # Mustang
