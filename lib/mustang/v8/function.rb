module Mustang
  module V8
    class Function
      def call(*args, &block)
        call_on(@receiver, *args, &block);
      end

      def to_proc
        origin.to_proc if origin
      end

      def kind_of?(klass)
        origin.is_a?(klass) or super(klass)
      end
    end # Function
  end # V8
end # Mustang
