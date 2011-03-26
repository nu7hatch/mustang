module Mustang
  module V8
    class String
      include Comparable

      def <=>(other)
        to_s <=> other
      end
    end # String
  end # V8
end # Mustang
