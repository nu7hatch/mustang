module Mustang
  module V8
    class Array
      include Enumerable
      include Delegated
      
      def <=>(other)
        to_a <=> other
      end
      
      def each(*args, &block)
        to_a.each(*args, &block)
      end
      
      def delegate
        to_a
      end

      def ==(other)
        super(other) or to_a == other
      end

      def kind_of?(klass)
        klass == ::Array or super(klass)
      end
    end # Array
  end # V8
end # Mustang
