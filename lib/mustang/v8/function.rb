module Mustang
  module V8
    class Function
      def call(*args, &block)
        call_on(@receiver || nil, *args, &block);
      end
    end # Function
  end # V8
end # Mustang
