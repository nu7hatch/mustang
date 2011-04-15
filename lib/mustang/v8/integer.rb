module Mustang
  module V8
    class Integer
      include Delegated

      def <=>(other)
        to_i <=> other
      end

      def delegate
        to_i
      end

      def kind_of?(klass)
        klass == ::Fixnum or super(klass)
      end

      def ==(other)
        super(other) or to_i == other
      end
    end # Integer
  end # V8
end # Mustang
