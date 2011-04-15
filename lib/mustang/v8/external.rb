module Mustang
  module V8
    class External
      include Delegated
      
      def <=>(other)
        to_obj <=> other
      end

      def delegate
        to_obj
      end

      def ==(other)
        super(other) or to_obj == other
      end
    end # External
  end # V8
end # Mustang
