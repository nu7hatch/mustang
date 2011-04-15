module Mustang
  module V8
    class Number
      include Delegated

      def <=>(other)
        to_f <=> other
      end

      def delegate
        to_f
      end

      def kind_of?(klass)
        klass == ::Float or super(klass)
      end

      def ==(other)
        super(other) or to_f == other
      end
    end # Number
  end # V8
end # Mustang
