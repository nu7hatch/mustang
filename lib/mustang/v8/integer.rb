module Mustang
  module V8
    class Integer
      include Comparable
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
    end # Integer
  end # V8
end # Mustang
