module Mustang
  module V8
    class Date
      include Comparable
      include Delegated

      def to_time
        Time.at(to_i);
      end
      
      def <=>(other)
        to_time <=> other
      end

      def delegate
        to_time
      end

      def kind_of?(klass)
        klass == ::Time or klass == ::Date or klass == ::DateTime or super(klass)
      end
    end # Date
  end # V8
end # Mustang
