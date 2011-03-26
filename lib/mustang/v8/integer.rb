module Mustang
  module V8
    class Integer
      include Comparable

      def <=>(other)
        to_i <=> other
      end
    end # Integer
  end # V8
end # Mustang
