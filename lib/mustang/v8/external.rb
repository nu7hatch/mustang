module Mustang
  module V8
    class External
      include Comparable
      include Delegated
      
      def <=>(other)
        to_obj <=> other
      end

      def delegate
        to_obj
      end
    end # External
  end # V8
end # Mustang
