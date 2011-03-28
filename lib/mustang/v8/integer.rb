module Mustang
  module V8
    class Integer
      include Comparable

      def <=>(other)
        to_i <=> other
      end

      def to_s
        to_i.to_s
      end

      def to_f
        to_i.to_f
      end
    end # Integer
  end # V8
end # Mustang
