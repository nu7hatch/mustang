module Mustang
  module V8
    class Error
      def inspect
        "#{name}: #{message}"
      end
      
      def to_s
        message
      end
      
      def name
        self.class.name.delete("Mustang::V8::")
      end
    end # Error
  end # V8
end # Mustang
