module Mustang
  module V8
    class Array
      include Comparable
      include Enumerable

      def <=>(other)
        to_a <=> other
      end

      def each(*args, &block)
        to_a.each(*args, &block)
      end
    end # Integer
  end # V8
end # Mustang
